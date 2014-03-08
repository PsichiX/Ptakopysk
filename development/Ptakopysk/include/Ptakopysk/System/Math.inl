#ifndef __PTAKOPYSK__MATH__INLINE__
#define __PTAKOPYSK__MATH__INLINE__

namespace Ptakopysk
{
    namespace Math
    {
        float length( const sf::Vector2f& v )
        {
            return std::sqrt( ( v.x * v.x ) + ( v.y * v.y ) );
        }

        float length( const sf::Vector3f& v )
        {
            return std::sqrt( ( v.x * v.x ) + ( v.y * v.y ) + ( v.z * v.z ) );
        }

        sf::Vector2f normalize( const sf::Vector2f& v )
        {
            float len = length( v );
            return len == 0.0f ? sf::Vector2f() : ( v / len );
        }

        sf::Vector3f normalize( const sf::Vector3f& v )
        {
            float len = length( v );
            return 0.0f ? sf::Vector3f() : ( v / len );
        }

        float dot( const sf::Vector2f& a, const sf::Vector2f& b )
        {
            return ( a.x * b.x ) + ( a.y * b.y );
        }

        float dot( const sf::Vector3f& a, const sf::Vector3f& b )
        {
            return ( a.x * b.x ) + ( a.y * b.y ) + ( a.z * b.z );
        }

        sf::Vector3f cross( const sf::Vector3f& a, const sf::Vector3f& b )
        {
            return sf::Vector3f(
                ( a.y * b.z ) - ( a.z * b.y ),
                ( a.z * b.x ) - ( a.x * b.z ),
                ( a.x * b.y ) - ( a.y * b.x )
            );
        }

    }
}

#endif
