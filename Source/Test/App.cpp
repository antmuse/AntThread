#include "HConfig.h"
#include "IAppLogger.h"

#include "IRunnable.h"
#include "CThread.h"
#include "CThreadPool.h"


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



namespace irr{

    void AppQuit(){
        c8 key = '\0';
        while('*' != key){
            printf("@Please input [*] to quit\n");
            scanf("%c", &key);
        }
    }


    //for thread test
    class CWorker : public IRunnable{
    public:
        virtual void run(){
            CThread* td = CThread::getCurrentThread();
            printf("CWorker.start::thread id = %u\n", td->getID());
            CThread::sleep(5000);
            printf("CWorker.stop::thread id = %u\n", td->getID());
        }
    };

    //for thread test
    void AppWorker(void* param){
        CThread* td = CThread::getCurrentThread();
        printf("AppWorker.start::thread id = %u\n", td->getID());
        CThread::sleep(5000);
        printf("AppWorker.stop::thread id = %u\n", td->getID());
    }


    //for thread test
    void AppStartThreads(){
        const u32 max = 3;
        CThread* allthread[max];
        CWorker wk;
        for(u32 i=0; i<max; ++i){
            allthread[i] = new CThread();
            allthread[i]->start(wk);
        }

        for(u32 i=0; i<max; ++i){
            allthread[i]->join();
        }


        CThreadPool pool(max);
        pool.start();
        for(u32 i=0; i<max; ++i){
            pool.start(&wk);
        }
        for(u32 i=0; i<max; ++i){
            pool.start(AppWorker, 0);
        }
        pool.stop();

        AppQuit();
    }

}//namespace irr


int main(int argc, char** argv) {
    irr::AppStartThreads();
    printf("@Test quit success.\n"); 
    return 0;
}//main
