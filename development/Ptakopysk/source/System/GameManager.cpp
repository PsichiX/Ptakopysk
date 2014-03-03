#include "../../include/Ptakopysk/System/GameManager.h"
#include "../../include/Ptakopysk/Components/Transform.h"
#include "../../include/Ptakopysk/Components/SpriteRenderer.h"
#include "../../include/Ptakopysk/Components/Body.h"
#include "../../include/Ptakopysk/Components/Camera.h"
#include "../../include/Ptakopysk/Components/TextRenderer.h"
#include "../../include/Ptakopysk/System/Assets.h"
#include "../../include/Ptakopysk/Serialization/b2BodyTypeSerializer.h"
#include "../../include/Ptakopysk/Serialization/BlendModeSerializer.h"
#include "../../include/Ptakopysk/Serialization/StyleSerializer.h"
#include <Box2D/Box2D.h>
#include <fstream>

namespace Ptakopysk
{

    class DestructionListener
        : public virtual XeCore::Common::IRtti
        , public virtual XeCore::Common::MemoryManager
        , public b2DestructionListener
    {
        RTTI_CLASS_DECLARE( DestructionListener );

    public:
                                DestructionListener( GameManager* owner );
                                ~DestructionListener();

        void                    SayGoodbye( b2Joint* joint );
        void                    SayGoodbye( b2Fixture* fixture );

    private:
        GameManager*            m_owner;
    };

    RTTI_CLASS_DERIVATIONS( DestructionListener,
                            RTTI_DERIVATIONS_END
                            );

    DestructionListener::DestructionListener( GameManager* owner )
    : RTTI_CLASS_DEFINE( DestructionListener )
    , m_owner( owner )
    {
    }

    DestructionListener::~DestructionListener()
    {
    }

    void DestructionListener::SayGoodbye( b2Joint* joint )
    {

    }

    void DestructionListener::SayGoodbye( b2Fixture* fixture )
    {

    }

    class ContactListener
        : public virtual XeCore::Common::IRtti
        , public virtual XeCore::Common::MemoryManager
        , public b2ContactListener
    {
        RTTI_CLASS_DECLARE( ContactListener );

    public:
                                ContactListener( GameManager* owner );
                                ~ContactListener();

        void                    BeginContact( b2Contact* contact );
        void                    EndContact( b2Contact* contact );
        void                    PreSolve( b2Contact* contact, const b2Manifold* oldManifold );
        void                    PostSolve( b2Contact* contact, const b2ContactImpulse* impulse );

    private:
        GameManager*            m_owner;
    };

    RTTI_CLASS_DERIVATIONS( ContactListener,
                            RTTI_DERIVATIONS_END
                            );

    ContactListener::ContactListener( GameManager* owner )
    : RTTI_CLASS_DEFINE( ContactListener )
    , m_owner( owner )
    {
    }

    ContactListener::~ContactListener()
    {
    }

    void ContactListener::BeginContact( b2Contact* contact )
    {
        if( m_owner )
            m_owner->processContact(
                true,
                (GameObject*)contact->GetFixtureA()->GetUserData(),
                (GameObject*)contact->GetFixtureB()->GetUserData(),
                contact
            );
    }

    void ContactListener::EndContact( b2Contact* contact )
    {
        if( m_owner )
            m_owner->processContact(
                false,
                (GameObject*)contact->GetFixtureA()->GetUserData(),
                (GameObject*)contact->GetFixtureB()->GetUserData(),
                contact
            );
    }

    void ContactListener::PreSolve( b2Contact* contact, const b2Manifold* oldManifold )
    {

    }

    void ContactListener::PostSolve( b2Contact* contact, const b2ContactImpulse* impulse )
    {

    }

    RTTI_CLASS_DERIVATIONS( GameManager,
                            RTTI_DERIVATIONS_END
                            );

