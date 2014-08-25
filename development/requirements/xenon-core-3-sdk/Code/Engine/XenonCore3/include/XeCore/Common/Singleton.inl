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
		bool Singleton< T >::m_isShared = false;

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
			if( m_isShared )
            {
                Singleton< T >::m_instance = 0;
                Singleton< T >::m_isShared = false;
            }
            else
                DELETE_OBJECT( Singleton< T >::m_instance );
		}

		template< class T >
		void Singleton< T >::makeSharedFrom( T* source )
		{
		    Singleton< T >::destroy();
		    Singleton< T >::m_instance = source;
		    Singleton< T >::m_isShared = true;
		}
	}
}

#endif
