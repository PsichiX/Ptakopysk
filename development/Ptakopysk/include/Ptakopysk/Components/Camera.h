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
        FORCEINLINE sf::Vector2f getSize() { return m_size; };
        void setSize( sf::Vector2f v );
        FORCEINLINE sf::FloatRect getViewport() { return m_view->getViewport(); };
        FORCEINLINE void setViewport( sf::FloatRect v ) { m_view->setViewport( v ); };
        FORCEINLINE float getZoom() { return m_zoom; };
        void setZoom( float v );
        FORCEINLINE float getZoomOut() { return m_zoomInv; };
        void setZoomOut( float v );

        XeCore::Common::Property< sf::Vector2f, Camera > Size;
        XeCore::Common::Property< float, Camera > Zoom;
        XeCore::Common::Property< float, Camera > ZoomOut;
        XeCore::Common::Property< sf::FloatRect, Camera > Viewport;

    protected:
        virtual Json::Value onSerialize( const std::string& property );
        virtual void onDeserialize( const std::string& property, const Json::Value& root );

        virtual void onCreate();
        virtual void onDuplicate( Component* dst );
        virtual void onUpdate( float dt );
        virtual void onRender( sf::RenderTarget* target );

    private:
        sf::View* m_view;
        sf::Vector2f m_size;
        float m_zoom;
        float m_zoomInv;
    };

}

#endif
