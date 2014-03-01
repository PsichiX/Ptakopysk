#ifndef __XE_CORE__INTUICIO__PARALLEL_THREAD_VM__
#define __XE_CORE__INTUICIO__PARALLEL_THREAD_VM__

#include "../Common/IRtti.h"
#include "../Common/MemoryManager.h"
#include "../Common/List.h"
#include "../Common/Concurrent/Thread.h"

namespace XeCore
{
    namespace Intuicio
    {
        using namespace XeCore::Common;
        using namespace XeCore::Common::Concurrent;

        class ThreadVM;
        class ProgramVM;

        class ParallelThreadVM
			: public virtual IRtti
			, public virtual MemoryManager::Manageable
			, public Thread
        {
            friend class ContextVM;

            RTTI_CLASS_DECLARE( ParallelThreadVM );

        public:
                                            ParallelThreadVM( ThreadVM* owner, ProgramVM* prog );
            virtual                         ~ParallelThreadVM();

            void                            pushFragment( unsigned int begin, unsigned int end, unsigned int startAddress );
            void                            stop();
            virtual void                    run();

        private:
            struct ProgramFragment
            {
                unsigned int                begin;
                unsigned int                end;
                unsigned int                start;
            };

            struct StackPosition
            {
                unsigned int                last;
                unsigned int                address;
            };

            ThreadVM*                       m_owner;
            ProgramVM*                      m_program;
            List< ProgramFragment >         m_fragments;
            int*                            m_regI;
            float*                          m_regF;
            unsigned int                    m_regIcount;
            unsigned int                    m_regFcount;
            byte*                           m_stack;
            unsigned int                    m_stackSize;
            unsigned int                    m_stackPos;
            List< StackPosition >           m_stackFrames;
            volatile bool                   m_running;
            Synchronized                    m_sync;
        };
    }
}

#endif
