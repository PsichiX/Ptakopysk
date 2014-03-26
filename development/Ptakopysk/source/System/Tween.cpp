#include "../../include/Ptakopysk/System/Tween.h"
#include <algorithm>

namespace Ptakopysk
{

    TweenSequence::TweenSequence()
    : m_state( -1 )
    {
    }

    void TweenSequence::add( const ITween* t )
    {
        if( !t || m_state != -1 )
            return;
        m_tweens.push_back( t );
    }

    void TweenSequence::onStart()
    {
        if( m_state == 0 )
            return;
        m_state = 0;
    }

    void TweenSequence::onStop()
    {
        m_state = -1;
    }

    void TweenSequence::onUpdate( float dt )
    {
        if( m_state != 0 )
            return;
    }

    dword TweenSequence::getPropertyID()
    {
        return (dword)this;
    }

    RTTI_CLASS_DERIVATIONS( Tweener,
                            RTTI_DERIVATIONS_END
                            )

    Tweener::Tweener()
    : RTTI_CLASS_DEFINE( Tweener )
    {
    }

    Tweener::~Tweener()
    {
        killAllTweens();
    }

    bool Tweener::hasTween( dword id )
    {
        return std::find( m_tweens.begin(), m_tweens.end(), (ITween*)id ) != m_tweens.end();
    }

    dword Tweener::startTween( const ITween* t )
    {
        if( hasTween( (dword)t ) )
            return 0;
        m_tweens.push_back( (ITween*)t );
        return (dword)t;
    }

    void Tweener::killTween( dword id )
    {
        std::list< ITween* >::iterator it = std::find( m_tweens.begin(), m_tweens.end(), (ITween*)id );
        if( it != m_tweens.end() )
        {
            ITween* tf = *it;
            DELETE_OBJECT( tf );
        }
    }

    void Tweener::killAllTweens()
    {
        ITween* t;
        for( std::list< ITween* >::iterator it = m_tweens.begin(); it != m_tweens.end(); it++ )
        {
            t = *it;
            DELETE_OBJECT( t );
        }
        m_tweens.clear();
    }

    void Tweener::processTweens( float dt )
    {
        ITween* t;
        for( std::list< ITween* >::iterator it = m_tweens.begin(); it != m_tweens.end(); )
        {
            t = *it;
            if( t->getState() == -1 )
                t->onStart();
            if( t->getState() == 0 )
                t->onUpdate( dt );
            if( t->getState() == 1 )
            {
                t->onStop();
                DELETE_OBJECT( t );
                it = m_tweens.erase( it );
            }
            else
                it++;
        }
    }

}
