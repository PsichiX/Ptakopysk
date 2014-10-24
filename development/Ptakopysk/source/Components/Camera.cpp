#include "../../include/Ptakopysk/Components/Camera.h"
#include "../../include/Ptakopysk/Components/Transform.h"
#include "../../include/Ptakopysk/System/GameObject.h"
#include "../../include/Ptakopysk/System/GameManager.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Color.hpp>

namespace Ptakopysk
{

    RTTI_CLASS_DERIVATIONS( Camera,
                            RTTI_DERIVATION( Component ),
                            RTTI_DERIVATIONS_END
                            )

    sf::RenderTexture* Camera::s_currentRT = 0;
    sf::RenderTarget* Camera::s_mainRT = 0;

    Camera::Camera()
    : RTTI_CLASS_DEFINE( Camera )
    , Component( Component::tUpdate | Component::tRender )
    , Size( this, &Camera::getSize, &Camera::setSize )
    , Zoom( this, &Camera::getZoom, &Camera::setZoom )
    , ZoomOut( this, &Camera::getZoomOut, &Camera::setZoomOut )
    , Viewport( this, &Camera::getViewport, &Camera::setViewport )
    , TargetTexture( this, &Camera::getTargetTexture, &Camera::setTargetTexture )
    , ApplyViewToRenderTexture( this, &Camera::isApplyViewToRenderTexture, &Camera::setApplyViewToRenderTexture )
    , KeepAspectRatio( this, &Camera::getKeepAspectRatioMode, &Camera::setKeepAspectRatioMode )
    , GenerateRenderTexture( this, &Camera::getGenerateRenderTextureMode, &Camera::setGenerateRenderTextureMode )
    , RenderTextureSize( this, &Camera::getRenderTextureMineSize, &Camera::setRenderTextureMineSize )
    , m_isReady( false )
    , m_zoom( 1.0f )
    , m_zoomInv( 1.0f )
    , m_renderTexture( 0 )
    , m_applyViewToRT( false )
    , m_keepAspectRatioMode( karNone )
    , m_generateRenderTextureMode( grtNone )
    , m_renderTextureMine( false )
    {
        serializableProperty( "Size" );
        serializableProperty( "ZoomOut" );
        serializableProperty( "Zoom" );
        serializableProperty( "Viewport" );
        serializableProperty( "ApplyViewToRenderTexture" );
        serializableProperty( "KeepAspectRatio" );
        serializableProperty( "GenerateRenderTexture" );
        serializableProperty( "RenderTextureSize" );
        m_view = xnew sf::View();
    }

    Camera::~Camera()
    {
        DELETE_OBJECT( m_view );
        if( m_renderTextureMine )
            DELETE_OBJECT( m_renderTexture );
    }

    void Camera::setSize( sf::Vector2f v )
    {
        if( !GameManager::isEditMode() && getGameObject() )
        {
            GameManager* gm = getGameObject()->getGameManagerRoot();
            if( gm )
            {
                sf::RenderWindow* wnd = gm->getRenderWindow();
                if( wnd )
                {
                    if( v.x < 0.0f )
                        v.x = (float)wnd->getSize().x;
                    if( v.y < 0.0f )
                        v.y = (float)wnd->getSize().y;
                    if( m_keepAspectRatioMode == karAxisX )
                        v.x = v.y * (float)wnd->getSize().x / (float)wnd->getSize().y;
                    else if( m_keepAspectRatioMode == karAxisY )
                        v.y = v.x * (float)wnd->getSize().y / (float)wnd->getSize().x;
                }
            }
        }
        m_size = v;
        m_view->setSize( v );
        m_view->zoom( m_zoomInv );
    }

    void Camera::setZoom( float v )
    {
        m_zoom = v;
        m_zoomInv = v == 0.0f ? 0.0f : 1.0f / v;
        m_view->setSize( m_size );
        m_view->zoom( m_zoomInv );
    }

    void Camera::setZoomOut( float v )
    {
        m_zoomInv = v;
        m_zoom = v == 0.0f ? 0.0f : 1.0f / v;
        m_view->setSize( m_size );
        m_view->zoom( m_zoomInv );
    }

    Json::Value Camera::onSerialize( const std::string& property )
    {
        if( property == "Size" )
        {
            sf::Vector2f s = getSize();
            Json::Value v;
            v.append( Json::Value( s.x ) );
            v.append( Json::Value( s.y ) );
            return v;
        }
        else if( property == "Zoom" )
            return Json::Value( m_zoom );
        else if( property == "ZoomOut" )
            return Json::Value( m_zoomInv );
        else if( property == "Viewport" )
        {
            sf::FloatRect r = getViewport();
            Json::Value v;
            v.append( Json::Value( r.left ) );
            v.append( Json::Value( r.top ) );
            v.append( Json::Value( r.width ) );
            v.append( Json::Value( r.height ) );
            return v;
        }
        else if( property == "ApplyViewToRenderTexture" )
            return Json::Value( m_applyViewToRT );
        else if( property == "KeepAspectRatio" )
            return KeepAspectRatioModeSerializer().serialize( &m_keepAspectRatioMode );
        else if( property == "RenderTextureSize" )
        {
            sf::Vector2u s = getRenderTextureMineSize();
            Json::Value v;
            v.append( Json::Value( s.x ) );
            v.append( Json::Value( s.y ) );
            return v;
        }
        else if( property == "GenerateRenderTexture" )
            return GenerateRenderTextureModeSerializer().serialize( &m_generateRenderTextureMode );
        else
            return Component::onSerialize( property );
    }

