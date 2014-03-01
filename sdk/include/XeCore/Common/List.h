#ifndef __XE_CORE__COMMON__LIST__
#define __XE_CORE__COMMON__LIST__

#include <list>

namespace XeCore
{
	namespace Common
	{
	    template< typename T >
	    struct List : public std::list< T > {};
	}
}

#endif
