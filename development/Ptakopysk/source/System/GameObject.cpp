#include "../../include/Ptakopysk/System/GameObject.h"
#include "../../include/Ptakopysk/System/GameManager.h"
#include "../../include/Ptakopysk/Components/Component.h"
#include <XeCore/Common/Logger.h>
#include <sstream>

namespace Ptakopysk
{

    RTTI_CLASS_DERIVATIONS( GameObject,
                            RTTI_DERIVATION( Serialized ),
                            RTTI_DERIVATIONS_END
                            )

    GameObject::GameObject( const std::string& id )
    : RTTI_CLASS_DEFINE( GameObject )
    , Id( this, &GameObject::getId, &GameObject::setId )
    , Active( this, &GameObject::isActive, &GameObject::setActive )
    , Order( this, &GameObject::getOrder, &GameObject::setOrder )
    , MetaData( this, &GameObject::getMetaData, &GameObject::setMetaData )
    , Owner( this, &GameObject::getGameManager, 0 )
    , m_gameManager( 0 )
    , m_parent( 0 )
    , m_id( id )
    , m_active( true )
    , m_order( 0 )
    , m_metaData( Json::Value::null )
    {
        serializableProperty( "Id" );
        serializableProperty( "Active" );
        serializableProperty( "Order" );
        serializableProperty( "MetaData" );
    }

    GameObject::~GameObject()
    {
        removeAllComponents();
        removeAllGameObjects();
        processAdding();
        processRemoving();
    }

    GameManager* GameObject::getGameManagerRoot()
    {
        if( m_gameManager )
            return m_gameManager;
        GameObject* p = m_parent;
        while( p )
        {
            if( p->m_gameManager )
                return p->m_gameManager;
            p = p->getParent();
        }
        return 0;
    }

    void GameObject::fromJson( const Json::Value& root )
    {
        if( !root.isObject() )
            return;
        Json::Value properties = root[ "properties" ];
        if( !properties.isNull() )
            deserialize( properties );
        Json::Value components = root[ "components" ];
        if( components.isArray() )
        {
            Json::Value item;
            Json::Value itemType;
            Json::Value itemProps;
            Component* comp;
            for( unsigned int i = 0; i < components.size(); i++ )
            {
                item = components[ i ];
                if( item.isObject() )
                {
                    itemType = item[ "type" ];
                    if( itemType.isString() )
                    {
                        std::string typeId = itemType.asString();
                        XeCore::Common::IRtti::Derivation type = GameManager::findComponentFactoryTypeById( typeId );
                        if( !type )
                        {
                            std::stringstream ss;
                            ss << "Component factory for type '" << typeId.c_str() << "' not found!";
                            XWARNING( ss.str().c_str() );
                        }
                        comp = getComponent( type );
                        if( comp )
                            comp->fromJson( item );
                        else
                        {
                            comp = GameManager::buildComponent( typeId );
                            if( comp )
                            {
                                comp->fromJson( item );
                                addComponent( comp );
                            }
                        }
                    }
                }
            }
        }
        Json::Value gameObjects = root[ "gameObjects" ];
        GameManager* gm = getGameManagerRoot();
        if( gameObjects.isArray() && gm )
        {
            Json::Value item;
            Json::Value itemPrefab;
            for( unsigned int i = 0; i < gameObjects.size(); i++ )
            {
                item = gameObjects[ i ];
                itemPrefab = item[ "prefab" ];
                if( !itemPrefab.isNull() )
                {
                    GameObject* go = gm->instantiatePrefab( itemPrefab.asString() );
                    if( go )
                    {
                        addGameObject( go );
                        go->fromJson( item );
                    }
                }
                else
                {
                    GameObject* go = xnew GameObject();
                    addGameObject( go );
                    go->fromJson( item );
                }
            }
        }
    }

    Json::Value GameObject::toJson()
    {
        Json::Value root;
        serialize( root[ "properties" ] );
        Json::Value components;
        Json::Value comp;
        for( std::map< XeCore::Common::IRtti::Derivation, Component* >::iterator it = m_components.begin(); it != m_components.end(); it++ )
        {
            comp = it->second->toJson();
            if( !comp.isNull() )
                components.append( comp );
        }
        if( !components.isNull() )
            root[ "components" ] = components;
        if( !m_gameObjects.empty() )
        {
            Json::Value gameObjects;
            GameObject* go;
            Json::Value item;
            for( List::iterator it = m_gameObjects.begin(); it != m_gameObjects.end(); it++ )
            {
                go = *it;
                item = go->toJson();
                if( !item.isNull() )
                    gameObjects.append( item );
            }
            root[ "gameObjects" ] = gameObjects;
        }
        return root;
    }

    void GameObject::duplicate( GameObject* from )
    {
        if( from )
            from->onDuplicate( this );
    }

    void GameObject::addComponent( Component* c )
    {
        if( !c || m_components.count( c->getType() ) )
            return;
        m_components[ c->getType() ] = c;
        c->setGameObject( this );
    }

