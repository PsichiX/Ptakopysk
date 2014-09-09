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
    , m_instanceOf( 0 )
    , m_prefab( false )
    , m_id( id )
    , m_active( true )
    , m_order( 0 )
    , m_isDestroying( false )
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
        processRemovingDelayedComponents();
        processRemoving();
        GameObject* go;
        for( List::iterator it = m_gameObjectsToCreate.begin(); it != m_gameObjectsToCreate.end(); it++ )
        {
            go = *it;
            DELETE_OBJECT( go );
        }
        m_gameObjectsToCreate.clear();
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
                                addComponent( comp );
                                comp->fromJson( item );
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

    Json::Value GameObject::toJson( bool omitDefaultValues )
    {
        GameManager* gm = getGameManagerRoot();
        GameObject* prefab = gm && m_instanceOf && gm->containsGameObject( m_instanceOf, true ) ? m_instanceOf : 0;
        Json::Value root;
        if( prefab )
            root[ "prefab" ] = Json::Value( prefab->getId() );
        Json::Value properties;
        serialize( properties, omitDefaultValues ? prefab : 0 );
        if( !properties.isNull() )
            root[ "properties" ] = properties;
        Json::Value components;
        Json::Value comp;
        Component* compOmit;
        for( Components::iterator it = m_components.begin(); it != m_components.end(); it++ )
        {
            compOmit = omitDefaultValues && prefab ? prefab->getComponent( it->first ) : 0;
            comp = it->second->toJson( compOmit );
            if( !comp.isNull() && ( prefab ? comp.size() > 1 : true ) )
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
                item = go->toJson( omitDefaultValues && prefab );
                if( !item.isNull() )
                    gameObjects.append( item );
            }
            if( !gameObjects.isNull() )
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
        if( !c || hasComponent( c->getType() ) )
            return;
        m_components.push_back( std::make_pair( c->getType(), c ) );
        c->setGameObject( this );
    }

    void GameObject::removeComponent( Component* c, bool delayed )
    {
        if( !c )
            return;
        for( Components::iterator it = m_components.begin(); it != m_components.end(); it++ )
        {
            if( it->second == c )
            {
                if( delayed )
                    m_componentsToDestroyDelayed.push_back( std::make_pair( c->getType(), c ) );
                else
                {
                    c->setGameObject( 0 );
                    DELETE_OBJECT( c );
                    m_components.erase( it );
                }
                return;
            }
        }
    }

    void GameObject::removeComponent( XeCore::Common::IRtti::Derivation d, bool delayed )
    {
        Component* c;
        for( Components::iterator it = m_components.begin(); it != m_components.end(); it++ )
        {
            if( it->first == d )
            {
                c = it->second;
                if( delayed )
                    m_componentsToDestroyDelayed.push_back( std::make_pair( c->getType(), c ) );
                else
                {
                    c->setGameObject( 0 );
                    DELETE_OBJECT( c );
                    m_components.erase( it );
                }
                return;
            }
        }
    }

    void GameObject::removeAllComponents( bool delayed )
    {
        if( delayed )
        {
            for( Components::iterator it = m_components.begin(); it != m_components.end(); it++ )
                m_componentsToDestroyDelayed.push_back( *it );
        }
        else
        {
            Component* c;
            for( Components::iterator it = m_components.begin(); it != m_components.end(); it++ )
            {
                c = it->second;
                c->setGameObject( 0 );
                DELETE_OBJECT( c );
            }
            m_components.clear();
        }
    }

    bool GameObject::hasComponent( Component* c )
    {
        if( !c )
            return false;
        for( Components::iterator it = m_components.begin(); it != m_components.end(); it++ )
        {
            if( it->second == c )
                return true;
        }
        return false;
    }

    bool GameObject::hasComponent( XeCore::Common::IRtti::Derivation d )
    {
        for( Components::iterator it = m_components.begin(); it != m_components.end(); it++ )
        {
            if( it->first == d )
                return true;
        }
        return false;
    }

    Component* GameObject::getComponent( XeCore::Common::IRtti::Derivation d )
    {
        for( Components::iterator it = m_components.begin(); it != m_components.end(); it++ )
        {
            if( it->first == d )
                return it->second;
        }
        return 0;
    }

    Component* GameObject::getOrCreateComponent( XeCore::Common::IRtti::Derivation d )
    {
        Component* c = getComponent( d );
        if( !c )
        {
            c = GameManager::buildComponent( d );
            if( c && c->getType() == d )
                addComponent( c );
            else
                DELETE_OBJECT( c );
        }
        return c;
    }

    void GameObject::processRemovingDelayedComponents()
    {
        Component* c;
        for( Components::iterator it = m_componentsToDestroyDelayed.begin(); it != m_componentsToDestroyDelayed.end(); it++ )
        {
            if( std::find( m_components.begin(), m_components.end(), *it ) == m_components.end() )
                continue;
            c = it->second;
            m_components.remove( *it );
            c->setGameObject( 0 );
            DELETE_OBJECT( c );
        }
        m_componentsToDestroyDelayed.clear();
    }

    bool GameObject::isWaitingToRemoveDelayedComponent( Component* c )
    {
        for( Components::iterator it = m_componentsToDestroyDelayed.begin(); it != m_componentsToDestroyDelayed.end(); it++ )
            if( it->second == c )
                return true;
        return false;
    }

    bool GameObject::isWaitingToRemoveDelayedComponent( XeCore::Common::IRtti::Derivation d )
    {
        for( Components::iterator it = m_componentsToDestroyDelayed.begin(); it != m_componentsToDestroyDelayed.end(); it++ )
            if( it->first == d )
                return true;
        return false;
    }

    bool GameObject::componentsContainsPoint( const sf::Vector2f& worldPos, bool all )
    {
        if( all )
        {
            for( Components::iterator it = m_components.begin(); it != m_components.end(); it++ )
                if( it->second->getTypeFlags() & Component::tShape )
                    if( !it->second->onCheckContainsPoint( worldPos ) )
                        return false;
            return true;
        }
        else
        {
            for( Components::iterator it = m_components.begin(); it != m_components.end(); it++ )
                if( it->second->getTypeFlags() & Component::tShape )
                    if( it->second->onCheckContainsPoint( worldPos ) )
                        return true;
            return false;
        }
    }

    void GameObject::addGameObject( GameObject* go )
    {
        if( !go || go->getType() != RTTI_CLASS_TYPE( GameObject ) || hasGameObject( go ) || isWaitingToAdd( go ) )
            return;
        m_gameObjectsToCreate.push_back( go );
        go->setParent( this );
        go->setPrefab( m_prefab );
        go->setDestroying( m_isDestroying );
    }

    void GameObject::removeGameObject( GameObject* go )
    {
        if( !hasGameObject( go ) )
            return;
        m_gameObjectsToDestroy.push_back( go );
        go->setDestroying( true );
        go->onDestroy();
    }

    void GameObject::removeGameObject( const std::string& id )
    {
        GameObject* go = getGameObject( id );
        if( !go )
            return;
        m_gameObjectsToDestroy.push_back( go );
        go->setDestroying( true );
        go->onDestroy();
    }

    void GameObject::removeAllGameObjects()
    {
        GameObject* go;
        for( List::iterator it = m_gameObjects.begin(); it != m_gameObjects.end(); it++ )
        {
            go = *it;
            m_gameObjectsToDestroy.push_back( go );
            go->setDestroying( true );
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

    bool GameObject::containsGameObject( GameObject* go )
    {
        for( List::iterator it = m_gameObjects.begin(); it != m_gameObjects.end(); it++ )
            if( *it == go || (*it)->containsGameObject( go ) )
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

    GameObject* GameObject::findGameObject( const std::string& path )
    {
        return findGameObjectInPartOfPath( path, 0 );
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
            if( std::find( m_gameObjects.begin(), m_gameObjects.end(), go ) == m_gameObjects.end() )
                continue;
            m_gameObjects.remove( go );
            go->setParent( 0 );
            go->setPrefab( false );
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
        for( Components::iterator it = m_components.begin(); it != m_components.end(); it++ )
            it->second->onCreate();
        for( List::iterator it = m_gameObjects.begin(); it != m_gameObjects.end(); it++ )
            (*it)->onCreate();
    }

    void GameObject::onDestroy()
    {
        for( List::iterator it = m_gameObjects.begin(); it != m_gameObjects.end(); it++ )
            (*it)->onDestroy();
        for( Components::iterator it = m_components.begin(); it != m_components.end(); it++ )
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
        for( Components::iterator it = m_components.begin(); it != m_components.end(); it++ )
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
        for( List::iterator it = m_gameObjectsToCreate.begin(); it != m_gameObjectsToCreate.end(); it++ )
        {
            GameObject* go = xnew GameObject();
            dst->addGameObject( go );
            (*it)->onDuplicate( go );
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
            processRemovingDelayedComponents();
            Component* c;
            sf::Transform t = trans;
            for( Components::iterator it = m_components.begin(); it != m_components.end(); it++ )
            {
                c = it->second;
                if( c->isActive() )
                {
                    if( c->getTypeFlags() & Component::tUpdate )
                        c->onUpdate( dt );
                    if( c->getTypeFlags() & Component::tTransform )
                        c->onTransform( t, t );
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
            for( Components::iterator it = m_components.begin(); it != m_components.end(); it++ )
            {
                c = it->second;
                if( c->isActive() && c->getTypeFlags() & Component::tEvents )
                    c->onEvent( event );
            }
            for( List::iterator it = m_gameObjects.begin(); it != m_gameObjects.end(); it++ )
                (*it)->onEvent( event );
        }
    }

    void GameObject::onRender( sf::RenderTarget*& target )
    {
        if( m_active )
        {
            List::iterator it = m_gameObjects.begin();
            while( it != m_gameObjects.end() )
            {
                if( (*it)->getOrder() <= 0 )
                    break;
                (*it)->onRender( target );
                it++;
            }
            Component* c;
            for( Components::iterator it = m_components.begin(); it != m_components.end(); it++ )
            {
                c = it->second;
                if( c->isActive() && c->getTypeFlags() & Component::tRender )
                    c->onRender( target );
            }
            while( it != m_gameObjects.end() )
            {
                (*it)->onRender( target );
                it++;
            }
        }
    }

    void GameObject::onRenderEditor( sf::RenderTarget* target )
    {
        if( m_active )
        {
            List::iterator it = m_gameObjects.begin();
            while( it != m_gameObjects.end() )
            {
                if( (*it)->getOrder() <= 0 )
                    break;
                (*it)->onRenderEditor( target );
                it++;
            }
            Component* c;
            for( Components::iterator it = m_components.begin(); it != m_components.end(); it++ )
            {
                c = it->second;
                if( c->isActive() )
                    c->onRenderEditor( target );
            }
            while( it != m_gameObjects.end() )
            {
                (*it)->onRenderEditor( target );
                it++;
            }
        }
    }

    void GameObject::onCollide( GameObject* other, bool beginOrEnd, b2Contact* contact )
    {
        if( m_active )
        {
            Component* c;
            for( Components::iterator it = m_components.begin(); it != m_components.end(); it++ )
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
            for( Components::iterator it = m_components.begin(); it != m_components.end(); it++ )
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
            for( Components::iterator it = m_components.begin(); it != m_components.end(); it++ )
            {
                c = it->second;
                if( c->isActive() && c->getTypeFlags() & Component::tPhysics )
                    c->onFixtureGoodbye( fixture );
            }
        }
    }

    void GameObject::onTextureChanged( const sf::Texture* a, bool addedOrRemoved )
    {
        for( Components::iterator it = m_components.begin(); it != m_components.end(); it++ )
            it->second->onTextureChanged( a, addedOrRemoved );
        for( GameObject::List::iterator it = m_gameObjects.begin(); it != m_gameObjects.end(); it++ )
            (*it)->onTextureChanged( a, addedOrRemoved );
    }

    void GameObject::onShaderChanged( const sf::Shader* a, bool addedOrRemoved )
    {
        for( Components::iterator it = m_components.begin(); it != m_components.end(); it++ )
            it->second->onShaderChanged( a, addedOrRemoved );
        for( GameObject::List::iterator it = m_gameObjects.begin(); it != m_gameObjects.end(); it++ )
            (*it)->onShaderChanged( a, addedOrRemoved );
    }

    void GameObject::onSoundChanged( const sf::Sound* a, bool addedOrRemoved )
    {
        for( Components::iterator it = m_components.begin(); it != m_components.end(); it++ )
            it->second->onSoundChanged( a, addedOrRemoved );
        for( GameObject::List::iterator it = m_gameObjects.begin(); it != m_gameObjects.end(); it++ )
            (*it)->onSoundChanged( a, addedOrRemoved );
    }

    void GameObject::onMusicChanged( const sf::Music* a, bool addedOrRemoved )
    {
        for( Components::iterator it = m_components.begin(); it != m_components.end(); it++ )
            it->second->onMusicChanged( a, addedOrRemoved );
        for( GameObject::List::iterator it = m_gameObjects.begin(); it != m_gameObjects.end(); it++ )
            (*it)->onMusicChanged( a, addedOrRemoved );
    }

    void GameObject::onFontChanged( const sf::Font* a, bool addedOrRemoved )
    {
        for( Components::iterator it = m_components.begin(); it != m_components.end(); it++ )
            it->second->onFontChanged( a, addedOrRemoved );
        for( GameObject::List::iterator it = m_gameObjects.begin(); it != m_gameObjects.end(); it++ )
            (*it)->onFontChanged( a, addedOrRemoved );
    }

    void GameObject::onCustomAssetChanged( const ICustomAsset* a, bool addedOrRemoved )
    {
        for( Components::iterator it = m_components.begin(); it != m_components.end(); it++ )
            it->second->onCustomAssetChanged( a, addedOrRemoved );
        for( GameObject::List::iterator it = m_gameObjects.begin(); it != m_gameObjects.end(); it++ )
            (*it)->onCustomAssetChanged( a, addedOrRemoved );
    }

    void GameObject::setPrefab( bool mode )
    {
        m_prefab = mode;
        for( List::iterator it = m_gameObjectsToCreate.begin(); it != m_gameObjectsToCreate.end(); it++ )
            (*it)->setPrefab( mode );
        for( List::iterator it = m_gameObjects.begin(); it != m_gameObjects.end(); it++ )
            (*it)->setPrefab( mode );
        for( List::iterator it = m_gameObjectsToDestroy.begin(); it != m_gameObjectsToDestroy.end(); it++ )
            (*it)->setPrefab( mode );
    }

    void GameObject::setDestroying( bool mode )
    {
        m_isDestroying = mode;
        for( List::iterator it = m_gameObjectsToCreate.begin(); it != m_gameObjectsToCreate.end(); it++ )
            (*it)->setDestroying( mode );
        for( List::iterator it = m_gameObjects.begin(); it != m_gameObjects.end(); it++ )
            (*it)->setDestroying( mode );
        for( List::iterator it = m_gameObjectsToDestroy.begin(); it != m_gameObjectsToDestroy.end(); it++ )
            (*it)->setDestroying( mode );
    }

    GameObject* GameObject::findGameObjectInPartOfPath( const std::string& path, unsigned int from )
    {
        unsigned int p = path.find( '/', from );
        bool isRoot = from == 0 && p == 0 && p - from < 1;
        while( p - from < 1 && p != std::string::npos )
        {
            from++;
            p = path.find( '/', from );
        }
        if( isRoot )
        {
            GameManager* gm = getGameManagerRoot();
            if( gm )
                return gm->findGameObject( std::string( path, 1, std::string::npos ) );
            else
                return 0;
        }
        else
        {
            std::string part = std::string( path, from, p );
            if( part.empty() )
                return 0;
            else if( part == "." )
            {
                if( p == std::string::npos )
                    return this;
                else
                    return findGameObjectInPartOfPath( path, p + 1 );
            }
            else if( part == ".." )
            {
                if( p == std::string::npos )
                {
                    if( getParent() )
                        return getParent();
                    else
                        return 0;
                }
                else
                {
                    if( getParent() )
                        return getParent()->findGameObjectInPartOfPath( path, p + 1 );
                    else if( getGameManager() )
                        return getGameManager()->findGameObject( std::string( path, p + 1, std::string::npos ) );
                    else
                        return 0;
                }
            }
            else
            {
                GameObject* go = getGameObject( part );
                if( go )
                {
                    if( p == std::string::npos )
                        return go;
                    else
                        return go->findGameObjectInPartOfPath( path, p + 1 );
                }
                else
                    return 0;
            }
        }
        return 0;
    }

}
