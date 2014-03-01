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

		protected:
						Singleton() {}
						Singleton( const T& ) {}
						Singleton& operator= ( T const& ) {}

		private:
			static T*	m_instance;
		};
	}
}

#include "Singleton.inl"

#endif
