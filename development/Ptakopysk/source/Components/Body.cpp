#include "../../include/Ptakopysk/Components/Body.h"
#include "../../include/Ptakopysk/System/GameObject.h"
#include "../../include/Ptakopysk/System/GameManager.h"

namespace Ptakopysk
{

    RTTI_CLASS_DERIVATIONS( Body,
                            RTTI_DERIVATION( Component ),
                            RTTI_DERIVATIONS_END
                            )

    Body::Body()
    : RTTI_CLASS_DEFINE( Body )
    , Component( Component::Physics )
    , Vertices( this, &Body::getVertices, &Body::setVertices )
    , Density( this, &Body::getDensity, &Body::setDensity )
    , BodyType( this, &Body::getBodyType, &Body::setBodyType )
    , LinearVelocity( this, &Body::getLinearVelocity, &Body::setLinearVelocity )
    , AngularVelocity( this, &Body::getAngularVelocity, &Body::setAngularVelocity )
    , LinearDamping( this, &Body::getLinearDamping, &Body::setLinearDamping )
    , AngularDamping( this, &Body::getAngularDamping, &Body::setAngularDamping )
    , IsSleepingAllowed( this, &Body::isSleepingAllowed, &Body::setSleepingAllowed )
    , IsFixedRotation( this, &Body::isFixedRotation, &Body::setFixedRotation )
    , IsBullet( this, &Body::isBullet, &Body::setBullet )
    , GravityScale( this, &Body::getGravityScale, &Body::setGravityScale )
    , m_density( 1.0f )
    , m_body( 0 )
    , m_fixture( 0 )
    , m_shape( 0 )
    {
        serializableProperty( "Vertices" );
        serializableProperty( "Density" );
        serializableProperty( "BodyType" );
        serializableProperty( "LinearVelocity" );
        serializableProperty( "AngularVelocity" );
        serializableProperty( "LinearDamping" );
        serializableProperty( "AngularDamping" );
        serializableProperty( "IsSleepingAllowed" );
        serializableProperty( "IsFixedRotation" );
        serializableProperty( "IsBullet" );
        serializableProperty( "GravityScale" );
    }

    Body::~Body()
    {
        onDestroy();
    }

    void Body::setVertices( VerticesData& verts )
    {
        if( &verts != &m_verts )
        {
            m_verts.resize( 0 );
            if( !verts.empty() )
                m_verts.assign( verts.begin(), verts.end() );
        }
        applyVertices();
    }

    void Body::applyVertices()
    {
        onCreate();
    }

    Json::Value Body::onSerialize( const std::string& property )
    {
        if( property == "Vertices" )
        {
            Json::Value v( Json::arrayValue );
            for( VerticesData::iterator it = m_verts.begin(); it != m_verts.end(); it++ )
            {
                Json::Value iv;
                iv.append( Json::Value( it->x ) );
                iv.append( Json::Value( it->y ) );
                v.append( iv );
            }
            return v;
        }
        else if( property == "Density" )
            return Json::Value( m_density );
        else if( property == "BodyType" )
        {
            Serialized::ICustomSerializer* s = Serialized::getCustomSerializer( "b2BodyType" );
            if( s )
            {
                b2BodyType bt = getBodyType();
                return s->serialize( &bt );
            }
            return Json::Value::null;
        }
        else if( property == "LinearVelocity" )
        {
            b2Vec2 p = getLinearVelocity();
            Json::Value v;
            v.append( Json::Value( p.x ) );
            v.append( Json::Value( p.y ) );
            return v;
        }
        else if( property == "AngularVelocity" )
            return Json::Value( getAngularVelocity() );
        else if( property == "LinearDamping" )
            return Json::Value( getLinearDamping() );
        else if( property == "AngularDamping" )
            return Json::Value( getAngularDamping() );
        else if( property == "IsSleepingAllowed" )
            return Json::Value( isSleepingAllowed() );
        else if( property == "IsFixedRotation" )
            return Json::Value( isFixedRotation() );
        else if( property == "IsBullet" )
            return Json::Value( isBullet() );
        else if( property == "GravityScale" )
            return Json::Value( getGravityScale() );
        else
            return Component::onSerialize( property );
    }

