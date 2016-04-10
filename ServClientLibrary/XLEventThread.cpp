#include "XLEventThread.h"


TargetHandler::TargetHandler(XLEventThread* Task):
mTask(Task)
{
}

XLEventThread::XLEventThread(void):
TargetHandler(this),
mCreated(false),
mExit(false)
{
}

XLEventThread::~XLEventThread(void)
{
	Destroy();
}

bool XLEventThread::Create(const char* name)
{
	if(!mCreated)
	{
		if(!XLSimpleThreads::Create(name))
		{
			return false;
		}
		if(!XLEventQueue::Create())
		{
			XLSimpleThreads::Destroy();
			return false;
		}
		mCreated = true;
		t_OnCreate();
		return true;
	}
	return false;
}


void XLEventThread::Destroy()
{
	if(mCreated)
	{
		t_OnDestroy();
		XLSimpleThreads::Destroy();
		XLEventQueue::Destroy();
		mCreated = false;
	}
}


void XLEventThread::StopMainLoop()
{
	if(mCreated)
	{
		XLThreadEvent e;
		e.SetType(XLEventThreadEventType_Exit);
		Put(&e);
	}
}


void XLEventThread::RunProgram()
{
	if(!mCreated)
	{
		return;
	}
	while(!mExit)
	{
		if(!Get(&mEvent))
		{
			mExit = true;
		}
		else
		{
			OnEvent(&mEvent);
		}		
	}	

}


void XLEventThread::OnEvent(const XLThreadEvent* e)
{
	if(e->GetType() == XLEventThreadEventType_Exit)
	{
		mExit = true; 
	}
	else
	{
		t_OnEvent(e);
	}
}
