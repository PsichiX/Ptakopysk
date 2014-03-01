#ifndef __XE_CORE__COMMON__STRING__
#define __XE_CORE__COMMON__STRING__

#include <string>
#include "IRtti.h"
#include "MemoryManager.h"

namespace XeCore
{
	namespace Common
	{
		class String
			: public virtual IRtti
			, public virtual MemoryManager::Manageable
			, public std::string
		{
			RTTI_CLASS_DECLARE( String );

		public:
                                String();
                                String( const std::string& str );
                                String( const std::string& str, size_t pos, size_t len = std::string::npos );
                                String( const char* str );
                                String( const char* str, size_t n );
                                String( size_t n, char c );

			void	            format( const char* str, ... );
			String*             explode( const char* delim, unsigned int& outCount, bool keepEmpty = false, char multiDelims = 0, const char* blockers = 0 ) const;
			template< typename T >
			FORCEINLINE bool    convertTo( T& outVal );
			template< typename T >
			FORCEINLINE bool    convertFrom( const T& val );
		};
	}
}

#include "String.inl"

#endif