    std::map< std::string, GameManager::ComponentFactoryData > GameManager::s_componentsFactory = std::map< std::string, GameManager::ComponentFactoryData >();

    GameManager::GameManager( float gravX, float gravY )
    : RTTI_CLASS_DEFINE( GameManager )
    , PhysicsGravity( this, &GameManager::getWorldGravity, &GameManager::setWorldGravity )
    , m_world( 0 )
    , m_destructionListener( 0 )
    , m_contactListener( 0 )
    {
        m_world = xnew b2World( b2Vec2( gravX, gravY ) );
        m_destructionListener = xnew DestructionListener( this );
        m_world->SetDestructionListener( m_destructionListener );
        m_contactListener = xnew ContactListener( this );
        m_world->SetContactListener( m_contactListener );
    }

    GameManager::~GameManager()
    {
        removeScene();
        DELETE_OBJECT( m_world );
        DELETE_OBJECT( m_destructionListener );
        DELETE_OBJECT( m_contactListener );
    }

    void GameManager::initialize()
    {
        Serialized::registerCustomSerializer( "b2BodyType", xnew b2BodyTypeSerializer() );
        Serialized::registerCustomSerializer( "BlendMode", xnew BlendModeSerializer() );
        Serialized::registerCustomSerializer( "Style", xnew StyleSerializer() );
        registerComponentFactory( "Transform", RTTI_CLASS_TYPE( Transform ), Transform::onBuildComponent );
        registerComponentFactory( "SpriteRenderer", RTTI_CLASS_TYPE( SpriteRenderer ), SpriteRenderer::onBuildComponent );
        registerComponentFactory( "Body", RTTI_CLASS_TYPE( Body ), Body::onBuildComponent );
        registerComponentFactory( "Camera", RTTI_CLASS_TYPE( Camera ), Camera::onBuildComponent );
        registerComponentFactory( "TextRenderer", RTTI_CLASS_TYPE( TextRenderer ), TextRenderer::onBuildComponent );
    }

    void GameManager::cleanup()
    {
        Serialized::unregisterAllCustomSerializers();
        unregisterAllComponentFactories();
    }

    void GameManager::registerComponentFactory( const std::string& id, XeCore::Common::IRtti::Derivation type, Component::OnBuildComponentCallback builder )
    {
        if( id.empty() || s_componentsFactory.count( id ) || !type || !builder )
            return;
        ComponentFactoryData d;
        d.type = type;
        d.builder = builder;
        s_componentsFactory[ id ] = d;
    }

    void GameManager::unregisterComponentFactory( const std::string& id )
    {
        if( s_componentsFactory.count( id ) )
            s_componentsFactory.erase( id );
    }

    void GameManager::unregisterComponentFactory( XeCore::Common::IRtti::Derivation type )
    {
        for( std::map< std::string, ComponentFactoryData >::iterator it = s_componentsFactory.begin(); it != s_componentsFactory.end(); it++ )
        {
            if( it->second.type == type )
            {
                s_componentsFactory.erase( it );
                return;
            }
        }
    }

    void GameManager::unregisterComponentFactory( Component::OnBuildComponentCallback builder )
    {
        for( std::map< std::string, ComponentFactoryData >::iterator it = s_componentsFactory.begin(); it != s_componentsFactory.end(); it++ )
        {
            if( it->second.builder == builder )
            {
                s_componentsFactory.erase( it );
                return;
            }
        }
    }

    void GameManager::unregisterAllComponentFactories()
    {
        s_componentsFactory.clear();
    }

    XeCore::Common::IRtti::Derivation GameManager::findComponentFactoryTypeById( const std::string& id )
    {
        if( s_componentsFactory.count( id ) )
            return s_componentsFactory[ id ].type;
        return 0;
    }

