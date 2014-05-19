#include "../../include/Ptakopysk/Components/RevoluteJoint.h"
#include "../../include/Ptakopysk/Components/Body.h"
#include "../../include/Ptakopysk/System/GameObject.h"
#include "../../include/Ptakopysk/System/GameManager.h"
#include <XeCore/Common/Logger.h>

namespace Ptakopysk
{

    RTTI_CLASS_DERIVATIONS( RevoluteJoint,
                            RTTI_DERIVATION( Component ),
                            RTTI_DERIVATIONS_END
                            )

    RevoluteJoint::RevoluteJoint()
    : RTTI_CLASS_DEFINE( RevoluteJoint )
    , Component( Component::tPhysics )
    , BindingA( this, &RevoluteJoint::getBindingA, &RevoluteJoint::setBindingA )
    , BindingB( this, &RevoluteJoint::getBindingB, &RevoluteJoint::setBindingB )
    , CollideConnected( this, &RevoluteJoint::getCollideConnected, &RevoluteJoint::setCollideConnected )
    , ReferenceAngle( this, &RevoluteJoint::getReferenceAngle, &RevoluteJoint::setReferenceAngle )
    , LocalAnchorA( this, &RevoluteJoint::getLocalAnchorA, &RevoluteJoint::setLocalAnchorA )
    , LocalAnchorB( this, &RevoluteJoint::getLocalAnchorB, &RevoluteJoint::setLocalAnchorB )
    , LimitEnabled( this, &RevoluteJoint::isLimitEnabled, &RevoluteJoint::setLimitEnabled )
    , LowerLimit( this, &RevoluteJoint::getLowerLimit, &RevoluteJoint::setLowerLimit )
    , UpperLimit( this, &RevoluteJoint::getUpperLimit, &RevoluteJoint::setUpperLimit )
    , MotorEnabled( this, &RevoluteJoint::isMotorEnabled, &RevoluteJoint::setMotorEnabled )
    , MotorSpeed( this, &RevoluteJoint::getMotorSpeed, &RevoluteJoint::setMotorSpeed )
    , MotorTorque( this, &RevoluteJoint::getMotorTorque, &RevoluteJoint::setMotorTorque )
    , m_joint( 0 )
    {
        serializableProperty( "BindingA" );
        serializableProperty( "BindingB" );
        serializableProperty( "CollideConnected" );
        serializableProperty( "ReferenceAngle" );
        serializableProperty( "LocalAnchorA" );
        serializableProperty( "LocalAnchorB" );
        serializableProperty( "LimitEnabled" );
        serializableProperty( "LowerLimit" );
        serializableProperty( "UpperLimit" );
        serializableProperty( "MotorEnabled" );
        serializableProperty( "MotorSpeed" );
        serializableProperty( "MotorTorque" );
    }

    RevoluteJoint::~RevoluteJoint()
    {
        onDestroy();
    }

    Json::Value RevoluteJoint::onSerialize( const std::string& property )
    {
        if( property == "BindingA" )
            return Json::Value( m_bindingA );
        else if( property == "BindingB" )
            return Json::Value( m_bindingB );
        else if( property == "CollideConnected" )
            return Json::Value( getCollideConnected() );
        else if( property == "ReferenceAngle" )
            return Json::Value( getReferenceAngle() );
        else if( property == "LocalAnchorA" )
        {
            b2Vec2 v = getLocalAnchorA();
            Json::Value a;
            a.append( Json::Value( v.x ) );
            a.append( Json::Value( v.y ) );
            return a;
        }
        else if( property == "LocalAnchorB" )
        {
            b2Vec2 v = getLocalAnchorB();
            Json::Value a;
            a.append( Json::Value( v.x ) );
            a.append( Json::Value( v.y ) );
            return a;
        }
        else if( property == "LimitEnabled" )
            return Json::Value( isLimitEnabled() );
        else if( property == "LowerLimit" )
            return Json::Value( getLowerLimit() );
        else if( property == "UpperLimit" )
            return Json::Value( getUpperLimit() );
        else if( property == "MotorEnabled" )
            return Json::Value( isMotorEnabled() );
        else if( property == "MotorSpeed" )
            return Json::Value( getMotorSpeed() );
        else if( property == "MotorTorque" )
            return Json::Value( getMotorTorque() );
        else
            return Component::onSerialize( property );
    }

