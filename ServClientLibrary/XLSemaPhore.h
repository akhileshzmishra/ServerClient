#ifndef _XL_SEMAPHORE_H__
#define _XL_SEMAPHORE_H__
#include "XLThreadDependency.h"




class XLSemaPhore
{
#ifdef _C_11_COMPILER
    std::mutex                                 m_Mut;
    std::mutex                                 m_ProtMutex;
    std::condition_variable                    m_CV;
    volatile int                               mSemCount;
    volatile int                               mMaxCount;
    volatile bool                              mbCreated;
#else
	XLSemaphoreID mID;
#endif //_C_11_COMPILER
    
public:
	XLSemaPhore();
	virtual ~XLSemaPhore(void);


	bool Create(int count);
	bool Create(int count, int maxcount);
	bool IsCreated(void)
    {
#ifdef _C_11_COMPILER
        return mbCreated;
#else
        return (mID != 0);
#endif //_C_11_COMPILER
    }
	void Destroy();


	void Take(void);
	bool Try(unsigned long msec = 0);
	void Give(void);
};





#endif