    void Camera::onDeserialize( const std::string& property, const Json::Value& root )
    {
        if( property == "Size" && root.isArray() && root.size() == 2 )
        {
            setSize( sf::Vector2f(
                (float)root[ 0u ].asDouble(),
                (float)root[ 1u ].asDouble()
            ) );
        }
        else if( property == "Zoom" && root.isNumeric() )
            setZoom( (float)root.asDouble() );
        else if( property == "ZoomOut" && root.isNumeric() )
            setZoomOut( (float)root.asDouble() );
        else if( property == "Viewport" && root.isArray() && root.size() == 4 )
        {
            setViewport( sf::FloatRect(
                (float)root[ 0u ].asDouble(),
                (float)root[ 1u ].asDouble(),
                (float)root[ 2u ].asDouble(),
                (float)root[ 3u ].asDouble()
            ) );
        }
        else if( property == "ApplyViewToRenderTexture" && root.isBool() )
            setApplyViewToRenderTexture( root.asBool() );
        else if( property == "KeepAspectRatio" && root.isString() )
        {
            KeepAspectRatioModeSerializer().deserialize( &m_keepAspectRatioMode, root );
            setKeepAspectRatioMode( m_keepAspectRatioMode );
        }
        else if( property == "RenderTextureSize" && root.isArray() && root.size() == 2 )
        {
            setRenderTextureMineSize( sf::Vector2u(
                root[ 0u ].asUInt(),
                root[ 1u ].asUInt()
            ) );
        }
        else if( property == "GenerateRenderTexture" && root.isString() )
        {
            GenerateRenderTextureModeSerializer().deserialize( &m_generateRenderTextureMode, root );
            createRenderTexture();
        }
        else
            Component::onDeserialize( property, root );
    }

    void Camera::onCreate()
    {
        if( !getGameObject() || getGameObject()->isPrefab() )
            return;
        m_isReady = true;
        setSize( m_size );
        createRenderTexture();
    }

    void Camera::onDuplicate( Component* dst )
    {
        if( !dst )
            return;
        Component::onDuplicate( dst );
        if( !XeCore::Common::IRtti::isDerived< Camera >( dst ) )
            return;
        Camera* c = (Camera*)dst;
        c->setSize( getSize() );
        c->setZoomOut( getZoomOut() );
        c->setZoom( getZoom() );
        c->setViewport( getViewport() );
        c->setApplyViewToRenderTexture( isApplyViewToRenderTexture() );
        c->setKeepAspectRatioMode( getKeepAspectRatioMode() );
        c->setRenderTextureMineSize( getRenderTextureMineSize() );
        c->setGenerateRenderTextureMode( getGenerateRenderTextureMode() );
    }

    void Camera::onUpdate( float dt )
    {
        Transform* trans = getGameObject()->getComponent< Transform >();
        if( trans )
        {
            m_view->setCenter( trans->getPosition() );
            m_view->setRotation( trans->getRotation() );
        }
    }

    void Camera::onRender( sf::RenderTarget*& target )
    {
        if( s_currentRT )
            s_currentRT->display();
        if( m_renderTexture )
        {
            s_currentRT = m_renderTexture;
            target = m_renderTexture;
            target->clear( sf::Color( 0, 0, 0, 0 ) );
            if( m_applyViewToRT )
                target->setView( *m_view );
        }
        else
        {
            s_currentRT = 0;
            target = s_mainRT;
            target->setView( *m_view );
        }
    }

    void Camera::onRenderEditor( sf::RenderTarget* target )
    {
        if( !target )
            return;
        sf::Vector2f s = m_size;
        if( s.x < 0.0f )
            s.x = (float)target->getSize().x;
        if( s.y < 0.0f )
            s.y = (float)target->getSize().y;
        if( m_keepAspectRatioMode == karAxisX )
            s.x = s.y * (float)target->getSize().x / (float)target->getSize().y;
        else if( m_keepAspectRatioMode == karAxisX )
            s.y = s.x * (float)target->getSize().y / (float)target->getSize().x;
        s.x *= m_zoomInv;
        s.y *= m_zoomInv;
        sf::RectangleShape rect( s );
        rect.setOrigin( s * 0.5f );
        Transform* trans = getGameObject() ? getGameObject()->getComponent< Transform >() : 0;
        if( trans )
            rect.setPosition( trans->getPosition() );
        rect.setFillColor( sf::Color( 0, 0, 0, 64 ) );
        rect.setOutlineColor( sf::Color( 255, 255, 255, 64 ) );
        rect.setOutlineThickness( 4 );
        target->draw( rect );
    }

    void Camera::createRenderTexture()
    {
        if( !m_isReady || m_generateRenderTextureMode == grtNone || GameManager::isEditMode() || !getGameObject() || getGameObject()->isPrefab() )
            return;
        sf::RenderTexture* rt = xnew sf::RenderTexture();
        unsigned int w = m_renderTextureMineSize.x;
        unsigned int h = m_renderTextureMineSize.y;
        if( m_generateRenderTextureMode == grtFromView )
        {
            w = m_size.x;
            h = m_size.y;
        }
        if( rt->create( w, h ) )
        {
            setTargetTexture( rt );
            m_renderTextureMine = true;
        }
        else
        {
            setTargetTexture( 0 );
            m_renderTextureMine = false;
            DELETE_OBJECT( rt );
        }
    }

}