    void RevoluteJoint::onDeserialize( const std::string& property, const Json::Value& root )
    {
        if( property == "BindingA" && root.isString() )
            m_bindingA = root.asString();
        else if( property == "BindingB" && root.isString() )
            m_bindingB = root.asString();
        else if( property == "CollideConnected" && root.isBool() )
            setCollideConnected( root.asBool() );
        else if( property == "ReferenceAngle" && root.isNumeric() )
            setReferenceAngle( (float)root.asDouble() );
        else if( property == "LocalAnchorA" && root.isArray() && root.size() >= 2 )
            setLocalAnchorA( b2Vec2(
                (float)root[ 0u ].asDouble(),
                (float)root[ 1u ].asDouble()
            ) );
        else if( property == "LocalAnchorB" && root.isArray() && root.size() >= 2 )
            setLocalAnchorB( b2Vec2(
                (float)root[ 0u ].asDouble(),
                (float)root[ 1u ].asDouble()
            ) );
        else if( property == "LimitEnabled" && root.isBool() )
            setLimitEnabled( root.asBool() );
        else if( property == "LowerLimit" && root.isNumeric() )
            setLowerLimit( (float)root.asDouble() );
        else if( property == "UpperLimit" && root.isNumeric() )
            setUpperLimit( (float)root.asDouble() );
        else if( property == "MotorEnabled" && root.isBool() )
            setMotorEnabled( root.asBool() );
        else if( property == "MotorSpeed" && root.isNumeric() )
            setMotorSpeed( (float)root.asDouble() );
        else if( property == "MotorTorque" && root.isNumeric() )
            setMotorTorque( (float)root.asDouble() );
        else
            Component::onDeserialize( property, root );
    }

    void RevoluteJoint::onCreate()
    {
        if( !getGameObject() || getGameObject()->isPrefab() )
            return;
        onDestroy();
        if( m_bindingA.empty() || m_bindingB.empty() )
            return;
        GameObject* a = getGameObject()->findGameObject( m_bindingA );
        if( !a )
        {
            LOGNL( "Cannot find GameObject: '%s' for binding A!", m_bindingA.c_str() );
            return;
        }
        Body* ba = a->getComponent< Body >();
        if( !ba )
        {
            LOGNL( "GameObject: '%s' binding A does not have Body component!", m_bindingA.c_str() );
            return;
        }
        GameObject* b = getGameObject()->findGameObject( m_bindingB );
        if( !b )
        {
            LOGNL( "Cannot find GameObject: '%s' for binding B!", m_bindingB.c_str() );
            return;
        }
        Body* bb = b->getComponent< Body >();
        if( !bb )
        {
            LOGNL( "GameObject: '%s' binding B does not have Body component!", m_bindingB.c_str() );
            return;
        }
        if( ba == bb )
        {
            LOGNL( "GameObjects bindings A and B are the same!" );
            return;
        }
        m_jointDef.bodyA = ba->getBody();
        m_jointDef.bodyB = bb->getBody();
        m_joint = (b2RevoluteJoint*)getGameObject()->getGameManagerRoot()->getPhysicsWorld()->CreateJoint( &m_jointDef );
        m_joint->SetUserData( getGameObject() );
    }

    void RevoluteJoint::onDestroy()
    {
        if( !getGameObject() || getGameObject()->isPrefab() )
            return;
        if( m_joint )
        {
            m_jointDef.enableLimit = m_joint->IsLimitEnabled();
            m_jointDef.lowerAngle = m_joint->GetLowerLimit();
            m_jointDef.upperAngle = m_joint->GetUpperLimit();
            m_jointDef.enableMotor = m_joint->IsMotorEnabled();
            m_jointDef.motorSpeed = m_joint->GetMotorSpeed();
            m_jointDef.maxMotorTorque = m_joint->GetMaxMotorTorque();
            getGameObject()->getGameManagerRoot()->getPhysicsWorld()->DestroyJoint( m_joint );
        }
        m_joint = 0;
    }

    void RevoluteJoint::onDuplicate( Component* dst )
    {
        if( !dst )
            return;
        Component::onDuplicate( dst );
        if( !XeCore::Common::IRtti::isDerived< RevoluteJoint >( dst ) )
            return;
        RevoluteJoint* c = (RevoluteJoint*)dst;
        c->setBindingA( getBindingA() );
        c->setBindingB( getBindingB() );
        c->setCollideConnected( getCollideConnected() );
        c->setReferenceAngle( getReferenceAngle() );
        c->setLocalAnchorA( getLocalAnchorA() );
        c->setLocalAnchorB( getLocalAnchorB() );
        c->setLimitEnabled( isLimitEnabled() );
        c->setLowerLimit( getLowerLimit() );
        c->setUpperLimit( getUpperLimit() );
        c->setMotorEnabled( isMotorEnabled() );
        c->setMotorSpeed( getMotorSpeed() );
        c->setMotorTorque( getMotorTorque() );
    }

    void RevoluteJoint::onJointGoodbye( b2Joint* joint )
    {
        if( joint == m_joint )
        {
            m_joint = 0;
            if( getGameObject() )
                getGameObject()->removeComponent( this );
        }
    }

}
