#ifndef __PTAKOPYSK__SERIALIZED__INLINE__
#define __PTAKOPYSK__SERIALIZED__INLINE__

namespace Ptakopysk
{
    template< typename T >
    Json::Value Serialized::serializeCustom( const std::string& id, const T* srcValue )
    {
        Serialized::ICustomSerializer* s = Serialized::getCustomSerializer( id );
        return s ? s->serialize( srcValue ) : Json::Value::null;
    }

    template< typename T >
    Json::Value Serialized::serializeCustom( const std::string& id, const T& srcValue )
    {
        Serialized::ICustomSerializer* s = Serialized::getCustomSerializer( id );
        return s ? s->serialize( &srcValue ) : Json::Value::null;
    }

    template< typename T >
    void Serialized::deserializeCustom( const std::string& id, const T* dstValue, const Json::Value& root )
    {
        Serialized::ICustomSerializer* s = Serialized::getCustomSerializer( id );
        if( s )
            s->deserialize( dstValue, root );
    }

    template< typename T >
    T Serialized::deserializeCustom( const std::string& id, const Json::Value& root )
    {
        Serialized::ICustomSerializer* s = Serialized::getCustomSerializer( id );
        T v;
        if( s )
            s->deserialize( &v, root );
        return v;
    }

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
