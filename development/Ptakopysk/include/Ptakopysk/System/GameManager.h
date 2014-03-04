#ifndef __PTAKOPYSK__GAME_MANAGER__
#define __PTAKOPYSK__GAME_MANAGER__

#include <XeCore/Common/Property.h>
#include <XeCore/Common/IRtti.h>
#include <XeCore/Common/MemoryManager.h>
#include <json/json.h>
#include <list>
#include <string>
#include <Box2D/Box2D.h>
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
            All = -1
        };

        static const int DEFAULT_VEL_ITERS = 6;
        static const int DEFAULT_POS_ITERS = 2;

        GameManager( float gravX = 0.0f, float gravY = 0.0f );
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

        Json::Value loadJson( const std::string& path );
        bool saveJson( const std::string& path, const Json::Value& root );
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
        FORCEINLINE unsigned int gameObjectsCount( bool prefab = false ) { return prefab ? m_prefabGameObjects.size() : m_gameObjects.size(); };
        std::list< GameObject* >::iterator gameObjectAtBegin( bool prefab = false );
        std::list< GameObject* >::iterator gameObjectAtEnd( bool prefab = false );
        GameObject* instantiatePrefab( const std::string& id );

        FORCEINLINE b2Vec2 getWorldGravity() { return m_world->GetGravity(); };
        FORCEINLINE void setWorldGravity( b2Vec2 v ) { m_world->SetGravity( v ); };
        void processPhysics( float dt, int velIters = DEFAULT_VEL_ITERS, int posIters = DEFAULT_POS_ITERS );
        void processUpdate( float dt, bool sort = true );
        void processRender( sf::RenderTarget* target, const sf::Transform& trans = sf::Transform::Identity );
        void processAdding();
        void processRemoving();
        bool isWaitingToAdd( GameObject* go );
        bool isWaitingToRemove( GameObject* go );

        XeCore::Common::Property< b2Vec2, GameManager > PhysicsGravity;

    private:
        struct ComponentFactoryData
        {
            XeCore::Common::IRtti::Derivation type;
            Component::OnBuildComponentCallback builder;
        };

        void processContact( bool beginOrEnd, GameObject* a, GameObject* b, b2Contact* contact );

        static std::map< std::string, ComponentFactoryData > s_componentsFactory;

        b2World* m_world;
        DestructionListener* m_destructionListener;
        ContactListener* m_contactListener;
        std::list< GameObject* > m_prefabGameObjects;
        std::list< GameObject* > m_gameObjects;
        std::list< GameObject* > m_gameObjectsToCreate;
        std::list< GameObject* > m_gameObjectsToDestroy;
    };

    GameManager::SceneContentType operator|( GameManager::SceneContentType a, GameManager::SceneContentType b );

}

#endif