    XeCore::Common::IRtti::Derivation GameManager::findComponentFactoryTypeByBuilder( Component::OnBuildComponentCallback builder )
    {
        for( std::map< std::string, ComponentFactoryData >::iterator it = s_componentsFactory.begin(); it != s_componentsFactory.end(); it++ )
            if( it->second.builder == builder )
                return it->second.type;
        return 0;
    }

    std::string GameManager::findComponentFactoryIdByType( XeCore::Common::IRtti::Derivation type )
    {
        for( std::map< std::string, ComponentFactoryData >::iterator it = s_componentsFactory.begin(); it != s_componentsFactory.end(); it++ )
            if( it->second.type == type )
                return it->first;
        return std::string();
    }

    std::string GameManager::findComponentFactoryIdByBuilder( Component::OnBuildComponentCallback builder )
    {
        for( std::map< std::string, ComponentFactoryData >::iterator it = s_componentsFactory.begin(); it != s_componentsFactory.end(); it++ )
            if( it->second.builder == builder )
                return it->first;
        return std::string();
    }

    Component::OnBuildComponentCallback GameManager::findComponentFactoryBuilderById( const std::string& id )
    {
        if( s_componentsFactory.count( id ) )
            return s_componentsFactory[ id ].builder;
        return 0;
    }

    Component::OnBuildComponentCallback GameManager::findComponentFactoryBuilderByType( XeCore::Common::IRtti::Derivation type )
    {
        for( std::map< std::string, ComponentFactoryData >::iterator it = s_componentsFactory.begin(); it != s_componentsFactory.end(); it++ )
            if( it->second.type == type )
                return it->second.builder;
        return 0;
    }

    Component* GameManager::buildComponent( const std::string& id )
    {
        if( s_componentsFactory.count( id ) )
            return s_componentsFactory[ id ].builder();
        return 0;
    }

    Component* GameManager::buildComponent( XeCore::Common::IRtti::Derivation type )
    {
        for( std::map< std::string, ComponentFactoryData >::iterator it = s_componentsFactory.begin(); it != s_componentsFactory.end(); it++ )
            if( it->second.type == type )
                return it->second.builder();
        return 0;
    }

    Json::Value GameManager::loadJson( const std::string& path )
    {
        std::ifstream file( path.c_str(), std::ifstream::in | std::ifstream::binary );
        if( !file )
            return Json::Value::null;
        file.seekg( 0, std::ifstream::end );
        unsigned int fsize = file.tellg();
        file.seekg( 0, std::ifstream::beg );
        std::string content;
        content.resize( fsize + 1, 0 );
        file.read( (char*)content.c_str(), fsize );
        file.close();
        Json::Value root;
        Json::Reader reader;
        reader.parse( content, root );
        return root;
    }

    bool GameManager::saveJson( const std::string& path, const Json::Value& root )
    {
        std::ofstream file( path.c_str(), std::ifstream::out | std::ifstream::binary );
        if( !file )
            return false;
        Json::StyledWriter writer;
        std::string content = writer.write( root );
        file.write( content.c_str(), content.length() );
        file.close();
        return true;
    }

    void GameManager::jsonToScene( const Json::Value& root, SceneContentType contentFlags )
    {
        if( contentFlags == GameManager::None || !root.isObject() )
            return;
        Json::Value physics = root[ "physics" ];
        if( !physics.isNull() )
        {
            Json::Value gravity = physics[ "gravity" ];
            if( gravity.isArray() && gravity.size() == 2 )
                setWorldGravity( b2Vec2( (float)gravity[ 0u ].asDouble(), (float)gravity[ 1u ].asDouble() ) );
        }
        Json::Value assets = root[ "assets" ];
        if( contentFlags & GameManager::Assets && !assets.isNull() )
            Assets::use().jsonToAssets( assets );
        Json::Value prefabs = root[ "prefabs" ];
        if( contentFlags & GameManager::PrefabGameObjects && !prefabs.isNull() )
            jsonToGameObjects( prefabs, true );
        Json::Value scene = root[ "scene" ];
        if( contentFlags & GameManager::GameObjects && !scene.isNull() )
            jsonToGameObjects( scene, false );
    }

