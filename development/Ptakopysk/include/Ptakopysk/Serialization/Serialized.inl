#ifndef __PTAKOPYSK__SERIALIZED__INLINE__
#define __PTAKOPYSK__SERIALIZED__INLINE__

namespace Ptakopysk
{

    void Serialized::serializableProperty( const std::string& name )
    {
        for( std::vector< std::string >::iterator it = m_properties.begin(); it != m_properties.end(); it++ )
            if( *it == name )
                return;
        m_properties.push_back( name );
    }

    void Serialized::notSerializableProperty( const std::string& name )
    {
        for( std::vector< std::string >::iterator it = m_properties.begin(); it != m_properties.end(); it++ )
        {
            if( *it == name )
            {
                m_properties.erase( it );
                return;
            }
        }
    }

}

#endif
