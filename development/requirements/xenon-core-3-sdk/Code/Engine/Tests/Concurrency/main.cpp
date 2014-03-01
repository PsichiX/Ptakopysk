#include <iostream>
#include "TestThread.h"

using namespace std;

int main()
{
    cout << ">>> hardware concurrency: " << XeCore::Common::Concurrent::Thread::hardwareConcurrency() << endl;

    cout << ">>> create threads" << endl;
    TestThread* t0 = xnew TestThread();
    TestThread* t1 = xnew TestThread();
    TestThread* t2 = xnew TestThread();
    TestThread* t3 = xnew TestThread();

    cout << ">>> start threads" << endl;
    t0->start();
    t1->start();
    t2->start();
    t3->start();

    cout << ">>> join threads" << endl;
    t0->join();
    t1->join();
    t2->join();
    t3->join();

    cout << ">>> exit" << endl;
    return 0;
}
