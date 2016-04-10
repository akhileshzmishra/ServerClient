#ifndef XL_MUTEX__H__
#define XL_MUTEX__H__
#include "XLSemaPhore.h"
#include "XLThreadDependency.h"

class XLMutex
{
#ifdef _C_11_COMPILER
    std::recursive_mutex      mSem;
    volatile bool             mbCreated;
#else
	XLSemaPhore               mSem;
    Thread_Identifier         mThreadId;
    volatile int              mCount;
#endif
	
public:
	XLMutex(void);
	bool Create();
	void Destroy();
	void Lock();
	bool Unlock();
	bool TryLock(int msec = 0);
	~XLMutex(void);

	bool IsCreated()
    {
#ifdef _C_11_COMPILER
        return mbCreated;
#else
        return mSem.IsCreated();
#endif
    }
};


#endif
