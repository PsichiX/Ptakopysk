#include "../../include/Ptakopysk/Serialization/BitFieldSerializer.h"

namespace Ptakopysk
{

    BitFieldSerializer::BitFieldSerializer()
    {
    }

    BitFieldSerializer::BitFieldSerializer( const BitValues& bitvalues )
    : m_bitValues( bitvalues )
    {
    }

    BitFieldSerializer::~BitFieldSerializer()
    {
        m_bitValues.clear();
    }

    Json::Value BitFieldSerializer::serialize( const void* srcValue )
    {
        if( !srcValue )
            return Json::Value::null;
        int v = *(int*)srcValue;
        Json::Value a( Json::arrayValue );
        for( BitValues::iterator it = m_bitValues.begin(); it != m_bitValues.end(); it++ )
            if( it->second & v || it->second == v )
                a.append( Json::Value( it->first ) );
        return a;
    }

    void BitFieldSerializer::deserialize( const void* dstValue, const Json::Value& root )
    {
        if( !dstValue || !root.isArray() )
            return;
        int b = 0;
        Json::Value item;
        for( unsigned int i = 0; i < root.size(); i++ )
        {
            item = root[ i ];
            if( m_bitValues.count( item.asString() ) )
                b |= m_bitValues[ item.asString() ];
        }
        *(int*)dstValue = b;
    }

    void BitFieldSerializer::setup( const BitValues& bitvalues )
    {
        m_bitValues = bitvalues;
    }

}
