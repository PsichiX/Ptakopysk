#ifndef __PTAKOPYSK__SPRITE_RENDERER__
#define __PTAKOPYSK__SPRITE_RENDERER__

#include "Component.h"
#include <SFML/Graphics.hpp>

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

        FORCEINLINE ::sf::RectangleShape* getShape() { return m_shape; };
        FORCEINLINE ::sf::Texture* getTexture() { return (::sf::Texture*)m_shape->getTexture(); };
        FORCEINLINE void setTexture( ::sf::Texture* tex ) { m_shape->setTexture( (const ::sf::Texture*)tex ); };
        FORCEINLINE ::sf::Vector2f getSize() { return m_shape->getSize(); };
        void setSize( ::sf::Vector2f size );
        FORCEINLINE ::sf::Vector2f getOrigin() { return m_shape->getOrigin(); };
        FORCEINLINE void setOrigin( ::sf::Vector2f origin ) { m_shape->setOrigin( origin ); };
        ::sf::Vector2f getOriginPercent();
        void setOriginPercent( ::sf::Vector2f origin );
        FORCEINLINE ::sf::RenderStates getRenderStates() { return m_renderStates; };
        FORCEINLINE void setRenderStates( ::sf::RenderStates states ) { m_renderStates = states; };

        XeCore::Common::Property< ::sf::Texture*, SpriteRenderer > Texture;
        XeCore::Common::Property< ::sf::Vector2f, SpriteRenderer > Size;
        XeCore::Common::Property< ::sf::Vector2f, SpriteRenderer > Origin;
        XeCore::Common::Property< ::sf::Vector2f, SpriteRenderer > OriginPercent;
        XeCore::Common::Property< ::sf::RenderStates, SpriteRenderer > RenderStates;

    protected:
        virtual Json::Value onSerialize( const std::string& property );
        virtual void onDeserialize( const std::string& property, const Json::Value& root );

        virtual void onDuplicate( Component* dst );
        virtual void onUpdate( float dt );
        virtual void onRender( ::sf::RenderTarget* target );

    private:
        ::sf::RectangleShape* m_shape;
        ::sf::RenderStates m_renderStates;
    };

}

#endif
