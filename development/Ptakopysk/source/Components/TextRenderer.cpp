#include "../../include/Ptakopysk/Components/TextRenderer.h"
#include "../../include/Ptakopysk/Components/Transform.h"
#include "../../include/Ptakopysk/System/GameObject.h"
#include "../../include/Ptakopysk/System/Assets.h"

namespace Ptakopysk
{

    RTTI_CLASS_DERIVATIONS( TextRenderer,
                            RTTI_DERIVATION( Component ),
                            RTTI_DERIVATIONS_END
                            )

    TextRenderer::TextRenderer()
    : RTTI_CLASS_DEFINE( TextRenderer )
    , Component( Component::tTransform | Component::tRender | Component::tShape )
    , Text( this, &TextRenderer::getText, &TextRenderer::setText )
    , Font( this, &TextRenderer::getFont, &TextRenderer::setFont )
    , Size( this, &TextRenderer::getSize, &TextRenderer::setSize )
    , Origin( this, &TextRenderer::getOrigin, &TextRenderer::setOrigin )
    , OriginPercent( this, &TextRenderer::getOriginPercent, &TextRenderer::setOriginPercent )
    , Style( this, &TextRenderer::getStyle, &TextRenderer::setStyle )
    , Color( this, &TextRenderer::getColor, &TextRenderer::setColor )
    , RenderStates( this, &TextRenderer::getRenderStates, &TextRenderer::setRenderStates )
    , Material( this, &TextRenderer::getMaterial, &TextRenderer::setMaterial )
    , MaterialValidation( this, &TextRenderer::getMaterialValidation, &TextRenderer::setMaterialValidation )
    , m_renderStates( sf::RenderStates::Default )
    , m_materialValidation( false )
    {
        serializableProperty( "RenderStates" );
        serializableProperty( "Material" );
        serializableProperty( "MaterialValidation" );
        serializableProperty( "Font" );
        serializableProperty( "Size" );
        serializableProperty( "Style" );
        serializableProperty( "Color" );
        serializableProperty( "Text" );
        serializableProperty( "OriginPercent" );
        serializableProperty( "Origin" );
        m_text = xnew sf::Text();
    }

    TextRenderer::~TextRenderer()
    {
        DELETE_OBJECT( m_text );
    }

    sf::Vector2f TextRenderer::getOriginPercent()
    {
        sf::Vector2f o = getOrigin();
        sf::Vector2f s = getDimensions();
        o.x = s.x > 0.0f ? o.x / s.x : 0.0f;
        o.y = s.y > 0.0f ? o.y / s.y : 0.0f;
        return o;
    }

    void TextRenderer::setOriginPercent( sf::Vector2f origin )
    {
        sf::Vector2f s = getDimensions();
        setOrigin( sf::Vector2f( s.x * origin.x, s.y * origin.y ) );
    }

    Json::Value TextRenderer::onSerialize( const std::string& property )
    {
        if( property == "Text" )
            return Json::Value( getText() );
        else if( property == "Font" )
            return Json::Value( Assets::use().findFont( getFont() ) );
        else if( property == "Size" )
            return Json::Value( getSize() );
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
        else if( property == "Style" )
        {
            Serialized::ICustomSerializer* s = Serialized::getCustomSerializer( "Style" );
            if( s )
            {
                unsigned int style = getStyle();
                return s->serialize( &style );
            }
            return Json::Value::null;
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
        else if( property == "Material" )
            return m_material.serialize();
        else if( property == "MaterialValidation" )
            return Json::Value( m_materialValidation );
        else
            return Component::onSerialize( property );
    }

    void TextRenderer::onDeserialize( const std::string& property, const Json::Value& root )
    {
        if( property == "Text" && root.isString() )
            setText( root.asString() );
        else if( property == "Font" && root.isString() )
            setFont( Assets::use().getFont( root.asString() ) );
        else if( property == "Size" && root.isNumeric() )
            setSize( root.asUInt() );
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
        else if( property == "Style" && root.isArray() )
        {
            Serialized::ICustomSerializer* s = Serialized::getCustomSerializer( "Style" );
            if( s )
            {
                unsigned int style = getStyle();
                s->deserialize( &style, root );
                setStyle( style );
            }
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
        else if( property == "Material" && root.isObject() )
            m_material.deserialize( root );
        else if( property == "MaterialValidation" && root.isBool() )
            m_materialValidation = root.asBool();
        else
            Component::onDeserialize( property, root );
    }

    void TextRenderer::onDuplicate( Component* dst )
    {
        if( !dst )
            return;
        Component::onDuplicate( dst );
        if( !XeCore::Common::IRtti::isDerived< TextRenderer >( dst ) )
            return;
        TextRenderer* c = (TextRenderer*)dst;
        c->setMaterial( getMaterial() );
        c->setMaterialValidation( getMaterialValidation() );
        c->setText( getText() );
        c->setFont( getFont() );
        c->setSize( getSize() );
        c->setOrigin( getOrigin() );
        c->setStyle( getStyle() );
        c->setColor( getColor() );
        c->setRenderStates( getRenderStates() );
    }

    void TextRenderer::onTransform( const sf::Transform& inTrans, sf::Transform& outTrans )
    {
        m_renderStates.transform = inTrans;
    }

    void TextRenderer::onRender( sf::RenderTarget*& target )
    {
        if( m_renderStates.shader )
            m_material.apply( (sf::Shader*)m_renderStates.shader );
        target->draw( *m_text, m_renderStates );
    }

    void TextRenderer::onRenderEditor( sf::RenderTarget* target )
    {
        const sf::Shader* s = m_renderStates.shader;
        m_renderStates.shader = 0;
        target->draw( *m_text, m_renderStates );
        m_renderStates.shader = s;
    }

    void TextRenderer::onShaderChanged( const sf::Shader* a, bool addedOrRemoved )
    {
        if( m_renderStates.shader == a )
            m_renderStates.shader = 0;
    }

    void TextRenderer::onFontChanged( const sf::Font* a, bool addedOrRemoved )
    {
        if( m_text && m_text->getFont() == a )
            m_text->setFont( *a );
    }

    bool TextRenderer::onTriggerFunctionality( const std::string& name )
    {
        if( name == "Centralize origin" )
        {
            setOriginPercent( sf::Vector2f( 0.5f, 0.5f ) );
            return true;
        }
        return false;
    }

    bool TextRenderer::onCheckContainsPoint( const sf::Vector2f& worldPos )
    {
        sf::Vector2f p = m_renderStates.transform.getInverse().transformPoint( worldPos );
        return m_text->getGlobalBounds().contains( p );
    }

}