    void GameObject::removeComponent( Component* c )
    {
        if( !c || m_components.count( c->getType() ) )
            return;
        m_components.erase( c->getType() );
        c->setGameObject( 0 );
        DELETE_OBJECT( c );
    }

    void GameObject::removeComponent( XeCore::Common::IRtti::Derivation d )
    {
        if( !m_components.count( d ) )
            return;
        Component* c = m_components[ d ];
        m_components.erase( d );
        c->setGameObject( 0 );
        DELETE_OBJECT( c );
    }

    void GameObject::removeAllComponents()
    {
        Component* c;
        for( std::map< XeCore::Common::IRtti::Derivation, Component* >::iterator it = m_components.begin(); it != m_components.end(); it++ )
        {
            c = it->second;
            c->setGameObject( 0 );
            DELETE_OBJECT( c );
        }
        m_components.clear();
    }

    bool GameObject::hasComponent( Component* c )
    {
        return c && m_components.count( c->getType() );
    }

    bool GameObject::hasComponent( XeCore::Common::IRtti::Derivation d )
    {
        return m_components.count( d );
    }

    void GameObject::addGameObject( GameObject* go )
    {
        if( !go || go->getType() != RTTI_CLASS_TYPE( GameObject ) || hasGameObject( go ) || isWaitingToAdd( go ) )
            return;
        m_gameObjectsToCreate.push_back( go );
        go->setParent( this );
    }

    void GameObject::removeGameObject( GameObject* go )
    {
        if( !hasGameObject( go ) )
            return;
        m_gameObjectsToDestroy.push_back( go );
        go->onDestroy();
    }

    void GameObject::removeGameObject( const std::string& id )
    {
        GameObject* go = getGameObject( id );
        if( !go )
            return;
        m_gameObjectsToDestroy.push_back( go );
        go->onDestroy();
    }

    void GameObject::removeAllGameObjects()
    {
        GameObject* go;
        for( List::iterator it = m_gameObjects.begin(); it != m_gameObjects.end(); it++ )
        {
            go = *it;
            m_gameObjectsToDestroy.push_back( go );
            go->onDestroy();
        }
    }

    bool GameObject::hasGameObject( GameObject* go )
    {
        for( List::iterator it = m_gameObjects.begin(); it != m_gameObjects.end(); it++ )
            if( *it == go )
                return true;
        return false;
    }

    bool GameObject::hasGameObject( const std::string& id )
    {
        for( List::iterator it = m_gameObjects.begin(); it != m_gameObjects.end(); it++ )
            if( (*it)->getId() == id )
                return true;
        return false;
    }

    GameObject* GameObject::getGameObject( const std::string& id )
    {
        for( List::iterator it = m_gameObjects.begin(); it != m_gameObjects.end(); it++ )
            if( (*it)->getId() == id )
                return *it;
        return 0;
    }

    void GameObject::processAdding()
    {
        GameObject* go;
        for( List::iterator it = m_gameObjectsToCreate.begin(); it != m_gameObjectsToCreate.end(); it++ )
        {
            go = *it;
            m_gameObjects.push_back( go );
            go->onCreate();
        }
        m_gameObjectsToCreate.clear();
    }

    void GameObject::processRemoving()
    {
        GameObject* go;
        for( List::iterator it = m_gameObjectsToDestroy.begin(); it != m_gameObjectsToDestroy.end(); it++ )
        {
            go = *it;
            m_gameObjects.remove( go );
            go->setParent( 0 );
            DELETE_OBJECT( go );
        }
        m_gameObjectsToDestroy.clear();
    }

    bool GameObject::isWaitingToAdd( GameObject* go )
    {
        for( List::iterator it = m_gameObjectsToCreate.begin(); it != m_gameObjectsToCreate.end(); it++ )
            if( *it == go )
                return true;
        return false;
    }

    bool GameObject::isWaitingToRemove( GameObject* go )
    {
        for( List::iterator it = m_gameObjectsToDestroy.begin(); it != m_gameObjectsToDestroy.end(); it++ )
            if( *it == go )
                return true;
        return false;
    }

    Json::Value GameObject::onSerialize( const std::string& property )
    {
        if( property == "Id" )
            return Json::Value( m_id );
        else if( property == "Active" )
            return Json::Value( m_active );
        else if( property == "Order" )
            return Json::Value( m_order );
        else if( property == "MetaData" )
            return m_metaData;
        return Json::Value::null;
    }

    void GameObject::onDeserialize( const std::string& property, const Json::Value& root )
    {
        if( property == "Id" && root.isString() )
            m_id = root.asString();
        else if( property == "Active" && root.isBool() )
            m_active = root.asBool();
        else if( property == "Order" && root.isInt() )
            m_order = root.asInt();
        else if( property == "MetaData" )
            m_metaData = root;
    }

