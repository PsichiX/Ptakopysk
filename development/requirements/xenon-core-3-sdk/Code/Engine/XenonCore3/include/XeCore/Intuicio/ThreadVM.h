#ifndef __XE_CORE__INTUICIO__THREAD_VM__
#define __XE_CORE__INTUICIO__THREAD_VM__

#include "../Common/IRtti.h"
#include "../Common/MemoryManager.h"
#include "../Common/Vector.h"
#include "ParallelThreadVM.h"
#include "../Common/Concurrent/Thread.h"

namespace XeCore
{
    namespace Intuicio
    {
        using namespace XeCore::Common;

        class ContextVM;
        class ProgramVM;

        class ThreadVM
			: public virtual IRtti
			, public virtual MemoryManager::Manageable
        {
            friend class ContextVM;
            friend class ParallelThreadVM;

            RTTI_CLASS_DECLARE( ThreadVM );

        protected:
                                            ThreadVM( ContextVM* owner, ProgramVM* prog, unsigned int startAddress = 0 );

        public:
            virtual                         ~ThreadVM();

            bool                            start();
            void                            stop();
            void                            waitForTerminate();

        protected:
            void                            notifyCompleted( ParallelThreadVM* pt );
            void                            cleanup();

        private:
            ContextVM*                      m_owner;
            ProgramVM*                      m_program;
            unsigned int                    m_startAddress;
            Vector< ParallelThreadVM* >     m_parThreads;
            Vector< bool >                  m_completed;
            Synchronized                    m_sync;
        };
    }
}

#endif
