#include "HeartControler.h"
#include <Ptakopysk/System/GameObject.h>
#include <Ptakopysk/Components/SpriteRenderer.h>
#include <Ptakopysk/Components/Transform.h>
#include <Ptakopysk/System/Tween.h>
#include <sstream>

RTTI_CLASS_DERIVATIONS( HeartControler,
						RTTI_DERIVATION( Component ),
						RTTI_DERIVATIONS_END
						)

HeartControler::HeartControler()
: RTTI_CLASS_DEFINE( HeartControler )
, Component( Component::tUpdate )
, Size( this, &HeartControler::getSize, &HeartControler::setSize )
, PingSize( this, &HeartControler::getPingSize, &HeartControler::setPingSize )
, PongSize( this, &HeartControler::getPongSize, &HeartControler::setPongSize )
, Duration( this, &HeartControler::getDuration, &HeartControler::setDuration )
, PingDuration( this, &HeartControler::getPingDuration, &HeartControler::setPingDuration )
, PongDuration( this, &HeartControler::getPongDuration, &HeartControler::setPongDuration )
, Timer( this, &HeartControler::getTimer, &HeartControler::setTimer )
, m_size( 1.0f )
, m_pingSize( 1.0f )
, m_pongSize( 1.0f )
, m_duration( 0.0f )
, m_pingDuration( 0.0f )
, m_pongDuration( 0.0f )
, m_timer( 0.0f )
{
	serializableProperty( "Size" );
	serializableProperty( "PingSize" );
	serializableProperty( "PongSize" );
	serializableProperty( "Duration" );
	serializableProperty( "PingDuration" );
	serializableProperty( "PongDuration" );
	serializableProperty( "Timer" );
}

HeartControler::~HeartControler()
{
}

void HeartControler::setSize( float v )
{
    m_size = v;
    if( !getGameObject() )
        return;
    SpriteRenderer* spr = getGameObject()->getComponent< SpriteRenderer >();
	if( spr )
	    spr->getMaterial().set( "uSize", m_size );
}

void HeartControler::onCreate()
{
    setSize( m_size );
}

Json::Value HeartControler::onSerialize( const std::string& property )
{
	if( property == "Size" )
		return Json::Value( m_size );
	else if( property == "PingSize" )
		return Json::Value( m_pingSize );
	else if( property == "PongSize" )
		return Json::Value( m_pongSize );
	else if( property == "Duration" )
		return Json::Value( m_duration );
	else if( property == "PingDuration" )
		return Json::Value( m_pingDuration );
	else if( property == "PongDuration" )
		return Json::Value( m_pongDuration );
	else if( property == "Timer" )
		return Json::Value( m_timer );
	else
		return Component::onSerialize( property );
}

void HeartControler::onDeserialize( const std::string& property, const Json::Value& root )
{
	if( property == "Size" && root.isNumeric() )
		setSize( (float)root.asDouble() );
	else if( property == "PingSize" && root.isNumeric() )
		setPingSize( (float)root.asDouble() );
	else if( property == "PongSize" && root.isNumeric() )
		setPongSize( (float)root.asDouble() );
	else if( property == "Duration" && root.isNumeric() )
		setDuration( (float)root.asDouble() );
	else if( property == "PingDuration" && root.isNumeric() )
		setPingDuration( (float)root.asDouble() );
	else if( property == "PongDuration" && root.isNumeric() )
		setPongDuration( (float)root.asDouble() );
	else if( property == "Timer" && root.isNumeric() )
		setTimer( (float)root.asDouble() );
	else
		Component::onDeserialize( property, root );
}

void HeartControler::onDuplicate( Component* dst )
{
	if( !dst )
		return;
	Component::onDuplicate( dst );
	if( !XeCore::Common::IRtti::isDerived< HeartControler >( dst ) )
		return;
	HeartControler* c = (HeartControler*)dst;
	c->setSize( getSize() );
	c->setPingSize( getPingSize() );
	c->setPongSize( getPongSize() );
	c->setDuration( getDuration() );
	c->setPingDuration( getPingDuration() );
	c->setPongDuration( getPongDuration() );
	c->setTimer( getTimer() );
}

void HeartControler::onUpdate( float dt )
{
    if( m_duration <= 0.0f )
        return;
    m_timer += dt;
    if( m_timer >= m_duration )
    {
        m_timer = 0.0f;
        Tweener::use().killTweensOf( Size );
        Tweener::use().startTween( (xnew TweenSequence())->add(
            xnew Tween< float, HeartControler, Math::Easing::Back::inOut >( Size, m_pingSize, m_pingDuration )
        )->add(
            xnew Tween< float, HeartControler, Math::Easing::Back::inOut >( Size, m_pongSize, m_pongDuration )
        ) );
    }
}
