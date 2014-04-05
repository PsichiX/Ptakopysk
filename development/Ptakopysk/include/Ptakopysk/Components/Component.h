#ifndef __PTAKOPYSK__COMPONENT__
#define __PTAKOPYSK__COMPONENT__

#include <XeCore/Common/Property.h>
#include <XeCore/Common/IRtti.h>
#include <XeCore/Common/MemoryManager.h>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Transform.hpp>
#include <SFML/Window/Event.hpp>
#include <string>
#include "../Serialization/Serialized.h"

class b2Contact;

namespace Ptakopysk
{

    class GameObject;

    class Component
    : public virtual XeCore::Common::IRtti
    , public virtual XeCore::Common::MemoryManager::Manageable
    , public Serialized
    {
        RTTI_CLASS_DECLARE( Component );
        friend class GameObject;

    public:
        enum Type
        {
            tNone = 0,
            tEvents = 1 << 0,
            tUpdate = 1 << 1,
            tRender = 1 << 2,
            tPhysics = 1 << 3,
            tTransform = 1 << 4,
            tAll = -1
        };

        typedef Component* ( *OnBuildComponentCallback )();

        Component( Type typeFlags );
        virtual ~Component();

        FORCEINLINE Type getTypeFlags() { return m_typeFlags; };
        FORCEINLINE bool isActive() { return m_active; };
        FORCEINLINE void setActive( bool mode = true ) { m_active = mode; };
        FORCEINLINE GameObject* getGameObject() { return m_gameObject; };

        void fromJson( const Json::Value& root );
        Json::Value toJson();

        XeCore::Common::Property< Type, Component > TypeFlags;
        XeCore::Common::Property< bool, Component > Active;
        XeCore::Common::Property< GameObject*, Component > Owner;

    protected:
        virtual Json::Value onSerialize( const std::string& property );
        virtual void onDeserialize( const std::string& property, const Json::Value& root );

        virtual void onCreate() {};
        virtual void onDestroy() {};
        virtual void onDuplicate( Component* dst );
        virtual void onEvent( const sf::Event& event ) {};
        virtual void onUpdate( float dt ) {};
        virtual void onTransform( const sf::Transform& inTrans, sf::Transform& outTrans ) {};
        virtual void onRender( sf::RenderTarget* target ) {};
        virtual void onCollide( GameObject* other, bool beginOrEnd, b2Contact* contact ) {};

    private:
        void setGameObject( GameObject* gameObject );

        Type m_typeFlags;
        bool m_active;
        GameObject* m_gameObject;
    };

    Component::Type operator|( Component::Type a, Component::Type b );

}

#endif
