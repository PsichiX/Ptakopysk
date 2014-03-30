#include "../../include/Ptakopysk/System/Tween.h"
#include <algorithm>

namespace Ptakopysk
{

    TweenSequence::TweenSequence()
    : m_state( -1 )
    , m_duration( 0.0f )
    , m_time( 0.0f )
    {
    }

    TweenSequence::~TweenSequence()
    {
        onStop();
    }

    void TweenSequence::setDuration( float v )
    {
    }

    void TweenSequence::setTime( float v )
    {
        if( m_state == 0 )
            return;
        float r = v - m_time;
        ITween* t;
        for( std::vector< ITween* >::iterator it = m_tweens.begin(); it != m_tweens.end(); it++ )
        {
            t = *it;
            t->setTime( t->getTime() + r );
        }
        m_time = v;
    }

    TweenSequence* TweenSequence::add( ITween* t )
    {
        if( !t || m_state != -1 )
            return this;
        t->setTime( t->getTime() - m_duration );
        m_duration += t->getDuration();
        m_tweens.push_back( t );
        return this;
    }

    void TweenSequence::onStart()
    {
        if( m_state == 0 )
            return;
        m_state = 0;
        for( std::vector< ITween* >::iterator it = m_tweens.begin(); it != m_tweens.end(); it++ )
            m_working.push_back( Tweener::use().startTween( *it ) );
        m_tweens.clear();
    }

    void TweenSequence::onStop()
    {
        m_tweens.clear();
        for( std::vector< dword >::iterator it = m_working.begin(); it != m_working.end(); it++ )
            Tweener::use().killTween( *it );
        m_working.clear();
        m_state = -1;
    }

    void TweenSequence::onUpdate( float dt )
    {
        if( m_state != 0 )
            return;
        for( std::vector< dword >::iterator it = m_working.begin(); it != m_working.end(); )
        {
            if( Tweener::use().hasTween( *it ) )
                it++;
            else
                it = m_working.erase( it );
        }
        if( m_working.empty() )
            m_state = 1;
    }

    dword TweenSequence::getPropertyID()
    {
        return (dword)this;
    }

    TweenBlock::TweenBlock()
    : m_state( -1 )
    , m_duration( 0.0f )
    , m_time( 0.0f )
    {
    }

    TweenBlock::~TweenBlock()
    {
        onStop();
    }

    void TweenBlock::setDuration( float v )
    {
    }

    void TweenBlock::setTime( float v )
    {
        if( m_state == 0 )
            return;
        float r = v - m_time;
        ITween* t;
        for( std::vector< ITween* >::iterator it = m_tweens.begin(); it != m_tweens.end(); it++ )
        {
            t = *it;
            t->setTime( t->getTime() + r );
        }
        m_time = v;
    }

    TweenBlock* TweenBlock::add( ITween* t )
    {
        if( !t || m_state != -1 )
            return this;
        m_duration = std::max( m_duration, t->getDuration() );
        m_tweens.push_back( t );
        return this;
    }

    void TweenBlock::onStart()
    {
        if( m_state == 0 )
            return;
        m_state = 0;
        for( std::vector< ITween* >::iterator it = m_tweens.begin(); it != m_tweens.end(); it++ )
            m_working.push_back( Tweener::use().startTween( *it ) );
        m_tweens.clear();
    }

    void TweenBlock::onStop()
    {
        m_tweens.clear();
        for( std::vector< dword >::iterator it = m_working.begin(); it != m_working.end(); it++ )
            Tweener::use().killTween( *it );
        m_working.clear();
        m_state = -1;
    }

    void TweenBlock::onUpdate( float dt )
    {
        if( m_state != 0 )
            return;
        for( std::vector< dword >::iterator it = m_working.begin(); it != m_working.end(); )
        {
            if( Tweener::use().hasTween( *it ) )
                it++;
            else
                it = m_working.erase( it );
        }
        if( m_working.empty() )
            m_state = 1;
    }

    dword TweenBlock::getPropertyID()
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

    dword Tweener::startTween( ITween* t )
    {
        if( !t || hasTween( (dword)t ) )
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
            m_tweens.erase( it );
        }
    }

    void Tweener::killAllTweens()
    {
        std::list< ITween* >::iterator it;
        ITween* t;
        while( !m_tweens.empty() )
        {
            it = m_tweens.begin();
            t = *it;
            DELETE_OBJECT( t );
            m_tweens.erase( it );
        }
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
