#include "XLThreadPool.h"
#include <iostream>


GenThreadPool::XLThWorkThread::XLThWorkThread():
m_Allocator(0),
m_bWorking(false)
{
}


GenThreadPool::XLThWorkThread::~XLThWorkThread()
{
}


bool GenThreadPool::XLThWorkThread::Create(XLWorkThEventAllocator* allocator)
{
	if(!XLSimpleThreads::IsCreated())
	{
		XLSimpleThreads::Create("XLWorkerThread");
		m_Allocator = allocator;
		return true;
	}
	return false;
}


void GenThreadPool::XLThWorkThread::Destroy()
{
	if(XLSimpleThreads::IsCreated())
	{
		XLSimpleThreads::Destroy();
	}
}



void GenThreadPool::XLThWorkThread::RunProgram()
{
	if(XLSimpleThreads::IsCreated())
	{
		XLThreadEvent e;
		while(m_Allocator->GetEvent(&e))
		{
			m_bWorking = true;
			//std::cout<<"Running worker thread"<<std::endl;
			if(e.GetType() == ThreadPoolEvent_Exit)
			{
				return;
			}
			ThreadPoolJob* job = 0;
			if(e.Read(0, job))
			{
				if(job)
				{
					job->Work();
				}
			}
			e.Reset();
			m_bWorking = false;
		}
	}
}




GenThreadPool::XLThreadPool::XLThreadPool(int maxThread, bool Dynamic):
m_bDynamic(Dynamic),
m_iNumMaxThread(maxThread),
m_iCurrWorkLoad(0),
m_bCreated(false)
{
}

GenThreadPool::XLThreadPool::~XLThreadPool(void)
{
}


bool GenThreadPool::XLThreadPool::Create()
{
	if(!m_bCreated)
	{
		m_JobQueue = new XLEventQueue();
		if(m_JobQueue)
		{
			if(!m_JobQueue->Create(XLEventQueue::MultiReader | XLEventQueue::MultiWriter))
			{
				return false;
			}
			if(!m_ThCountGate.Create())
			{
				m_JobQueue->Destroy();
				return false;
			}
			m_bCreated = true;
			if(!m_bDynamic)
			{
				for(int i = 0; i < m_iNumMaxThread; i++)
				{
					JobThread wThread = new XLThWorkThread();
					m_ThreadList.push_back(wThread);
					wThread->Create(this);
				}
				m_iCurrWorkLoad = m_iNumMaxThread;
			}
			return true;
		}
	}
	return false;
}


void GenThreadPool::XLThreadPool::AddJob(ThreadPoolJob* job)
{
	if(GenThreadPool::XLThreadPool::IsCreated())
	{
		XLThreadEvent e;
		e.SetType(GenThreadPool::ThreadPoolEvent_Add);
		e.Write(0, job);
		if(m_bDynamic && (m_ThreadList.size() < m_iNumMaxThread))
		{
			m_ThCountGate.Lock();
			int count = m_ThreadList.size();			
			if(count < m_iNumMaxThread)
			{
				JobThread jThread = new XLThWorkThread();
				m_ThreadList.push_back(jThread);
				jThread->Create(this);
			}
			m_ThCountGate.Unlock();
		}

		//std::cout<<"Adding job "<<std::endl;
		//e.Print();
		m_JobQueue->Put(&e);
	}
}


void GenThreadPool::XLThreadPool::Destroy()
{
	if(GenThreadPool::XLThreadPool::IsCreated())
	{
		DestroyAllJobThreads();
		//DestroyAllJobThreads();
		for(int i = 0; i < m_ThreadList.size(); i++)
		{
			m_ThreadList[i]->Destroy();
		}
		m_ThCountGate.Destroy();	
		m_JobQueue->Destroy();
		m_bCreated = false;
	}
}



void GenThreadPool::XLThreadPool::DestroyAllJobThreads()
{
	if(GenThreadPool::XLThreadPool::IsCreated())
	{
		XLThreadEvent e;
		e.SetType(ThreadPoolEvent_Exit);
		for(int i = 0; i < m_ThreadList.size(); i++)
		{
			m_JobQueue->Put(&e);
		}
	}
}


bool GenThreadPool::XLThreadPool::GetEvent(XLThreadEvent* e)
{
	if(GenThreadPool::XLThreadPool::IsCreated())
	{
		//m_ThCountGate.Lock();
		if(m_JobQueue->Get(e))
		{
			//m_ThCountGate.Unlock();
			return true;
		}
		//m_ThCountGate.Unlock();
		return false;
	}
}