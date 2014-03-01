#ifndef __XE_CORE__COMMON__SMART__INLINE__
#define __XE_CORE__COMMON__SMART__INLINE__

namespace XeCore
{
    namespace Common
    {
        template< typename T >
        T Smart< T >::s_temp;

        template< typename T >
        Smart< T >::Smart()
            : m_info( 0 )
            , m_prev( 0 )
            , m_next( 0 )
            , m_onDataDestroyListener( 0 )
        {
        }

        template< typename T >
        Smart< T >::Smart( T* pointer )
            : m_info( 0 )
            , m_prev( 0 )
            , m_next( 0 )
            , m_onDataDestroyListener( 0 )
        {
            ref( pointer );
        }

        template< typename T >
        Smart< T >::Smart( const T& clone )
            : m_info( 0 )
            , m_prev( 0 )
            , m_next( 0 )
            , m_onDataDestroyListener( 0 )
        {
            ref( clone );
        }

        template< typename T >
        Smart< T >::Smart( Smart< T >& smart )
            : m_info( 0 )
            , m_prev( 0 )
            , m_next( 0 )
            , m_onDataDestroyListener( 0 )
        {
            ref( smart );
        }

        template< typename T >
        Smart< T >::~Smart()
        {
            unref();
        }

        template< typename T >
        void Smart< T >::ref( T* pointer )
        {
            unref();
            m_info = xnew Info( pointer );
            m_prev = 0;
            m_next = 0;
        }

        template< typename T >
        void Smart< T >::ref( const T& clone )
        {
            unref();
            m_info = xnew Info( xnew T( clone ) );
            m_prev = 0;
            m_next = 0;
        }

        template< typename T >
        void Smart< T >::ref( Smart< T >& smart )
        {
            unref();
            if( smart.m_info )
            {
                m_info = smart.m_info;
                m_info->counter++;
                if( !smart.m_next )
                {
                    smart.m_next = this;
                    m_prev = &smart;
                }
                else
                {
                    Smart< T >* t = smart.m_next;
                    smart.m_next = this;
                    t->m_prev = this;
                    m_prev = &smart;
                    m_next = t;
                }
            }
        }

        template< typename T >
        void Smart< T >::unref()
        {
            if( !m_info )
                return;
            m_info->counter--;
            validate();
            if( m_prev )
                m_prev->m_next = m_next;
            if( m_next )
                m_next->m_prev = m_prev;
            m_info = 0;
            m_prev = 0;
            m_next = 0;
        }

        template< typename T >
        void Smart< T >::abstractRef()
        {
            if( m_info )
                m_info->counter++;
        }

        template< typename T >
        void Smart< T >::abstractUnref()
        {
            if( m_info )
                m_info->counter--;
        }

        template< typename T >
        T* Smart< T >::pointer()
        {
            return m_info ? m_info->data : 0;
        }

        template< typename T >
        const T& Smart< T >::value()
        {
            return m_info && m_info->data ? *m_info->data : s_temp;
        }

        template< typename T >
        bool Smart< T >::empty()
        {
            return !m_info;
        }

        template< typename T >
        unsigned int Smart< T >::count()
        {
            return m_info ? m_info->counter : 0;
        }

        template< typename T >
        void Smart< T >::operator= ( const T& clone )
        {
            ref( clone );
        }

        template< typename T >
        void Smart< T >::operator= ( const Smart< T >& smart )
        {
            ref( smart );
        }

        template< typename T >
        T* Smart< T >::operator-> ()
        {
            return pointer();
        }

        template< typename T >
        const T& Smart< T >::operator() ()
        {
            return value();
        }

        template< typename T >
        void Smart< T >::validate()
        {
            if( m_info && m_info->counter <= 0 )
            {
                if( m_onDataDestroyListener )
                    m_onDataDestroyListener->onDataDestroy( this );
                m_info->counter = 0;
                DELETE_OBJECT( m_info->data );
                DELETE_OBJECT( m_info );
                Smart< T >* o = m_prev;
                while( o )
                    o = o->onInvalid( false );
                o = m_next;
                while( o )
                    o = o->onInvalid( true );
            }
        }

        template< typename T >
        Smart< T >* Smart< T >::onInvalid( bool dir )
        {
            Smart< T >* o = dir ? m_next : m_prev;
            m_info = 0;
            m_prev = 0;
            m_next = 0;
            return o;
        }
    }
}

#endif
