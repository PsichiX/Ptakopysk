#ifndef __PTAKOPYSK__TRANSFORM__
#define __PTAKOPYSK__TRANSFORM__

#include "Component.h"
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Transform.hpp>

namespace Ptakopysk
{
    class Transform
    : public virtual XeCore::Common::IRtti
    , public virtual XeCore::Common::MemoryManager::Manageable
    , public Component
    {
        RTTI_CLASS_DECLARE( Transform );

    public:
        enum ModeType
        {
            mHierarchy,
            mParent,
            mGlobal
        };

        Transform();
        virtual ~Transform();

        FORCEINLINE static Component* onBuildComponent() { return xnew Transform(); }

        FORCEINLINE sf::Vector2f getPosition() { return m_position; };
        FORCEINLINE void setPosition( sf::Vector2f pos ) { m_position = pos; };
        FORCEINLINE float getRotation() { return m_rotation; };
        FORCEINLINE void setRotation( float rot ) { m_rotation = rot; };
        FORCEINLINE sf::Vector2f getScale() { return m_scale; };
        FORCEINLINE void setScale( sf::Vector2f scl ) { m_scale = scl; };
        FORCEINLINE ModeType getMode() { return m_mode; };
        FORCEINLINE void setMode( ModeType mode ) { m_mode = mode; };
        FORCEINLINE const sf::Transform& getTransform() { return m_transform; };
        FORCEINLINE const sf::Transform& getTransformGlobal() { return m_transformGlobal; };
        void recomputeTransform();

        XeCore::Common::Property< sf::Vector2f, Transform > Position;
        XeCore::Common::Property< float, Transform > Rotation;
        XeCore::Common::Property< sf::Vector2f, Transform > Scale;
        XeCore::Common::Property< ModeType, Transform > Mode;

    protected:
        virtual Json::Value onSerialize( const std::string& property );
        virtual void onDeserialize( const std::string& property, const Json::Value& root );

        virtual void onDuplicate( Component* dst );
        virtual void onTransform( const sf::Transform& inTrans, sf::Transform& outTrans );

    private:
        sf::Vector2f m_position;
        float m_rotation;
        sf::Vector2f m_scale;
        ModeType m_mode;
        sf::Transform m_transform;
        sf::Transform m_transformGlobal;
    };

}

#endif

