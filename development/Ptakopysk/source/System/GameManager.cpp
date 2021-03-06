#include "../../include/Ptakopysk/System/GameManager.h"
#include "../../include/Ptakopysk/System/Assets.h"
#include "../../include/Ptakopysk/System/Events.h"
#include "../../include/Ptakopysk/System/Tween.h"
#include "../../include/Ptakopysk/Components/Body.h"
#include "../../include/Ptakopysk/Components/Camera.h"
#include "../../include/Ptakopysk/Components/RevoluteJoint.h"
#include "../../include/Ptakopysk/Components/SpriteAtlas.h"
#include "../../include/Ptakopysk/Components/SpriteRenderer.h"
#include "../../include/Ptakopysk/Components/TextRenderer.h"
#include "../../include/Ptakopysk/Components/Transform.h"
#include "../../include/Ptakopysk/Serialization/b2BodyTypeSerializer.h"
#include "../../include/Ptakopysk/Serialization/b2FilterSerializer.h"
#include "../../include/Ptakopysk/Serialization/BlendModeSerializer.h"
#include "../../include/Ptakopysk/Serialization/StyleSerializer.h"
#include "../../include/Ptakopysk/Serialization/TransformModeSerializer.h"
#include <XeCore/Common/Logger.h>
#include <XeCore/Common/Concurrent/Thread.h>
#include <XeCore/Common/Timer.h>
#include <Box2D/Box2D.h>

namespace Ptakopysk
{

    class AssetsListener
        : public virtual XeCore::Common::IRtti
        , public virtual XeCore::Common::MemoryManager
        , public AssetsChangedListener
    {
        RTTI_CLASS_DECLARE( AssetsListener );

    public:
        AssetsListener( GameManager* owner );
        ~AssetsListener() {};

        void onTextureChanged( const std::string& id, const sf::Texture* asset, bool addedOrRemoved ) { if( m_owner ) m_owner->processTextureChanged( asset, addedOrRemoved ); };
        void onShaderChanged( const std::string& id, const sf::Shader* asset, bool addedOrRemoved ) { if( m_owner ) m_owner->processShaderChanged( asset, addedOrRemoved ); };
        void onSoundChanged( const std::string& id, const sf::Sound* asset, bool addedOrRemoved ) { if( m_owner ) m_owner->processSoundChanged( asset, addedOrRemoved ); };
        void onMusicChanged( const std::string& id, const sf::Music* asset, bool addedOrRemoved ) { if( m_owner ) m_owner->processMusicChanged( asset, addedOrRemoved ); };
        void onFontChanged( const std::string& id, const sf::Font* asset, bool addedOrRemoved ) { if( m_owner ) m_owner->processFontChanged( asset, addedOrRemoved ); };
        void onCustomAssetChanged( const std::string& id, const ICustomAsset* asset, bool addedOrRemoved ) { if( m_owner ) m_owner->processCustomAssetChanged( asset, addedOrRemoved ); };

    private:
        GameManager* m_owner;
    };

    RTTI_CLASS_DERIVATIONS( AssetsListener,
                            RTTI_DERIVATIONS_END
                            );

