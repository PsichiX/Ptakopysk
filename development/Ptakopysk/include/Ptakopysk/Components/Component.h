#ifndef __PTAKOPYSK__COMPONENT__
#define __PTAKOPYSK__COMPONENT__

#include <XeCore/Common/Property.h>
#include <XeCore/Common/IRtti.h>
#include <XeCore/Common/MemoryManager.h>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Transform.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/Music.hpp>
#include <SFML/Graphics/Font.hpp>
#include <string>
#include "../Serialization/Serialized.h"
#include "../System/Meta.h"
#include "../System/Assets.h"

class b2Contact;
class b2Joint;
class b2Fixture;

namespace Ptakopysk
{

    class GameObject;

    META_COMPONENT(
        META_ATTR_DESCRIPTION( "Base component." )
    )
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
            tShape = 1 << 5,
            tAll = -1
        };

        typedef Component* ( *OnBuildComponentCallback )();

        Component( Type typeFlags );
        virtual ~Component();

        FORCEINLINE Type getTypeFlags() { return m_typeFlags; };
        FORCEINLINE void setTypeFlags( Type flags ) { m_typeFlags = flags; };
        FORCEINLINE bool isActive() { return m_active; };
        FORCEINLINE void setActive( bool mode = true ) { m_active = mode; };
        FORCEINLINE GameObject* getGameObject() { return m_gameObject; };

        void fromJson( const Json::Value& root );
        Json::Value toJson( Component* omitFrom = 0 );
        FORCEINLINE bool triggerFunctionality( const std::string& name ) { return onTriggerFunctionality( name ); };

        XeCore::Common::Property< Type, Component > TypeFlags;

        META_PROPERTY(
            META_ATTR_DESCRIPTION( "Determines if component is active." ),
            META_ATTR_DEFAULT_VALUE( "true" )
        )
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
        virtual void onRender( sf::RenderTarget*& target ) {};
        virtual void onRenderEditor( sf::RenderTarget* target ) { onRender( target ); };
        virtual void onCollide( GameObject* other, bool beginOrEnd, b2Contact* contact ) {};
        virtual void onJointGoodbye( b2Joint* joint ) {};
        virtual void onFixtureGoodbye( b2Fixture* fixture ) {};
        virtual void onTextureChanged( const sf::Texture* a, bool addedOrRemoved ) {};
        virtual void onShaderChanged( const sf::Shader* a, bool addedOrRemoved ) {};
        virtual void onSoundChanged( const sf::Sound* a, bool addedOrRemoved ) {};
        virtual void onMusicChanged( const sf::Music* a, bool addedOrRemoved ) {};
        virtual void onFontChanged( const sf::Font* a, bool addedOrRemoved ) {};
        virtual void onCustomAssetChanged( const ICustomAsset* a, bool addedOrRemoved ) {};
        virtual bool onTriggerFunctionality( const std::string& name ) { return false; };
        virtual bool onCheckContainsPoint( const sf::Vector2f& worldPos ) { return false; };

    private:
        void setGameObject( GameObject* gameObject );

        Type m_typeFlags;
        bool m_active;
        GameObject* m_gameObject;
    };

    Component::Type operator|( Component::Type a, Component::Type b );

}

#endif
