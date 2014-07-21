#include "../../include/Ptakopysk/Components/Transform.h"
#include "../../include/Ptakopysk/System/GameObject.h"

namespace Ptakopysk
{

    RTTI_CLASS_DERIVATIONS( Transform,
                            RTTI_DERIVATION( Component ),
                            RTTI_DERIVATIONS_END
                            )

    Transform::Transform()
    : RTTI_CLASS_DEFINE( Transform )
    , Component( Component::tTransform )
    , Position( this, &Transform::getPosition, &Transform::setPosition )
    , Rotation( this, &Transform::getRotation, &Transform::setRotation )
    , Scale( this, &Transform::getScale, &Transform::setScale )
    , Mode( this, &Transform::getMode, &Transform::setMode )
    , m_position( sf::Vector2f( 0.0f, 0.0f ) )
    , m_rotation( 0.0f )
    , m_scale( sf::Vector2f( 1.0f, 1.0f ) )
    , m_mode( mHierarchy )
    {
        serializableProperty( "Position" );
        serializableProperty( "Rotation" );
        serializableProperty( "Scale" );
        serializableProperty( "Mode" );
    }

    Transform::~Transform()
    {
    }

    void Transform::recomputeTransform()
    {
        if( getGameObject() )
        {
            if( getGameObject()->getParent() )
            {
                Transform* trans = getGameObject()->getParent()->getComponent< Transform >();
                if( trans )
                    trans->recomputeTransform();
            }
        }
        sf::Transform t;
        onTransform( t, t );
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
            return Json::Value( m_rotation );
        else if( property == "Scale" )
        {
            Json::Value v;
            v.append( Json::Value( m_scale.x ) );
            v.append( Json::Value( m_scale.y ) );
            return v;
        }
        else if( property == "Mode" )
            return Serialized::serializeCustom< ModeType >( "Transform::ModeType", m_mode );
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
        else if( property == "Mode" && root.isString() )
            m_mode = Serialized::deserializeCustom< ModeType >( "Transform::ModeType", root );
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
        c->setMode( getMode() );
    }

    void Transform::onTransform( const sf::Transform& inTrans, sf::Transform& outTrans )
    {
        sf::Transform t;
        if( m_mode == mHierarchy )
        {
            t.translate( m_position );
            t.rotate( m_rotation );
            t.scale( m_scale );
            outTrans = inTrans * t;
        }
        else if( m_mode == mParent )
            outTrans = inTrans;
        else
        {
            t.translate( m_position );
            t.rotate( m_rotation );
            t.scale( m_scale );
            outTrans = t;
        }
        m_transform = t;
        m_transformGlobal = outTrans;
    }

}
