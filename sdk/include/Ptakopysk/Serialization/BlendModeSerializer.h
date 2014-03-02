#ifndef __PTAKOPYSK__BLEND_MODE_SERIALIZER__
#define __PTAKOPYSK__BLEND_MODE_SERIALIZER__

#include "EnumSerializer.h"
#include <SFML/Graphics/BlendMode.hpp>

namespace Ptakopysk
{

    class BlendModeSerializer
    : public EnumSerializer
    {
    public:
        BlendModeSerializer()
        {
            EnumSerializer::EnumKeyValues kv;
            kv[ "BlendAlpha" ] = sf::BlendAlpha;
            kv[ "BlendAdd" ] = sf::BlendAdd;
            kv[ "BlendMultiply" ] = sf::BlendMultiply;
            kv[ "BlendNone" ] = sf::BlendNone;
            setup( kv );
        }
    };

}

#endif
