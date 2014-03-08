#ifndef __PTAKOPYSK__MATH__
#define __PTAKOPYSK__MATH__

#include <XeCore/Common/Base.h>
#include <SFML/System/Vector2.hpp>
#include <SFML/System/Vector3.hpp>

namespace Ptakopysk
{
    namespace Math
    {

        FORCEINLINE float length( const sf::Vector2f& v );
        FORCEINLINE float length( const sf::Vector3f& v );
        FORCEINLINE sf::Vector2f normalize( const sf::Vector2f& v );
        FORCEINLINE sf::Vector3f normalize( const sf::Vector3f& v );
        FORCEINLINE float dot( const sf::Vector2f& a, const sf::Vector2f& b );
        FORCEINLINE float dot( const sf::Vector3f& a, const sf::Vector3f& b );
        FORCEINLINE sf::Vector3f cross( const sf::Vector3f& a, const sf::Vector3f& b );

    }
}

#include "Math.inl"

#endif