    AssetsListener::AssetsListener( GameManager* owner )
    : RTTI_CLASS_DEFINE( AssetsListener )
    , m_owner( owner )
    {
    }

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
        if( m_owner )
            m_owner->processJointGoodbye( (GameObject*)joint->GetUserData(), joint );
    }

    void DestructionListener::SayGoodbye( b2Fixture* fixture )
    {
        if( m_owner )
            m_owner->processFixtureGoodbye( (GameObject*)fixture->GetUserData(), fixture );
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
    bool GameManager::s_editMode = false;

    GameManager::GameManager( const Json::Value& config )
    : RTTI_CLASS_DEFINE( GameManager )
    , PhysicsGravity( this, &GameManager::getWorldGravity, &GameManager::setWorldGravity )
    , RenderWindow( this, &GameManager::getRenderWindow, &GameManager::setRenderWindow )
    , m_renderWindow( 0 )
    , m_bgColor( sf::Color::Black )
    , m_fixedStep( 0.0f )
    {
        setupFromConfig( config );
        m_world = xnew b2World( b2Vec2( 0.0f, 0.0f ) );
        m_destructionListener = xnew DestructionListener( this );
        m_world->SetDestructionListener( m_destructionListener );
        m_contactListener = xnew ContactListener( this );
        m_world->SetContactListener( m_contactListener );
        m_assetsListener = xnew AssetsListener( this );
        Assets::use().setAssetsChangedListener( m_assetsListener );
    }

    GameManager::~GameManager()
    {
        Assets::use().setAssetsChangedListener( 0 );
        DELETE_OBJECT( m_assetsListener );
        removeScene();
        processRemoving();
        GameObject* go;
        for( GameObject::List::iterator it = m_gameObjectsToCreate.begin(); it != m_gameObjectsToCreate.end(); it++ )
        {
            go = *it;
            DELETE_OBJECT( go );
        }
        m_gameObjectsToCreate.clear();
        DELETE_OBJECT( m_world );
        DELETE_OBJECT( m_destructionListener );
        DELETE_OBJECT( m_contactListener );
        DELETE_OBJECT( m_renderWindow );
    }

    void GameManager::initialize()
    {
        Serialized::registerCustomSerializer( "b2BodyType", xnew b2BodyTypeSerializer() );
        Serialized::registerCustomSerializer( "b2Filter", xnew b2FilterSerializer() );
        Serialized::registerCustomSerializer( "BlendMode", xnew BlendModeSerializer() );
        Serialized::registerCustomSerializer( "Style", xnew StyleSerializer() );
        Serialized::registerCustomSerializer( "Transform::ModeType", xnew TransformModeSerializer() );
        registerComponentFactory( "Body", RTTI_CLASS_TYPE( Body ), Body::onBuildComponent );
        registerComponentFactory( "Camera", RTTI_CLASS_TYPE( Camera ), Camera::onBuildComponent );
        registerComponentFactory( "RevoluteJoint", RTTI_CLASS_TYPE( RevoluteJoint ), RevoluteJoint::onBuildComponent );
        registerComponentFactory( "SpriteAtlas", RTTI_CLASS_TYPE( SpriteAtlas ), SpriteAtlas::onBuildComponent );
        registerComponentFactory( "SpriteRenderer", RTTI_CLASS_TYPE( SpriteRenderer ), SpriteRenderer::onBuildComponent );
        registerComponentFactory( "TextRenderer", RTTI_CLASS_TYPE( TextRenderer ), TextRenderer::onBuildComponent );
        registerComponentFactory( "Transform", RTTI_CLASS_TYPE( Transform ), Transform::onBuildComponent );
    }

    void GameManager::cleanup()
    {
        Serialized::unregisterAllCustomSerializers();
        unregisterAllComponentFactories();
    }

    bool GameManager::registerComponentFactory( const std::string& id, XeCore::Common::IRtti::Derivation type, Component::OnBuildComponentCallback builder )
    {
        if( id.empty() || s_componentsFactory.count( id ) || !type || !builder )
            return false;
        ComponentFactoryData d;
        d.type = type;
        d.builder = builder;
        s_componentsFactory[ id ] = d;
        return true;
    }

    bool GameManager::unregisterComponentFactory( const std::string& id )
    {
        if( s_componentsFactory.count( id ) )
        {
            s_componentsFactory.erase( id );
            return true;
        }
        return false;
    }

    bool GameManager::unregisterComponentFactory( XeCore::Common::IRtti::Derivation type )
    {
        for( std::map< std::string, ComponentFactoryData >::iterator it = s_componentsFactory.begin(); it != s_componentsFactory.end(); it++ )
        {
            if( it->second.type == type )
            {
                s_componentsFactory.erase( it );
                return true;
            }
        }
        return false;
    }

    bool GameManager::unregisterComponentFactory( Component::OnBuildComponentCallback builder )
    {
        for( std::map< std::string, ComponentFactoryData >::iterator it = s_componentsFactory.begin(); it != s_componentsFactory.end(); it++ )
        {
            if( it->second.builder == builder )
            {
                s_componentsFactory.erase( it );
                return true;
            }
        }
        return false;
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

    unsigned int GameManager::getComponentsIds( std::vector< std::string >& result )
    {
        result.clear();
        for( std::map< std::string, ComponentFactoryData >::iterator it = s_componentsFactory.begin(); it != s_componentsFactory.end(); it++ )
            result.push_back( it->first );
        return result.size();
    }

    unsigned int GameManager::getComponentsTypes( std::vector< XeCore::Common::IRtti::Derivation >& result )
    {
        result.clear();
        for( std::map< std::string, ComponentFactoryData >::iterator it = s_componentsFactory.begin(); it != s_componentsFactory.end(); it++ )
            result.push_back( it->second.type );
        return result.size();
    }

    unsigned int GameManager::getComponentsBuilders( std::vector< Component::OnBuildComponentCallback >& result )
    {
        result.clear();
        for( std::map< std::string, ComponentFactoryData >::iterator it = s_componentsFactory.begin(); it != s_componentsFactory.end(); it++ )
            result.push_back( it->second.builder );
        return result.size();
    }

    std::string GameManager::sceneAt( unsigned int index )
    {
        if( index >= m_scenes.size() )
            return "";
        unsigned int i = 0;
        for( ScenesList::iterator it = m_scenes.begin(); it != m_scenes.end(); it++, i++ )
            if( i == index )
                return it->second;
        return "";
    }

    void GameManager::jsonToScene( const Json::Value& root, SceneContentType contentFlags )
    {
        if( contentFlags == GameManager::None || !root.isObject() )
            return;
        Json::Value physics = root[ "physics" ];
        if( contentFlags & GameManager::PhysicsSettings && !physics.isNull() )
        {
            Json::Value gravity = physics[ "gravity" ];
            if( gravity.isArray() && gravity.size() == 2 )
                setWorldGravity( b2Vec2( (float)gravity[ 0u ].asDouble(), (float)gravity[ 1u ].asDouble() ) );
            Json::Value filters = physics[ "filters" ];
            if( filters.isObject() && filters.size() > 0 )
            {
                Json::Value::Members m = filters.getMemberNames();
                Json::Value item;
                std::string name;
                for( Json::Value::Members::iterator it = m.begin(); it != m.end(); it++ )
                {
                    name = *it;
                    if( m_filters.count( name ) )
                        continue;
                    item = filters[ name ];
                    if( item.isObject() )
                        m_filters[ name ] = Serialized::deserializeCustom< b2Filter >( "b2Filter", item );
                }
            }
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

    Json::Value GameManager::sceneToJson( SceneContentType contentFlags, bool omitDefaultValues )
    {
        Json::Value root;
        if( contentFlags & GameManager::PhysicsSettings )
        {
            Json::Value physics;
            Json::Value physicsGravity;
            b2Vec2 grav = getWorldGravity();
            physicsGravity.append( Json::Value( grav.x ) );
            physicsGravity.append( Json::Value( grav.y ) );
            physics[ "gravity" ] = physicsGravity;
            Json::Value physicsFilters;
            for( FiltersMap::iterator it = m_filters.begin(); it != m_filters.end(); it++ )
                physicsFilters[ it->first ] = Serialized::serializeCustom< b2Filter >( "b2Filter", it->second );
            if( physicsFilters.isObject() )
                physics[ "filters" ] = physicsFilters;
            root[ "physics" ] = physics;
        }
        if( contentFlags & GameManager::Assets )
        {
            Json::Value assets = Assets::use().assetsToJson();
            if( !assets.isNull() )
                root[ "assets" ] = assets;
        }
        if( contentFlags & GameManager::PrefabGameObjects )
        {
            Json::Value prefabs = gameObjectsToJson( true, omitDefaultValues );
            if( !prefabs.isNull() )
                root[ "prefabs" ] = prefabs;
        }
        if( contentFlags & GameManager::GameObjects )
        {
            Json::Value scene = gameObjectsToJson( false, omitDefaultValues );
            if( !scene.isNull() )
                root[ "scene" ] = scene;
        }
        return root;
    }

    Json::Value GameManager::gameObjectsToJson( bool prefab, bool omitDefaultValues )
    {
        GameObject::List& cgo = prefab ? m_prefabGameObjects : m_gameObjects;
        if( cgo.empty() )
            return Json::Value::null;
        Json::Value root;
        GameObject* go;
        Json::Value item;
        for( GameObject::List::iterator it = cgo.begin(); it != cgo.end(); it++ )
        {
            go = *it;
            item = go->toJson( omitDefaultValues );
            if( !item.isNull() )
                root.append( item );
        }
        return root;
    }

    void GameManager::removeScene( SceneContentType contentFlags )
    {
        if( contentFlags & GameManager::PrefabGameObjects )
            removeAllGameObjects( true );
        if( contentFlags & GameManager::GameObjects )
        {
            processAdding();
            removeAllGameObjects( false );
            processRemoving();
        }
        if( contentFlags & GameManager::PhysicsSettings )
            m_filters.clear();
        if( contentFlags & GameManager::Assets )
            Assets::use().freeAll();
    }

    void GameManager::addGameObject( GameObject* go, bool prefab )
    {
        if( !go || go->getType() != RTTI_CLASS_TYPE( GameObject ) || hasGameObject( go ) || isWaitingToAdd( go ) )
            return;
        if( prefab )
        {
            go->setGameManager( this );
            m_prefabGameObjects.push_back( go );
            go->setPrefab( prefab );
            go->setDestroying( false );
            go->processAdding();
        }
        else
        {
            m_gameObjectsToCreate.push_back( go );
            go->setGameManager( this );
            go->setPrefab( prefab );
            go->setDestroying( false );
        }
    }

    void GameManager::removeGameObject( GameObject* go, bool prefab )
    {
        if( !hasGameObject( go, prefab ) )
            return;
        if( prefab )
        {
            m_prefabGameObjects.remove( go );
            go->processRemoving();
            go->setGameManager( 0 );
            go->setPrefab( false );
            DELETE_OBJECT( go );
        }
        else
        {
            m_gameObjectsToDestroy.push_back( go );
            go->setDestroying( true );
            go->onDestroy();
        }
    }

    void GameManager::removeGameObject( const std::string& id, bool prefab )
    {
        GameObject* go = getGameObject( id, prefab );
        if( !go )
            return;
        if( prefab )
        {
            m_prefabGameObjects.remove( go );
            go->processRemoving();
            go->setGameManager( 0 );
            go->setPrefab( false );
            DELETE_OBJECT( go );
        }
        else
        {
            m_gameObjectsToDestroy.push_back( go );
            go->setDestroying( true );
            go->onDestroy();
        }
    }

    void GameManager::removeAllGameObjects( bool prefab )
    {
        GameObject* go;
        if( prefab )
        {
            for( GameObject::List::iterator it = m_prefabGameObjects.begin(); it != m_prefabGameObjects.end(); it++ )
            {
                go = *it;
                go->processRemoving();
                go->setGameManager( 0 );
                go->setPrefab( false );
                DELETE_OBJECT( go );
            }
            m_prefabGameObjects.clear();
        }
        else
        {
            for( GameObject::List::iterator it = m_gameObjects.begin(); it != m_gameObjects.end(); it++ )
            {
                go = *it;
                m_gameObjectsToDestroy.push_back( go );
                go->setDestroying( true );
                go->onDestroy();
            }
        }
    }

    bool GameManager::hasGameObject( GameObject* go, bool prefab )
    {
        GameObject::List& cgo = prefab ? m_prefabGameObjects : m_gameObjects;
        for( GameObject::List::iterator it = cgo.begin(); it != cgo.end(); it++ )
            if( *it == go )
                return true;
        return false;
    }

    bool GameManager::hasGameObject( const std::string& id, bool prefab )
    {
        GameObject::List& cgo = prefab ? m_prefabGameObjects : m_gameObjects;
        for( GameObject::List::iterator it = cgo.begin(); it != cgo.end(); it++ )
            if( (*it)->getId() == id )
                return true;
        return false;
    }

    bool GameManager::containsGameObject( GameObject* go, bool prefab )
    {
        GameObject::List& cgo = prefab ? m_prefabGameObjects : m_gameObjects;
        for( GameObject::List::iterator it = cgo.begin(); it != cgo.end(); it++ )
            if( *it == go || (*it)->containsGameObject( go ) )
                return true;
        return false;
    }

    GameObject* GameManager::getGameObject( const std::string& id, bool prefab )
    {
        GameObject::List& cgo = prefab ? m_prefabGameObjects : m_gameObjects;
        for( GameObject::List::iterator it = cgo.begin(); it != cgo.end(); it++ )
            if( (*it)->getId() == id )
                return *it;
        return 0;
    }

    GameObject* GameManager::findGameObject( const std::string& path )
    {
        unsigned int from = 0;
        unsigned int p = path.find( '/', from );
        while( p - from < 1 && p != std::string::npos )
        {
            from++;
            p = path.find( '/', from );
        }
        std::string part = std::string( path, from, p );
        if( part.empty() )
            return 0;
        else if( part == "." )
        {
            if( p == std::string::npos )
                return 0;
            else
                return findGameObject( std::string( path, p + 1, std::string::npos ) );
        }
        else if( part == ".." )
            return 0;
        else
        {
            GameObject* go = getGameObject( part );
            if( go )
            {
                if( p == std::string::npos )
                    return go;
                else
                    return go->findGameObject( std::string( path, p + 1, std::string::npos ) );
            }
            else
                return 0;
        }
    }

    GameObject::List::iterator GameManager::gameObjectAtBegin( bool prefab )
    {
        GameObject::List& cgo = prefab ? m_prefabGameObjects : m_gameObjects;
        return cgo.begin();
    }

    GameObject::List::reverse_iterator GameManager::gameObjectAtReversedBegin( bool prefab )
    {
        GameObject::List& cgo = prefab ? m_prefabGameObjects : m_gameObjects;
        return cgo.rbegin();
    }

    GameObject::List::iterator GameManager::gameObjectAtEnd( bool prefab )
    {
        GameObject::List& cgo = prefab ? m_prefabGameObjects : m_gameObjects;
        return cgo.end();
    }

    GameObject::List::reverse_iterator GameManager::gameObjectAtReversedEnd( bool prefab )
    {
        GameObject::List& cgo = prefab ? m_prefabGameObjects : m_gameObjects;
        return cgo.rend();
    }

    GameObject* GameManager::gameObjectAt( unsigned int index )
    {
        if( index >= m_gameObjects.size() )
            return 0;
        unsigned int i = 0;
        for( GameObject::List::iterator it = m_gameObjects.begin(); it != m_gameObjects.end(); it++, i++ )
            if( i == index )
                return *it;
        return 0;
    }

    GameObject* GameManager::instantiatePrefab( const std::string& id )
    {
        GameObject* p = getGameObject( id, true );
        if( !p )
            return 0;
        GameObject* go = xnew GameObject();
        go->setInstanceOf( p );
        p->onDuplicate( go );
        return go;
    }

    void GameManager::processLifeCycle()
    {
        if( !m_renderWindow )
            return;
        XeCore::Common::Timer timer;
        timer.start();
        float fixedStepAccum = 0.0f;
        while( m_renderWindow->isOpen() )
        {
            processRunningScene();
            sf::Event event;
            while( m_renderWindow->pollEvent( event ) )
            {
                processEvents( event );
                if( event.type == sf::Event::Closed )
                    m_renderWindow->close();
            }
            timer.update();
            float dt = timer.deltaSeconds();
            fixedStepAccum += dt;
            bool processStep = true;
            float stepDt = dt;
            if( m_fixedStep > 0.0f )
            {
                processStep = false;
                while( fixedStepAccum > m_fixedStep )
                {
                    processStep = true;
                    stepDt = m_fixedStep;
                    fixedStepAccum -= m_fixedStep;
                }
            }

            Events::use().dispatch();
            if( processStep )
            {
                Tweener::use().processTweens( stepDt );
                processPhysics( stepDt );
                processUpdate( stepDt );
            }
            m_renderWindow->clear( m_bgColor );
            processRender( m_renderWindow );
            m_renderWindow->display();
            if( stepDt > 0.0f )
                XeCore::Common::Concurrent::Thread::sleep( (unsigned int)( 1000.0f * stepDt ) );
        }
        timer.stop();
    }

    void GameManager::processRunningScene()
    {
        if( m_sceneToRun.empty() )
            return;
        removeScene( All );
        Json::Value scene = Assets::loadJson( m_sceneToRun );
        jsonToScene( scene );
        m_sceneToRun.clear();
    }

    void GameManager::processEvents( const sf::Event& event )
    {
        for( GameObject::List::iterator it = m_gameObjects.begin(); it != m_gameObjects.end(); it++ )
            (*it)->onEvent( event );
    }

    void GameManager::processPhysics( float dt, int velIters, int posIters )
    {
        m_world->Step( dt, velIters, posIters );
    }

    void GameManager::processUpdate( float dt, bool sort )
    {
        processAdding();
        processRemoving();
        if( sort )
            m_gameObjects.sort( CompareGameObjects() );
        for( GameObject::List::iterator it = m_gameObjects.begin(); it != m_gameObjects.end(); it++ )
            (*it)->onUpdate( dt, sf::Transform::Identity, sort );
    }

    void GameManager::processRender( sf::RenderTarget* target )
    {
        if( !target )
            target = m_renderWindow;
        if( !target )
        {
            XWARNING( "Cannot process render without target!" );
            return;
        }
        Camera::s_mainRT = target;
        target->setView( target->getDefaultView() );
        sf::RenderTarget*& currentTarget = target;
        for( GameObject::List::iterator it = m_gameObjects.begin(); it != m_gameObjects.end(); it++ )
            (*it)->onRender( currentTarget );
        target->setView( target->getDefaultView() );
        if( Camera::s_currentRT )
            Camera::s_currentRT->display();
        Camera::s_mainRT = 0;
    }

    void GameManager::processRenderEditor( sf::View& view, sf::RenderTarget* target )
    {
        if( !target )
            target = m_renderWindow;
        if( !target )
        {
            XWARNING( "Cannot process render without target!" );
            return;
        }
        target->setView( view );
        for( GameObject::List::iterator it = m_gameObjects.begin(); it != m_gameObjects.end(); it++ )
            (*it)->onRenderEditor( target );
        target->setView( view );
    }

    void GameManager::processAdding()
    {
        GameObject* go;
        for( GameObject::List::iterator it = m_gameObjectsToCreate.begin(); it != m_gameObjectsToCreate.end(); it++ )
        {
            go = *it;
            m_gameObjects.push_back( go );
            go->onCreate();
            go->processAdding();
        }
        m_gameObjectsToCreate.clear();
    }

    void GameManager::processRemoving()
    {
        GameObject* go;
        for( GameObject::List::iterator it = m_gameObjectsToDestroy.begin(); it != m_gameObjectsToDestroy.end(); it++ )
        {
            go = *it;
            if( std::find( m_gameObjects.begin(), m_gameObjects.end(), go ) == m_gameObjects.end() )
                continue;
            go->processRemoving();
            m_gameObjects.remove( go );
            go->setGameManager( 0 );
            go->setPrefab( false );
            DELETE_OBJECT( go );
        }
        m_gameObjectsToDestroy.clear();
    }

    bool GameManager::isWaitingToAdd( GameObject* go )
    {
        for( GameObject::List::iterator it = m_gameObjectsToCreate.begin(); it != m_gameObjectsToCreate.end(); it++ )
            if( *it == go )
                return true;
        return false;
    }

    bool GameManager::isWaitingToRemove( GameObject* go )
    {
        for( GameObject::List::iterator it = m_gameObjectsToDestroy.begin(); it != m_gameObjectsToDestroy.end(); it++ )
            if( *it == go )
                return true;
        return false;
    }

    void GameManager::setupFromConfig( const Json::Value& config )
    {
        if( !config.isNull() && config.isObject() )
        {
            if( config.isMember( "window" ) )
            {
                Json::Value window = config[ "window" ];
                m_bgColor = sf::Color::White;
                sf::VideoMode vm = sf::VideoMode::getDesktopMode();
                std::string nm;
                sf::Uint32 st = sf::Style::Default;
                if( window.isMember( "color" ) )
                {
                    Json::Value color = window[ "color" ];
                    if( color.isArray() && color.size() == 4 )
                        m_bgColor = sf::Color(
                            (sf::Uint8)color[ 0u ].asUInt(),
                            (sf::Uint8)color[ 1u ].asUInt(),
                            (sf::Uint8)color[ 2u ].asUInt(),
                            (sf::Uint8)color[ 3u ].asUInt()
                        );
                }
                if( window.isMember( "videoMode" ) )
                {
                    Json::Value videoMode = window[ "videoMode" ];
                    if( videoMode.isObject() )
                    {
                        if( videoMode.isMember( "width" ) )
                        {
                            Json::Value width = videoMode[ "width" ];
                            if( width.isNumeric() )
                                vm.width = width.asUInt();
                        }
                        if( videoMode.isMember( "height" ) )
                        {
                            Json::Value height = videoMode[ "height" ];
                            if( height.isNumeric() )
                                vm.height = height.asUInt();
                        }
                        if( videoMode.isMember( "bitsPerPixel" ) )
                        {
                            Json::Value bitsPerPixel = videoMode[ "bitsPerPixel" ];
                            if( bitsPerPixel.isNumeric() )
                                vm.bitsPerPixel = bitsPerPixel.asUInt();
                        }
                    }
                    else if( videoMode.isString() && videoMode.asString() == "fullscreen" )
                        st = sf::Style::Fullscreen;
                }
                if( window.isMember( "name" ) )
                {
                    Json::Value name = window[ "name" ];
                    if( name.isString() )
                        nm = name.asString();
                }
                if( window.isMember( "style" ) )
                {
                    Json::Value style = window[ "style" ];
                    if( style.isArray() )
                    {
                        st = sf::Style::None;
                        Json::Value item;
                        for( unsigned int i = 0; i < style.size(); i++ )
                        {
                            item = style[ i ];
                            if( item.isString() )
                            {
                                if( item.asString() == "Titlebar" )
                                    st |= sf::Style::Titlebar;
                                else if( item.asString() == "Resize" )
                                    st |= sf::Style::Resize;
                                else if( item.asString() == "Close" )
                                    st |= sf::Style::Close;
                                else if( item.asString() == "Fullscreen" )
                                    st = sf::Style::Fullscreen;
                            }
                        }
                    }
                }
                if( st == sf::Style::Fullscreen && !vm.isValid() )
                    vm = sf::VideoMode::getDesktopMode();
                m_renderWindow = xnew sf::RenderWindow( vm, nm, st );
            }
            if( config.isMember( "lifeCycle" ) )
            {
                Json::Value lifeCycle = config[ "lifeCycle" ];
                if( lifeCycle.isObject() )
                {
                    if( lifeCycle.isMember( "fixedFps" ) )
                    {
                        Json::Value fixedFps = lifeCycle[ "fixedFps" ];
                        if( fixedFps.isNumeric() )
                        {
                            float v = (float)fixedFps.asDouble();
                            m_fixedStep = v > 0.0f ? 1.0f / v : 0.0f;
                        }
                    }
                    if( lifeCycle.isMember( "fixedStep" ) )
                    {
                        Json::Value fixedStep = lifeCycle[ "fixedStep" ];
                        if( fixedStep.isNumeric() )
                            m_fixedStep = (float)fixedStep.asDouble();
                    }
                }
            }
            if( config.isMember( "scenes" ) )
            {
                Json::Value scenes = config[ "scenes" ];
                if( scenes.isObject() )
                {
                    Json::Value::Members m = scenes.getMemberNames();
                    Json::Value item;
                    for( Json::Value::Members::iterator it = m.begin(); it != m.end(); it++ )
                    {
                        item = scenes[ *it ];
                        if( item.isString() )
                            m_scenes[ *it ] = item.asString();
                    }
                }
            }
        }
    }

    void GameManager::processContact( bool beginOrEnd, GameObject* a, GameObject* b, b2Contact* contact )
    {
        if( a && b )
        {
            a->onCollide( b, beginOrEnd, contact );
            b->onCollide( a, beginOrEnd, contact );
        }
    }

    void GameManager::processJointGoodbye( GameObject* o, b2Joint* joint )
    {
        if( o )
            o->onJointGoodbye( joint );
    }

    void GameManager::processFixtureGoodbye( GameObject* o, b2Fixture* fixture )
    {
        if( o )
            o->onFixtureGoodbye( fixture );
    }

    void GameManager::processTextureChanged( const sf::Texture* p, bool addedOrRemoved )
    {
        for( GameObject::List::iterator it = m_gameObjects.begin(); it != m_gameObjects.end(); it++ )
            (*it)->onTextureChanged( p, addedOrRemoved );
    }

    void GameManager::processShaderChanged( const sf::Shader* p, bool addedOrRemoved )
    {
        for( GameObject::List::iterator it = m_gameObjects.begin(); it != m_gameObjects.end(); it++ )
            (*it)->onShaderChanged( p, addedOrRemoved );
    }

    void GameManager::processSoundChanged( const sf::Sound* p, bool addedOrRemoved )
    {
        for( GameObject::List::iterator it = m_gameObjects.begin(); it != m_gameObjects.end(); it++ )
            (*it)->onSoundChanged( p, addedOrRemoved );
    }

    void GameManager::processMusicChanged( const sf::Music* p, bool addedOrRemoved )
    {
        for( GameObject::List::iterator it = m_gameObjects.begin(); it != m_gameObjects.end(); it++ )
            (*it)->onMusicChanged( p, addedOrRemoved );
    }

    void GameManager::processFontChanged( const sf::Font* p, bool addedOrRemoved )
    {
        for( GameObject::List::iterator it = m_gameObjects.begin(); it != m_gameObjects.end(); it++ )
            (*it)->onFontChanged( p, addedOrRemoved );
    }

    void GameManager::processCustomAssetChanged( const ICustomAsset* p, bool addedOrRemoved )
    {
        for( GameObject::List::iterator it = m_gameObjects.begin(); it != m_gameObjects.end(); it++ )
            (*it)->onCustomAssetChanged( p, addedOrRemoved );
    }

    GameManager::SceneContentType operator|( GameManager::SceneContentType a, GameManager::SceneContentType b )
    {
        return (GameManager::SceneContentType)( (int)a | (int)b );
    };

}