    void GameManager::jsonToGameObjects( const Json::Value& root, bool prefab )
    {
        if( !root.isArray() )
            return;
        Json::Value item;
        Json::Value itemPrefab;
        for( unsigned int i = 0; i < root.size(); i++ )
        {
            item = root[ i ];
            itemPrefab = item[ "prefab" ];
            if( !itemPrefab.isNull() )
            {
                GameObject* go = instantiatePrefab( itemPrefab.asString() );
                if( go )
                {
                    addGameObject( go, prefab );
                    go->fromJson( item );
                }
            }
            else
            {
                GameObject* go = xnew GameObject();
                addGameObject( go, prefab );
                go->fromJson( item );
            }
        }
    }

    Json::Value GameManager::sceneToJson( SceneContentType contentFlags )
    {
        Json::Value root;
        Json::Value physics;
        Json::Value physicsGravity;
        b2Vec2 grav = getWorldGravity();
        physicsGravity.append( Json::Value( grav.x ) );
        physicsGravity.append( Json::Value( grav.y ) );
        physics[ "gravity" ] = physicsGravity;
        root[ "physics" ] = physics;
        if( contentFlags & GameManager::Assets )
        {
            Json::Value assets = Assets::use().assetsToJson();
            if( !assets.isNull() )
                root[ "assets" ] = assets;
        }
        if( contentFlags & GameManager::PrefabGameObjects )
        {
            Json::Value prefabs = gameObjectsToJson( true );
            if( !prefabs.isNull() )
                root[ "prefabs" ] = prefabs;
        }
        if( contentFlags & GameManager::GameObjects )
        {
            Json::Value scene = gameObjectsToJson( false );
            if( !scene.isNull() )
                root[ "scene" ] = scene;
        }
        return root;
    }

    Json::Value GameManager::gameObjectsToJson( bool prefab )
    {
        std::list< GameObject* >& cgo = prefab ? m_prefabGameObjects : m_gameObjects;
        if( cgo.empty() )
            return Json::Value::null;
        Json::Value root;
        GameObject* go;
        Json::Value item;
        for( std::list< GameObject* >::iterator it = cgo.begin(); it != cgo.end(); it++ )
        {
            go = *it;
            item = go->toJson();
            if( !item.isNull() )
                root.append( item );
        }
        return root;
    }

    void GameManager::removeScene( SceneContentType contentFlags )
    {
        if( contentFlags & GameManager::Assets )
            Assets::use().freeAll();
        if( contentFlags & GameManager::PrefabGameObjects )
            removeAllGameObjects( false );
        if( contentFlags & GameManager::GameObjects )
            removeAllGameObjects( true );
    }

    void GameManager::addGameObject( GameObject* go, bool prefab )
    {
        if( !go || hasGameObject( go, prefab ) )
            return;
        std::list< GameObject* >& cgo = prefab ? m_prefabGameObjects : m_gameObjects;
        cgo.push_back( go );
        go->setGameManager( this );
        if( !prefab )
            go->onCreate();
    }

    void GameManager::removeGameObject( GameObject* go, bool prefab, bool del )
    {
        if( !hasGameObject( go, prefab ) )
            return;
        std::list< GameObject* >& cgo = prefab ? m_prefabGameObjects : m_gameObjects;
        cgo.remove( go );
        if( !prefab )
            go->onDestroy();
        go->setGameManager( 0 );
        if( del )
            DELETE_OBJECT( go );
    }

    void GameManager::removeGameObject( const std::string& id, bool prefab, bool del )
    {
        GameObject* go = getGameObject( id, prefab );
        if( !go )
            return;
        std::list< GameObject* >& cgo = prefab ? m_prefabGameObjects : m_gameObjects;
        cgo.remove( go );
        if( !prefab )
            go->onDestroy();
        go->setGameManager( 0 );
        if( del )
            DELETE_OBJECT( go );
    }

