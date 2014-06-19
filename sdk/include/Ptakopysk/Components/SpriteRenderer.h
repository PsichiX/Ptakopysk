#ifndef __PTAKOPYSK__SPRITE_RENDERER__
#define __PTAKOPYSK__SPRITE_RENDERER__

#include "Component.h"
#include <SFML/Graphics.hpp>
#include "../System/RenderMaterial.h"

namespace Ptakopysk
{

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
        FORCEINLINE sf::Vector2f getSize() { return m_shape->getSize(); };
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

        XeCore::Common::Property< sf::Texture*, SpriteRenderer > Texture;
        XeCore::Common::Property< sf::Vector2f, SpriteRenderer > Size;
        XeCore::Common::Property< sf::Vector2f, SpriteRenderer > Origin;
        XeCore::Common::Property< sf::Vector2f, SpriteRenderer > OriginPercent;
        XeCore::Common::Property< sf::Color, SpriteRenderer > Color;
        XeCore::Common::Property< sf::RenderStates, SpriteRenderer > RenderStates;
        XeCore::Common::Property< RenderMaterial&, SpriteRenderer > Material;
        XeCore::Common::Property< bool, SpriteRenderer > MaterialValidation;

    protected:
        virtual Json::Value onSerialize( const std::string& property );
        virtual void onDeserialize( const std::string& property, const Json::Value& root );

        virtual void onDuplicate( Component* dst );
        virtual void onTransform( const sf::Transform& inTrans, sf::Transform& outTrans );
        virtual void onRender( sf::RenderTarget*& target );

    private:
        sf::RectangleShape* m_shape;
        sf::RenderStates m_renderStates;
        RenderMaterial m_material;
        bool m_materialValidation;
    };

}

#endif
