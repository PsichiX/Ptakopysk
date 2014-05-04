#ifndef __PTAKOPYSK__B2_FILTER_SERIALIZER__
#define __PTAKOPYSK__B2_FILTER_SERIALIZER__

#include "Serialized.h"
#include <Box2D/Box2D.h>

namespace Ptakopysk
{

    class b2FilterSerializer
    : public Serialized::ICustomSerializer
    {
    public:
        b2FilterSerializer() {}
        virtual ~b2FilterSerializer() {};

        virtual Json::Value serialize( const void* srcValue );
        virtual void deserialize( const void* dstValue, const Json::Value& root );
    };

}

#endif
