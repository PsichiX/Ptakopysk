#ifndef __XE_CORE__COMMON__CONCURRENT__THREAD__
#define __XE_CORE__COMMON__CONCURRENT__THREAD__

#include "TinyThreads/tinythread.h"
#include "Synchronized.h"
#include "Runnable.h"

namespace XeCore
{
    namespace Common
    {
        namespace Concurrent
        {
            class Thread
                : public virtual IRtti
                , public virtual MemoryManager::Manageable
                , public Synchronized
                , public Runnable
            {
                RTTI_CLASS_DECLARE( Thread );

                friend class RunnableData;

            public:
                enum State
                {
                    NEW,
                    RUNNING,
                    TERMINATED
                };

                                        Thread();
                                        Thread( Runnable* r );
                                        ~Thread();

                unsigned long			getId();
                State					getState();
                void					join();
                bool                    isJoinable();
                void					start();
                virtual void			run() {}

                static unsigned int		hardwareConcurrency();
                static void             sleep( unsigned int millis );

            private:
                class RunnableData;

                static void				_run( void* args );

                void                    _reset();

                tthread::thread*		m_thread;
                Runnable*				m_runnable;
                RunnableData*			m_runnableData;
                volatile State			m_state;

                class RunnableData
                {
                public:
                    Runnable*			runnable;
                    Thread*				owner;

                                        RunnableData( Runnable* r, Thread* o );
                };
            };
        }
    }
}

#endif
