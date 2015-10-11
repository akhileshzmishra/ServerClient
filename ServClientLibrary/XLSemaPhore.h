#ifndef _XL_SEMAPHORE_H__
#define _XL_SEMAPHORE_H__
#include "XLThreadDependency.h"




class XLSemaPhore
{
	XLSemaphoreID mID;
public:
	XLSemaPhore();
	virtual ~XLSemaPhore(void);


	bool Create(int count);
	bool Create(int count, int maxcount);
	bool IsCreated(void)                 { return mID != 0; }
	void Destroy();


	void Take(void);
	bool Try(unsigned long msec = 0);
	void Give(void);
};





#endif
