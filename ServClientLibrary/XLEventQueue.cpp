#include "XLEventQueue.h"
#include <iostream>
#include "Windows.h"
using namespace std;


XLEventQueue::XLEventQueue(int MaxSize):
mMaxSize(MaxSize),
mCreated(false)
{
}

XLEventQueue::~XLEventQueue(void)
{
}

bool XLEventQueue::Create()
{
	if(mCreated)
	{
		return false;
	}
	mMut.Create();
	if(!mMut.IsCreated())
	{
		return false;
	}
	mSem.Create(0);
	if(!mSem.IsCreated())
	{
		mMut.Destroy();
	}

	mQ.SetMaxSize(mMaxSize);
	mCreated = true;
	return true;
}


void XLEventQueue::Destroy()
{
	if(mCreated)
	{
		mMut.Destroy();
		mSem.Destroy();
		mCreated = false;
	}
}

bool XLEventQueue::Put(const XLThreadEvent* e)
{ 
	if(!IsCreated())
	{
		return false;
	}
	bool bRet = true;
	mMut.Lock();

	mEvent = *e;

	if(mQ.push(mEvent))
	{
		mMut.Unlock();
		mSem.Give();
		
	}
	else
	{	
		bRet = false;
		mMut.Unlock();		
	}

	return bRet;

}

bool XLEventQueue::Get(XLThreadEvent* e)
{
	if(!IsCreated())
	{
		return false;
	}
	mSem.Take();
	mQ.pop(*e);
	return true;
}
