#ifndef __PTAKOPYSK__CAMERA__
#define __PTAKOPYSK__CAMERA__

#include "Component.h"
#include "../Serialization/EnumSerializer.h"
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
        enum GenerateRenderTextureMode
        {
            grtNone,
            grtFromValue,
            grtFromView
        };

        enum KeepAspectRatioMode
        {
            karNone,
            karAxisX,
            karAxisY
        };

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
        FORCEINLINE KeepAspectRatioMode getKeepAspectRatioMode() { return m_keepAspectRatioMode; };
        FORCEINLINE void setKeepAspectRatioMode( KeepAspectRatioMode v ) { m_keepAspectRatioMode = v; setSize( m_size ); };
        FORCEINLINE sf::RenderTexture* getTargetTexture() { return m_renderTexture; };
        FORCEINLINE void setTargetTexture( sf::RenderTexture* v ) { if( m_renderTexture != v && m_renderTextureMine ) { m_renderTextureMine = false; DELETE_OBJECT( m_renderTexture ) }; m_renderTexture = v; };
        FORCEINLINE bool isTargetTextureMine() { return m_renderTextureMine; }
        FORCEINLINE GenerateRenderTextureMode getGenerateRenderTextureMode() { return m_generateRenderTextureMode; };
        FORCEINLINE void setGenerateRenderTextureMode( GenerateRenderTextureMode v ) { m_generateRenderTextureMode = v; };
        FORCEINLINE sf::Vector2u getRenderTextureMineSize() { return m_renderTextureMineSize; };
        FORCEINLINE void setRenderTextureMineSize( sf::Vector2u v ) { m_renderTextureMineSize = v; createRenderTexture(); };
        FORCEINLINE bool isApplyViewToRenderTexture() { return m_applyViewToRT; };
        FORCEINLINE void setApplyViewToRenderTexture( bool v ) { m_applyViewToRT = v; };
        FORCEINLINE int getRenderTextureSizePower() { return m_renderTextureSizePower; };
        FORCEINLINE void setRenderTextureSizePower( int v ) { m_renderTextureSizePower = v; };

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

        META_PROPERTY(
            META_ATTR_DESCRIPTION( "Determines how view should keep aspect ratio relative to window." ),
            META_ATTR_VALUE_TYPE( "@Enum:[ \"karNone\", \"karAxisX\", \"karAxisY\" ]" ),
            META_ATTR_DEFAULT_VALUE( "\"karNone\"" )
        )
        XeCore::Common::Property< KeepAspectRatioMode, Camera > KeepAspectRatio;

        META_PROPERTY(
            META_ATTR_DESCRIPTION( "Determines how to generate render texture." ),
            META_ATTR_VALUE_TYPE( "@Enum:[ \"grtNone\", \"grtFromValue\", \"grtFromView\" ]" ),
            META_ATTR_DEFAULT_VALUE( "\"grtNone\"" )
        )
        XeCore::Common::Property< GenerateRenderTextureMode, Camera > GenerateRenderTexture;

        META_PROPERTY(
            META_ATTR_DESCRIPTION( "Size of render texture." ),
            META_ATTR_DEFAULT_VALUE( "[0, 0]" )
        )
        XeCore::Common::Property< sf::Vector2u, Camera > RenderTextureSize;

        META_PROPERTY(
            META_ATTR_DESCRIPTION( "Power of render texture size." ),
            META_ATTR_DEFAULT_VALUE( "0" )
        )
        XeCore::Common::Property< int, Camera > RenderTextureSizePower;

    protected:
        virtual Json::Value onSerialize( const std::string& property );
        virtual void onDeserialize( const std::string& property, const Json::Value& root );

        virtual void onCreate();
        virtual void onDuplicate( Component* dst );
        virtual void onUpdate( float dt );
        virtual void onRender( sf::RenderTarget*& target );
        virtual void onRenderEditor( sf::RenderTarget* target );

    private:
        class GenerateRenderTextureModeSerializer
        : public EnumSerializer
        {
        public:
            GenerateRenderTextureModeSerializer()
            {
                EnumSerializer::EnumKeyValues kv;
                kv[ "grtNone" ] = grtNone;
                kv[ "grtFromValue" ] = grtFromValue;
                kv[ "grtFromView" ] = grtFromView;
                setup( kv );
            }
        };

        class KeepAspectRatioModeSerializer
        : public EnumSerializer
        {
        public:
            KeepAspectRatioModeSerializer()
            {
                EnumSerializer::EnumKeyValues kv;
                kv[ "karNone" ] = karNone;
                kv[ "karAxisX" ] = karAxisX;
                kv[ "karAxisY" ] = karAxisY;
                setup( kv );
            }
        };

        void createRenderTexture();

        static sf::RenderTexture* s_currentRT;
        static sf::RenderTarget* s_mainRT;

        bool m_isReady;
        sf::View* m_view;
        sf::Vector2f m_size;
        float m_zoom;
        float m_zoomInv;
        sf::RenderTexture* m_renderTexture;
        bool m_applyViewToRT;
        KeepAspectRatioMode m_keepAspectRatioMode;
        GenerateRenderTextureMode m_generateRenderTextureMode;
        bool m_renderTextureMine;
        sf::Vector2u m_renderTextureMineSize;
        int m_renderTextureSizePower;
    };

}

#endif
