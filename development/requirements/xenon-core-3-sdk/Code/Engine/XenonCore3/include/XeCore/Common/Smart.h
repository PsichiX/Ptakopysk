#ifndef __XE_CORE__COMMON__SMART__
#define __XE_CORE__COMMON__SMART__

namespace XeCore
{
    namespace Common
    {
        template< typename T >
        class Smart
        {
        public:
            class OnDataDestroyListener
            {
            public:
                virtual                 ~OnDataDestroyListener() {};
                virtual void            onDataDestroy( Smart< T >* owner ) = 0;
            };

        private:
            struct Info
            {
                unsigned int            counter;
                T*                      data;
                                        Info( T* d ) : counter( 1 ), data( d ) {}
            };

            Info*                       m_info;
            Smart< T >*                 m_prev;
            Smart< T >*                 m_next;
            OnDataDestroyListener*      m_onDataDestroyListener;

            static T                    s_temp;

        public:
                                        Smart();
                                        Smart( T* pointer );
                                        Smart( const T& clone );
                                        Smart( Smart< T >& smart );
                                        ~Smart();

            FORCEINLINE void            ref( T* pointer );
            FORCEINLINE void            ref( const T& clone );
            FORCEINLINE void            ref( Smart< T >& smart );
            FORCEINLINE void            unref();
            FORCEINLINE void            abstractRef();
            FORCEINLINE void            abstractUnref();
            FORCEINLINE T*              pointer();
            FORCEINLINE const T&        value();
            FORCEINLINE bool            empty();
            FORCEINLINE unsigned int    count();

            FORCEINLINE void            operator= ( const T& clone );
            FORCEINLINE void            operator= ( const Smart< T >& smart );
            FORCEINLINE T*              operator-> ();
            FORCEINLINE const T&        operator() ();

        private:
            FORCEINLINE void            validate();
            FORCEINLINE Smart< T >*     onInvalid( bool dir );
        };
    }
}

#include "Smart.inl"

#endif
