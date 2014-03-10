#ifndef __PTAKOPYSK__TWEEN__
#define __PTAKOPYSK__TWEEN__

#include <XeCore/Common/Property.h>
#include "Math.h"

namespace Ptakopysk
{

    class ITween
    {
    public:
        ITween() {};
        virtual ~ITween() {};

    protected:
        virtual void onStart() = 0;
        virtual void onStop() = 0;
        virtual void onUpdate( float dt ) = 0;
    };

    template< typename PT, typename OT, Math::Easing::Basic::EasingFunc::Type FT >
    class Tween
    : public ITween
    {
    public:
        FORCEINLINE Tween( XeCore::Common::Property< PT, OT >& property, PT to, float duration );
        FORCEINLINE virtual ~Tween();

        FORCEINLINE int getState() { return m_state; };

    protected:
        FORCEINLINE virtual void onStart();
        FORCEINLINE virtual void onStop();
        FORCEINLINE virtual void onUpdate( float dt );

    private:
        XeCore::Common::Property< PT, OT >& m_property;
        PT m_from;
        PT m_to;
        float m_duration;
        float m_time;
        int m_state; // -1: not started; 0: in progress; 1: complete.
    };

}

#include "Tween.inl"

#endif
