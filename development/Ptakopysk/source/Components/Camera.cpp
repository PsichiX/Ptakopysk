#include "../../include/Ptakopysk/Components/Camera.h"
#include "../../include/Ptakopysk/Components/Transform.h"
#include "../../include/Ptakopysk/System/GameObject.h"

namespace Ptakopysk
{

    RTTI_CLASS_DERIVATIONS( Camera,
                            RTTI_DERIVATION( Component ),
                            RTTI_DERIVATIONS_END
                            )

    Camera::Camera()
    : RTTI_CLASS_DEFINE( Camera )
    , Component( Component::tUpdate | Component::tRender )
    , Size( this, &Camera::getSize, &Camera::setSize )
    , Viewport( this, &Camera::getViewport, &Camera::setViewport )
    {
        serializableProperty( "Size" );
        serializableProperty( "Viewport" );
        m_view = xnew sf::View();
    }

    Camera::~Camera()
    {
        DELETE_OBJECT( m_view );
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
        else if( property == "Viewport" && root.isArray() && root.size() == 4 )
        {
            setViewport( sf::FloatRect(
                (float)root[ 0u ].asDouble(),
                (float)root[ 1u ].asDouble(),
                (float)root[ 2u ].asDouble(),
                (float)root[ 3u ].asDouble()
            ) );
        }
        else
            Component::onDeserialize( property, root );
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
        c->setViewport( getViewport() );
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

    void Camera::onRender( sf::RenderTarget* target )
    {
        target->setView( *m_view );
    }

}
