//#include "StdAfx.h"

#include "XLMutex.h"

XLMutex::XLMutex(void):
#ifdef _C_11_COMPILER
mbCreated(false)
#else
mThreadId(0),
mCount(0)
#endif //_C_11_COMPILER
{
}



XLMutex::~XLMutex(void)
{
}




bool XLMutex::Create()
{
    bool bRet = false;
    if(!IsCreated())
    {
#ifdef _C_11_COMPILER
        bRet = true;
        mbCreated = true;
#else
        if(!mSem.IsCreated())
        {
            bRet = mSem.Create(1);
        }
#endif //_C_11_COMPILER
    }
	return bRet;
}


void XLMutex::Destroy()
{
    if(IsCreated())
    {
#ifdef _C_11_COMPILER
        //do nothing here
        mbCreated = false;
#else
        mSem.Destroy();
        mThreadId = 0;
#endif //_C_11_COMPILER
    }
    
	
}



void XLMutex::Lock()
{
	if(!IsCreated())
	{
		return;
	}
#ifdef _C_11_COMPILER
    mSem.lock();
#else
#ifdef __WINDOWS_OS_
	if (mThreadId == GetCurrentThreadId())
#else
	if (mThreadId == pthread_self())
#endif //__WINDOWS_OS_

	{
		mCount++;
		return;
	}
	mSem.Take();
#ifdef __WINDOWS_OS_
	mThreadId = GetCurrentThreadId();
#else
	mThreadId = pthread_self();
#endif //__WINDOWS_OS_
	mCount    = 1;
#endif //_C_11_COMPILER
}



bool XLMutex::Unlock()
{
	if(!IsCreated())
	{
		return false;
	}
#ifdef _C_11_COMPILER
    mSem.unlock();
#else

#ifdef __WINDOWS_OS_
	if (mThreadId != GetCurrentThreadId())
#else
	if (mThreadId != pthread_self())
#endif //__WINDOWS_OS_
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
#endif //_C_11_COMPILER

	return true;
}



bool XLMutex::TryLock(int msec)
{
	if(!IsCreated())
	{
		return false;
	}
#ifdef _C_11_COMPILER
    mSem.try_lock();
#else

#ifdef __WINDOWS_OS_
	if (mThreadId == GetCurrentThreadId())
#else
	if (mThreadId == pthread_self())
#endif //__WINDOWS_OS_
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
#endif //__WINDOWS_OS_
	mCount    = 1;
#endif //_C_11_COMPILER

	return true;
}
