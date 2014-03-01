#ifndef __XE_CORE__COMMON__VECTOR__
#define __XE_CORE__COMMON__VECTOR__

#include <vector>

namespace XeCore
{
	namespace Common
	{
	    template< typename T >
	    struct Vector : public std::vector< T > {};
	}
}

#endif
