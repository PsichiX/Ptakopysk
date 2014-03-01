#ifndef __XE_CORE__INTUICIO__COMPILER_VM__
#define __XE_CORE__INTUICIO__COMPILER_VM__

#include <map>
#include "ProgramVM.h"
#include "../Common/Buffer.h"
#include "../Common/String.h"

namespace XeCore
{
    namespace Intuicio
    {
        using namespace XeCore::Common;

        class CompilerVM
		{
        public:
            static ProgramVM*                           compile( const String& input, String& outErrors );

            static const ProgramVM::Version             version;
            static const char                           versionString[];

            struct Data
            {
                dword                                   address;
                dword                                   size;
                ProgramVM::DataType                     type;
            };

            struct Field
            {
                String                                  type;
                unsigned int                            count;
                unsigned int                            offset;
            };

            struct Struct
            {
                std::map< String, Field >               fields;
                unsigned int                            size;
            };
        };
    }
}

#endif
