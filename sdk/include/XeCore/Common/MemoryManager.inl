#ifndef __XE_CORE__COMMON__MEMORY_MANAGER__INLINE__
#define __XE_CORE__COMMON__MEMORY_MANAGER__INLINE__

namespace XeCore
{
	namespace Common
	{
		template< class T >
		T* MemoryManager::__new()
		{
			T* p = (T*)MemoryManager::_new( sizeof( T ) );
			new( p ) T();
			return( p );
		}

		template< class T >
		void MemoryManager::__delete( T* p )
		{
			p->~T();
			MemoryManager::_delete( p );
		}

		template< class T >
		T* MemoryManager::__newDebug( const char* f, int l )
		{
			T* p = (T*)MemoryManager::_newDebug( sizeof( T ), f, l );
			new( p ) T();
			return( p );
		}

		template< class T >
		void MemoryManager::__deleteDebug( T* p )
		{
			p->~T();
			MemoryManager::_deleteDebug( p );
		}

		template< class T >
		T* MemoryManager::__newArray( unsigned int c )
		{
			T* p = (T*)MemoryManager::_new( sizeof( T ) * c );
			T* _p = p;
			for( unsigned int i = 0; i < c; i++, _p++ )
				new( _p ) T();
			return( p );
		}

		template< class T >
		void MemoryManager::__deleteArray( T* p, unsigned int c )
		{
			T* _p = p;
			for( unsigned int i = 0; i < c; i++, _p++ )
				_p->~T();
			MemoryManager::_delete( p );
		}

		template< class T >
		T* MemoryManager::__newArrayDebug( unsigned int c, const char* f, int l )
		{
			T* p = (T*)MemoryManager::_newDebug( sizeof( T ), f, l );
			T* _p = p;
			for( unsigned int i = 0; i < c; i++, _p++ )
				new( _p ) T();
			return( p );
		}

		template< class T >
		void MemoryManager::__deleteArrayDebug( T* p, unsigned int c )
		{
			T* _p = p;
			for( unsigned int i = 0; i < c; i++, _p++ )
				_p->~T();
			MemoryManager::_deleteDebug( p );
		}
	}
}

#endif
