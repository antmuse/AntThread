AntThread V1.0.0.3
====
A cross platform thread lib, current for Windows&amp;Linux&amp;Andriod.
# Usage
----

```cpp
#include "IRunnable.h"
#include "CThread.h"
#include "CThreadPool.h"
#include "CProcessManager.h"
#include "CAtomicValue32.h"

namespace irr {

void AppQuit() {
    c8 key = '\0';
    while('*' != key) {
        printf("@Please input [*] to quit\n");
        scanf("%c", &key);
    }
}


//for thread test
class CWorker : public IRunnable {
public:
    virtual void run() {
        CThread* td = CThread::getCurrentThread();
        printf("CWorker::run>>thread id = %u\n", td->getID());
        //CThread::sleep(10);
    }
};

//for thread test
void AppWorker(void* param) {
    CAtomicS32& count = *(CAtomicS32*) (param);
    CThread* td = CThread::getCurrentThread();
    printf("AppWorker::>>[thread=%u], [count=%d]\n", td->getID(), ++count);
    CThread::sleep(10);
}


//for thread test
void AppStartThread() {
    CThread td;
    CWorker wk;
    td.start(wk);
    td.join();
}


void AppStartThreadPool() {
    CAtomicS32 count;
    const u32 max = 111;
    CThreadPool pool(9);
    pool.start();
    CWorker wk;
    for(u32 i = 0; i < 10; ++i) {
        pool.start(&wk);
    }
    for(u32 i = 0; i < max; ++i) {
        pool.start(AppWorker, (void*) (&count));
    }
    pool.join();
    printf("AppStartThreadPool::>>[count=%d]\n", count.getValue());
}

//test process
void AppStartProcesses() {
    CProcessManager::DProcessParam params;
#if defined(APP_PLATFORM_WINDOWS)
    //params.push_back(io::path("f:\\test.txt"));
    CProcessHandle* proc = CProcessManager::launch("notepad.exe", params);
#else
    CProcessHandle* proc = CProcessManager::launch("/usr/bin/gnome-calculator", params);
#endif
    if(proc) {
        printf("AppStartProcesses success\n");
        proc->wait();
    } else {
        printf("AppStartProcesses failed\n");
    }
}

}//namespace irr


int main(int argc, char** argv) {
    printf("@1 Start Thread.\n");
    printf("@2 Start Thread Pool.\n");
    printf("@3 Start Process.\n");
    int key = 0;
    scanf("%d", &key);
    switch(key) {
    case 1:
        irr::AppStartThread();
        break;
    case 2:
        irr::AppStartThreadPool();
        break;
    case 3:
        irr::AppStartProcesses();
        break;
    default:
        printf("@unknown command, pls restart.\n");
        break;
    }
    printf("@Test finish.\n");
    irr::AppQuit();
    return 0;
}//main

```
