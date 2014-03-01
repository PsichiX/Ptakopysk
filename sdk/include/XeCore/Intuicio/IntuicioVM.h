#ifndef __XE_CORE__INTUICIO__INTUICIO_VM__
#define __XE_CORE__INTUICIO__INTUICIO_VM__

#include "../Common/IRtti.h"
#include "../Common/MemoryManager.h"
#include "../Common/Vector.h"
#include "ContextVM.h"

namespace XeCore
{
    namespace Intuicio
    {
        using namespace XeCore::Common;

        class IntuicioVM
			: public virtual IRtti
			, public virtual MemoryManager::Manageable
        {
            RTTI_CLASS_DECLARE( IntuicioVM );

        public:
                                    IntuicioVM();
            virtual                 ~IntuicioVM();

            ContextVM*              createContext( ProgramVM* prog, const char* name = 0 );
            bool                    destroyContext( ContextVM* cntx );
            ContextVM*              getContext( const char* name );

        private:
            Vector< ContextVM* >    m_contexts;
        };
    }
}

#endif
