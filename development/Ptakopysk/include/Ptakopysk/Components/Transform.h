#ifndef __PTAKOPYSK__TRANSFORM__
#define __PTAKOPYSK__TRANSFORM__

#include "Component.h"
#include <SFML/System/Vector2.hpp>

namespace Ptakopysk
{
    class Transform
    : public virtual XeCore::Common::IRtti
    , public virtual XeCore::Common::MemoryManager::Manageable
    , public Component
    {
        RTTI_CLASS_DECLARE( Transform );

    public:
        Transform();
        virtual ~Transform();

        FORCEINLINE static Component* onBuildComponent() { return xnew Transform(); }

        FORCEINLINE sf::Vector2f getPosition() { return m_position; };
        FORCEINLINE void setPosition( sf::Vector2f pos ) { m_position = pos; };
        FORCEINLINE float getRotation() { return m_rotation; };
        FORCEINLINE void setRotation( float rot ) { m_rotation = rot; };
        FORCEINLINE sf::Vector2f getScale() { return m_scale; };
        FORCEINLINE void setScale( sf::Vector2f scl ) { m_scale = scl; };

        XeCore::Common::Property< sf::Vector2f, Transform > Position;
        XeCore::Common::Property< float, Transform > Rotation;
        XeCore::Common::Property< sf::Vector2f, Transform > Scale;

    protected:
        virtual Json::Value onSerialize( const std::string& property );
        virtual void onDeserialize( const std::string& property, const Json::Value& root );

        virtual void onDuplicate( Component* dst );
        virtual void onUpdate( float dt );

    private:
        sf::Vector2f m_position;
        float m_rotation;
        sf::Vector2f m_scale;
    };

}

#endif

