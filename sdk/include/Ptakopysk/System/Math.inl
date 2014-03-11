#ifndef __PTAKOPYSK__MATH__INLINE__
#define __PTAKOPYSK__MATH__INLINE__

#define EASING_FUNC_IMPL( name, valtype, basiceasing ) \
    EASING_FUNC( name, valtype ) \
    { \
        return Basic::ease< basiceasing >( t, b, c, d ); \
    }
#define EASING_FUNC_IMPL_CONST_REF( name, valtype, basiceasing ) \
    EASING_FUNC_CONST_REF( name, valtype ) \
    { \
        return Basic::ease< basiceasing >( t, b, c, d ); \
    }
#define EASING_FUNCS_SECTION_IMPL( name, basiceasing ) \
    namespace name \
    { \
        EASING_FUNC_IMPL( in, float, basiceasing##In ); \
        EASING_FUNC_IMPL( out, float, basiceasing##Out ); \
        EASING_FUNC_IMPL( inOut, float, basiceasing##InOut ); \
        EASING_FUNC_IMPL_CONST_REF( in, sf::Vector2f, basiceasing##In ); \
        EASING_FUNC_IMPL_CONST_REF( out, sf::Vector2f, basiceasing##Out ); \
        EASING_FUNC_IMPL_CONST_REF( inOut, sf::Vector2f, basiceasing##InOut ); \
        EASING_FUNC_IMPL_CONST_REF( in, sf::Vector3f, basiceasing##In ); \
        EASING_FUNC_IMPL_CONST_REF( out, sf::Vector3f, basiceasing##Out ); \
        EASING_FUNC_IMPL_CONST_REF( inOut, sf::Vector3f, basiceasing##InOut ); \
    }

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

        namespace Easing
        {

            namespace Basic
            {

                template< BasicEasingFunc F >
                float ease( float t, float b, float c, float d )
                {
                    t = F( d == 0.0f ? 1.0f : t / d );
                    return LERP( t, b, c );
                }

                template< BasicEasingFunc F >
                sf::Vector2f ease( float t, sf::Vector2f b, sf::Vector2f c, float d )
                {
                    t = F( d == 0.0f ? 1.0f : t / d );
                    return sf::Vector2f(
                        LERP( t, b.x, c.x ),
                        LERP( t, b.y, c.y )
                    );
                }

                template< BasicEasingFunc F >
                sf::Vector3f ease( float t, sf::Vector3f b, sf::Vector3f c, float d )
                {
                    t = F( d == 0.0f ? 1.0f : t / d );
                    return sf::Vector3f(
                        LERP( t, b.x, c.x ),
                        LERP( t, b.y, c.y ),
                        LERP( t, b.z, c.z )
                    );
                }

                float linearIn( float p )
                {
                    return p;
                }

                float linearOut( float p )
                {
                    return p;
                }

                float linearInOut( float p )
                {
                    return p;
                }

                float quadraticIn( float p )
                {
                    return p * p;
                }

                float quadraticOut( float p )
                {
                    return -( p * ( p - 2.0f ) );
                }

                float quadraticInOut( float p )
                {
                    if( p < 0.5f )
                        return 2.0f * p * p;
                    else
                        return ( -2.0f * p * p ) + ( 4.0f * p ) - 1.0f;
                }

                float cubicIn( float p )
                {
                    return p * p * p;
                }

                float cubicOut( float p )
                {
                    float f = p - 1.0f;
                    return ( f * f * f ) + 1.0f;
                }

                float cubicInOut( float p )
                {
                    if( p < 0.5f )
                        return 4.0f * p * p * p;
                    else
                    {
                        float f = ( 2.0f * p ) - 2.0f;
                        return 0.5f * f * f * f * 1.0f;
                    }
                }

                float quarticIn( float p )
                {
                    return p * p * p * p;
                }

                float quarticOut( float p )
                {
                    float f = p - 1.0f;
                    return ( f * f * f * ( 1.0f - p ) ) + 1.0f;
                }

                float quarticInOut( float p )
                {
                    if( p < 0.5f )
                        return 8.0f * p * p * p * p;
                    else
                    {
                        float f = p - 1.0f;
                        return ( -8.0f * f * f * f * f ) + 1.0f;
                    }
                }

                float quinticIn( float p )
                {
                    return p * p * p * p * p;
                }

                float quinticOut( float p )
                {
                    float f = p - 1.0f;
                    return ( f * f * f * f * f ) + 1.0f;
                }

                float quinticInOut( float p )
                {
                    if( p < 0.5f )
                        return 16.0f * p * p * p * p * p;
                    else
                    {
                        float f = ( 2.0f * p ) - 2.0f;
                        return ( 0.5f * f * f * f * f * f ) + 1.0f;
                    }
                }

                float sineIn( float p )
                {
                    return sinf( ( p - 1.0f ) * (float)M_PI_2 ) + 1.0f;
                }

                float sineOut( float p )
                {
                    return sinf( p * (float)M_PI_2 );
                }

                float sineInOut( float p )
                {
                    return 0.5f * sinf( 1.0f - cosf(  p * (float)M_PI ) );
                }

                float circuralIn( float p )
                {
                    return 1.0f - sqrtf( 1.0f - ( p * p ) );
                }

                float circuralOut( float p )
                {
                    return sqrtf( ( 2.0f - p ) * p );
                }

                float circuralInOut( float p )
                {
                    if( p < 0.5f )
                        return 0.5f * ( 1.0f - sqrtf( 1.0f - ( 4.0f * p * p ) ) );
                    else
                        return 0.5f * ( sqrtf( -( ( 2.0f * p ) - 3.0f ) * ( ( 2.0f * p ) - 1.0f ) ) + 1.0f );
                }

                float exponentialIn( float p )
                {
                    return p == 0.0f ? p : std::pow( 2.0f, 10.0f * ( p - 1.0f ) );
                }

                float exponentialOut( float p )
                {
                    return p == 1.0f ? p : 1.0f - std::pow( 2.0f, -10.0f * p );
                }

                float exponentialInOut( float p )
                {
                    if( p == 0.0f || p == 1.0f )
                        return p;
                    if( p < 0.5f )
                        return 0.5f * std::pow( 2.0f, ( 20.0f * p ) - 10.0f );
                    else
                        return ( -0.5f * std::pow( 2.0f, ( -20.0f * p ) + 10.0f ) ) + 1.0f;
                }

                float elasticIn( float p )
                {
                    return sinf( 13.0f * (float)M_PI_2 * p ) * std::pow( 2.0f, 10.0f * ( p - 1.0f ) );
                }

                float elasticOut( float p )
                {
                    return ( sinf( -13.0f * (float)M_PI_2 * ( p + 1.0f ) ) * std::pow( 2.0f, -10.0f * p ) ) + 1.0f;
                }

                float elasticInOut( float p )
                {
                    if( p < 0.5f )
                        return 0.5f * sinf( 13.0f * (float)M_PI_2 * 2.0f * p ) * std::pow( 2.0f, 10.0f * ( ( 2.0f * p ) - 1.0f ) );
                    else
                        return 0.5f * ( ( sinf( -13.0f * (float)M_PI_2 * ( ( ( 2.0f * p ) - 1.0f ) + 1.0f ) ) * std::pow( 2.0f, -10.0f * ( ( 2.0f * p ) - 1.0f ) ) ) + 1.0f );
                }

                float backIn( float p )
                {
                    return ( p * p * p ) - ( p * sinf( p * (float)M_PI ) );
                }

                float backOut( float p )
                {
                    float f = 1.0f - p;
                    return 1.0f - ( ( f * f * f ) - ( f * sinf( f * (float)M_PI ) ) );
                }

                float backInOut( float p )
                {
                    if( p < 0.5f )
                    {
                        float f = 2.0f * p;
                        return 0.5f * ( ( f * f * f ) - ( f * sinf( f * (float)M_PI ) ) );
                    }
                    else
                    {
                        float f = 1.0f - ( ( 2.0f * p ) - 1.0f );
                        return ( 0.5f * ( 1.0f - ( ( f * f * f ) - ( f * sinf( f * (float)M_PI ) ) ) ) ) + 0.5f;
                    }
                }

                float bounceIn( float p )
                {
                    return 1.0f - bounceOut( 1.0f - p );
                }

                float bounceOut( float p )
                {
                    if( p < 4.0f / 11.0f )
                        return ( 121.0f * p * p ) / 16.0f;
                    else if( p < 8.0f / 11.0f )
                        return ( ( 363.0f / 40.0f ) * p * p) - ( ( 99.0f / 10.0f ) * p ) + ( 17.0f / 5.0f );
                    else if( p < 9.0f / 10.0f )
                        return ( ( 4356.0f / 361.0f ) * p * p ) - ( ( 35442.0f / 1805.0f ) * p ) + ( 16061.0f / 1805.0f );
                    else
                        return ( ( 54.0f / 5.0f ) * p * p ) - ( ( 513.0f / 25.0f ) * p ) + ( 268.0f / 25.0f );
                }

                float bounceInOut( float p )
                {
                    if( p < 0.5f )
                        return 0.5f * bounceIn( p * 2.0f );
                    else
                        return ( 0.5f * bounceOut( ( p * 2.0f ) - 1.0f ) ) + 0.5f;
                }

            }

            EASING_FUNCS_SECTION_IMPL( Linear, Basic::linear );
            EASING_FUNCS_SECTION_IMPL( Quadratic, Basic::quadratic );
            EASING_FUNCS_SECTION_IMPL( Cubic, Basic::cubic );
            EASING_FUNCS_SECTION_IMPL( Quartic, Basic::quartic );
            EASING_FUNCS_SECTION_IMPL( Quintic, Basic::quintic );
            EASING_FUNCS_SECTION_IMPL( Sine, Basic::sine );
            EASING_FUNCS_SECTION_IMPL( Circural, Basic::circural );
            EASING_FUNCS_SECTION_IMPL( Exponential, Basic::exponential );
            EASING_FUNCS_SECTION_IMPL( Elastic, Basic::elastic );
            EASING_FUNCS_SECTION_IMPL( Back, Basic::back );
            EASING_FUNCS_SECTION_IMPL( Bounce, Basic::bounce );

        }

    }
}

#endif
