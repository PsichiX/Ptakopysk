#ifndef __PTAKOPYSK__EVENTS__
#define __PTAKOPYSK__EVENTS__

#include <XeCore/Common/Base.h>
#include <XeCore/Common/IRtti.h>
#include <XeCore/Common/MemoryManager.h>
#include <XeCore/Common/Singleton.h>
#include <string>
#include <list>

namespace Ptakopysk
{

    class Events
    : public virtual XeCore::Common::IRtti
    , public virtual XeCore::Common::MemoryManager::Manageable
    , public XeCore::Common::Singleton< Events >
    {
        RTTI_CLASS_DECLARE( Events );

    public:
        class Event;
        typedef void ( *OnEventCallback )( Event* );

        Events();
        ~Events();

        void setCallback( OnEventCallback cb );
        OnEventCallback getCallback();
        void push( Event* event );
        void dispatch();

    protected:
        void onEvent( Event* event );

    private:
        std::list< Event* > m_events;
        OnEventCallback m_callback;

    public:
        class Event
        {
        public:
            Event( const std::string& id );
            ~Event();

            std::string getId();
            void setId( const std::string& id );

        private:
            std::string m_id;
        };
    };

}

#endif
