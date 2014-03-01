#ifndef __PTAKOPYSK__B2_BODY_TYPE_SERIALIZER__
#define __PTAKOPYSK__B2_BODY_TYPE_SERIALIZER__

#include "EnumSerializer.h"
#include <Box2D/Box2D.h>

namespace Ptakopysk
{

    class b2BodyTypeSerializer
    : public EnumSerializer
    {
    public:
        b2BodyTypeSerializer()
        {
            EnumSerializer::EnumKeyValues kv;
            kv[ "b2_staticBody" ] = b2_staticBody;
            kv[ "b2_kinematicBody" ] = b2_kinematicBody;
            kv[ "b2_dynamicBody" ] = b2_dynamicBody;
            setup( kv );
        }
    };

}

#endif
