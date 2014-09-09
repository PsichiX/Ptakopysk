#ifndef __XE_CORE__COMMON__CONCURRENT__SYNCHRONIZED__
#define __XE_CORE__COMMON__CONCURRENT__SYNCHRONIZED__

#include "TinyThreads/tinythread.h"
#include "../MemoryManager.h"

#define __SYNCHRONIZED_OBJECT(name, obj)    XeCore::Common::Concurrent::Synchronized::ScopeGuard \
                                            __XeCore_Common_Concurrent_Synchronized_ScopeGuard_##name( ( obj ) )

#define __SYNCHRONIZED_SCOPE_BY_NAME(name)  static XeCore::Common::Concurrent::Synchronized \
                                            __XeCore_Common_Concurrent_Synchronized_##name; \
                                            __SYNCHRONIZED_OBJECT( name, __XeCore_Common_Concurrent_Synchronized_##name )

#define SYNCHRONIZED_HERE                   __SYNCHRONIZED_SCOPE_BY_NAME( __COUNTER__ )
#define SYNCHRONIZED_OBJECT(obj)            __SYNCHRONIZED_OBJECT( __COUNTER__, obj )

namespace XeCore
{
    namespace Common
    {
        namespace Concurrent
        {
            class Synchronized
                : public virtual IRtti
                , public virtual MemoryManager::Manageable
            {
                RTTI_CLASS_DECLARE( Synchronized );

            public:
                                        Synchronized();
                                        ~Synchronized();

                void					lock();
                bool                    tryLock();
                void					unlock();
                bool                    isLocked();

            private:
                tthread::mutex*			m_mutex;

            public:
                class ScopeGuard
                    : public virtual IRtti
                    , public virtual MemoryManager::Manageable
                {
                    RTTI_CLASS_DECLARE( ScopeGuard );

                public:
                                        ScopeGuard();
                                        explicit ScopeGuard( Synchronized& sync );
                                        ~ScopeGuard();

                private:
                    Synchronized*       m_sync;
                };
            };
        }
    }
}

#endif
