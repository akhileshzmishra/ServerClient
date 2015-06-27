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
	XLMutex                   mMut[2];
	XLSemaPhore               mSem;
	XDS::XLLFQ<XLThreadEvent> mQ;
	int                       mMaxSize;
	XLThreadEvent             mEvent;
	bool                      mCreated;
	int                       mProperties;
public:
	enum 
	{
		SingleReader = 1,
		MultiReader = 2,
		SingleWriter = 4,
		MultiWriter = 8,		
	};
public:
	XLEventQueue(int MaxSize = XLEventQueueConfig_MaxSize);
	~XLEventQueue(void);
	bool Create(int properties = (XLEventQueue::SingleReader | XLEventQueue::MultiWriter));
	void Destroy();
	bool Put(const XLThreadEvent* e);
	bool Get(XLThreadEvent* e);
	bool IsCreated()               { return mCreated;}
private:
	void ReaderLock();
	void ReaderUnlock();
	void WriterLock();
	void WriterUnlock();
};



#endif