    void Body::onDeserialize( const std::string& property, const Json::Value& root )
    {
        if( property == "Vertices" && root.isArray() )
        {
            VerticesData v;
            v.reserve( root.size() );
            Json::Value item;
            for( unsigned int i = 0; i < root.size(); i++ )
            {
                item = root[ i ];
                if( item.isArray() && item.size() == 2 )
                    v.push_back( b2Vec2(
                        (float)item[ 0u ].asDouble(),
                        (float)item[ 1u ].asDouble()
                    ) );
                else
                    v.push_back( b2Vec2() );
            }
            setVertices( v );
        }
        else if( property == "Density" && root.isNumeric() )
            setDensity( (float)root.asDouble() );
        else if( property == "BodyType" && root.isString() )
        {
            Serialized::ICustomSerializer* s = Serialized::getCustomSerializer( "b2BodyType" );
            if( s )
            {
                b2BodyType bt = getBodyType();
                s->deserialize( &bt, root );
                setBodyType( bt );
            }
        }
        else if( property == "LinearVelocity" && root.isArray() && root.size() == 2 )
            setLinearVelocity( b2Vec2( (float)root[ 0u ].asDouble(), (float)root[ 1u ].asDouble() ) );
        else if( property == "AngularVelocity" && root.isNumeric() )
            setAngularVelocity( (float)root.asDouble() );
        else if( property == "LinearDamping" && root.isNumeric() )
            setLinearDamping( (float)root.asDouble() );
        else if( property == "AngularDamping" && root.isNumeric() )
            setAngularDamping( (float)root.asDouble() );
        else if( property == "IsSleepingAllowed" && root.isBool() )
            setSleepingAllowed( root.asBool() );
        else if( property == "IsFixedRotation" && root.isBool() )
            setFixedRotation( root.asBool() );
        else if( property == "IsBullet" && root.isBool() )
            setBullet( root.asBool() );
        else if( property == "GravityScale" && root.isNumeric() )
            setGravityScale( (float)root.asDouble() );
        else
            Component::onDeserialize( property, root );
    }

    void Body::onCreate()
    {
        if( !getGameObject() )
            return;
        onDestroy();
        m_body = getGameObject()->getGameManager()->getPhysicsWorld()->CreateBody( &m_bodyDef );
        m_shape = xnew b2PolygonShape();
        if( m_verts.size() )
        {
            m_shape->Set( m_verts.data(), m_verts.size() );
            m_fixture = m_body->CreateFixture( m_shape, 1.0f );
        }
        m_body->SetUserData( this );
    }

    void Body::onDestroy()
    {
        if( !getGameObject() )
            return;
        if( m_body )
        {
            m_bodyDef.type = m_body->GetType();
            m_bodyDef.linearVelocity = m_body->GetLinearVelocity();
            m_bodyDef.angularVelocity = m_body->GetAngularVelocity();
            m_bodyDef.linearDamping = m_body->GetLinearDamping();
            m_bodyDef.angularDamping = m_body->GetAngularDamping();
            m_bodyDef.allowSleep = m_body->IsSleepingAllowed();
            m_bodyDef.fixedRotation = m_body->IsFixedRotation();
            m_bodyDef.bullet = m_body->IsBullet();
            m_bodyDef.gravityScale = m_body->GetGravityScale();
            m_bodyDef.position = m_body->GetPosition();
            m_bodyDef.angle = m_body->GetAngle();
            if( m_fixture )
            {
                m_density = m_fixture->GetDensity();
                m_body->DestroyFixture( m_fixture );
            }
            getGameObject()->getGameManager()->getPhysicsWorld()->DestroyBody( m_body );
        }
        m_body = 0;
        m_fixture = 0;
        DELETE_OBJECT( m_shape );
    }

    void Body::onDuplicate( Component* dst )
    {
        if( !dst )
            return;
        Component::onDuplicate( dst );
        if( !XeCore::Common::IRtti::isDerived< Body >( dst ) )
            return;
        Body* c = (Body*)dst;
        c->setVertices( getVertices() );
        c->setDensity( getDensity() );
        c->setBodyType( getBodyType() );
        c->setLinearVelocity( getLinearVelocity() );
        c->setAngularVelocity( getAngularVelocity() );
        c->setLinearDamping( getLinearDamping() );
        c->setAngularDamping( getAngularDamping() );
        c->setSleepingAllowed( isSleepingAllowed() );
        c->setFixedRotation( isFixedRotation() );
        c->setBullet( isBullet() );
        c->setGravityScale( getGravityScale() );
    }

}