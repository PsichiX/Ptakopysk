#ifndef __PTAKOPYSK__REVOLUTE_JOINT__
#define __PTAKOPYSK__REVOLUTE_JOINT__

#include "Component.h"
#include <Box2D/Box2D.h>

namespace Ptakopysk
{

    META_COMPONENT(
        META_ATTR_DESCRIPTION( "Physics revolute joint component." )
    )
    class RevoluteJoint
    : public virtual XeCore::Common::IRtti
    , public virtual XeCore::Common::MemoryManager::Manageable
    , public Component
    {
        RTTI_CLASS_DECLARE( RevoluteJoint );

    public:
        RevoluteJoint();
        virtual ~RevoluteJoint();

        FORCEINLINE static Component* onBuildComponent() { return xnew RevoluteJoint(); }

        FORCEINLINE b2RevoluteJoint* getJoint() { return m_joint; };
        FORCEINLINE std::string getBindingA() { return m_bindingA; };
        FORCEINLINE void setBindingA( std::string v ) { m_bindingA = v; };
        FORCEINLINE std::string getBindingB() { return m_bindingB; };
        FORCEINLINE void setBindingB( std::string v ) { m_bindingB = v; };
        FORCEINLINE bool getCollideConnected() { return m_jointDef.collideConnected; };
        FORCEINLINE void setCollideConnected( bool v ) { m_jointDef.collideConnected = v; };
        FORCEINLINE float getReferenceAngle() { return m_jointDef.referenceAngle; };
        FORCEINLINE void setReferenceAngle( float v ) { m_jointDef.referenceAngle = v; };
        FORCEINLINE b2Vec2 getLocalAnchorA() { return m_jointDef.localAnchorA; };
        FORCEINLINE void setLocalAnchorA( b2Vec2 v ) { m_jointDef.localAnchorA = v; };
        FORCEINLINE b2Vec2 getLocalAnchorB() { return m_jointDef.localAnchorB; };
        FORCEINLINE void setLocalAnchorB( b2Vec2 v ) { m_jointDef.localAnchorB = v; };
        FORCEINLINE bool isLimitEnabled() { return m_joint ? m_joint->IsLimitEnabled() : m_jointDef.enableLimit; };
        FORCEINLINE void setLimitEnabled( bool v ) { if( m_joint ) m_joint->EnableLimit( v ); else m_jointDef.enableLimit = v; };
        FORCEINLINE float getLowerLimit() { return m_joint ? m_joint->GetLowerLimit() : m_jointDef.lowerAngle; };
        FORCEINLINE void setLowerLimit( float v ) { if( m_joint ) m_joint->SetLimits( v, m_joint->GetUpperLimit() ); else m_jointDef.lowerAngle = v; };
        FORCEINLINE float getUpperLimit() { return m_joint ? m_joint->GetUpperLimit() : m_jointDef.upperAngle; };
        FORCEINLINE void setUpperLimit( float v ) { if( m_joint ) m_joint->SetLimits( m_joint->GetLowerLimit(), v ); else m_jointDef.upperAngle = v; };
        FORCEINLINE bool isMotorEnabled() { return m_joint ? m_joint->IsMotorEnabled() : m_jointDef.enableMotor; };
        FORCEINLINE void setMotorEnabled( bool v ) { if( m_joint ) m_joint->EnableMotor( v ); else m_jointDef.enableMotor = v; };
        FORCEINLINE float getMotorSpeed() { return m_joint ? m_joint->GetMotorSpeed() : m_jointDef.motorSpeed; };
        FORCEINLINE void setMotorSpeed( float v ) { if( m_joint ) m_joint->SetMotorSpeed( v ); else m_jointDef.motorSpeed = v; };
        FORCEINLINE float getMotorTorque() { return m_joint ? m_joint->GetMaxMotorTorque() : m_jointDef.maxMotorTorque; };
        FORCEINLINE void setMotorTorque( float v ) { if( m_joint ) m_joint->SetMaxMotorTorque( v ); else m_jointDef.maxMotorTorque = v; };

        META_PROPERTY(
            META_ATTR_DESCRIPTION( "First binding body ID." )
        )
        XeCore::Common::Property< std::string, RevoluteJoint > BindingA;

        META_PROPERTY(
            META_ATTR_DESCRIPTION( "Second binding body ID." )
        )
        XeCore::Common::Property< std::string, RevoluteJoint > BindingB;

        META_PROPERTY(
            META_ATTR_DESCRIPTION( "Determines if connected bodies should collide." ),
            META_ATTR_DEFAULT_VALUE( "false" )
        )
        XeCore::Common::Property< bool, RevoluteJoint > CollideConnected;

        META_PROPERTY(
            META_ATTR_DESCRIPTION( "Reference angle." ),
            META_ATTR_DEFAULT_VALUE( "0" )
        )
        XeCore::Common::Property< float, RevoluteJoint > ReferenceAngle;

        META_PROPERTY(
            META_ATTR_DESCRIPTION( "Local anchor of the first body." ),
            META_ATTR_DEFAULT_VALUE( "[0, 0]" )
        )
        XeCore::Common::Property< b2Vec2, RevoluteJoint > LocalAnchorA;

        META_PROPERTY(
            META_ATTR_DESCRIPTION( "Local anchor of the second body." ),
            META_ATTR_DEFAULT_VALUE( "[0, 0]" )
        )
        XeCore::Common::Property< b2Vec2, RevoluteJoint > LocalAnchorB;

        META_PROPERTY(
            META_ATTR_DESCRIPTION( "Determines if angle difference limits are enabled." ),
            META_ATTR_DEFAULT_VALUE( "false" )
        )
        XeCore::Common::Property< bool, RevoluteJoint > LimitEnabled;

        META_PROPERTY(
            META_ATTR_DESCRIPTION( "Lower angle difference limit." ),
            META_ATTR_DEFAULT_VALUE( "0" )
        )
        XeCore::Common::Property< float, RevoluteJoint > LowerLimit;

        META_PROPERTY(
            META_ATTR_DESCRIPTION( "Uper angle difference limit." ),
            META_ATTR_DEFAULT_VALUE( "0" )
        )
        XeCore::Common::Property< float, RevoluteJoint > UpperLimit;

        META_PROPERTY(
            META_ATTR_DESCRIPTION( "Determines if motor is enabled." ),
            META_ATTR_DEFAULT_VALUE( "false" )
        )
        XeCore::Common::Property< bool, RevoluteJoint > MotorEnabled;

        META_PROPERTY(
            META_ATTR_DESCRIPTION( "Motor speed." ),
            META_ATTR_DEFAULT_VALUE( "0" )
        )
        XeCore::Common::Property< float, RevoluteJoint > MotorSpeed;

        META_PROPERTY(
            META_ATTR_DESCRIPTION( "Motor torque." ),
            META_ATTR_DEFAULT_VALUE( "0" )
        )
        XeCore::Common::Property< float, RevoluteJoint > MotorTorque;

    protected:
        virtual Json::Value onSerialize( const std::string& property );
        virtual void onDeserialize( const std::string& property, const Json::Value& root );

        virtual void onCreate();
        virtual void onDestroy();
        virtual void onDuplicate( Component* dst );
        virtual void onJointGoodbye( b2Joint* joint );

    private:
        b2RevoluteJoint* m_joint;
        b2RevoluteJointDef m_jointDef;
        std::string m_bindingA;
        std::string m_bindingB;
    };

}

#endif
