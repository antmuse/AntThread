# AntThread V1.0.0.0
A cross platform thread lib, current for Windows&amp;Linux(It's easy to port to Andriod platform).
# Usage

#include "IRunnable.h"
#include "CThread.h"
#include "CThreadPool.h"

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
int main(int argc, char** argv){
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
    return 0;
}
