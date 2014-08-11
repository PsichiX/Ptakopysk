#ifndef __PTAKOPYSK__CAMERA__
#define __PTAKOPYSK__CAMERA__

#include "Component.h"
#include <SFML/Graphics/View.hpp>
#include <SFML/Graphics/RenderTexture.hpp>

namespace Ptakopysk
{

    META_COMPONENT(
        META_ATTR_DESCRIPTION( "Camera component." )
    )
    class Camera
    : public virtual XeCore::Common::IRtti
    , public virtual XeCore::Common::MemoryManager::Manageable
    , public Component
    {
        friend class GameManager;

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
        FORCEINLINE sf::RenderTexture* getTargetTexture() { return m_renderTexture; };
        FORCEINLINE void setTargetTexture( sf::RenderTexture* v ) { m_renderTexture = v; };
        FORCEINLINE bool isApplyViewToRenderTexture() { return m_applyViewToRT; };
        FORCEINLINE void setApplyViewToRenderTexture( bool v ) { m_applyViewToRT = v; };

        META_PROPERTY(
            META_ATTR_DESCRIPTION( "Size of view in world-space." ),
            META_ATTR_DEFAULT_VALUE( "[100, 100]" )
        )
        XeCore::Common::Property< sf::Vector2f, Camera > Size;

        META_PROPERTY(
            META_ATTR_DESCRIPTION( "Zoom factor." ),
            META_ATTR_DEFAULT_VALUE( "1" )
        )
        XeCore::Common::Property< float, Camera > Zoom;

        META_PROPERTY(
            META_ATTR_DESCRIPTION( "Zoom-out factor." ),
            META_ATTR_DEFAULT_VALUE( "1" )
        )
        XeCore::Common::Property< float, Camera > ZoomOut;

        META_PROPERTY(
            META_ATTR_DESCRIPTION( "Viewport rectangle." ),
            META_ATTR_DEFAULT_VALUE( "[0, 0, 1, 1]" )
        )
        XeCore::Common::Property< sf::FloatRect, Camera > Viewport;

        XeCore::Common::Property< sf::RenderTexture*, Camera > TargetTexture;

        META_PROPERTY(
            META_ATTR_DESCRIPTION( "Determines if view settings should be used when camera renders content into RenderTexture." ),
            META_ATTR_DEFAULT_VALUE( "false" )
        )
        XeCore::Common::Property< bool, Camera > ApplyViewToRenderTexture;

    protected:
        virtual Json::Value onSerialize( const std::string& property );
        virtual void onDeserialize( const std::string& property, const Json::Value& root );

        virtual void onCreate();
        virtual void onDuplicate( Component* dst );
        virtual void onUpdate( float dt );
        virtual void onRender( sf::RenderTarget*& target );
        virtual void onRenderEditor( sf::RenderTarget* target );

    private:
        static sf::RenderTexture* s_currentRT;

        sf::View* m_view;
        sf::Vector2f m_size;
        float m_zoom;
        float m_zoomInv;
        sf::RenderTexture* m_renderTexture;
        bool m_applyViewToRT;
    };

}

#endif
