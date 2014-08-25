#ifndef __XE_CORE__COMMON__SINGLETON__
#define __XE_CORE__COMMON__SINGLETON__

namespace XeCore
{
	namespace Common
	{
		template< class T >
		class Singleton
		{
		public:
			static T&	use();
			static T*	instance();
			static void	destroy();
			static void makeSharedFrom( T* source );

		protected:
						Singleton() {}
						Singleton( const T& ) {}
						Singleton& operator= ( T const& ) {}

		private:
			static T*	m_instance;
			static bool m_isShared;
		};
	}
}

#include "Singleton.inl"

#endif
