#ifndef __XE_CORE__INTUICIO__PROGRAM_VM__
#define __XE_CORE__INTUICIO__PROGRAM_VM__

#include "../Common/IRtti.h"
#include "../Common/MemoryManager.h"
#include "../Common/Vector.h"
#include "../Common/Buffer.h"
#include "../Common/String.h"
#include <map>

namespace XeCore
{
    namespace Intuicio
    {
        using namespace XeCore::Common;

        class ProgramVM
			: public virtual IRtti
			, public virtual MemoryManager::Manageable
        {
        public:
            struct Version;
            struct Info;
            struct Pointer;

        private:
            RTTI_CLASS_DECLARE( ProgramVM );

        public:
                                    ProgramVM();
            virtual                 ~ProgramVM();

            ByteBuffer*             getBuffer();
            bool                    validate();
            bool                    getVersion( Version& outVersion );
            bool                    getInfo( Info& outInfo );
            bool                    getExternals( Vector< String >& outNames );
            bool                    getExports( std::map< String, Pointer >& outExports );
            bool                    getImports( Vector< String >& outNames );
            bool                    fromBytes( const unsigned char* bytes, unsigned int size );

        private:
            ByteBuffer*             m_buffer;

        public:
            struct Version
            {
                word                major;
                word                minor;
            };

            struct Header
            {
                byte                symbol[ 4 ];
                Version             version;
            };

            struct Info
            {
                word                registersI;
                word                registersF;
                dword               stackSize;
                dword               externals;
                dword               exports;
                dword               imports;
                dword               addressData;
                dword               addressCode;
                dword               addressExternals;
                dword               addressExports;
                dword               addressImports;
                dword               sizeData;
                dword               sizeCode;
                dword               sizeExternals;
                dword               sizeExports;
                dword               sizeImports;
            };

            enum MemoryLocation
            {
                ML_NONE,
                ML_DATA,
                ML_REGI,
                ML_REGF,
                ML_CODE,
                ML_EXTERNAL
            };

            struct Pointer
            {
                dword               address;
                MemoryLocation      location;
                word                isPointer;
                word                isManaged;
                dword               pointerOffset;
            };

            enum OpCode
            {
                OP_SP_EXIT,
                OP_SP_REGISTERS_I,
                OP_SP_REGISTERS_F,
                OP_SP_STACK,
                OP_SP_DATA,
                OP_SP_START,
                OP_SP_JUMP,
                OP_SP_EXTERNAL,
                OP_SP_STRUCT_DEF,
                OP_SP_STRUCT_END,
                OP_SP_FIELD,
                OP_SP_EXPORT,
                OP_SP_IMPORT,
                OP_SP_NAMESPACE,
                OP_SP_NAMESPACE_END,
                OP_SP_NAMESPACE_USING,
                OP_INTC,
                OP_INTS,
                OP_DBGI,
                OP_DBGF,
                OP_DBGB,
                OP_MOVI,
                OP_MOVF,
                OP_MOVB,
                OP_MOV,
                OP_ADDI,
                OP_ADDF,
                OP_SUBI,
                OP_SUBF,
                OP_MULI,
                OP_MULF,
                OP_DIVI,
                OP_DIVF,
                OP_INCI,
                OP_INCF,
                OP_DECI,
                OP_DECF,
                OP_RCPF,
                OP_MINI,
                OP_MINF,
                OP_MAXI,
                OP_MAXF,
                OP_FLRF,
                OP_CILF,
                OP_FRCF,
                OP_SQTI,
                OP_SQTF,
                OP_RSQF,
                OP_POWI,
                OP_POWF,
                OP_LOGF,
                OP_EXPF,
                OP_SINF,
                OP_COSF,
                OP_TANF,
                OP_ABSI,
                OP_ABSF,
                OP_NEGI,
                OP_NEGF,
                OP_SGNI,
                OP_SGNF,
                OP_MODF,
                OP_CLPI,
                OP_CLPF,
                OP_MIXF,
                OP_TLTI,
                OP_TLTF,
                OP_TGTI,
                OP_TGTF,
                OP_TETI,
                OP_TETF,
                OP_NOT,
                OP_AND,
                OP_OR,
                OP_XOR,
                OP_BSHL,
                OP_BSHR,
                OP_GOTO,
                OP_JIFI,
                OP_JIFF,
                OP_CALL,
                OP_RET,
                OP_PSHI,
                OP_PSHF,
                OP_PSHB,
                OP_PSH,
                OP_POPI,
                OP_POPF,
                OP_POPB,
                OP_POP,
                OP_SHFI,
                OP_SHFF,
                OP_SHFB,
                OP_SHF,
                OP_SHBI,
                OP_SHBF,
                OP_SHBB,
                OP_SHB,
                OP_SAVE,
                OP_LOAD,
                OP_HEAP,
                OP_NEW,
                OP_DEL,
                OP_PTR,
                OP_OFF,
                OP_MOBJ,
                OP_MDEL,
                OP_MNEW,
                OP_MREF,
                OP_MDER,
                OP_MCNT,
                OP_MPSH,
                OP_MPOP,
                OP_ITF,
                OP_FTI
            };

            enum DataType
            {
                DT_VOID,
                DT_INTEGER,
                DT_FLOAT,
                DT_BYTES
            };
        };
    }
}

#endif
