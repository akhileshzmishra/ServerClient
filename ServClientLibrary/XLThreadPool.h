#ifndef _XLTHEADPOOL_H_
#define _XLTHEADPOOL_H_

#include "XLEventThread.h"
#include "XLAutoPtr.h"
#include "XLMutex.h"
#include <vector>

namespace GenThreadPool
{


typedef XLAutoPtr::XLSharedSmPtr<XLEventQueue> EventQPtr;

class ThreadPoolJob
{
public:
	virtual void Work() = 0;
};



enum ThreadPoolEventType
{
	ThreadPoolEvent_Add,
	ThreadPoolEvent_Exit,
	ThreadPoolEvent_End
};


class XLWorkThEventAllocator
{
public:
	XLWorkThEventAllocator()  {}
	~XLWorkThEventAllocator() {}
	virtual bool GetEvent(XLThreadEvent* e) = 0;
};

class XLThWorkThread:public XLSimpleThreads
{
	XLWorkThEventAllocator*    m_Allocator;
	bool                       m_bWorking;
private:
	//Not implemented
	XLThWorkThread(const XLThWorkThread&);
	XLThWorkThread& operator = (const XLThWorkThread&);
public:
	XLThWorkThread();
	~XLThWorkThread();
	bool Create(XLWorkThEventAllocator* allocator);
	void Destroy();
	void RunProgram();
	bool IsEngaged()                       {return m_bWorking;}

};

class XLThreadPool: public XLWorkThEventAllocator
{
	typedef XLAutoPtr::XLSharedSmPtr<XLThWorkThread>   JobThread;
	typedef std::vector<JobThread>                     JobThreadList;
	bool                                               m_bDynamic;
	int                                                m_iNumMaxThread;
	int                                                m_iCurrWorkLoad;
	JobThreadList                                      m_ThreadList;
	XLMutex                                            m_Protection;
	EventQPtr                                          m_JobQueue;
	bool                                               m_bCreated;
	static const int                                   DefaultMaxThread = 5;

private:
	//Not implemented
	XLThreadPool(const XLThreadPool&);
	XLThreadPool& operator = (const XLThreadPool&);

public:
	
	//constructors/destructors
	XLThreadPool(int maxThread = XLThreadPool::DefaultMaxThread , bool Dynamic = false);
	virtual ~XLThreadPool(void);

public:
	bool Create();
	void Destroy();
	void AddJob(ThreadPoolJob* job);
	bool IsCreated()                                   {return m_bCreated;}

private:
	//Not to be exposed
	void DestroyAllJobThreads();
	virtual bool GetEvent(XLThreadEvent* e);
	
};




}; // namespace GenThreadPool








#endif