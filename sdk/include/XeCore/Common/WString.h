#ifndef __XE_CORE__COMMON__WSTRING__
#define __XE_CORE__COMMON__WSTRING__

#include <string>
#include <XeCore/Common/IRtti.h>
#include <XeCore/Common/MemoryManager.h>

namespace XeCore
{
	namespace Common
	{
		class WString
			: public virtual IRtti
			, public virtual MemoryManager::Manageable
			, public std::wstring
		{
			RTTI_CLASS_DECLARE( WString );

		public:
                                WString();
                                WString( const std::wstring& str );
                                WString( const std::wstring& str, size_t pos, size_t len = std::wstring::npos );
                                WString( const wchar_t* str );
                                WString( const wchar_t* str, size_t n );
                                WString( size_t n, wchar_t c );

			void	            format( const wchar_t* str, ... );
			WString*            explode( const wchar_t* delim, unsigned int& outCount, bool keepEmpty = false, wchar_t multiDelims = 0, const wchar_t* blockers = 0 ) const;
			template< typename T >
			FORCEINLINE bool    convertTo( T& outVal );
			template< typename T >
			FORCEINLINE bool    convertFrom( const T& val );
		};
	}
}

#include "WString.inl"

#endif
