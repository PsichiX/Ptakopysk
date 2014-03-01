#ifndef __XE_CORE__COMMON__BOOL_POOL__INLINE__
#define __XE_CORE__COMMON__BOOL_POOL__INLINE__

namespace XeCore
{
	namespace Common
	{
		template< int Capacity >
		BoolPool< Capacity >::BoolPool()
		{
			for( int i = (Capacity / 8) + 1; i-- > 0; )
				m_pool[ i ] = 0;
		}

		template< int Capacity >
		BoolPool< Capacity >::~BoolPool()
		{
		}

		template< int Capacity >
		void BoolPool< Capacity >::set( int id, bool v )
		{
			if( id < 0 || id >= Capacity )
				return;
			if( v )
				m_pool[ id / 8 ] |= 1 << ( id % 8 );
			else
				m_pool[ id / 8 ] &= ~( 1 << ( id % 8 ) );
		}

		template< int Capacity >
		void BoolPool< Capacity >::toggle( int id )
		{
			if( id >= Capacity )
				return;
			m_pool[ id / 8 ] ^= 1 << ( id % 8 );
		}

		template< int Capacity >
		bool BoolPool< Capacity >::get( int id )
		{
			if( id >= Capacity )
				return( false );
			return( m_pool[ id / 8 ] & ( 1 << ( id % 8 ) ) ? true : false );
		}

		template< int Capacity >
		void BoolPool< Capacity >::operator() ( int id, bool v )
		{
			set( id, v );
		}

		template< int Capacity >
		bool BoolPool< Capacity >::operator[] ( int id )
		{
			return( get( id ) );
		}
	}
}

#endif