    void GameObject::onCreate()
    {
        for( std::map< XeCore::Common::IRtti::Derivation, Component* >::iterator it = m_components.begin(); it != m_components.end(); it++ )
            it->second->onCreate();
        for( List::iterator it = m_gameObjects.begin(); it != m_gameObjects.end(); it++ )
            (*it)->onCreate();
    }

    void GameObject::onDestroy()
    {
        for( List::iterator it = m_gameObjects.begin(); it != m_gameObjects.end(); it++ )
            (*it)->onDestroy();
        for( std::map< XeCore::Common::IRtti::Derivation, Component* >::iterator it = m_components.begin(); it != m_components.end(); it++ )
            it->second->onDestroy();
    }

    void GameObject::onDuplicate( GameObject* dst )
    {
        if( !dst )
            return;
        dst->setId( m_id );
        dst->setActive( m_active );
        dst->setOrder( m_order );
        dst->setMetaData( m_metaData );
        XeCore::Common::IRtti::Derivation type;
        Component* comp;
        for( std::map< XeCore::Common::IRtti::Derivation, Component* >::iterator it = m_components.begin(); it != m_components.end(); it++ )
        {
            type = it->first;
            comp = dst->getComponent( type );
            if( comp )
                it->second->onDuplicate( comp );
            else
            {
                comp = GameManager::buildComponent( type );
                it->second->onDuplicate( comp );
                dst->addComponent( comp );
            }
        }
        for( List::iterator it = m_gameObjects.begin(); it != m_gameObjects.end(); it++ )
        {
            GameObject* go = xnew GameObject();
            dst->addGameObject( go );
            (*it)->onDuplicate( go );
        }
    }

    void GameObject::onUpdate( float dt, const sf::Transform& trans, bool sort )
    {
        if( m_active )
        {
            Component* c;
            sf::Transform t = trans;
            for( std::map< XeCore::Common::IRtti::Derivation, Component* >::iterator it = m_components.begin(); it != m_components.end(); it++ )
            {
                c = it->second;
                if( c->isActive() )
                {
                    if( c->getTypeFlags() & Component::tUpdate )
                        c->onUpdate( dt );
                    if( c->getTypeFlags() & Component::tTransform )
                        c->onTransform( trans, t );
                }
            }
            processAdding();
            processRemoving();
            if( sort )
                m_gameObjects.sort( GameManager::CompareGameObjects() );
            for( List::iterator it = m_gameObjects.begin(); it != m_gameObjects.end(); it++ )
                (*it)->onUpdate( dt, t, sort );
        }
    }

    void GameObject::onEvent( const sf::Event& event )
    {
        if( m_active )
        {
            Component* c;
            for( std::map< XeCore::Common::IRtti::Derivation, Component* >::iterator it = m_components.begin(); it != m_components.end(); it++ )
            {
                c = it->second;
                if( c->isActive() && c->getTypeFlags() & Component::tEvents )
                    c->onEvent( event );
            }
            for( List::iterator it = m_gameObjects.begin(); it != m_gameObjects.end(); it++ )
                (*it)->onEvent( event );
        }
    }

    void GameObject::onRender( sf::RenderTarget* target )
    {
        if( m_active )
        {
            Component* c;
            for( std::map< XeCore::Common::IRtti::Derivation, Component* >::iterator it = m_components.begin(); it != m_components.end(); it++ )
            {
                c = it->second;
                if( c->isActive() && c->getTypeFlags() & Component::tRender )
                    c->onRender( target );
            }
            for( List::iterator it = m_gameObjects.begin(); it != m_gameObjects.end(); it++ )
                (*it)->onRender( target );
        }
    }

    void GameObject::onCollide( GameObject* other, bool beginOrEnd, b2Contact* contact )
    {
        if( m_active )
        {
            Component* c;
            for( std::map< XeCore::Common::IRtti::Derivation, Component* >::iterator it = m_components.begin(); it != m_components.end(); it++ )
            {
                c = it->second;
                if( c->isActive() && c->getTypeFlags() & Component::tPhysics )
                    c->onCollide( other, beginOrEnd, contact );
            }
        }
    }

    void GameObject::onJointGoodbye( b2Joint* joint )
    {
        if( m_active )
        {
            Component* c;
            for( std::map< XeCore::Common::IRtti::Derivation, Component* >::iterator it = m_components.begin(); it != m_components.end(); it++ )
            {
                c = it->second;
                if( c->isActive() && c->getTypeFlags() & Component::tPhysics )
                    c->onJointGoodbye( joint );
            }
        }
    }

    void GameObject::onFixtureGoodbye( b2Fixture* fixture )
    {
        if( m_active )
        {
            Component* c;
            for( std::map< XeCore::Common::IRtti::Derivation, Component* >::iterator it = m_components.begin(); it != m_components.end(); it++ )
            {
                c = it->second;
                if( c->isActive() && c->getTypeFlags() & Component::tPhysics )
                    c->onFixtureGoodbye( fixture );
            }
        }
    }

}
