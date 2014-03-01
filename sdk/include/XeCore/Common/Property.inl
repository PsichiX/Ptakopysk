#ifndef __XE_CORE__COMMON__PROPERTY__INLINE__
#define __XE_CORE__COMMON__PROPERTY__INLINE__

namespace XeCore
{
    namespace Common
    {
        template< typename PT, typename OT >
        Property< PT, OT >::Property( OT* obj, Property::_Getter getter, Property::_Setter setter)
            : m_obj( obj )
            , m_getter( getter )
            , m_setter( setter )
        {
        }

        template< typename PT, typename OT >
        Property< PT, OT >::operator PT ()
        {
            return m_obj && m_getter ? ( *m_obj.*m_getter )() : PT();
        };

        template< typename PT, typename OT >
        void Property< PT, OT >::operator= ( PT value )
        {
            if( m_obj && m_setter )
                ( *m_obj.*m_setter )( value );
        };
    }
}

#endif
