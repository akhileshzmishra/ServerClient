//#include "StdAfx.h"

#include "XLMutex.h"

XLMutex::XLMutex(void):
mThreadId(0),
mCount(0)
{
}

XLMutex::~XLMutex(void)
{
}

bool XLMutex::Create()
{
	if(!mSem.IsCreated())
	{
	     return mSem.Create(1);
	}
	return false;
}
void XLMutex::Destroy()
{
	mSem.Destroy();
	mThreadId = 0;
}
void XLMutex::Lock()
{
	if(!IsCreated())
	{
		return;
	}
#ifdef __WINDOWS_OS_
	if (mThreadId == GetCurrentThreadId())
#else
	if (mThreadId == pthread_self())
#endif
	{
		mCount++;
		return;
	}
	mSem.Take();
#ifdef __WINDOWS_OS_
	mThreadId = GetCurrentThreadId();
#else
	mThreadId = pthread_self();
#endif
	mCount    = 1;
}
bool XLMutex::Unlock()
{
	if(!IsCreated())
	{
		return false;
	}

#ifdef __WINDOWS_OS_
	if (mThreadId != GetCurrentThreadId())
#else
	if (mThreadId != pthread_self())
#endif
	{
		return false;
	}

	mCount--;

	if (mCount > 0)
	{
		return true;
	}

	mThreadId = 0;

	mSem.Give();

	return true;
}
bool XLMutex::TryLock(int msec)
{
	if(!IsCreated())
	{
		return false;
	}

#ifdef __WINDOWS_OS_
	if (mThreadId == GetCurrentThreadId())
#else
	if (mThreadId == pthread_self())
#endif
	{
		mCount++;

		return true;
	}

	if (mSem.Try(msec) == false)
	{
		return false;
	}

#ifdef __WINDOWS_OS_
	mThreadId = GetCurrentThreadId();
#else
	mThreadId = pthread_self();
#endif
	mCount    = 1;

	return true;
}
