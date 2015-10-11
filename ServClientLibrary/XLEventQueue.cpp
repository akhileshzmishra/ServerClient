#include "XLEventQueue.h"
#include <iostream>
#include "XLThreadDependency.h"
using namespace std;


XLEventQueue::XLEventQueue(int MaxSize):
mMaxSize(MaxSize),
mCreated(false),
mProperties(0)
{
}

XLEventQueue::~XLEventQueue(void)
{
}

bool XLEventQueue::Create(int properties)
{
	if(mCreated)
	{
		return false;
	}
	mMut[0].Create();
	if(!mMut[0].IsCreated())
	{
		return false;
	}
	mMut[1].Create();
	if(!mMut[1].IsCreated())
	{
		mMut[0].Destroy();
		return false;
	}
	mSem.Create(0);
	if(!mSem.IsCreated())
	{
		mMut[0].Destroy();
		mMut[1].Destroy();
	}

	mQ.SetMaxSize(mMaxSize);
	mProperties = properties;
	mCreated = true;
	return true;
}


void XLEventQueue::Destroy()
{
	if(mCreated)
	{
		mMut[0].Destroy();
		mMut[1].Destroy();
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
	WriterLock();

	mEvent = *e;

	if(mQ.push(mEvent))
	{
		//std::cout<<"QUEUE "<<mQ.size()<<std::endl;
		WriterUnlock();
		mSem.Give();
		
	}
	else
	{	
		bRet = false;
		WriterUnlock();		
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
	//std::cout<<"DE-QUEUE "<<mQ.size()<<std::endl;
	ReaderLock();
	mQ.pop(*e);
	ReaderUnlock();

	return true;
}


void XLEventQueue::ReaderLock()
{
	if(mProperties & MultiReader)
	{
		mMut[0].Lock();
	}
}
void XLEventQueue::ReaderUnlock()
{
	if(mProperties & MultiReader)
	{
		mMut[0].Unlock();
	}
}
void XLEventQueue::WriterLock()
{
	if(mProperties & MultiWriter)
	{
		mMut[1].Lock();
	}
}
void XLEventQueue::WriterUnlock()
{
	if(mProperties & MultiWriter)
	{
		mMut[1].Unlock();
	}
}
