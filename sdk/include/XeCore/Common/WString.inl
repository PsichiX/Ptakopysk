#ifndef __XE_CORE__COMMON__STRING__INLINE__
#define __XE_CORE__COMMON__STRING__INLINE__

#include <sstream>

namespace XeCore
{
	namespace Common
	{
		template< typename T >
        bool WString::convertTo( T& outVal )
        {
            std::wstringstream ss( c_str() );
            ss >> outVal;
            return !ss.fail();
        }

        template< typename T >
        bool WString::convertFrom( const T& val )
        {
            std::wstringstream ss;
            ss << val;
            assign( ss.str() );
            return !ss.fail();
        }
	}
}

#endif
