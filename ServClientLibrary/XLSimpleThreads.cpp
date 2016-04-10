//#include "StdAfx.h"

#include "XLSimpleThreads.h"
#include "XLThreadDependency.h"
#include <iostream>

THFUN_RETVAL OTHER_EXPANSION RunSimpleThread(LPVOID data);

XLSimpleThreads::XLSimpleThreads(void):
m_Sync1(),
m_Sync2(),
m_handle(0),
#ifdef _C_11_COMPILER
mThreadID()
#else
mThreadID(0)
#endif
{
}

XLSimpleThreads::~XLSimpleThreads(void)
{
    try
    {
#ifdef _C_11_COMPILER
        if(m_handle)
        {
            delete m_handle;
        }
        m_handle = 0;
#endif
    }
    catch(...)
    {
        
    }
}

bool XLSimpleThreads::Create(const char *threadname, unsigned long stackSize)
{
	mName = threadname;
	m_Sync1.Create(0,1);
	if(!m_Sync1.IsCreated())
	{
		return false;
	}
	m_Sync2.Create(0,1);
	if(!m_Sync2.IsCreated())
	{
		m_Sync1.Destroy();
		return false;
	}
#ifdef _C_11_COMPILER
    m_handle = new std::thread(RunSimpleThread, (LPVOID)this);
#else
#ifdef __WINDOWS_OS_
	m_handle = (int)CreateThread(NULL, (unsigned int)stackSize, RunSimpleThread, (LPVOID)this, 0, (DWORD*)&mID); 
#else

	pthread_attr_t attr;
	pthread_attr_init(&attr);
	//pthread_attr_setstack(&attr, stackSize);
    int retVal = pthread_create(&m_handle, 0, RunSimpleThread, this);
    if(retVal != 0)
    {
    	std::cout<<"Error in Creating PThread"<<std::endl;
    }
	pthread_attr_destroy(&attr);

#endif //__WINDOWS_OS_
#endif // _C_11_COMPILER
	m_Sync1.Give();

	return true;
}

void XLSimpleThreads::Destroy()
{
	if(!IsCreated())
	{
		return;
	}
	m_Sync2.Take();
	if(	m_Sync1.IsCreated())
	{
		m_Sync1.Destroy();
	}
	if(	m_Sync2.IsCreated())
	{
		m_Sync2.Destroy();
	}
#ifdef _C_11_COMPILER
    if(m_handle)
    {
        m_handle->join();
        delete m_handle;
    }
    m_handle = 0;
#else
#ifdef __WINDOWS_OS_
	CloseHandle((HANDLE)m_handle);
#else
	pthread_kill(m_handle, 0);
#endif //__WINDOWS_OS_
	m_handle = 0;
	mThreadID = 0;
#endif // _C_11_COMPILER

}

THFUN_RETVAL OTHER_EXPANSION RunSimpleThread(LPVOID data)
{
	XLSimpleThreads* Thread = (XLSimpleThreads*)data;
	if(Thread)
	{
		Thread->GetThreadID();
		Thread->m_Sync1.Take();
		if(Thread->IsCreated())
		{
			Thread->RunProgram();
		}
		Thread->m_Sync2.Give();
		Thread->RanOut();
	}

	return 0;
}


void XLSimpleThreads::Sleep(int millsec)
{
	if(IsCreated())
	{
#ifdef _C_11_COMPILER
        std::chrono::milliseconds ms(millsec);
        std::this_thread::sleep_for(ms);
#else
		Thread_Identifier id;
#ifdef __WINDOWS_OS_
	    id = (int)::GetCurrentThreadId();
#else
#endif //__WINDOWS_OS_
		if(id == mThreadID)
		{
#ifdef __WINDOWS_OS_
			::Sleep(millsec);
#else
			sleep((unsigned int)(millsec/1000));
#endif //__WINDOWS_OS_
		}
#endif //_C_11_COMPILER
	}
}

void XLSimpleThreads::GetThreadID()
{
#ifdef _C_11_COMPILER
    if(m_handle)
    {
        mThreadID = m_handle->get_id();
    }
#else
#ifdef __WINDOWS_OS_
	mThreadID = (int)::GetCurrentThreadId();
#else
	mThreadID = pthread_self();
#endif
#endif
}
