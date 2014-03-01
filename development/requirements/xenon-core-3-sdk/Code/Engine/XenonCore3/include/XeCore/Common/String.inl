#ifndef __XE_CORE__COMMON__STRING__INLINE__
#define __XE_CORE__COMMON__STRING__INLINE__

#include <sstream>

namespace XeCore
{
	namespace Common
	{
		template< typename T >
        bool String::convertTo( T& outVal )
        {
            std::stringstream ss( c_str() );
            ss >> outVal;
            return !ss.fail();
        }

        template< typename T >
        bool String::convertFrom( const T& val )
        {
            std::stringstream ss;
            ss << val;
            assign( ss.str() );
            return !ss.fail();
        }
	}
}

#endif
