#include "HConfig.h"
#include "IAppLogger.h"

#include "IRunnable.h"
#include "CThread.h"
#include "CThreadPool.h"
#include "CProcessManager.h"
#include "CAtomicValue32.h"


#if defined(APP_PLATFORM_WINDOWS)
#if defined(APP_OS_64BIT)
#if defined(APP_DEBUG)
#pragma comment (lib, "Thread-64D.lib")
#else
#pragma comment (lib, "Thread-64.lib")
#endif //APP_DEBUG
#else
#if defined(APP_DEBUG)
#pragma comment (lib, "Thread-32D.lib")
#else
#pragma comment (lib, "Thread-32.lib")
#endif //APP_DEBUG
#endif //APP_OS_64BIT
#endif  //APP_PLATFORM_WINDOWS



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
    }
};

//for thread test
void AppWorker(void* param) {
    CAtomicS32& count = *(CAtomicS32*) (param);
    CThread* td = CThread::getCurrentThread();
    printf("AppWorker::>>[thread=%u], [count=%d]\n", td->getID(), ++count);
}


//for thread test
void AppStartThreads() {
    CAtomicS32 count;
    CThread* td;
    CWorker wk;
    td = new CThread();
    td->start(wk);
    td->join();
    delete td;
    printf("----------------------------------------\n");

    const u32 max = 111;
    CThreadPool pool(9);
    pool.start();
    for(u32 i = 0; i < 10; ++i) {
        pool.start(&wk);
    }
    for(u32 i = 0; i < max; ++i) {
        pool.start(AppWorker, (void*) (&count));
    }
    //AppQuit();
    //pool.stop();
    pool.join();
    printf("AppStartThreads::>>[count=%d]\n", count.getValue());
    AppQuit();
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
    AppQuit();
}

}//namespace irr


int main(int argc, char** argv) {
    irr::AppStartThreads();
    irr::AppStartProcesses();
    printf("@Test quit success.\n");
    return 0;
}//main
