#include "../../include/Ptakopysk/Serialization/Serialized.h"

namespace Ptakopysk
{

    std::map< std::string, Serialized::ICustomSerializer* > Serialized::s_customSerializers = std::map< std::string, Serialized::ICustomSerializer* >();

    RTTI_CLASS_DERIVATIONS( Serialized,
                            RTTI_DERIVATIONS_END
                            );

    Serialized::Serialized()
    : RTTI_CLASS_DEFINE( Serialized )
    {
    }

    Serialized::~Serialized()
    {
        m_properties.clear();
    }

    void Serialized::registerCustomSerializer( const std::string& id, ICustomSerializer* serializer )
    {
        if( !serializer || s_customSerializers.count( id ) )
            return;
        s_customSerializers[ id ] = serializer;
    }

    void Serialized::unregisterCustomSerializer( const std::string& id, bool del )
    {
        if( !s_customSerializers.count( id ) )
            return;
        if( del )
        {
            ICustomSerializer* s = s_customSerializers[ id ];
            DELETE_OBJECT( s );
        }
        s_customSerializers.erase( id );
    }

    void Serialized::unregisterCustomSerializer( ICustomSerializer* serializer, bool del )
    {
        for( std::map< std::string, ICustomSerializer* >::iterator it = s_customSerializers.begin(); it != s_customSerializers.end(); it++ )
        {
            if( it->second == serializer )
            {
                if( del )
                {
                    ICustomSerializer* s = it->second;
                    DELETE_OBJECT( s );
                }
                s_customSerializers.erase( it );
                return;
            }
        }
    }

    void Serialized::unregisterAllCustomSerializers( bool del )
    {
        if( del )
        {
            for( std::map< std::string, ICustomSerializer* >::iterator it = s_customSerializers.begin(); it != s_customSerializers.end(); it++ )
            {
                ICustomSerializer* s = it->second;
                DELETE_OBJECT( s );
            }
        }
        s_customSerializers.clear();
    }

    Serialized::ICustomSerializer* Serialized::getCustomSerializer( const std::string& id )
    {
        return s_customSerializers.count( id ) ? s_customSerializers[ id ] : 0;
    }

    void Serialized::serialize( Json::Value& dstRoot )
    {
        Json::Value item;
        for( std::vector< std::string >::iterator it = m_properties.begin(); it != m_properties.end(); it++ )
        {
            item = onSerialize( *it );
            if( !item.isNull() )
                dstRoot[ *it ] = item;
        }
    }

    void Serialized::deserialize( const Json::Value& srcRoot )
    {
        if( !srcRoot.isObject() )
            return;
        Json::Value item;
        for( std::vector< std::string >::iterator it = m_properties.begin(); it != m_properties.end(); it++ )
        {
            item = srcRoot[ *it ];
            if( !item.isNull() )
                onDeserialize( *it, item );
        }
    }

}
