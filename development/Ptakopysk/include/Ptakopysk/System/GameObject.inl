#ifndef __PTAKOPYSK__GAME_OBJECT__INLINE__
#define __PTAKOPYSK__GAME_OBJECT__INLINE__

#include "../Components/Component.h"

namespace Ptakopysk
{
    template< typename T >
    T* GameObject::getOrCreateComponent()
    {
        T* c = getComponent< T >();
        if( !c )
        {
            Component* comp = T::onBuildComponent();
            if( comp && comp->getType() == RTTI_CLASS_TYPE( T ) )
            {
                c = (T*)comp;
                addComponent( c );
            }
            else
                DELETE_OBJECT( comp );
        }
        return c;
    }
}

#endif
