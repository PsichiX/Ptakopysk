#ifndef __XE_CORE__COMMON__BOOL_POOL__
#define __XE_CORE__COMMON__BOOL_POOL__

#include "Base.h"

namespace XeCore
{
	namespace Common
	{
		template< int Capacity >
		class BoolPool
		{
		public:
								BoolPool();
								~BoolPool();

			FORCEINLINE void	set( int id, bool v );
			FORCEINLINE void	toggle( int id );
			FORCEINLINE bool	get( int id );

			FORCEINLINE void	operator() ( int id, bool v );
			FORCEINLINE bool	operator[] ( int id );

		private:
			unsigned char		m_pool[ ( Capacity / 8 ) + 1 ];
		};
	}
}

#include "BoolPool.inl"

#endif
