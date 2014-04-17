#ifndef __PTAKOPYSK__TRANSFORM_MODE_SERIALIZER__
#define __PTAKOPYSK__TRANSFORM_MODE_SERIALIZER__

#include "EnumSerializer.h"
#include "../Components/Transform.h"

namespace Ptakopysk
{

    class TransformModeSerializer
    : public EnumSerializer
    {
    public:
        TransformModeSerializer()
        {
            EnumSerializer::EnumKeyValues kv;
            kv[ "mHierarchy" ] = Transform::mHierarchy;
            kv[ "mParent" ] = Transform::mParent;
            kv[ "mGlobal" ] = Transform::mGlobal;
            setup( kv );
        }
    };

}

#endif

