#ifndef __PTAKOPYSK__SPRITE_RENDERER__
#define __PTAKOPYSK__SPRITE_RENDERER__

#include "Component.h"
#include <SFML/Graphics.hpp>
#include "../System/RenderMaterial.h"

namespace Ptakopysk
{

    META_COMPONENT(
        META_ATTR_DESCRIPTION( "Sprite renderer component." ),
        META_ATTR_FUNCTIONALITY_TRIGGERS( "Centralize origin" )
    )
    class SpriteRenderer
    : public virtual XeCore::Common::IRtti
    , public virtual XeCore::Common::MemoryManager::Manageable
    , public Component
    {
        RTTI_CLASS_DECLARE( SpriteRenderer );

    public:
        SpriteRenderer();
        virtual ~SpriteRenderer();

        FORCEINLINE static Component* onBuildComponent() { return xnew SpriteRenderer(); }

        FORCEINLINE sf::RectangleShape* getRenderer() { return m_shape; };
        sf::Texture* getTexture();
        void setTexture( sf::Texture* tex );
        sf::Vector2f getSize();
        void setSize( sf::Vector2f size );
        FORCEINLINE sf::Vector2f getOrigin() { return m_shape->getOrigin(); };
        FORCEINLINE void setOrigin( sf::Vector2f origin ) { m_shape->setOrigin( origin ); };
        sf::Vector2f getOriginPercent();
        void setOriginPercent( sf::Vector2f origin );
        FORCEINLINE sf::Color getColor() { return m_shape->getFillColor(); };
        FORCEINLINE void setColor( sf::Color col ) { m_shape->setFillColor( col ); };
        FORCEINLINE sf::RenderStates getRenderStates() { return m_renderStates; };
        FORCEINLINE void setRenderStates( sf::RenderStates states ) { m_renderStates = states; };
        FORCEINLINE RenderMaterial& getMaterial() { return m_material; };
        FORCEINLINE void setMaterial( RenderMaterial& v ) { m_material.copyFrom( v ); };
        FORCEINLINE bool getMaterialValidation() { return m_materialValidation; };
        FORCEINLINE void setMaterialValidation( bool v ) { m_materialValidation = v; };

        META_PROPERTY(
            META_ATTR_DESCRIPTION( "Texture name." )
        )
        XeCore::Common::Property< sf::Texture*, SpriteRenderer > Texture;

        META_PROPERTY(
            META_ATTR_DESCRIPTION( "Size of the sprite." ),
            META_ATTR_DEFAULT_VALUE( "[0, 0]" )
        )
        XeCore::Common::Property< sf::Vector2f, SpriteRenderer > Size;

        META_PROPERTY(
            META_ATTR_DESCRIPTION( "Origin (local anchor point)." ),
            META_ATTR_DEFAULT_VALUE( "[0, 0]" )
        )
        XeCore::Common::Property< sf::Vector2f, SpriteRenderer > Origin;

        META_PROPERTY(
            META_ATTR_DESCRIPTION( "Origin percent (local anchor point in unit-space)." ),
            META_ATTR_DEFAULT_VALUE( "[0, 0]" )
        )
        XeCore::Common::Property< sf::Vector2f, SpriteRenderer > OriginPercent;

        META_PROPERTY(
            META_ATTR_DESCRIPTION( "Color." ),
            META_ATTR_DEFAULT_VALUE( "[255, 255, 255, 255]" )
        )
        XeCore::Common::Property< sf::Color, SpriteRenderer > Color;

        META_PROPERTY(
            META_ATTR_DESCRIPTION( "Render states." )
        )
        XeCore::Common::Property< sf::RenderStates, SpriteRenderer > RenderStates;

        META_PROPERTY(
            META_ATTR_DESCRIPTION( "Render material." )
        )
        XeCore::Common::Property< RenderMaterial&, SpriteRenderer > Material;

        META_PROPERTY(
            META_ATTR_DESCRIPTION( "Determines if material should be validated in rendering process." ),
            META_ATTR_DEFAULT_VALUE( "false" )
        )
        XeCore::Common::Property< bool, SpriteRenderer > MaterialValidation;

    protected:
        virtual Json::Value onSerialize( const std::string& property );
        virtual void onDeserialize( const std::string& property, const Json::Value& root );

        virtual void onDuplicate( Component* dst );
        virtual void onTransform( const sf::Transform& inTrans, sf::Transform& outTrans );
        virtual void onRender( sf::RenderTarget*& target );
        virtual void onTextureChanged( const sf::Texture* a, bool addedOrRemoved );
        virtual void onShaderChanged( const sf::Shader* a, bool addedOrRemoved );
        virtual bool onTriggerFunctionality( const std::string& name );
        virtual bool onCheckContainsPoint( const sf::Vector2f& worldPos );

    private:
        sf::Vector2f m_size;
        sf::RectangleShape* m_shape;
        sf::RenderStates m_renderStates;
        RenderMaterial m_material;
        bool m_materialValidation;
    };

}

#endif
