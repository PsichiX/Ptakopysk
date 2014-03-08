#include "../../include/Ptakopysk/Components/SpriteRenderer.h"
#include "../../include/Ptakopysk/Components/Transform.h"
#include "../../include/Ptakopysk/System/GameObject.h"
#include "../../include/Ptakopysk/System/Assets.h"

namespace Ptakopysk
{

    RTTI_CLASS_DERIVATIONS( SpriteRenderer,
                            RTTI_DERIVATION( Component ),
                            RTTI_DERIVATIONS_END
                            )

    SpriteRenderer::SpriteRenderer()
    : RTTI_CLASS_DEFINE( SpriteRenderer )
    , Component( Component::tUpdate | Component::tTransform | Component::tRender )
    , Texture( this, &SpriteRenderer::getTexture, &SpriteRenderer::setTexture )
    , Size( this, &SpriteRenderer::getSize, &SpriteRenderer::setSize )
    , Origin( this, &SpriteRenderer::getOrigin, &SpriteRenderer::setOrigin )
    , OriginPercent( this, &SpriteRenderer::getOriginPercent, &SpriteRenderer::setOriginPercent )
    , Color( this, &SpriteRenderer::getColor, &SpriteRenderer::setColor )
    , RenderStates( this, &SpriteRenderer::getRenderStates, &SpriteRenderer::setRenderStates )
    , m_renderStates( sf::RenderStates::Default )
    {
        serializableProperty( "RenderStates" );
        serializableProperty( "Color" );
        serializableProperty( "Texture" );
        serializableProperty( "Size" );
        serializableProperty( "OriginPercent" );
        serializableProperty( "Origin" );
        m_shape = xnew sf::RectangleShape();
        setTexture( 0 );
    }

    SpriteRenderer::~SpriteRenderer()
    {
        DELETE_OBJECT( m_shape );
    }

    sf::Texture* SpriteRenderer::getTexture()
    {
        sf::Texture* t = (sf::Texture*)m_shape->getTexture();
        return t == Assets::use().getDefaultTexture() ? 0 : t;
    }

    void SpriteRenderer::setTexture( sf::Texture* tex )
    {
        sf::Texture* t = tex ? tex : Assets::use().getDefaultTexture();
        m_shape->setTexture( t );
        if( t )
        {
            sf::Vector2u s = t->getSize();
            m_shape->setTextureRect( sf::IntRect( 0, 0, s.x, s.y ) );
        }
    }

    void SpriteRenderer::setSize( sf::Vector2f size )
    {
        sf::Texture* t = getTexture();
        if( size.x < 0.0f )
            size.x = t ? t->getSize().x : 0.0f;
        if( size.y < 0.0f )
            size.y = t ? t->getSize().y : 0.0f;
        m_shape->setSize( size );
    }

    sf::Vector2f SpriteRenderer::getOriginPercent()
    {
        sf::Vector2f o = getOrigin();
        sf::Vector2f s = getSize();
        o.x = s.x > 0.0f ? o.x / s.x : 0.0f;
        o.y = s.y > 0.0f ? o.y / s.y : 0.0f;
        return o;
    }

    void SpriteRenderer::setOriginPercent( sf::Vector2f origin )
    {
        sf::Vector2f s = getSize();
        setOrigin( sf::Vector2f( s.x * origin.x, s.y * origin.y ) );
    }

    Json::Value SpriteRenderer::onSerialize( const std::string& property )
    {
        if( property == "Texture" )
            return Json::Value( Assets::use().findTexture( getTexture() ) );
        else if( property == "Size" )
        {
            sf::Vector2f s = getSize();
            Json::Value v;
            v.append( Json::Value( s.x ) );
            v.append( Json::Value( s.y ) );
            return v;
        }
        else if( property == "Origin" )
        {
            sf::Vector2f o = getOrigin();
            Json::Value v;
            v.append( Json::Value( o.x ) );
            v.append( Json::Value( o.y ) );
            return v;
        }
        else if( property == "OriginPercent" )
        {
            sf::Vector2f o = getOriginPercent();
            Json::Value v;
            v.append( Json::Value( o.x ) );
            v.append( Json::Value( o.y ) );
            return v;
        }
        else if( property == "Color" )
        {
            sf::Color c = getColor();
            Json::Value v;
            v.append( Json::Value( c.r ) );
            v.append( Json::Value( c.g ) );
            v.append( Json::Value( c.b ) );
            v.append( Json::Value( c.a ) );
            return v;
        }
        else if( property == "RenderStates" )
        {
            Json::Value v;
            v[ "blendMode" ] = Serialized::serializeCustom< sf::BlendMode >( "BlendMode", m_renderStates.blendMode );
            v[ "shader" ] = Json::Value( Assets::use().findShader( m_renderStates.shader ) );
            return v;
        }
        else
            return Component::onSerialize( property );
    }

    void SpriteRenderer::onDeserialize( const std::string& property, const Json::Value& root )
    {
        if( property == "Texture" && root.isString() )
            setTexture( Assets::use().getTexture( root.asString() ) );
        else if( property == "Size" && root.isArray() && root.size() == 2 )
        {
            setSize( sf::Vector2f(
                (float)root[ 0u ].asDouble(),
                (float)root[ 1u ].asDouble()
            ) );
        }
        else if( property == "Origin" && root.isArray() && root.size() == 2 )
        {
            setOrigin( sf::Vector2f(
                (float)root[ 0u ].asDouble(),
                (float)root[ 1u ].asDouble()
            ) );
        }
        else if( property == "OriginPercent" && root.isArray() && root.size() == 2 )
        {
            setOriginPercent( sf::Vector2f(
                (float)root[ 0u ].asDouble(),
                (float)root[ 1u ].asDouble()
            ) );
        }
        else if( property == "Color" && root.isArray() && root.size() == 4 )
        {
            setColor( sf::Color(
                root[ 0u ].asUInt(),
                root[ 1u ].asUInt(),
                root[ 2u ].asUInt(),
                root[ 3u ].asUInt()
            ) );
        }
        else if( property == "RenderStates" && root.isObject() )
        {
            Json::Value blendMode = root[ "blendMode" ];
            if( blendMode.isString() )
                m_renderStates.blendMode = Serialized::deserializeCustom< sf::BlendMode >( "BlendMode", blendMode );
            Json::Value shader = root[ "shader" ];
            if( shader.isString() )
                m_renderStates.shader = Assets::use().getShader( shader.asString() );
        }
        else
            Component::onDeserialize( property, root );
    }

    void SpriteRenderer::onDuplicate( Component* dst )
    {
        if( !dst )
            return;
        Component::onDuplicate( dst );
        if( !XeCore::Common::IRtti::isDerived< SpriteRenderer >( dst ) )
            return;
        SpriteRenderer* c = (SpriteRenderer*)dst;
        c->setTexture( getTexture() );
        c->setSize( getSize() );
        c->setOrigin( getOrigin() );
        c->setRenderStates( getRenderStates() );
    }

    void SpriteRenderer::onUpdate( float dt )
    {
        Transform* trans = getGameObject()->getComponent< Transform >();
        if( trans )
        {
            m_shape->setPosition( trans->getPosition() );
            m_shape->setRotation( trans->getRotation() );
            m_shape->setScale( trans->getScale() );
        }
    }

    void SpriteRenderer::onTransform( const sf::Transform& inTrans, sf::Transform& outTrans )
    {
        m_renderStates.transform = inTrans;
    }

    void SpriteRenderer::onRender( sf::RenderTarget* target )
    {
        target->draw( *m_shape, m_renderStates );
    }

}
