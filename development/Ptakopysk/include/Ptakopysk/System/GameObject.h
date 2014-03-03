#ifndef __PTAKOPYSK__GAME_OBJECT__
#define __PTAKOPYSK__GAME_OBJECT__

#include <XeCore/Common/Property.h>
#include <XeCore/Common/IRtti.h>
#include <XeCore/Common/MemoryManager.h>
#include <json/json.h>
#include <SFML/Graphics/RenderTarget.hpp>
#include <map>
#include <list>
#include <string>
#include "../Serialization/Serialized.h"

class b2Contact;

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
        GameObject( const std::string& id = "" );
        virtual ~GameObject();

        FORCEINLINE std::string getId() { return m_id; };
        FORCEINLINE void setId( std::string id ) { m_id = id; };
        FORCEINLINE bool isActive() { return m_active; };
        FORCEINLINE void setActive( bool mode = true ) { m_active = mode; };
        FORCEINLINE int getOrder() { return m_order; };
        FORCEINLINE void setOrder( int order ) { m_order = order; };
        FORCEINLINE GameManager* getGameManager() { return m_gameManager; };
        FORCEINLINE GameObject* getParent() { return m_parent; };
        GameManager* getGameManagerRoot();

        void fromJson( const Json::Value& root );
        Json::Value toJson();

        void addComponent( Component* c );
        void removeComponent( Component* c );
        void removeComponent( XeCore::Common::IRtti::Derivation d );
        void removeAllComponents();
        bool hasComponent( Component* c );
        bool hasComponent( XeCore::Common::IRtti::Derivation d );
        FORCEINLINE Component* getComponent( XeCore::Common::IRtti::Derivation d ) { return m_components.count( d ) ? m_components[ d ] : 0; };
        template< typename T >
        FORCEINLINE T* getComponent() { return (T*)getComponent( RTTI_CLASS_TYPE( T ) ); };

        void addGameObject( GameObject* go );
        void removeGameObject( GameObject* go, bool del = true );
        void removeGameObject( const std::string& id, bool del = true );
        void removeAllGameObjects( bool del = true );
        bool hasGameObject( GameObject* go );
        bool hasGameObject( const std::string& id );
        GameObject* getGameObject( const std::string& id );
        FORCEINLINE unsigned int gameObjectsCount() { return m_gameObjects.size(); };
        FORCEINLINE std::list< GameObject* >::iterator gameObjectAtBegin( bool prefab = false ) { return m_gameObjects.begin(); };
        FORCEINLINE std::list< GameObject* >::iterator gameObjectAtEnd( bool prefab = false ) { return m_gameObjects.end(); };

        XeCore::Common::Property< std::string, GameObject > Id;
        XeCore::Common::Property< bool, GameObject > Active;
        XeCore::Common::Property< int, GameObject > Order;
        XeCore::Common::Property< GameManager*, GameObject > Owner;

    protected:
        virtual Json::Value onSerialize( const std::string& property );
        virtual void onDeserialize( const std::string& property, const Json::Value& root );

        void onCreate();
        void onDestroy();
        void onDuplicate( GameObject* dst );
        void onUpdate( float dt, const sf::Transform& trans, bool sort = true );
        void onRender( sf::RenderTarget* target );
        void onCollide( GameObject* other, bool beginOrEnd, b2Contact* contact );

    private:
        FORCEINLINE void setGameManager( GameManager* gm ) { m_gameManager = gm; };
        FORCEINLINE void setParent( GameObject* go ) { m_parent = go; };

        GameManager* m_gameManager;
        GameObject* m_parent;
        std::string m_id;
        bool m_active;
        int m_order;
        std::map< XeCore::Common::IRtti::Derivation, Component* > m_components;
        std::list< GameObject* > m_gameObjects;
    };

}

#endif
