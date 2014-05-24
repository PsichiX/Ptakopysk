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

    class DestructionListener;
    class ContactListener;

    class GameManager
        : public virtual XeCore::Common::IRtti
        , public virtual XeCore::Common::MemoryManager::Manageable
    {
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

        static const int DEFAULT_VEL_ITERS = 8;
        static const int DEFAULT_POS_ITERS = 3;

        GameManager();
        ~GameManager();

        static void initialize();
        static void cleanup();
        static void registerComponentFactory( const std::string& id, XeCore::Common::IRtti::Derivation type, Component::OnBuildComponentCallback builder );
        static void unregisterComponentFactory( const std::string& id );
        static void unregisterComponentFactory( XeCore::Common::IRtti::Derivation type );
        static void unregisterComponentFactory( Component::OnBuildComponentCallback factory );
        static void unregisterAllComponentFactories();
        static XeCore::Common::IRtti::Derivation findComponentFactoryTypeById( const std::string& id );
        static XeCore::Common::IRtti::Derivation findComponentFactoryTypeByBuilder( Component::OnBuildComponentCallback builder );
        static std::string findComponentFactoryIdByType( XeCore::Common::IRtti::Derivation type );
        static std::string findComponentFactoryIdByBuilder( Component::OnBuildComponentCallback builder );
        static Component::OnBuildComponentCallback findComponentFactoryBuilderById( const std::string& id );
        static Component::OnBuildComponentCallback findComponentFactoryBuilderByType( XeCore::Common::IRtti::Derivation type );
        static Component* buildComponent( const std::string& id );
        static Component* buildComponent( XeCore::Common::IRtti::Derivation type );

        FORCEINLINE b2World* getPhysicsWorld() { return m_world; };

        static Json::Value loadJson( const std::string& path, bool binary = false, dword binaryKeyHash = 0 );
        static bool saveJson( const std::string& path, const Json::Value& root, bool binary = false, dword binaryKeyHash = 0 );
        void jsonToScene( const Json::Value& root, SceneContentType contentFlags = All );
        void jsonToGameObjects( const Json::Value& root, bool prefab = false );
        Json::Value sceneToJson( SceneContentType contentFlags = All );
        Json::Value gameObjectsToJson( bool prefab = false );

        void removeScene( SceneContentType contentFlags = All );
        void addGameObject( GameObject* go, bool prefab = false );
        void removeGameObject( GameObject* go, bool prefab = false );
        void removeGameObject( const std::string& id, bool prefab = false );
        void removeAllGameObjects( bool prefab = false );
        bool hasGameObject( GameObject* go, bool prefab = false );
        bool hasGameObject( const std::string& id, bool prefab = false );
        GameObject* getGameObject( const std::string& id, bool prefab = false );
        GameObject* findGameObject( const std::string& path );
        FORCEINLINE unsigned int gameObjectsCount( bool prefab = false ) { return prefab ? m_prefabGameObjects.size() : m_gameObjects.size(); };
        GameObject::List::iterator gameObjectAtBegin( bool prefab = false );
        GameObject::List::iterator gameObjectAtEnd( bool prefab = false );
        GameObject* instantiatePrefab( const std::string& id );

        FORCEINLINE b2Vec2 getWorldGravity() { return m_world->GetGravity(); };
        FORCEINLINE void setWorldGravity( b2Vec2 v ) { m_world->SetGravity( v ); };
        FORCEINLINE sf::RenderWindow* getRenderWindow() { return m_renderWindow; };
        FORCEINLINE void setRenderWindow( sf::RenderWindow* v ) { m_renderWindow = v; };
        FORCEINLINE FiltersMap& accessFilters() { return m_filters; }

        void processEvents( const sf::Event& event );
        void processPhysics( float dt, int velIters = DEFAULT_VEL_ITERS, int posIters = DEFAULT_POS_ITERS );
        void processUpdate( float dt, bool sort = true );
        void processRender( sf::RenderTarget* target = 0, const sf::Transform& trans = sf::Transform::Identity );
        void processAdding();
        void processRemoving();
        bool isWaitingToAdd( GameObject* go );
        bool isWaitingToRemove( GameObject* go );

        XeCore::Common::Property< b2Vec2, GameManager > PhysicsGravity;
        XeCore::Common::Property< sf::RenderWindow*, GameManager > RenderWindow;

    private:
        struct ComponentFactoryData
        {
            XeCore::Common::IRtti::Derivation type;
            Component::OnBuildComponentCallback builder;
        };

        void processContact( bool beginOrEnd, GameObject* a, GameObject* b, b2Contact* contact );
        void processJointGoodbye( GameObject* o, b2Joint* joint );
        void processFixtureGoodbye( GameObject* o, b2Fixture* fixture );

        static std::map< std::string, ComponentFactoryData > s_componentsFactory;

        b2World* m_world;
        sf::RenderWindow* m_renderWindow;
        DestructionListener* m_destructionListener;
        ContactListener* m_contactListener;
        GameObject::List m_prefabGameObjects;
        GameObject::List m_gameObjects;
        GameObject::List m_gameObjectsToCreate;
        GameObject::List m_gameObjectsToDestroy;
        FiltersMap m_filters;
    };

    GameManager::SceneContentType operator|( GameManager::SceneContentType a, GameManager::SceneContentType b );

}

#endif
