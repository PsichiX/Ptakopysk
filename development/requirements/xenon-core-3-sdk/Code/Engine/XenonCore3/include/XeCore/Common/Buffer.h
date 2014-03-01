#ifndef __XE_CORE__COMMON__BUFFER__
#define __XE_CORE__COMMON__BUFFER__

#include "Base.h"

namespace XeCore
{
	namespace Common
	{
	    enum BufferDataCopyMethod
	    {
	        BDCM_ELEMENTS,
	        BDCM_BYTES
	    };

		template< typename T, BufferDataCopyMethod BDCM = BDCM_BYTES >
		class Buffer
		{
		public:
                                        Buffer( unsigned int size = 0 );
                                        ~Buffer();

            FORCEINLINE void            clear();
            FORCEINLINE void            resize( unsigned int size, bool keepData = false );
            FORCEINLINE unsigned int    position();
            FORCEINLINE void            reposition( unsigned int p = 0 );
			FORCEINLINE unsigned int    size();
            FORCEINLINE void	        set( unsigned int id, const T& v );
			FORCEINLINE T&   	        get( unsigned int id );
			FORCEINLINE unsigned int    write( const T& v);
			FORCEINLINE unsigned int    write( const T* data, unsigned int count );
			FORCEINLINE unsigned int    write( const T* src, unsigned int pos, unsigned int count );
			FORCEINLINE unsigned int    read( T& out );
			FORCEINLINE unsigned int    read( T* out, unsigned int count );
			FORCEINLINE unsigned int    read( T* dst, unsigned int pos, unsigned int count );
            FORCEINLINE T*              data( bool current = false );
            FORCEINLINE void            setAutoResize( bool mode = true );
            FORCEINLINE bool            isAutoResize();
            FORCEINLINE void            setAutoResizeCount( unsigned int count = 1024 );
            FORCEINLINE unsigned int    getAutoResizeCount();

			FORCEINLINE void	        operator() ( unsigned int id, const T& v );
			FORCEINLINE const T&	    operator[] ( unsigned int id );

		private:
			T*		                    m_data;
			unsigned int                m_position;
			unsigned int                m_size;
			bool                        m_autoResize;
			unsigned int                m_autoResizeCount;
		};

		typedef Buffer< char >          CharBuffer;
		typedef Buffer< byte >          ByteBuffer;
		typedef Buffer< short >         ShortBuffer;
		typedef Buffer< word >          WordBuffer;
		typedef Buffer< int >           IntBuffer;
		typedef Buffer< dword >         DwordBuffer;
		typedef Buffer< float >         FloatBuffer;
		typedef Buffer< double >        DoubleBuffer;

	}
}

#include "Buffer.inl"

#endif
