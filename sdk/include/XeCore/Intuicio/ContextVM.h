#ifndef __XE_CORE__INTUICIO__CONTEXT_VM__
#define __XE_CORE__INTUICIO__CONTEXT_VM__

#include "../Common/IRtti.h"
#include "../Common/MemoryManager.h"
#include "../Common/Vector.h"
#include "../Common/String.h"
#include "ProgramVM.h"
#include "ThreadVM.h"
#include <map>

namespace XeCore
{
    namespace Intuicio
    {
        class IntuicioVM;

        using namespace XeCore::Common;

        class ContextVM
			: public virtual IRtti
			, public virtual MemoryManager::Manageable
        {
            friend class IntuicioVM;
            friend class ParallelThreadVM;

            RTTI_CLASS_DECLARE( ContextVM );

        public:
            class OnInterceptListener
            {
            public:
                virtual                         ~OnInterceptListener() {};
                virtual bool                    onIntercept( ParallelThreadVM* caller, unsigned int code ) = 0;
            };

        protected:
                                                ContextVM( IntuicioVM* owner, ProgramVM* prog, const char* name = 0 );

        public:
            virtual                             ~ContextVM();

            String&                             getName();
            bool                                bindExternals( std::map< String, void* >& externals );
            void                                unbindExternals();
            bool                                registerInterceptionListener( const String& name, OnInterceptListener* listener );
            bool                                unregisterInterceptionListener( const String& name );
            void                                unregisterAllInterceptionListeners();
            bool                                runProgram( bool wait = false, unsigned int startAddress = 0 );
            void                                terminateProgram();
            void                                waitForTerminate();
            bool                                stackPush( ParallelThreadVM* caller, const void* src, unsigned int size );
            bool                                stackPop( ParallelThreadVM* caller, void* dst, unsigned int size );
            void*                               stackAllocOn( ParallelThreadVM* caller, unsigned int size );
            bool                                stackShiftForward( ParallelThreadVM* caller, unsigned int size );
            bool                                stackShiftBackward( ParallelThreadVM* caller, unsigned int size );
            bool                                stackCall( ParallelThreadVM* caller, const unsigned int addr );
            bool                                stackReturn( ParallelThreadVM* caller, unsigned int& outAddr );
            bool                                stackSave( ParallelThreadVM* caller, unsigned int& dst );
            bool                                stackLoad( ParallelThreadVM* caller, const unsigned int src );
            void*                               resolveAddress( ParallelThreadVM* caller, const ProgramVM::Pointer& ptr );

        protected:
            void                                cleanup();
            void                                execute( ParallelThreadVM* caller, unsigned int begin, unsigned int end, unsigned int start );

        private:
            class ManagedMemory
            {
            public:
                                                ManagedMemory( unsigned int size );
                                                ~ManagedMemory();

                void*                           data;
            };

            IntuicioVM*                                 m_owner;
            ProgramVM*                                  m_program;
            String                                      m_name;
            std::map< String, void* >                   m_exports;
            std::map< String, void* >                   m_imports;
            ProgramVM::Info                             m_info;
            Vector< ThreadVM* >                         m_threads;
            byte*                                       m_data;
            void**                                      m_externals;
            std::map< String, OnInterceptListener* >    m_intercepts;
            OnInterceptListener*                        m_intercept;
        };
    }
}

#endif
