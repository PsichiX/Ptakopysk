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
    , Component( Component::tTransform | Component::tRender )
    , Text( this, &TextRenderer::getText, &TextRenderer::setText )
    , Font( this, &TextRenderer::getFont, &TextRenderer::setFont )
    , Size( this, &TextRenderer::getSize, &TextRenderer::setSize )
    , Style( this, &TextRenderer::getStyle, &TextRenderer::setStyle )
    , Color( this, &TextRenderer::getColor, &TextRenderer::setColor )
    , RenderStates( this, &TextRenderer::getRenderStates, &TextRenderer::setRenderStates )
    , Material( this, &TextRenderer::getMaterial, &TextRenderer::setMaterial )
    , m_renderStates( sf::RenderStates::Default )
    {
        serializableProperty( "RenderStates" );
        serializableProperty( "Material" );
        serializableProperty( "Font" );
        serializableProperty( "Size" );
        serializableProperty( "Style" );
        serializableProperty( "Color" );
        serializableProperty( "Text" );
        m_text = xnew sf::Text();
    }

    TextRenderer::~TextRenderer()
    {
        DELETE_OBJECT( m_text );
    }

    Json::Value TextRenderer::onSerialize( const std::string& property )
    {
        if( property == "Text" )
            return Json::Value( getText() );
        else if( property == "Font" )
            return Json::Value( Assets::use().findFont( getFont() ) );
        else if( property == "Size" )
            return Json::Value( getSize() );
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
        else
            return Component::onSerialize( property );
    }

    void TextRenderer::onDeserialize( const std::string& property, const Json::Value& root )
    {
        if( property == "Text" && root.isString() )
            setText( root.asString() );
        else if( property == "Font" && root.isString() )
            setFont( Assets::use().getFont( root.asString() ) );
        else if( property == "Size" && root.isUInt() )
            setSize( root.asUInt() );
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
        c->setText( getText() );
        c->setFont( getFont() );
        c->setSize( getSize() );
        c->setStyle( getStyle() );
        c->setColor( getColor() );
        c->setRenderStates( getRenderStates() );
        c->setMaterial( getMaterial() );
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

}
