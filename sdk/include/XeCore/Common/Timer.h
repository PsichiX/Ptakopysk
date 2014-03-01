#ifndef __XE_CORE__COMMON__TIMER__
#define __XE_CORE__COMMON__TIMER__

#include <ctime>
#include "IRtti.h"
#include "MemoryManager.h"

namespace XeCore
{
	namespace Common
	{
		class Timer
            : public virtual IRtti
			, public virtual MemoryManager::Manageable
        {
            RTTI_CLASS_DECLARE( Timer );

		public:
						Timer();
						~Timer();

            void        start();
            void        update();
            void        stop();
            time_t      ticks();
            float       millis();
            time_t      deltaTicks();
            float       deltaMillis();

        private:
            time_t      m_ticksStart;
            float       m_millisStart;
            time_t      m_ticksLast;
            float       m_millisLast;
            time_t      m_ticksCurr;
            float       m_millisCurr;
            bool        m_running;
        };
	}
}

#endif
