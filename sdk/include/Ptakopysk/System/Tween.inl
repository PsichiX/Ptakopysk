#ifndef __PTAKOPYSK__TWEEN__INLINE__
#define __PTAKOPYSK__TWEEN__INLINE__

#include <XeCore/Common/Base.h>

namespace Ptakopysk
{

    template< typename PT, typename OT, PT(*EF)(float,PT,PT,float) >
    Tween< PT, OT, EF >::Tween( XeCore::Common::Property< PT, OT >& property, PT to, float duration )
    : m_property( property )
    , m_to( to )
    , m_duration( duration )
    , m_time( 0.0f )
    , m_state( -1 )
    {
    }

    template< typename PT, typename OT, PT(*EF)(float,PT,PT,float) >
    void Tween< PT, OT, EF >::onStart()
    {
        if( m_state == 0 )
            return;
        m_from = m_property;
        m_state = 0;
        m_time = 0.0f;
    }

    template< typename PT, typename OT, PT(*EF)(float,PT,PT,float) >
    void Tween< PT, OT, EF >::onStop()
    {
        m_state = -1;
    }

    template< typename PT, typename OT, PT(*EF)(float,PT,PT,float) >
    void Tween< PT, OT, EF >::onUpdate( float dt )
    {
        if( m_state != 0 )
            return;
        m_time += dt;
        if( m_time >= m_duration )
            m_state = 1;
        m_time = m_time < m_duration ? ( m_time > 0.0f ? m_time : 0.0f ) : m_duration;
        m_property = EF( m_time, m_from, m_to, m_duration );
    }

    template< typename PT, typename OT, PT(*EF)(float,PT,PT,float) >
    dword Tween< PT, OT, EF >::getPropertyID()
    {
        return (dword)&m_property;
    }

    template< typename PT, typename OT >
    void Tweener::killTweensOf( const XeCore::Common::Property< PT, OT >& p )
    {
        dword id = (dword)&p;
        ITween* t;
        for( std::list< ITween* >::iterator it = m_tweens.begin(); it != m_tweens.end(); )
        {
            t = *it;
            if( t->getPropertyID() == id )
            {
                DELETE_OBJECT( t );
                it = m_tweens.erase( it );
            }
            else
                it++;
        }
    }

}

#endif