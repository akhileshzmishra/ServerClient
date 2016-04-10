//#include "StdAfx.h"

#include "XLSemaPhore.h"
#include <iostream>

XLSemaPhore::XLSemaPhore()
#ifdef _C_11_COMPILER
:mSemCount(0),
mMaxCount(0),
mbCreated(false)
#else
:mID(0)
#endif
{ 	
}

bool XLSemaPhore::Create(int count)
{
    if(!IsCreated())
    {
#ifdef _C_11_COMPILER
        //do nothing here.
        mSemCount = 0;
        mMaxCount = count;
        mbCreated = true;
#else
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
#endif // __MAC_OS_
#endif // __WINDOWS_OS_
#endif //_C_11_COMPILER
    }

	return IsCreated();
}

bool XLSemaPhore::Create(int count, int maxcount)
{
    if(!IsCreated())
    {
#ifdef _C_11_COMPILER
        // do nothing here.
        mSemCount = count;
        mMaxCount = maxcount;
        mbCreated = true;
#else
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
#endif // __MAC_OS_
#endif // __WINDOWS_OS_
#endif //_C_11_COMPILER
    }

	return IsCreated();
}


void XLSemaPhore::Destroy()
{
	if(IsCreated())
	{
#ifdef _C_11_COMPILER
        // do nothing here.
        mSemCount = 0;
        mMaxCount = 0;
        mbCreated = false;
#else
#ifdef __WINDOWS_OS_
		::CloseHandle(mID);
#else
#ifdef __MAC_OS_
		sem_close(mID);
#else
		sem_destroy(mID);
#endif //__MAC_OS_
#endif //__WINDOWS_OS_
		mID = 0;
#endif //_C_11_COMPILER
	}
}

XLSemaPhore::~XLSemaPhore(void)
{
	
}

void XLSemaPhore::Take(void)
{
	if(IsCreated())
	{
#ifdef _C_11_COMPILER
        std::unique_lock<std::mutex> lock(m_Mut);
        
        while(mSemCount >= mMaxCount)
        {
            m_CV.wait(lock);
        }
        ++mSemCount;
#else
#ifdef __WINDOWS_OS_
		::WaitForSingleObject(mID, INFINITE);
#else
		sem_wait(mID);
#endif
#endif
	}
}



bool XLSemaPhore::Try(unsigned long msec)
{
    bool bRet = false;
	if(!IsCreated())
	{
		return bRet;
	}
#ifdef _C_11_COMPILER
    std::unique_lock<std::mutex> lock(m_Mut);

    while(mSemCount >= mMaxCount)
    {
        std::this_thread::sleep_for(std::chrono::seconds(msec/1000));
        m_CV.wait(lock);
    }
    
    ++mSemCount;
   
    bRet = true;
#else

	if (msec == (unsigned long)0)
	{
		Take();

		bRet = true;
	}
	else
	{
#ifdef __WINDOWS_OS_
		return ::WaitForSingleObject(mID, msec) == WAIT_OBJECT_0;
#else
		sem_trywait(mID);
#endif
	}
#endif
	return bRet;
}


void XLSemaPhore::Give(void)
{
	if(IsCreated())
	{
#ifdef _C_11_COMPILER
        //std::lock_guard<std::mutex> lock(m_Mut);
        std::unique_lock<std::mutex> lck(m_Mut);
        //m_Mut.lock();
        --mSemCount;
        //m_Mut.unlock();
        m_CV.notify_one();
#else
#ifdef __WINDOWS_OS_
		::ReleaseSemaphore(mID, 1, NULL);
#else
		sem_post(mID);
#endif
#endif
	}
}

