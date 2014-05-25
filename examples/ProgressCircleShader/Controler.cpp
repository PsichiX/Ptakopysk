#include "Controler.h"
#include <Ptakopysk/System/GameObject.h>
#include <Ptakopysk/System/GameManager.h>
#include <Ptakopysk/System/Assets.h>
#include <Ptakopysk/Components/Transform.h>
#include <Ptakopysk/Components/SpriteRenderer.h>
#include <Ptakopysk/Components/TextRenderer.h>
#include <sstream>

RTTI_CLASS_DERIVATIONS( Controler,
						RTTI_DERIVATION( Component ),
						RTTI_DERIVATIONS_END
						)

Controler::Controler()
: RTTI_CLASS_DEFINE( Controler )
, Component( Component::tUpdate | Component::tEvents )
, m_status( 0 )
, m_phase( 0.0f )
, m_current( 0 )
{
    m_shaders.push_back( Assets::use().getShader( "progressCircle0" ) );
    m_shaders.push_back( Assets::use().getShader( "progressCircle1" ) );
    m_shaders.push_back( Assets::use().getShader( "progressCircle2" ) );
    m_shaders.push_back( Assets::use().getShader( "progressCircle3" ) );
    m_shaders.push_back( Assets::use().getShader( "progressCircle4" ) );
    m_shaders.push_back( Assets::use().getShader( "progressCircle5" ) );
    m_shaders.push_back( Assets::use().getShader( "progressCircle6" ) );
    m_shaders.push_back( Assets::use().getShader( "progressCircle" ) );
}

Controler::~Controler()
{
}

Json::Value Controler::onSerialize( const std::string& property )
{
    return Component::onSerialize( property );
}

void Controler::onDeserialize( const std::string& property, const Json::Value& root )
{
    Component::onDeserialize( property, root );
}

void Controler::onCreate()
{
    m_status = getGameObject()->getGameManagerRoot()->getGameObject( "status" );
    applyShader();
    applyStatus();
}

void Controler::onDuplicate( Component* dst )
{
	if( !dst )
		return;
	Component::onDuplicate( dst );
}

void Controler::onEvent( const sf::Event& event )
{
    if( event.type == sf::Event::KeyPressed )
    {
        if( event.key.code == sf::Keyboard::Left && !m_shaders.empty() )
        {
            if( m_current > 0 )
                m_current--;
            applyShader();
            applyStatus();
        }
        else if( event.key.code == sf::Keyboard::Right && !m_shaders.empty() )
        {
            if( m_current < m_shaders.size() - 1 )
                m_current++;
            applyShader();
            applyStatus();
        }
    }
}

void Controler::onUpdate( float dt )
{
    if( !m_shaders.empty() && m_current == m_shaders.size() - 1 )
        m_phase += dt * 90.0f;
    else
        m_phase = 0.0f;
    Transform* trans = getGameObject()->getComponent< Transform >();
    if( trans )
        trans->setRotation( m_phase );
}

void Controler::applyShader()
{
    SpriteRenderer* spr = getGameObject()->getComponent< SpriteRenderer >();
    if( spr )
    {
        sf::Shader* s = m_current < m_shaders.size() ? m_shaders[ m_current ] : 0;
        if( s )
        {
            sf::RenderStates rs = spr->getRenderStates();
            rs.shader = s;
            spr->setRenderStates( rs );
        }
    }
}

void Controler::applyStatus()
{
    if( !m_status )
        return;
    TextRenderer* txt = m_status->getComponent< TextRenderer >();
    if( txt )
    {
        std::stringstream ss;
        ss << "Step: " << m_current;
        txt->setText( ss.str() );
    }
}
