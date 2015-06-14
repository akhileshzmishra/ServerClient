//#include "StdAfx.h"
#include "Windows.h"
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
	if (mThreadId == (int) ::GetCurrentThreadId())
	{
		mCount++;
		return;
	}
	mSem.Take();
	mThreadId = (int) ::GetCurrentThreadId();
	mCount    = 1;
}
bool XLMutex::Unlock()
{
	if(!IsCreated())
	{
		return false;
	}

	if (mThreadId != (int) ::GetCurrentThreadId())
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

	if (mThreadId == (int)::GetCurrentThreadId())
	{
		mCount++;

		return true;
	}

	if (mSem.Try(msec) == false)
	{
		return false;
	}

	mThreadId = (int) ::GetCurrentThreadId();
	mCount    = 1;

	return true;
}