    void GameManager::removeAllGameObjects( bool prefab, bool del )
    {
        GameObject* go;
        std::list< GameObject* >& cgo = prefab ? m_prefabGameObjects : m_gameObjects;
        for( std::list< GameObject* >::iterator it = cgo.begin(); it != cgo.end(); it++ )
        {
            go = *it;
            if( !prefab )
                go->onDestroy();
            go->setGameManager( 0 );
            if( del )
                DELETE_OBJECT( *it );
        }
        cgo.clear();
    }

    bool GameManager::hasGameObject( GameObject* go, bool prefab )
    {
        std::list< GameObject* >& cgo = prefab ? m_prefabGameObjects : m_gameObjects;
        for( std::list< GameObject* >::iterator it = cgo.begin(); it != cgo.end(); it++ )
            if( *it == go )
                return true;
        return false;
    }

    bool GameManager::hasGameObject( const std::string& id, bool prefab )
    {
        std::list< GameObject* >& cgo = prefab ? m_prefabGameObjects : m_gameObjects;
        for( std::list< GameObject* >::iterator it = cgo.begin(); it != cgo.end(); it++ )
            if( (*it)->getId() == id )
                return true;
        return false;
    }

    GameObject* GameManager::getGameObject( const std::string& id, bool prefab )
    {
        std::list< GameObject* >& cgo = prefab ? m_prefabGameObjects : m_gameObjects;
        for( std::list< GameObject* >::iterator it = cgo.begin(); it != cgo.end(); it++ )
            if( (*it)->getId() == id )
                return *it;
        return 0;
    }

    std::list< GameObject* >::iterator GameManager::gameObjectAtBegin( bool prefab )
    {
        std::list< GameObject* >& cgo = prefab ? m_prefabGameObjects : m_gameObjects;
        return cgo.begin();
    }

    std::list< GameObject* >::iterator GameManager::gameObjectAtEnd( bool prefab )
    {
        std::list< GameObject* >& cgo = prefab ? m_prefabGameObjects : m_gameObjects;
        return cgo.end();
    }

    GameObject* GameManager::instantiatePrefab( const std::string& id )
    {
        GameObject* p = getGameObject( id, true );
        if( !p )
            return 0;
        GameObject* go = xnew GameObject();
        p->onDuplicate( go );
        return go;
    }

    void GameManager::processPhysics( float dt, int velIters, int posIters )
    {
        m_world->Step( dt, velIters, posIters );
    }

    void GameManager::processUpdate( float dt, bool sort )
    {
        if( sort )
            m_gameObjects.sort( CompareGameObjects() );
        GameObject* go;
        for( std::list< GameObject* >::iterator it = m_gameObjects.begin(); it != m_gameObjects.end(); it++ )
        {
            go = *it;
            if( go->isActive() )
                go->onUpdate( dt, sf::Transform::Identity, sort );
        }
    }

    void GameManager::processRender( sf::RenderTarget* target, const sf::Transform& trans )
    {
        if( !target )
            return;
        target->setView( target->getDefaultView() );
        GameObject* go;
        for( std::list< GameObject* >::iterator it = m_gameObjects.begin(); it != m_gameObjects.end(); it++ )
        {
            go = *it;
            if( go->isActive() )
                go->onRender( target );
        }
        target->setView( target->getDefaultView() );
    }

    void GameManager::processContact( bool beginOrEnd, GameObject* a, GameObject* b, b2Contact* contact )
    {
        if( a && b )
        {
            a->onCollide( b, beginOrEnd, contact );
            b->onCollide( a, beginOrEnd, contact );
        }
    }

    GameManager::SceneContentType operator|( GameManager::SceneContentType a, GameManager::SceneContentType b )
    {
        return (GameManager::SceneContentType)( (int)a | (int)b );
    };

}
