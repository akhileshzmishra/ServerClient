#ifndef XLEVENT_THREAD_H___
#define XLEVENT_THREAD_H___
#include "XLEventQueue.h"
#include "XLSimpleThreads.h"
#include "XLMutex.h"
#include <set>



class TargetHandler
{
	class XLEventThread* mTask;
public:
	TargetHandler(XLEventThread* Task);
	virtual void OnEvent(const XLThreadEvent* e) {}
};

class XLEventThread: public XLEventQueue, public XLSimpleThreads, public TargetHandler
{
	bool                      mCreated;
	XLThreadEvent             mEvent;
	bool                      mExit;
public:
	enum
	{
		XLEventThreadEventType_Exit = 12,
		XLEventThreadEventType_Max
	};
public:
	XLEventThread(void);
	~XLEventThread(void);
	bool Create(const char* name);
	void Destroy();
	bool IsCreated()         { return mCreated;}
	void RunProgram();
	void StopMainLoop();
protected:
	void OnEvent(const XLThreadEvent* e);
	virtual void t_OnEvent(const XLThreadEvent* e){}
	virtual void t_OnCreate(){}
	virtual void t_OnDestroy(){}
};



#endif