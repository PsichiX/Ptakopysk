#ifndef __PTAKOPYSK__GAME_MANAGER__
#define __PTAKOPYSK__GAME_MANAGER__

#include <XeCore/Common/Property.h>
#include <XeCore/Common/IRtti.h>
#include <XeCore/Common/MemoryManager.h>
#include <json/json.h>
#include <list>
#include <string>
#include <Box2D/Box2D.h>
#include <SFML/Graphics/RenderWindow.hpp>
#include "GameObject.h"
#include "../Components/Component.h"

namespace Ptakopysk
{

    class AssetsListener;
    class DestructionListener;
    class ContactListener;

    class GameManager
        : public virtual XeCore::Common::IRtti
        , public virtual XeCore::Common::MemoryManager::Manageable
    {
        friend class AssetsListener;
        friend class DestructionListener;
        friend class ContactListener;

        RTTI_CLASS_DECLARE( GameManager );

    public:
        struct CompareGameObjects
        {
            FORCEINLINE bool operator() ( GameObject* a, GameObject* b ) { return a->getOrder() > b->getOrder(); };
        };

        enum SceneContentType
        {
            None = 0,
            Assets = 1 << 0,
            PrefabGameObjects = 1 << 1,
            GameObjects = 1 << 2,
            PhysicsSettings = 1 << 3,
            All = -1
        };

        typedef std::map< std::string, b2Filter > FiltersMap;
        typedef std::map< std::string, std::string > ScenesList;

        static const int DEFAULT_VEL_ITERS = 8;
        static const int DEFAULT_POS_ITERS = 3;

        GameManager( const Json::Value& config = Json::Value::null );
        ~GameManager();

        static void initialize();
        static void cleanup();
        FORCEINLINE static bool isEditMode() { return s_editMode; };
        FORCEINLINE static void setEditMode( bool mode ) { s_editMode = mode; };
        static bool registerComponentFactory( const std::string& id, XeCore::Common::IRtti::Derivation type, Component::OnBuildComponentCallback builder );
        static bool unregisterComponentFactory( const std::string& id );
        static bool unregisterComponentFactory( XeCore::Common::IRtti::Derivation type );
        static bool unregisterComponentFactory( Component::OnBuildComponentCallback factory );
        static void unregisterAllComponentFactories();
        static XeCore::Common::IRtti::Derivation findComponentFactoryTypeById( const std::string& id );
        static XeCore::Common::IRtti::Derivation findComponentFactoryTypeByBuilder( Component::OnBuildComponentCallback builder );
        static std::string findComponentFactoryIdByType( XeCore::Common::IRtti::Derivation type );
        static std::string findComponentFactoryIdByBuilder( Component::OnBuildComponentCallback builder );
        static Component::OnBuildComponentCallback findComponentFactoryBuilderById( const std::string& id );
        static Component::OnBuildComponentCallback findComponentFactoryBuilderByType( XeCore::Common::IRtti::Derivation type );
        static Component* buildComponent( const std::string& id );
        static Component* buildComponent( XeCore::Common::IRtti::Derivation type );
        static unsigned int getComponentsIds( std::vector< std::string >& result );
        static unsigned int getComponentsTypes( std::vector< XeCore::Common::IRtti::Derivation >& result );
        static unsigned int getComponentsBuilders( std::vector< Component::OnBuildComponentCallback >& result );

        FORCEINLINE b2World* getPhysicsWorld() { return m_world; };

        FORCEINLINE bool addScene( const std::string& id, const std::string& path ) { if( !m_scenes.count( id ) ) { m_scenes[ id ] = path; return true; } else return false; };
        FORCEINLINE bool removeScene( const std::string& id ) { if( m_scenes.count( id ) ) { m_scenes.erase( id ); return true; } else return false; };
        FORCEINLINE bool hasScene( const std::string& id ) { return m_scenes.count( id ); };
        FORCEINLINE unsigned int scenesCount() { return m_scenes.size(); };
        FORCEINLINE ScenesList::iterator sceneAtBegin() { return m_scenes.begin(); };
        FORCEINLINE ScenesList::reverse_iterator sceneAtReverseBegin() { return m_scenes.rbegin(); };
        FORCEINLINE ScenesList::iterator sceneAtEnd() { return m_scenes.end(); };
        FORCEINLINE ScenesList::reverse_iterator sceneAtReverseEnd() { return m_scenes.rend(); };
        std::string sceneAt( unsigned int index );
        FORCEINLINE bool runScene( const std::string& id ) { if( m_scenes.count( id ) ) { m_sceneToRun = m_scenes[ id ]; return true; } else return false; };
        void jsonToScene( const Json::Value& root, SceneContentType contentFlags = All );
        void jsonToGameObjects( const Json::Value& root, bool prefab = false );
        Json::Value sceneToJson( SceneContentType contentFlags = All, bool omitDefaultValues = false );
        Json::Value gameObjectsToJson( bool prefab = false, bool omitDefaultValues = false );
        void removeScene( SceneContentType contentFlags = All );

        void addGameObject( GameObject* go, bool prefab = false );
        void removeGameObject( GameObject* go, bool prefab = false );
        void removeGameObject( const std::string& id, bool prefab = false );
        void removeAllGameObjects( bool prefab = false );
        bool hasGameObject( GameObject* go, bool prefab = false );
        bool hasGameObject( const std::string& id, bool prefab = false );
        bool containsGameObject( GameObject* go, bool prefab = false );
        GameObject* getGameObject( const std::string& id, bool prefab = false );
        GameObject* findGameObject( const std::string& path );
        FORCEINLINE unsigned int gameObjectsCount( bool prefab = false ) { return prefab ? m_prefabGameObjects.size() : m_gameObjects.size(); };
        GameObject::List::iterator gameObjectAtBegin( bool prefab = false );
        GameObject::List::reverse_iterator gameObjectAtReversedBegin( bool prefab = false );
        GameObject::List::iterator gameObjectAtEnd( bool prefab = false );
        GameObject::List::reverse_iterator gameObjectAtReversedEnd( bool prefab = false );
        GameObject* gameObjectAt( unsigned int index );
        GameObject* instantiatePrefab( const std::string& id );

        FORCEINLINE b2Vec2 getWorldGravity() { return m_world->GetGravity(); };
        FORCEINLINE void setWorldGravity( b2Vec2 v ) { m_world->SetGravity( v ); };
        FORCEINLINE sf::RenderWindow* getRenderWindow() { return m_renderWindow; };
        FORCEINLINE void setRenderWindow( sf::RenderWindow* v ) { m_renderWindow = v; };
        FORCEINLINE FiltersMap& accessFilters() { return m_filters; }

        void processLifeCycle();
        void processRunningScene();
        void processEvents( const sf::Event& event );
        void processPhysics( float dt, int velIters = DEFAULT_VEL_ITERS, int posIters = DEFAULT_POS_ITERS );
        void processUpdate( float dt, bool sort = true );
        void processRender( sf::RenderTarget* target = 0 );
        void processRenderEditor( sf::View& view, sf::RenderTarget* target = 0 );
        void processAdding();
        void processRemoving();
        bool isWaitingToAdd( GameObject* go );
        bool isWaitingToRemove( GameObject* go );
        FORCEINLINE float getFixedStep() { return m_fixedStep; };
        FORCEINLINE void setFixedStep( float v ) { m_fixedStep = v; };

        XeCore::Common::Property< b2Vec2, GameManager > PhysicsGravity;
        XeCore::Common::Property< sf::RenderWindow*, GameManager > RenderWindow;

    private:
        struct ComponentFactoryData
        {
            XeCore::Common::IRtti::Derivation type;
            Component::OnBuildComponentCallback builder;
        };

        void setupFromConfig( const Json::Value& config );
        void processContact( bool beginOrEnd, GameObject* a, GameObject* b, b2Contact* contact );
        void processJointGoodbye( GameObject* o, b2Joint* joint );
        void processFixtureGoodbye( GameObject* o, b2Fixture* fixture );
        void processTextureChanged( const sf::Texture* p, bool addedOrRemoved );
        void processShaderChanged( const sf::Shader* p, bool addedOrRemoved );
        void processSoundChanged( const sf::Sound* p, bool addedOrRemoved );
        void processMusicChanged( const sf::Music* p, bool addedOrRemoved );
        void processFontChanged( const sf::Font* p, bool addedOrRemoved );
        void processCustomAssetChanged( const ICustomAsset* p, bool addedOrRemoved );

        static std::map< std::string, ComponentFactoryData > s_componentsFactory;
        static bool s_editMode;

        b2World* m_world;
        sf::RenderWindow* m_renderWindow;
        AssetsListener* m_assetsListener;
        DestructionListener* m_destructionListener;
        ContactListener* m_contactListener;
        GameObject::List m_prefabGameObjects;
        GameObject::List m_gameObjects;
        GameObject::List m_gameObjectsToCreate;
        GameObject::List m_gameObjectsToDestroy;
        FiltersMap m_filters;
        sf::Color m_bgColor;
        float m_fixedStep;
        ScenesList m_scenes;
        std::string m_sceneToRun;
    };

    GameManager::SceneContentType operator|( GameManager::SceneContentType a, GameManager::SceneContentType b );

}

#endif
