#ifndef __XE_CORE__INTUICIO__PRECOMPILER_VM__
#define __XE_CORE__INTUICIO__PRECOMPILER_VM__

#include <list>
#include <map>
#include "ProgramVM.h"
#include "../Common/Buffer.h"
#include "../Common/String.h"

namespace XeCore
{
    namespace Intuicio
    {
        using namespace XeCore::Common;

        class PrecompilerVM
		{
        public:
            class OnLoadContentListener
            {
            public:
                virtual bool    onLoadContent( const String& fname, String& outOutput, String& outErrors ) { return false; }
            };

            class CrossPrecompilationData
            {
            public:
                std::list< std::vector< String > > args;
                std::map< String, String > defines;
                std::map< String, int > counters;
            };

            static bool         precompile( const String& input, String& outOutput, OnLoadContentListener* onLoadContent, String& outErrors, CrossPrecompilationData* cpd = 0 );
        };
    }
}

#endif
