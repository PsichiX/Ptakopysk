#ifndef __TEST_THREAD__
#define __TEST_THREAD__

#include <XeCore/Common/Concurrent/Thread.h>

using namespace XeCore::Common::Concurrent;
using namespace XeCore::Common;

class TestThread
: public virtual IRtti
, public virtual MemoryManager::Manageable
, public Thread
{
    RTTI_CLASS_DECLARE( TestThread );

public:
    TestThread();

    void            start();
    virtual void    run();
};

#endif
