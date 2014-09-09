#include <iostream>
#include "TestThread.h"
#include <XeCore/Common/Concurrent/Synchronized.h>

using namespace std;
using namespace XeCore::Common::Concurrent;

RTTI_CLASS_DERIVATIONS( TestThread,
                        RTTI_DERIVATION( Thread )
                        );

TestThread::TestThread()
: RTTI_CLASS_DEFINE( TestThread )
{
    cout << "create TestThread" << endl;
}

void TestThread::start()
{
    Thread::start();
    cout << "start TestThread: " << getId() << endl;
}

void TestThread::run()
{
    SYNCHRONIZED_HERE;
    cout << "run TestThread: " << getId() << endl;
    for( int i = 0; i < 25; i++ )
        cout << "job #" << i << " of TestThread: " << getId() << endl;
}
