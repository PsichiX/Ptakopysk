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
            float       seconds();
            time_t      deltaTicks();
            float       deltaSeconds();

        private:
            time_t      m_ticksStart;
            float       m_secondsStart;
            time_t      m_ticksLast;
            float       m_secondsLast;
            time_t      m_ticksCurr;
            float       m_secondsCurr;
            bool        m_running;
        };
	}
}

#endif
