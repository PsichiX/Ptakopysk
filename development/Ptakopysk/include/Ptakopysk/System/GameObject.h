#ifndef __PTAKOPYSK__GAME_OBJECT__
#define __PTAKOPYSK__GAME_OBJECT__

#include <XeCore/Common/Property.h>
#include <XeCore/Common/IRtti.h>
#include <XeCore/Common/MemoryManager.h>
#include <json/json.h>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Window/Event.hpp>
#include <list>
#include <string>
#include "../Serialization/Serialized.h"

class b2Contact;
class b2Joint;
class b2Fixture;

namespace Ptakopysk
{

    class Component;
    class GameManager;

    class GameObject
    : public virtual XeCore::Common::IRtti
    , public virtual XeCore::Common::MemoryManager::Manageable
    , public Serialized
    {
        RTTI_CLASS_DECLARE( GameObject );
        friend class GameManager;

    public:
        typedef std::list< GameObject* > List;

        GameObject( const std::string& id = "" );
        virtual ~GameObject();

        FORCEINLINE bool isPrefab() { return m_prefab; };
        FORCEINLINE std::string getId() { return m_id; };
        FORCEINLINE void setId( std::string id ) { m_id = id; };
        FORCEINLINE bool isActive() { return m_active; };
        FORCEINLINE void setActive( bool mode = true ) { m_active = mode; };
        FORCEINLINE int getOrder() { return m_order; };
        FORCEINLINE void setOrder( int order ) { m_order = order; };
        FORCEINLINE const Json::Value& getMetaData() { return m_metaData; };
        FORCEINLINE void setMetaData( const Json::Value& meta ) { m_metaData = meta; };
        FORCEINLINE GameManager* getGameManager() { return m_gameManager; };
        FORCEINLINE GameObject* getParent() { return m_parent; };
        GameManager* getGameManagerRoot();

        void fromJson( const Json::Value& root );
        Json::Value toJson();
        void duplicate( GameObject* from );

        void addComponent( Component* c );
        void removeComponent( Component* c );
        void removeComponent( XeCore::Common::IRtti::Derivation d );
        template< typename T >
        FORCEINLINE void removeComponent() { removeComponent( RTTI_CLASS_TYPE( T ) ); };
        void removeAllComponents();
        bool hasComponent( Component* c );
        bool hasComponent( XeCore::Common::IRtti::Derivation d );
        template< typename T >
        FORCEINLINE bool hasComponent() { return hasComponent( RTTI_CLASS_TYPE( T ) ); };
        Component* getComponent( XeCore::Common::IRtti::Derivation d );
        template< typename T >
        FORCEINLINE T* getComponent() { return (T*)getComponent( RTTI_CLASS_TYPE( T ) ); };
        template< typename T >
        T* getOrCreateComponent();

        void addGameObject( GameObject* go );
        void removeGameObject( GameObject* go );
        void removeGameObject( const std::string& id );
        void removeAllGameObjects();
        bool hasGameObject( GameObject* go );
        bool hasGameObject( const std::string& id );
        GameObject* getGameObject( const std::string& id );
        GameObject* findGameObject( const std::string& path );
        FORCEINLINE unsigned int gameObjectsCount() { return m_gameObjects.size(); };
        FORCEINLINE List::iterator gameObjectAtBegin() { return m_gameObjects.begin(); };
        FORCEINLINE List::iterator gameObjectAtEnd() { return m_gameObjects.end(); };
        void processAdding();
        void processRemoving();
        bool isWaitingToAdd( GameObject* go );
        bool isWaitingToRemove( GameObject* go );

        XeCore::Common::Property< std::string, GameObject > Id;
        XeCore::Common::Property< bool, GameObject > Active;
        XeCore::Common::Property< int, GameObject > Order;
        XeCore::Common::Property< const Json::Value&, GameObject > MetaData;
        XeCore::Common::Property< GameManager*, GameObject > Owner;

    protected:
        virtual Json::Value onSerialize( const std::string& property );
        virtual void onDeserialize( const std::string& property, const Json::Value& root );

        void onCreate();
        void onDestroy();
        void onDuplicate( GameObject* dst );
        void onEvent( const sf::Event& event );
        void onUpdate( float dt, const sf::Transform& trans, bool sort = true );
        void onRender( sf::RenderTarget* target );
        void onCollide( GameObject* other, bool beginOrEnd, b2Contact* contact );
        void onJointGoodbye( b2Joint* joint );
        void onFixtureGoodbye( b2Fixture* fixture );

    private:
        typedef std::pair< XeCore::Common::IRtti::Derivation, Component* > ComponentDataPair;
        typedef std::list< ComponentDataPair > Components;

        FORCEINLINE void setGameManager( GameManager* gm ) { m_gameManager = gm; };
        FORCEINLINE void setParent( GameObject* go ) { m_parent = go; };
        void setPrefab( bool mode );
        GameObject* findGameObjectInPartOfPath( const std::string& path, unsigned int from );

        GameManager* m_gameManager;
        GameObject* m_parent;
        bool m_prefab;
        std::string m_id;
        bool m_active;
        int m_order;
        Json::Value m_metaData;
        Components m_components;
        List m_gameObjects;
        List m_gameObjectsToCreate;
        List m_gameObjectsToDestroy;
    };

}

#include "GameObject.inl"

#endif
