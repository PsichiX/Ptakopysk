#include "../../include/Ptakopysk/Serialization/b2FilterSerializer.h"

namespace Ptakopysk
{

    Json::Value b2FilterSerializer::serialize( const void* srcValue )
    {
        if( !srcValue )
            return Json::Value::null;
        b2Filter* v = (b2Filter*)srcValue;
        Json::Value category = Json::Value( v->categoryBits );
        Json::Value mask = Json::Value( v->maskBits );
        Json::Value group = Json::Value( v->groupIndex );
        Json::Value r;
        r[ "category" ] = category;
        r[ "mask" ] = mask;
        r[ "group" ] = group;
        return r;
    }

    void b2FilterSerializer::deserialize( const void* dstValue, const Json::Value& root )
    {
        if( !dstValue || !root.isObject() )
            return;
        b2Filter filter;
        Json::Value category = root[ "category" ];
        Json::Value mask = root[ "mask" ];
        Json::Value group = root[ "group" ];
        if( category.isUInt() )
            filter.categoryBits = (uint16)category.asUInt();
        if( mask.isUInt() )
            filter.maskBits = (uint16)mask.asUInt();
        if( group.isInt() )
            filter.groupIndex = (int16)group.asInt();
        *(b2Filter*)dstValue = filter;
    }

}
