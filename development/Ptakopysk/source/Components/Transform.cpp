#include "../../include/Ptakopysk/Components/Transform.h"
#include "../../include/Ptakopysk/Components/Body.h"
#include "../../include/Ptakopysk/System/GameObject.h"

namespace Ptakopysk
{

    RTTI_CLASS_DERIVATIONS( Transform,
                            RTTI_DERIVATION( Component ),
                            RTTI_DERIVATIONS_END
                            )

    Transform::Transform()
    : RTTI_CLASS_DEFINE( Transform )
    , Component( Component::tUpdate | Component::tTransform )
    , Position( this, &Transform::getPosition, &Transform::setPosition )
    , Rotation( this, &Transform::getRotation, &Transform::setRotation )
    , Scale( this, &Transform::getScale, &Transform::setScale )
    , m_position( sf::Vector2f( 0.0f, 0.0f ) )
    , m_rotation( 0.0f )
    , m_scale( sf::Vector2f( 1.0f, 1.0f ) )
    {
        serializableProperty( "Position" );
        serializableProperty( "Rotation" );
        serializableProperty( "Scale" );
    }

    Transform::~Transform()
    {
    }

    Json::Value Transform::onSerialize( const std::string& property )
    {
        if( property == "Position" )
        {
            Json::Value v;
            v.append( Json::Value( m_position.x ) );
            v.append( Json::Value( m_position.y ) );
            return v;
        }
        else if( property == "Rotation" )
            return Json::Value( Rotation );
        else if( property == "Scale" )
        {
            Json::Value v;
            v.append( Json::Value( m_scale.x ) );
            v.append( Json::Value( m_scale.y ) );
            return v;
        }
        else
            return Component::onSerialize( property );
    }

    void Transform::onDeserialize( const std::string& property, const Json::Value& root )
    {
        if( property == "Position" && root.isArray() && root.size() >= 2 )
        {
            m_position = sf::Vector2f(
                (float)root[ 0u ].asDouble(),
                (float)root[ 1u ].asDouble()
            );
        }
        else if( property == "Rotation" && root.isNumeric() )
            m_rotation = (float)root.asDouble();
        else if( property == "Scale" && root.isArray() && root.size() >= 2 )
        {
            m_scale = sf::Vector2f(
                (float)root[ 0u ].asDouble(),
                (float)root[ 1u ].asDouble()
            );
        }
        else
            Component::onDeserialize( property, root );
    }

    void Transform::onDuplicate( Component* dst )
    {
        if( !dst )
            return;
        Component::onDuplicate( dst );
        if( !XeCore::Common::IRtti::isDerived< Transform >( dst ) )
            return;
        Transform* c = (Transform*)dst;
        c->setPosition( getPosition() );
        c->setRotation( getRotation() );
        c->setScale( getScale() );
    }

    void Transform::onUpdate( float dt )
    {
        Body* body = getGameObject()->getComponent< Body >();
        if( body )
        {
            b2Vec2 pos = body->getPosition();
            setPosition( sf::Vector2f( pos.x, pos.y ) );
            setRotation( body->getAngle() );
        }
    }

    void Transform::onTransform( const sf::Transform& inTrans, sf::Transform& outTrans )
    {
        sf::Transform t;
        t.translate( m_position );
        t.rotate( m_rotation );
        t.scale( m_scale );
        outTrans = inTrans * t;
    }

}
