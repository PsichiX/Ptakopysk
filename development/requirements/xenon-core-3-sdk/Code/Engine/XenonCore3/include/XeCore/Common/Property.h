#ifndef __XE_CORE__COMMON__PROPERTY__
#define __XE_CORE__COMMON__PROPERTY__

#include "Base.h"

#define XECORE_COMMON_PROPERTY_DECLARE( PT, OT, name )          XeCore::Common::Property< PT, OT > name
#define XECORE_COMMON_PROPERTY_DEFINE( OT, name )               name( this, &OT::get#name, &OT::set#name )
#define XECORE_COMMON_PROPERTY_GETTER_SETTER( PT, name, field ) PT get#name() { return field; }; void set#name( PT v ) { field = v; };

namespace XeCore
{
	namespace Common
	{
        template< typename PT, typename OT >
        class Property
        {
        private:
            typedef PT          ( OT::* _Getter )();
            typedef void        ( OT::* _Setter )( PT );

        public:
            Property( OT* obj, _Getter getter, _Setter setter);

            FORCEINLINE PT get();
            FORCEINLINE void set( PT value );
            FORCEINLINE OT* object();

            FORCEINLINE operator PT ();
            FORCEINLINE void operator= ( PT value );

        private:
            OT* m_obj;
            _Getter m_getter;
            _Setter m_setter;
        };
	}
}

#include "Property.inl"

#endif
