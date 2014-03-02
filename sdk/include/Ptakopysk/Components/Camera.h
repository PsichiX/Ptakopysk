#ifndef __PTAKOPYSK__CAMERA__
#define __PTAKOPYSK__CAMERA__

#include "Component.h"
#include <SFML/Graphics/View.hpp>

namespace Ptakopysk
{
    class Camera
    : public virtual XeCore::Common::IRtti
    , public virtual XeCore::Common::MemoryManager::Manageable
    , public Component
    {
        RTTI_CLASS_DECLARE( Camera );

    public:
        Camera();
        virtual ~Camera();

        FORCEINLINE static Component* onBuildComponent() { return xnew Camera(); }

        FORCEINLINE sf::View* getView() { return m_view; };
        FORCEINLINE sf::Vector2f getSize() { return m_view->getSize(); };
        FORCEINLINE void setSize( sf::Vector2f v ) { m_view->setSize( v ); };
        FORCEINLINE sf::FloatRect getViewport() { return m_view->getViewport(); };
        FORCEINLINE void setViewport( sf::FloatRect v ) { m_view->setViewport( v ); };
        FORCEINLINE void applyZoom( float v ) { m_view->zoom( v ); };

        XeCore::Common::Property< sf::Vector2f, Camera > Size;
        XeCore::Common::Property< sf::FloatRect, Camera > Viewport;

    protected:
        virtual Json::Value onSerialize( const std::string& property );
        virtual void onDeserialize( const std::string& property, const Json::Value& root );

        virtual void onDuplicate( Component* dst );
        virtual void onUpdate( float dt );
        virtual void onRender( sf::RenderTarget* target );

    private:
        sf::View* m_view;
    };

}

#endif
