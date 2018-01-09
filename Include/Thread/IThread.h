/**
*@file IThread.h
*@brief This file defined a thread that wrapped native threads of Windows and Linux.
*@author antmuse@live.cn
*@date 2014-09-22
*/

#ifndef  APP_ITHREAD_H
#define  APP_ITHREAD_H

#include "HConfig.h"
#include "irrString.h"

namespace irr {
class IRunnable;
class CThreadEvent;

///A callable function pointer for threads.
typedef void(*AppCallable)(void*);

///A callable packet for threads.
struct SCallbackData {
    SCallbackData() : mCallback(0), mData(0) {
    }
    SCallbackData(AppCallable call, void* data) : mCallback(call), mData(data) {
    }
    AppCallable  mCallback;
    void* mData;
};


/**
*@class IThread
*@brief This class implements a platform-independent
* wrapper to an operating system thread.
*/
class IThread {
public:
#if defined(APP_OS_32BIT)
    typedef u32 TID;
#elif defined(APP_OS_64BIT)
    typedef u64 TID;
#endif //APP_OS_64BIT

#if defined( APP_PLATFORM_WINDOWS )
#elif
#endif //APP_PLATFORM_WINDOWS

    IThread() {
    }

    /// Destructor
    virtual ~IThread() {
    }


    /// Returns the unique thread ID of the thread.
    virtual s32 getID() const = 0;


    /// Returns the native thread ID of the thread.
    virtual TID getNativeID() const = 0;


    /// Returns the name of the thread.
    virtual const core::stringc& getName() const = 0;


    /// Sets the name of the thread.
    virtual void setName(const core::stringc& iName) = 0;


    /**
    *@brief Sets the thread's priority.
    *@param iPriority Priority.
    */
    virtual void setPriority(EThreadPriority iPriority) = 0;


    /// Returns the thread's priority.
    virtual EThreadPriority getPriority() const = 0;


    /**
    *@brief Sets the thread's priority.
    *@param iPolicy Policy.
    *@return The mininum operating system-specific priority value by policy.
    */
    static s32 getMinPriority(s32 iPolicy);



    /**
    *@brief Sets the thread's priority.
    *@param iPolicy Policy.
    *@return The maxinum operating system-specific priority value by policy.
    */
    static s32 getMaxPriority(s32 iPolicy);


    /**
    *@brief Sets the thread's stack size in bytes.
    * Setting the stack size to 0 will use the OS's default stack size.
    * Typically, the real stack size is rounded up to the nearest page size multiple.
    *@param size Thread stack size in bytes.
    */
    virtual void setStackSize(s32 size) = 0;


    /**
    *@brief Gets the thread's stack size.
    *@return The thread's stack size in bytes, or 0 if the default stack size is used.
    */
    virtual s32 getStackSize() const = 0;


    /**
    *@brief Start the thread with the given target.
    *@param target The runnable target.
    *@note The given runnable target must be
    * valid during the entire lifetime of the thread, as
    * only a reference to it is stored internally.
    */
    virtual void start(IRunnable& target) = 0;


    /**
    *@brief Start the thread with the given target and parameter.
    *@param target The runnable target.
    *@param iData The parameter for runnable target.
    *@note The given runnable target must be
    * valid during the entire lifetime of the thread, as
    * only a reference to it is stored internally.
    */
    virtual void start(AppCallable target, void* iData) = 0;


    /**
    *@brief Waitting until the thread completes execution.
    *@note If multiple threads try to join the same thread, the result is undefined.
    */
    virtual void join() = 0;


    /**
    *@brief Wait for at most the given interval for the thread to complete.
    *@note If multiple threads try to join the same thread, the result is undefined.
    *@param milliseconds Wait time in millisecond.
    *@return false if the thread does not complete within the specified time interval, else true.
    */
    virtual bool join(long milliseconds) = 0;


    /**
    *@return true if the thread is running.
    */
    virtual bool isRunning() const = 0;


    /**
    *@brief Wake up the thread which is in the state of interruptible sleep.
    *For threads that are not suspended, calling this
    * function has the effect of preventing the subsequent
    * wait() call to put thread in a suspended state.
    */
    virtual void wakeUp() = 0;


    /**
    *@brief Starts an interruptible sleep. When wait() is called,
    * the thread will remain suspended until the timeout expires or wakeUp() is called.
    *
    *@note The wait() and wakeUp() calls should be used with
    * understanding that the suspended state is not a true sleep,
    * but rather a state of waiting for an event, with timeout
    * expiration. This makes order of calls significant; calling
    * wakeUp() before calling wait() will prevent the next
    * wait() call to actually suspend the thread (which, in
    * some scenarios, may be desirable behavior).
    *
    *@param milliseconds Wait time in millisecond.
    *@return true if sleep attempt was completed, false
    * if sleep was interrupted by a wakeUp() call.
    */
    static bool wait(long milliseconds);

    /**
    *@brief Let current thread wait for a given event for given times.
    *@param iEvent Wait event.
    *@param milliseconds Wait time in millisecond.
    *@return true if sleep attempt was completed, false
    * if sleep was interrupted by the given event's wakeUp() call.
    */
    static bool wait(CThreadEvent& iEvent, long milliseconds);

    /**
    *@brief Suspends the current thread for the specified amount of time.
    *@param milliseconds Sleep time in millisecond.
    */
    static void sleep(long milliseconds);


    /// Yields cpu to other threads.
    static void yield();


    /**
    *@return The IThread object for the currently active thread, or 0 if
    * the current thread is the main thread.
    */
    static IThread* getCurrentThread();


    /**
    *@return The native thread ID for the current thread.
    */
    static TID getCurrentNativeID();


protected:
    /**
    *@brief Creates a unique id for a thread.
    *@return A unique id for a thread.
    */
    static s32 getUniqueID();

    IThread(const IThread& other) = delete;
    IThread& operator = (const IThread& other) = delete;
};


} //namespace irr

#endif  //APP_ITHREAD_H