#ifndef __PTAKOPYSK__TEXT_RENDERER__
#define __PTAKOPYSK__TEXT_RENDERER__

#include "Component.h"
#include <SFML/Graphics.hpp>
#include "../System/RenderMaterial.h"

namespace Ptakopysk
{

    META_COMPONENT(
        META_ATTR_DESCRIPTION( "Text renderer component." ),
        META_ATTR_FUNCTIONALITY_TRIGGERS( "Centralize origin" )
    )
    class TextRenderer
    : public virtual XeCore::Common::IRtti
    , public virtual XeCore::Common::MemoryManager::Manageable
    , public Component
    {
        RTTI_CLASS_DECLARE( TextRenderer );

    public:
        TextRenderer();
        virtual ~TextRenderer();

        FORCEINLINE static Component* onBuildComponent() { return xnew TextRenderer(); }

        FORCEINLINE sf::Text* getRenderer() { return m_text; };
        FORCEINLINE sf::String getText() { return m_text->getString(); };
        FORCEINLINE void setText( sf::String v ) { m_text->setString( v ); };
        FORCEINLINE sf::Font* getFont() { return (sf::Font*)m_text->getFont(); };
        FORCEINLINE void setFont( sf::Font* v ) { if( v ) m_text->setFont( *v ); };
        FORCEINLINE dword getSize() { return m_text->getCharacterSize(); };
        FORCEINLINE void setSize( dword v ) { m_text->setCharacterSize( v ); };
        FORCEINLINE sf::FloatRect measureText() { return m_text->getLocalBounds(); };
        FORCEINLINE sf::Vector2f getDimensions() { sf::FloatRect r = measureText(); return sf::Vector2f( r.left + r.width, r.top + r.height ); };
        FORCEINLINE sf::Vector2f getOrigin() { return m_text->getOrigin(); };
        FORCEINLINE void setOrigin( sf::Vector2f origin ) { m_text->setOrigin( origin ); };
        sf::Vector2f getOriginPercent();
        void setOriginPercent( sf::Vector2f origin );
        FORCEINLINE dword getStyle() { return m_text->getStyle(); };
        FORCEINLINE void setStyle( dword v ) { m_text->setStyle( v ); };
        FORCEINLINE sf::Color getColor() { return m_text->getColor(); };
        FORCEINLINE void setColor( sf::Color v ) { m_text->setColor( v ); };
        FORCEINLINE sf::RenderStates getRenderStates() { return m_renderStates; };
        FORCEINLINE void setRenderStates( sf::RenderStates states ) { m_renderStates = states; };
        FORCEINLINE RenderMaterial& getMaterial() { return m_material; };
        FORCEINLINE void setMaterial( RenderMaterial& v ) { m_material.copyFrom( v ); };
        FORCEINLINE bool getMaterialValidation() { return m_materialValidation; };
        FORCEINLINE void setMaterialValidation( bool v ) { m_materialValidation = v; };

        META_PROPERTY(
            META_ATTR_DESCRIPTION( "Text value." ),
            META_ATTR_DEFAULT_VALUE( "" )
        )
        XeCore::Common::Property< sf::String, TextRenderer > Text;

        META_PROPERTY(
            META_ATTR_DESCRIPTION( "Font name." )
        )
        XeCore::Common::Property< sf::Font*, TextRenderer > Font;

        META_PROPERTY(
            META_ATTR_DESCRIPTION( "Font size." ),
        )
        XeCore::Common::Property< dword, TextRenderer > Size;

        META_PROPERTY(
            META_ATTR_DESCRIPTION( "Origin (local anchor point)." ),
            META_ATTR_DEFAULT_VALUE( "[0, 0]" )
        )
        XeCore::Common::Property< sf::Vector2f, TextRenderer > Origin;

        META_PROPERTY(
            META_ATTR_DESCRIPTION( "Origin percent (local anchor point in unit-space)." ),
            META_ATTR_DEFAULT_VALUE( "[0, 0]" )
        )
        XeCore::Common::Property< sf::Vector2f, TextRenderer > OriginPercent;

        META_PROPERTY(
            META_ATTR_DESCRIPTION( "Style." ),
            META_ATTR_VALUE_TYPE( "@Enum:[ \"Regular\", \"Bold\", \"Italic\", \"Underlined\" ]" )
        )
        XeCore::Common::Property< dword, TextRenderer > Style;

        META_PROPERTY(
            META_ATTR_DESCRIPTION( "Color." ),
            META_ATTR_DEFAULT_VALUE( "[255, 255, 255, 255]" )
        )
        XeCore::Common::Property< sf::Color, TextRenderer > Color;

        META_PROPERTY(
            META_ATTR_DESCRIPTION( "Render states." )
        )
        XeCore::Common::Property< sf::RenderStates, TextRenderer > RenderStates;

        META_PROPERTY(
            META_ATTR_DESCRIPTION( "Render material." )
        )
        XeCore::Common::Property< RenderMaterial&, TextRenderer > Material;

        META_PROPERTY(
            META_ATTR_DESCRIPTION( "Determines if material should be validated in rendering process." ),
            META_ATTR_DEFAULT_VALUE( "false" )
        )
        XeCore::Common::Property< bool, TextRenderer > MaterialValidation;

    protected:
        virtual Json::Value onSerialize( const std::string& property );
        virtual void onDeserialize( const std::string& property, const Json::Value& root );

        virtual void onDuplicate( Component* dst );
        virtual void onTransform( const sf::Transform& inTrans, sf::Transform& outTrans );
        virtual void onRender( sf::RenderTarget*& target );
        virtual void onRenderEditor( sf::RenderTarget* target );
        virtual void onShaderChanged( const sf::Shader* a, bool addedOrRemoved );
        virtual void onFontChanged( const sf::Font* a, bool addedOrRemoved );
        virtual bool onTriggerFunctionality( const std::string& name );
        virtual bool onCheckContainsPoint( const sf::Vector2f& worldPos );

    private:
        sf::Text* m_text;
        sf::RenderStates m_renderStates;
        RenderMaterial m_material;
        bool m_materialValidation;
    };

}

#endif
