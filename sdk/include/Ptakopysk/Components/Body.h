#ifndef __PTAKOPYSK__BODY__
#define __PTAKOPYSK__BODY__

#include "Component.h"
#include <Box2D/Box2D.h>

namespace Ptakopysk
{

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
        FORCEINLINE float getDensity() { return m_density; };
        FORCEINLINE void setDensity( float v ) { m_density = v; if( m_fixture ) m_fixture->SetDensity( v ); };
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
        FORCEINLINE b2Filter getFilter() { return m_fixture ? m_fixture->GetFilterData() : m_filter; };
        FORCEINLINE void setFilter( b2Filter v ) { if( m_fixture ) m_fixture->SetFilterData( v ); else m_filter = v; };
        FORCEINLINE b2Vec2 getPosition() { return m_body ? m_body->GetPosition() : m_bodyDef.position; };
        FORCEINLINE float getAngle() { return RADTODEG( m_body ? m_body->GetAngle() : m_bodyDef.angle ); };

        XeCore::Common::Property< VerticesData&, Body > Vertices;
        XeCore::Common::Property< float, Body > Radius;
        XeCore::Common::Property< float, Body > Density;
        XeCore::Common::Property< b2BodyType, Body > BodyType;
        XeCore::Common::Property< b2Vec2, Body > LinearVelocity;
        XeCore::Common::Property< float, Body > AngularVelocity;
        XeCore::Common::Property< float, Body > LinearDamping;
        XeCore::Common::Property< float, Body > AngularDamping;
        XeCore::Common::Property< bool, Body > IsSleepingAllowed;
        XeCore::Common::Property< bool, Body > IsFixedRotation;
        XeCore::Common::Property< bool, Body > IsBullet;
        XeCore::Common::Property< float, Body > GravityScale;
        XeCore::Common::Property< b2Filter, Body > Filter;

    protected:
        virtual Json::Value onSerialize( const std::string& property );
        virtual void onDeserialize( const std::string& property, const Json::Value& root );

        virtual void onCreate();
        virtual void onDestroy();
        virtual void onDuplicate( Component* dst );
        virtual void onFixtureGoodbye( b2Fixture* fixture );

    private:
        float m_density;
        b2Body* m_body;
        b2Fixture* m_fixture;
        b2Shape* m_shape;
        b2BodyDef m_bodyDef;
        VerticesData m_verts;
        float m_radius;
        b2Filter m_filter;
    };

}

#endif
