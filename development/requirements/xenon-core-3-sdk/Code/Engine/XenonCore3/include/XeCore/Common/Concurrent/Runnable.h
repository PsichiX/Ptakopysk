#ifndef __XE_CORE__COMMON__CONCURRENT__RUNNABLE__
#define __XE_CORE__COMMON__CONCURRENT__RUNNABLE__

#include "../MemoryManager.h"

namespace XeCore
{
    namespace Common
    {
        namespace Concurrent
        {
            class Runnable
                : public virtual IRtti
                , public virtual MemoryManager::Manageable
            {
                RTTI_CLASS_DECLARE( Runnable );

            public:
                                Runnable();

                virtual void    run() {};
            };
        }
    }
}

#endif
