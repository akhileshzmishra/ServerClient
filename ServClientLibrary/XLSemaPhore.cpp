//#include "StdAfx.h"

#include "XLSemaPhore.h"
#include <iostream>

XLSemaPhore::XLSemaPhore():
mID(0)
{ 	
}

bool XLSemaPhore::Create(int count)
{
#ifdef __WINDOWS_OS_
	mID = ::CreateSemaphore(NULL, (long)count, 0x7fffffff, NULL);
#else
#ifdef __MAC_OS_
	void* ptr = (void*)this;
	mID = sem_open(std::to_string((long long) (ptr)).c_str(), O_CREAT, 0777, count);
	if(mID == SEM_FAILED)
	{
		int errorCode = errno;
		std::cout<<"Error in Semaphore Creation: "<<errorCode<<std::endl;
	}
#else
	int retVal = 0;
	if((retVal = sem_init(mID, 0, count)) != 0)
	{
		int errorCode = errno;
		std::cout<<"Error in Semaphore Creation: "<<errorCode<<std::endl;
		mID = 0;
	}
#endif
#endif

	return IsCreated();
}

bool XLSemaPhore::Create(int count, int maxcount)
{
#ifdef __WINDOWS_OS_
	mID = ::CreateSemaphore(NULL, (long)count, (long) maxcount, NULL);
#else
#ifdef __MAC_OS_
	void* ptr = (void*)this;
	mID = sem_open(std::to_string((long long )(ptr)).c_str(), O_CREAT, 0777, maxcount);
	if(mID == SEM_FAILED)
	{
		int errorCode = errno;
		std::cout<<"Error in Semaphore Creation: "<<errorCode<<std::endl;
	}
	else
	{
		for(int i = 0; i < count; i++)
		{
			sem_post(mID);
		}
	}
#else
	int retVal = 0;
	if((retVal = sem_init(mID, 0, maxcount)) != 0)
	{
		int errorCode = errno;
		std::cout<<"Error in Semaphore Creation: "<<errorCode<<std::endl;
	}
	else
	{
		for(int i = 0; i < count; i++)
		{
			sem_post(mID);
		}
	}
#endif
#endif

	return IsCreated();
}


void XLSemaPhore::Destroy()
{
	if(IsCreated())
	{
#ifdef __WINDOWS_OS_
		::CloseHandle(mID);
#else
#ifdef __MAC_OS_
		sem_close(mID);
#else
		sem_destroy(mID);
#endif
#endif
		mID = 0;
	}
}

XLSemaPhore::~XLSemaPhore(void)
{
	
}

void XLSemaPhore::Take(void)
{
	if(IsCreated())
	{
#ifdef __WINDOWS_OS_
		::WaitForSingleObject(mID, INFINITE);
#else
		sem_wait(mID);
#endif
	}
}



bool XLSemaPhore::Try(unsigned long msec)
{
	if(!IsCreated())
	{
		return false;
	}

	if (msec == (unsigned long)0)
	{
		Take();

		return true;
	}
	else
	{
#ifdef __WINDOWS_OS_
		return ::WaitForSingleObject(mID, msec) == WAIT_OBJECT_0;
#else
		sem_trywait(mID);
#endif
	}
	return false;
}


void XLSemaPhore::Give(void)
{
	if(IsCreated())
	{
#ifdef __WINDOWS_OS_
		::ReleaseSemaphore(mID, 1, NULL);
#else
		sem_post(mID);
#endif
	}
}

