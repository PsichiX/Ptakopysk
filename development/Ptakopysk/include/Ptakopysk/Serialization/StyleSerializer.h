#ifndef __PTAKOPYSK__STYLE_SERIALIZER__
#define __PTAKOPYSK__STYLE_SERIALIZER__

#include "BitFieldSerializer.h"
#include <SFML/Graphics/Text.hpp>

namespace Ptakopysk
{

    class StyleSerializer
    : public BitFieldSerializer
    {
    public:
        StyleSerializer()
        {
            BitFieldSerializer::BitValues kv;
            kv[ "Regular" ] = sf::Text::Regular;
            kv[ "Bold" ] = sf::Text::Bold;
            kv[ "Italic" ] = sf::Text::Italic;
            kv[ "Underlined" ] = sf::Text::Underlined;
            setup( kv );
        }
    };

}

#endif
