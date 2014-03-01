#ifndef __XE_CORE__COMMON__IRTTI__INLINE__
#define __XE_CORE__COMMON__IRTTI__INLINE__

namespace XeCore
{
	namespace Common
	{
		template< class T >
		bool IRtti::isType( IRtti* o )
		{
			return( o ? ( o->___RttiObjectDerivationTable___ == T::___RttiClassDerivationTable___ ) : false );
		}

		template< class T >
		bool IRtti::isDerived( IRtti* o )
		{
			return( o ? ( ___RttiDerived___( RTTI_DERIVATION( T ), o->___RttiObjectDerivationTable___ ) ) : false );
		}

		template< class Tf,class Tt >
		Tt* IRtti::derivationCast( Tf* o )
		{
			return( isDerived< Tt >( o ) ? (Tt*)(void*)o : 0 );
		}
	}
}

#endif
