#include "../../include/Ptakopysk/System/Events.h"

namespace Ptakopysk
{

    RTTI_CLASS_DERIVATIONS( Events,
                            RTTI_DERIVATIONS_END
                            )

    Events::Events()
    : RTTI_CLASS_DEFINE( Events )
    , m_callback( 0 )
    {
    }

    Events::~Events()
    {
    }

    void Events::setCallback( OnEventCallback cb )
    {
        m_callback = cb;
    }

    Events::OnEventCallback Events::getCallback()
    {
        return m_callback;
    }

    void Events::push( Events::Event* event )
    {
        if( !event )
            return;
        m_events.push_back( event );
    }

    void Events::dispatch()
    {
        for( std::list< Event* >::iterator it = m_events.begin(); it != m_events.end(); it++ )
        {
            Event* event = *it;
            onEvent( event );
            DELETE_OBJECT( event )
        }
        m_events.clear();
    }

    void Events::onEvent( Event* event )
    {
        if( m_callback )
            m_callback( event );
    }

    Events::Event::Event( const std::string& id )
    : m_id( id )
    {
    }

    Events::Event::~Event()
    {
    }

    std::string Events::Event::getId()
    {
        return m_id;
    }

    void Events::Event::setId( const std::string& id )
    {
        m_id = id;
    }

}
