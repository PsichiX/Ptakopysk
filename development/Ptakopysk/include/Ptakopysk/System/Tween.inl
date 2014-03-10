#ifndef __PTAKOPYSK__TWEEN__INLINE__
#define __PTAKOPYSK__TWEEN__INLINE__

#include <XeCore/Common/Base.h>

namespace Ptakopysk
{

    template< typename PT, typename OT, Math::Easing::Basic::EasingFunc::Type FT >
    Tween::Tween( XeCore::Common::Property< PT, OT >& property, PT to, float duration )
    : m_property( property )
    , m_to( to )
    , m_duration( duration )
    , m_state( -1 )
    {
    };

    template< typename PT, typename OT, Math::Easing::Basic::EasingFunc::Type FT >
    Tween::~Tween()
    {
    }

    template< typename PT, typename OT, Math::Easing::Basic::EasingFunc::Type FT >
    void Tween::onStart()
    {
        if( m_state == 0 )
            return;
        m_from = m_property;
        m_time = 0.0f;
        m_state = 0;
    }

    template< typename PT, typename OT, Math::Easing::Basic::EasingFunc::Type FT >
    void Tween::onStop()
    {
        m_state = -1;
    }

    template< typename PT, typename OT, Math::Easing::Basic::EasingFunc::Type FT >
    void Tween::onUpdate( float dt )
    {
        if( m_state != 0 )
            return;
        m_time += dt;
        if( m_time >= m_duration )
            m_state = 1;
        m_time = CLAMP( m_time, 0.0f, m_duration );
        m_property = FT( m_time, m_from, m_to, m_duration );
    }

}

#endif
