#ifndef __XE_CORE__COMMON__LOGGER__
#define __XE_CORE__COMMON__LOGGER__

#include <cstdio>
#include "Base.h"
#include "Singleton.h"
#include "IRtti.h"

#if !defined(NDEBUG)
#define LOG_SETUP(f)					XeCore::Common::Logger::use().setup( f )
#else
#define LOG_SETUP(...)
#endif

#if !defined(NDEBUG)
#include <assert.h>
#define LOG(str...)					    XeCore::Common::Logger::use().log( str )
#define LOGNL(str...)					XeCore::Common::Logger::use().lognl( str )
#define XASSERT(expr,str...)			if( !( expr ) ) { LOGNL( ">>>>>> Assertion Report:" ); LOGNL( str ); LOGNL( "" ); assert( false ); }
#define XWARNING(str...)				{ LOGNL( ">>>>>> Warning Report:" ); LOGNL( str ); LOGNL( "" ); }
#if defined( PEDANTIC_MODE )
#define XPEDANTICWARNING				XASSERT
#else
#define XPEDANTICWARNING				XWARNING
#endif
#else
#define LOG(...)
#define LOGNL(...)
#define XBREAKPOINT
#define XASSERT(...)
#define XWARNING(...)
#define XPEDANTICWARNING
#endif

#include "MemoryManager.h"

namespace XeCore
{
	namespace Common
	{
		class Logger
			: public virtual IRtti
			, public virtual MemoryManager::Manageable
			, public Singleton< Logger >
		{
			RTTI_CLASS_DECLARE( Logger );

		public:
								Logger();
								~Logger();

			void	            setup( const char* fname );
			void	            log( const char* format, ... );
			void	            lognl( const char* format, ... );

		private:
			FILE*				m_fstdout;
			const char*			m_fname;
		};
	}
}

#endif
