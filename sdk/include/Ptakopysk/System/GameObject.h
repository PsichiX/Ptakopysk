#ifndef __PTAKOPYSK__GAME_OBJECT__
#define __PTAKOPYSK__GAME_OBJECT__

#include <XeCore/Common/Property.h>
#include <XeCore/Common/IRtti.h>
#include <XeCore/Common/MemoryManager.h>
#include <json/json.h>
#include <map>
#include <string>
#include "../Serialization/Serialized.h"

namespace Ptakopysk
{

    class Component;
    class GameManager;
    namespace sf
    {
        class RenderTarget;
    }

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

        void fromJson( const Json::Value& root );
        Json::Value toJson();

        void addComponent( Component* c );
        void removeComponent( Component* c );
        void removeComponent( XeCore::Common::IRtti::Derivation d );
        void removeAllComponents();
        bool hasComponent( Component* c );
        bool hasComponent( XeCore::Common::IRtti::Derivation d );
        Component* getComponent( XeCore::Common::IRtti::Derivation d );

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
        void onUpdate( float dt );
        void onRender( sf::RenderTarget* target );
        void onCollide( GameObject* other );

    private:
        void setGameManager( GameManager* gm );

        std::string m_id;
        bool m_active;
        int m_order;
        std::map< XeCore::Common::IRtti::Derivation, Component* > m_components;
        GameManager* m_gameManager;
    };

}

#endif
