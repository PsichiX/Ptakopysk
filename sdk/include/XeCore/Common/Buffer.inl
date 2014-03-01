#ifndef __XE_CORE__COMMON__BUFFER__INLINE__
#define __XE_CORE__COMMON__BUFFER__INLINE__

#include <cstring>
#include <algorithm>
#include "MemoryManager.h"

namespace XeCore
{
	namespace Common
	{
		template< typename T, BufferDataCopyMethod BDCM >
		Buffer< T, BDCM >::Buffer( unsigned int size )
		: m_data( 0 )
		, m_position( 0 )
		, m_size( 0 )
		, m_autoResize( false )
		, m_autoResizeCount( 1024 )
		{
            resize( size );
		}

		template< typename T, BufferDataCopyMethod BDCM >
		Buffer< T, BDCM >::~Buffer()
		{
		    clear();
		}

        template< typename T, BufferDataCopyMethod BDCM >
		void Buffer< T, BDCM >::clear()
		{
			DELETE_ARRAY( m_data );
			m_position = 0;
			m_size = 0;
		}

        template< typename T, BufferDataCopyMethod BDCM >
		void Buffer< T, BDCM >::resize( unsigned int size, bool keepData )
		{
		    if( keepData )
            {
                if( size == 0 )
                {
                    clear();
                    return;
                }
                T* old = m_data;
                m_data = xnew T[ size ];
                memcpy( m_data, old, std::min( m_size, size ) * sizeof( T ) );
                DELETE_ARRAY( old );
                m_size = size;
                m_position = std::min( m_size, m_position );
            }
            else
            {
                unsigned int pos = m_position;
                clear();
                if( size == 0 )
                    return;
                m_data = xnew T[ size ];
                m_size = size;
                m_position = std::min( m_size, pos );
            }
		}

		template< typename T, BufferDataCopyMethod BDCM >
		unsigned int Buffer< T, BDCM >::position()
		{
			return m_position;
		}

		template< typename T, BufferDataCopyMethod BDCM >
		void Buffer< T, BDCM >::reposition( unsigned int pos )
		{
		    if( !m_data || m_size == 0 )
                return;
            m_position = pos < m_size - 1 ? pos : m_size - 1;
		}

		template< typename T, BufferDataCopyMethod BDCM >
		unsigned int Buffer< T, BDCM >::size()
		{
			return m_size;
		}

		template< typename T, BufferDataCopyMethod BDCM >
		void Buffer< T, BDCM >::set( unsigned int id, const T& v )
		{
		    if( m_autoResize && m_autoResizeCount > 0 && m_size < id )
                resize( ( ( id / m_autoResizeCount ) + 1 ) * m_autoResizeCount , true );
            if( !m_data || id >= m_size )
				return;
            m_data[ id ] = v;
		}

		template< typename T, BufferDataCopyMethod BDCM >
		T& Buffer< T, BDCM >::get( unsigned int id )
		{
			if( !m_data || id >= m_size )
				return T();
            return m_data[ id ];
		}

		template< typename T, BufferDataCopyMethod BDCM >
		unsigned int Buffer< T, BDCM >::write( const T& v )
		{
		    return write( &v, 1 );
		}

		template< typename T, BufferDataCopyMethod BDCM >
		unsigned int Buffer< T, BDCM >::write( const T* data, unsigned int count )
		{
		    unsigned int need = m_position + count;
            if( m_autoResize && m_autoResizeCount > 0 && m_size < need )
                resize( ( ( need / m_autoResizeCount ) + 1 ) * m_autoResizeCount , true );
            if( !m_data )
                return 0;
            need = std::min( m_size, need );
            unsigned int k = 0;
            for( unsigned int i = m_position; i < need; i++, k++ )
                m_data[ i ] = data[ k ];
            m_position = need;
            return k;
		}

		template< typename T, BufferDataCopyMethod BDCM >
		unsigned int Buffer< T, BDCM >::write( const T* src, unsigned int pos, unsigned int count )
		{
		    if( src && ( pos + count <= m_size ) )
            {
                unsigned int k = 0;
                for( unsigned int i = pos; i < pos + count; i++, k++ )
                    m_data[ i ] = src[ k ];
                return k;
            }
            return 0;
		}

		template< typename T, BufferDataCopyMethod BDCM >
		unsigned int Buffer< T, BDCM >::read( T& out )
		{
            return read( &out, 1 );
		}

		template< typename T, BufferDataCopyMethod BDCM >
		unsigned int Buffer< T, BDCM >::read( T* out, unsigned int count )
		{
            unsigned int need = m_position + count;
            if( !m_data )
                return 0;
            need = std::min( m_size, need );
            unsigned int k = 0;
            for( unsigned int i = m_position; i < need; i++, k++ )
                out[ k ] = m_data[ i ];
            m_position = need;
            return k;
		}

		template< typename T, BufferDataCopyMethod BDCM >
		unsigned int Buffer< T, BDCM >::read( T* dst, unsigned int pos, unsigned int count )
		{
		    if( dst && ( pos + count <= m_size ) )
            {
                unsigned int k = 0;
                for( unsigned int i = pos; i < pos + count; i++, k++ )
                    dst[ k ] = m_data[ i ];
                return k;
            }
            return 0;
		}

        template< typename T, BufferDataCopyMethod BDCM >
        T* Buffer< T, BDCM >::data( bool current )
        {
            return m_data && m_size > 0 && m_position < m_size ? ( current ? &m_data[ m_position ] : m_data ) : 0;
        }

        template< typename T, BufferDataCopyMethod BDCM >
        void Buffer< T, BDCM >::setAutoResize( bool mode )
        {
            m_autoResize = mode;
        }

        template< typename T, BufferDataCopyMethod BDCM >
        bool Buffer< T, BDCM >::isAutoResize()
        {
            return m_autoResize;
        }

        template< typename T, BufferDataCopyMethod BDCM >
        void Buffer< T, BDCM >::setAutoResizeCount( unsigned int count )
        {
            m_autoResizeCount = count;
        }

        template< typename T, BufferDataCopyMethod BDCM >
        unsigned int Buffer< T, BDCM >::getAutoResizeCount()
        {
            return m_autoResizeCount;
        }

		template< typename T, BufferDataCopyMethod BDCM >
		void Buffer< T, BDCM >::operator() ( unsigned int id, const T& v )
		{
			set( id, v );
		}

		template< typename T, BufferDataCopyMethod BDCM >
		const T& Buffer< T, BDCM >::operator[] ( unsigned int id )
		{
			return( get( id ) );
		}
	}
}

#endif
