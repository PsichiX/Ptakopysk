#ifndef __PTAKOPYSK__BODY__
#define __PTAKOPYSK__BODY__

#include "Component.h"
#include <Box2D/Box2D.h>

namespace Ptakopysk
{

    META_COMPONENT(
        META_ATTR_DESCRIPTION( "Physics body component." ),
        META_ATTR_FUNCTIONALITY_TRIGGERS( "Make as circle|Make as box from SpriteRenderer|Make as box from TextRenderer" )
    )
    class Body
    : public virtual XeCore::Common::IRtti
    , public virtual XeCore::Common::MemoryManager::Manageable
    , public Component
    {
        RTTI_CLASS_DECLARE( Body );

    public:
        typedef std::vector< b2Vec2 > VerticesData;

        Body();
        virtual ~Body();

        FORCEINLINE static Component* onBuildComponent() { return xnew Body(); }

        FORCEINLINE b2Body* getBody() { return m_body; };
        FORCEINLINE b2Shape* getShape() { return m_shape; };
        FORCEINLINE VerticesData& getVertices() { return m_verts; };
        void setVertices( VerticesData& verts );
        void applyVertices();
        FORCEINLINE float getRadius() { return m_shape ? m_shape->m_radius : m_radius; };
        FORCEINLINE void setRadius( float v ) { m_radius = v; if( m_shape ) m_shape->m_radius = v; };
        FORCEINLINE float getDensity() { return m_fixture ? m_fixture->GetDensity() : m_fixtureDef.density; };
        FORCEINLINE void setDensity( float v ) { if( m_fixture ) m_fixture->SetDensity( v ); else m_fixtureDef.density = v; };
        FORCEINLINE float getFriction() { return  m_fixture ? m_fixture->GetFriction() : m_fixtureDef.friction; };
        FORCEINLINE void setFriction( float v ) { if( m_fixture ) m_fixture->SetFriction( v ); else m_fixtureDef.friction = v; };
        FORCEINLINE float getRestitution() { return  m_fixture ? m_fixture->GetRestitution() : m_fixtureDef.restitution; };
        FORCEINLINE void setRestitution( float v ) { if( m_fixture ) m_fixture->SetRestitution( v ); else m_fixtureDef.restitution = v; };
        FORCEINLINE b2Filter getFilter() { return m_fixture ? m_fixture->GetFilterData() : m_fixtureDef.filter; };
        FORCEINLINE void setFilter( b2Filter v ) { if( m_fixture ) m_fixture->SetFilterData( v ); else m_fixtureDef.filter = v; };
        FORCEINLINE b2BodyType getBodyType() { return m_body ? m_body->GetType() : m_bodyDef.type; };
        FORCEINLINE void setBodyType( b2BodyType v ) { if( m_body ) m_body->SetType( v ); else m_bodyDef.type = v; };
        FORCEINLINE bool isCircleShape() { return m_shape ? m_shape->GetType() == b2Shape::e_circle : !m_verts.size(); };
        FORCEINLINE b2Vec2 getLinearVelocity() { return m_body ? m_body->GetLinearVelocity() : m_bodyDef.linearVelocity; };
        FORCEINLINE void setLinearVelocity( b2Vec2 v ) { if( m_body ) m_body->SetLinearVelocity( v ); else m_bodyDef.linearVelocity = v; };
        FORCEINLINE float getAngularVelocity() { return m_body ? m_body->GetAngularVelocity() : m_bodyDef.angularVelocity; };
        FORCEINLINE void setAngularVelocity( float v ) { if( m_body ) m_body->SetAngularVelocity( v ); else m_bodyDef.angularVelocity = v; };
        FORCEINLINE float getLinearDamping() { return m_body ? m_body->GetLinearDamping() : m_bodyDef.linearDamping; };
        FORCEINLINE void setLinearDamping( float v ) { if( m_body ) m_body->SetLinearDamping( v ); else m_bodyDef.linearDamping = v; };
        FORCEINLINE float getAngularDamping() { return m_body ? m_body->GetAngularDamping() : m_bodyDef.angularDamping; };
        FORCEINLINE void setAngularDamping( float v ) { if( m_body ) m_body->SetAngularDamping( v ); else m_bodyDef.angularDamping = v; };
        FORCEINLINE bool isSleepingAllowed() { return m_body ? m_body->IsSleepingAllowed() : m_bodyDef.allowSleep; };
        FORCEINLINE void setSleepingAllowed( bool v ) { if( m_body ) m_body->SetSleepingAllowed( v ); else m_bodyDef.allowSleep = v; };
        FORCEINLINE bool isFixedRotation() { return m_body ? m_body->IsFixedRotation() : m_bodyDef.fixedRotation; };
        FORCEINLINE void setFixedRotation( bool v ) { if( m_body ) m_body->SetFixedRotation( v ); else m_bodyDef.fixedRotation = v; };
        FORCEINLINE bool isBullet() { return m_body ? m_body->IsBullet() : m_bodyDef.bullet; };
        FORCEINLINE void setBullet( bool v ) { if( m_body ) m_body->SetBullet( v ); else m_bodyDef.bullet = v; };
        FORCEINLINE float getGravityScale() { return m_body ? m_body->GetGravityScale() : m_bodyDef.gravityScale; };
        FORCEINLINE void setGravityScale( float v ) { if( m_body ) m_body->SetGravityScale( v ); else m_bodyDef.gravityScale = v; };
        FORCEINLINE b2Vec2 getPosition() { return m_body ? m_body->GetPosition() : m_bodyDef.position; };
        FORCEINLINE void setPosition( const b2Vec2& v ) { if( m_body ) m_body->SetTransform( v, getAngle() ); else m_bodyDef.position = v; };
        FORCEINLINE float getAngle() { return m_body ? m_body->GetAngle() : m_bodyDef.angle; };
        FORCEINLINE void setAngle( float v ) { if( m_body ) m_body->SetTransform( getPosition(), v ); else m_bodyDef.angle = v; };

        META_PROPERTY(
            META_ATTR_DESCRIPTION( "Contains array of body shape vertices." ),
            META_ATTR_DEFAULT_VALUE( "null" ),
            META_ATTR_VALUE_TYPE( "@Mesh<System.Single>" )
        )
        XeCore::Common::Property< VerticesData&, Body > Vertices;

        META_PROPERTY(
            META_ATTR_DESCRIPTION( "Radius of the body (if radius is greater than 0, body drops current vertices and becomes a ball)." ),
            META_ATTR_DEFAULT_VALUE( "0" )
        )
        XeCore::Common::Property< float, Body > Radius;

        META_PROPERTY(
            META_ATTR_DESCRIPTION( "Density." ),
            META_ATTR_DEFAULT_VALUE( "0" )
        )
        XeCore::Common::Property< float, Body > Density;

        META_PROPERTY(
            META_ATTR_DESCRIPTION( "Friction." ),
            META_ATTR_DEFAULT_VALUE( "0.2" )
        )
        XeCore::Common::Property< float, Body > Friction;

        META_PROPERTY(
            META_ATTR_DESCRIPTION( "Restitution." ),
            META_ATTR_DEFAULT_VALUE( "0" )
        )
        XeCore::Common::Property< float, Body > Restitution;

        META_PROPERTY(
            META_ATTR_DESCRIPTION( "Collision filter." )
        )
        XeCore::Common::Property< b2Filter, Body > Filter;

        META_PROPERTY(
            META_ATTR_DESCRIPTION( "Body type." ),
            META_ATTR_DEFAULT_VALUE( "\"b2_staticBody\"" ),
            META_ATTR_VALUE_TYPE( "@Enum:[ \"b2_staticBody\", \"b2_kinematicBody\", \"b2_dynamicBody\" ]" )
        )
        XeCore::Common::Property< b2BodyType, Body > BodyType;

        META_PROPERTY(
            META_ATTR_DESCRIPTION( "Linear velocity." ),
            META_ATTR_DEFAULT_VALUE( "[0, 0]" )
        )
        XeCore::Common::Property< b2Vec2, Body > LinearVelocity;

        META_PROPERTY(
            META_ATTR_DESCRIPTION( "Angular velocity." ),
            META_ATTR_DEFAULT_VALUE( "0" )
        )
        XeCore::Common::Property< float, Body > AngularVelocity;

        META_PROPERTY(
            META_ATTR_DESCRIPTION( "Linear damping." ),
            META_ATTR_DEFAULT_VALUE( "0" )
        )
        XeCore::Common::Property< float, Body > LinearDamping;

        META_PROPERTY(
            META_ATTR_DESCRIPTION( "Angular damping." ),
            META_ATTR_DEFAULT_VALUE( "0" )
        )
        XeCore::Common::Property< float, Body > AngularDamping;

        META_PROPERTY(
            META_ATTR_DESCRIPTION( "Determines if sleeping is allowed for this body." ),
            META_ATTR_DEFAULT_VALUE( "true" )
        )
        XeCore::Common::Property< bool, Body > IsSleepingAllowed;

        META_PROPERTY(
            META_ATTR_DESCRIPTION( "Determines if body have fixed rotation." ),
            META_ATTR_DEFAULT_VALUE( "false" )
        )
        XeCore::Common::Property< bool, Body > IsFixedRotation;

        META_PROPERTY(
            META_ATTR_DESCRIPTION( "Determines if body is a bullet." ),
            META_ATTR_DEFAULT_VALUE( "false" )
        )
        XeCore::Common::Property< bool, Body > IsBullet;

        META_PROPERTY(
            META_ATTR_DESCRIPTION( "Gravity scale." ),
            META_ATTR_DEFAULT_VALUE( "0" )
        )
        XeCore::Common::Property< float, Body > GravityScale;

    protected:
        virtual Json::Value onSerialize( const std::string& property );
        virtual void onDeserialize( const std::string& property, const Json::Value& root );

        virtual void onCreate();
        virtual void onDestroy();
        virtual void onUpdate( float dt );
        virtual void onDuplicate( Component* dst );
        virtual void onRenderEditor( sf::RenderTarget* target );
        virtual void onFixtureGoodbye( b2Fixture* fixture );
        virtual bool onTriggerFunctionality( const std::string& name );

    private:
        float m_radius;
        b2BodyDef m_bodyDef;
        b2FixtureDef m_fixtureDef;
        VerticesData m_verts;
        b2Body* m_body;
        b2Fixture* m_fixture;
        b2Shape* m_shape;
    };

}

#endif
