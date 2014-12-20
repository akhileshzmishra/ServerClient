#ifndef XLEVENT_Q___H_
#define XLEVENT_Q___H_
#include <queue>
#include <list>
#include "XLMutex.h"
#include <memory.h>
#include "XLEvents.h"
#include "LockFreeQueue.h"



//Multiple writer single reader lock free reading till 
//empty
class XLEventQueue
{
	XLMutex                   mMut;
	XLSemaPhore               mSem;
	XDS::XLLFQ<XLThreadEvent> mQ;
	int                       mMaxSize;
	XLThreadEvent             mEvent;
	bool                      mCreated;
public:
	XLEventQueue(int MaxSize = XLEventQueueConfig_MaxSize);
	~XLEventQueue(void);
	bool Create();
	void Destroy();
	bool Put(const XLThreadEvent* e);
	bool Get(XLThreadEvent* e);
	bool IsCreated()               { return mCreated;}
};



#endif