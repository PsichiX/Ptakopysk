#ifndef __XE_CORE__COMMON__PROPERTY__
#define __XE_CORE__COMMON__PROPERTY__

#include "Base.h"

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
