#include "../../include/Ptakopysk/Serialization/EnumSerializer.h"

namespace Ptakopysk
{

    EnumSerializer::EnumSerializer()
    {
    }

    EnumSerializer::EnumSerializer( const EnumKeyValues& keyvalues )
    : m_keyValues( keyvalues )
    {
    }

    EnumSerializer::~EnumSerializer()
    {
        m_keyValues.clear();
    }

    Json::Value EnumSerializer::serialize( const void* srcValue )
    {
        if( !srcValue )
            return Json::Value::null;
        int v = *(int*)srcValue;
        for( EnumKeyValues::iterator it = m_keyValues.begin(); it != m_keyValues.end(); it++ )
            if( it->second == v )
                return Json::Value( it->first );
        return Json::Value::null;
    }

    void EnumSerializer::deserialize( const void* dstValue, const Json::Value& root )
    {
        if( !dstValue )
            return;
        if( root.isString() && m_keyValues.count( root.asString() ) )
            *(int*)dstValue = m_keyValues[ root.asString() ];
        else
            *(int*)dstValue = 0;
    }

    void EnumSerializer::setup( const EnumKeyValues& keyvalues )
    {
        m_keyValues = keyvalues;
    }

}
