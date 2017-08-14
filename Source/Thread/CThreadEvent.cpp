#include "CThreadEvent.h"
#if defined(APP_PLATFORM_WINDOWS)
#include <windows.h>
#elif defined(APP_PLATFORM_LINUX)
//
#endif

namespace irr {


#if defined(APP_PLATFORM_WINDOWS)

	CThreadEvent::CThreadEvent(fschar_t* iName/* = 0*/, bool autoReset/* = true*/){
		mHandle = CreateEvent(NULL, autoReset ? FALSE : TRUE, FALSE, iName);
		if (!mHandle){
			//("cannot create event");
		}
	}


	CThreadEvent::~CThreadEvent(){
		CloseHandle(mHandle);
	}


	void CThreadEvent::set(){
		if (!SetEvent(mHandle))	{
			//throw SystemException("cannot signal event");
		}
	}


	void  CThreadEvent::reset(){
		if (!ResetEvent(mHandle))	{
			//throw SystemException("cannot reset event");
		}
	}


	bool CThreadEvent::wait(){
		switch (WaitForSingleObject(mHandle, INFINITE)) {
		case WAIT_OBJECT_0:
			return true;
		default:
			break;//throw SystemException("wait for event failed");
		}
		return false;
	}


	bool CThreadEvent::wait(long milliseconds){
		switch (WaitForSingleObject(mHandle, milliseconds + 1)) {
		case WAIT_TIMEOUT:
			return false;
		case WAIT_OBJECT_0:
			return true;
		default:
			break; //throw SystemException("wait for event failed");		
		}
		return false;
	}




#elif defined(APP_PLATFORM_LINUX)

	CThreadEvent::CThreadEvent(fschar_t* iName/* = 0*/, bool autoReset/* = true*/) : mAutoReset(autoReset), mStatus(false) {
		if (pthread_mutex_init(&mMutex, NULL)){
			// SystemException("cannot create event (mutex)");
		}
		pthread_condattr_t attr;
		if (pthread_condattr_init(&attr))	{
			pthread_mutex_destroy(&mMutex);
			// SystemException("cannot create event (condition attribute)");
		}
		if (pthread_condattr_setclock(&attr, CLOCK_MONOTONIC))   {
			pthread_condattr_destroy(&attr);
			pthread_mutex_destroy(&mMutex);
			// SystemException("cannot create event (condition attribute clock)");
		}
		if (pthread_cond_init(&mCond, &attr))	{
			pthread_condattr_destroy(&attr);
			pthread_mutex_destroy(&mMutex);
			// SystemException("cannot create event (condition)");
		}
		pthread_condattr_destroy(&attr);
	}


	CThreadEvent::~CThreadEvent(){
		pthread_cond_destroy(&mCond);
		pthread_mutex_destroy(&mMutex);
	}


	void CThreadEvent::set(){
		if (pthread_mutex_lock(&mMutex)){
			// SystemException("cannot signal event (lock)");
		}
		mStatus = true;
		if (pthread_cond_broadcast(&mCond))	{
			pthread_mutex_unlock(&mMutex);
			// SystemException("cannot signal event");
		}
		pthread_mutex_unlock(&mMutex);
	}


	void  CThreadEvent::reset(){
		if (pthread_mutex_lock(&mMutex)){
			// SystemException("cannot reset event");
		}
		mStatus = false;
		pthread_mutex_unlock(&mMutex);
	}


	bool CThreadEvent::wait(){
		if (pthread_mutex_lock(&mMutex)){
			// SystemException("wait for event failed (lock)");
		}
		while (!mStatus) 	{
			if (pthread_cond_wait(&mCond, &mMutex))	{
				pthread_mutex_unlock(&mMutex);
				// SystemException("wait for event failed");
			}
		}
		if (mAutoReset){
			mStatus = false;
		}
		pthread_mutex_unlock(&mMutex);
	}


	bool CThreadEvent::wait(long milliseconds){
		s32 rc = 0;
		struct timespec abstime;
		clock_gettime(CLOCK_MONOTONIC, &abstime);
		abstime.tv_sec  += milliseconds / 1000;
		abstime.tv_nsec += (milliseconds % 1000)*1000000;
		if (abstime.tv_nsec >= 1000000000)	{
			abstime.tv_nsec -= 1000000000;
			abstime.tv_sec++;
		}
#if 0
		struct timeval tv;
		gettimeofday(&tv, NULL);
		abstime.tv_sec  = tv.tv_sec + milliseconds / 1000;
		abstime.tv_nsec = tv.tv_usec*1000 + (milliseconds % 1000)*1000000;
		if (abstime.tv_nsec >= 1000000000)	{
			abstime.tv_nsec -= 1000000000;
			abstime.tv_sec++;
		}
#endif


		if (pthread_mutex_lock(&mMutex) != 0){
			// SystemException("wait for event failed (lock)");
		}
		while (!mStatus) 	{
			if ((rc = pthread_cond_timedwait(&mCond, &mMutex, &abstime)))	{
				if (rc == ETIMEDOUT) break;
				pthread_mutex_unlock(&mMutex);
				// SystemException("cannot wait for event");
			}
		}
		if (rc == 0 && mAutoReset){
			mStatus = false;
		}
		pthread_mutex_unlock(&mMutex);

		return rc == 0;
	}

#endif //APP_PLATFORM_LINUX


} //end namespace irr