#ifndef __XE_CORE__COMMON__SINGLETON__INLINE__
#define __XE_CORE__COMMON__SINGLETON__INLINE__

#include "MemoryManager.h"

namespace XeCore
{
	namespace Common
	{
		template< class T >
		T* Singleton< T >::m_instance = 0;

		template< class T >
		T& Singleton< T >::use()
		{
			return( *instance() );
		}

		template< class T >
		T* Singleton< T >::instance()
		{
			if( !Singleton< T >::m_instance )
				Singleton< T >::m_instance = xnew T();
			return( Singleton< T >::m_instance );
		}

		template< class T >
		void Singleton< T >::destroy()
		{
			DELETE_OBJECT( Singleton< T >::m_instance );
		}
	}
}

#endif
