#ifndef __PTAKOPYSK__MATH__
#define __PTAKOPYSK__MATH__

#include <XeCore/Common/Base.h>
#include <SFML/System/Vector2.hpp>
#include <SFML/System/Vector3.hpp>

#define BASIC_EASING_FUNCS_GROUP_DECL( basename ) \
    FORCEINLINE float basename##In( float p ); \
    FORCEINLINE float basename##Out( float p ); \
    FORCEINLINE float basename##InOut( float p );
#define EASING_FUNC( name, valtype )    valtype name( float t, valtype b, valtype c, float d )
#define EASING_FUNCS_GROUP_DECL( valtype ) \
    FORCEINLINE EASING_FUNC( in, valtype ); \
    FORCEINLINE EASING_FUNC( out, valtype ); \
    FORCEINLINE EASING_FUNC( inOut, valtype );
#define EASING_FUNC_CONST_REF( name, valtype )  valtype name( float t, const valtype& b, const valtype& c, float d )
#define EASING_FUNCS_GROUP_DECL_CONST_REF( valtype ) \
    FORCEINLINE EASING_FUNC_CONST_REF( in, valtype ); \
    FORCEINLINE EASING_FUNC_CONST_REF( out, valtype ); \
    FORCEINLINE EASING_FUNC_CONST_REF( inOut, valtype );
#define EASING_FUNCS_SECTION_DECL( name ) \
    namespace name \
    { \
        EASING_FUNCS_GROUP_DECL( float ); \
        EASING_FUNCS_GROUP_DECL_CONST_REF( sf::Vector2f ); \
        EASING_FUNCS_GROUP_DECL_CONST_REF( sf::Vector3f ); \
    }

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

        /// Easing parameters:
        /// t - current time;
        /// b - start value;
        /// c - end value;
        /// d - time duration.
        namespace Easing
        {

            namespace Basic
            {

                typedef float ( *BasicEasingFunc )( float );

                template< BasicEasingFunc F >
                FORCEINLINE float ease( float t, float b, float c, float d );

                template< BasicEasingFunc F >
                FORCEINLINE sf::Vector2f ease( float t, sf::Vector2f b, sf::Vector2f c, float d );

                template< BasicEasingFunc F >
                FORCEINLINE sf::Vector3f ease( float t, sf::Vector3f b, sf::Vector3f c, float d );

                BASIC_EASING_FUNCS_GROUP_DECL( linear );
                BASIC_EASING_FUNCS_GROUP_DECL( quadratic );
                BASIC_EASING_FUNCS_GROUP_DECL( cubic );
                BASIC_EASING_FUNCS_GROUP_DECL( quartic );
                BASIC_EASING_FUNCS_GROUP_DECL( quintic );
                BASIC_EASING_FUNCS_GROUP_DECL( sine );
                BASIC_EASING_FUNCS_GROUP_DECL( circural );
                BASIC_EASING_FUNCS_GROUP_DECL( exponential );
                BASIC_EASING_FUNCS_GROUP_DECL( elastic );
                BASIC_EASING_FUNCS_GROUP_DECL( back );
                BASIC_EASING_FUNCS_GROUP_DECL( bounce );

            }

            EASING_FUNCS_SECTION_DECL( Linear );
            EASING_FUNCS_SECTION_DECL( Quadratic );
            EASING_FUNCS_SECTION_DECL( Cubic );
            EASING_FUNCS_SECTION_DECL( Quartic );
            EASING_FUNCS_SECTION_DECL( Quintic );
            EASING_FUNCS_SECTION_DECL( Sine );
            EASING_FUNCS_SECTION_DECL( Circural );
            EASING_FUNCS_SECTION_DECL( Exponential );
            EASING_FUNCS_SECTION_DECL( Elastic );
            EASING_FUNCS_SECTION_DECL( Back );
            EASING_FUNCS_SECTION_DECL( Bounce );

        }

    }

}

#include "Math.inl"

#endif
