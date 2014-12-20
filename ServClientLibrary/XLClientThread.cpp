#include "XLClientThread.h"
#include <iostream>
#include "CommunicationHeader.h"
using namespace std;

XLClientThread::XLClientThread(SocketConfig& config):
m_SocketConfig(config),
m_DataListenTh(m_SocketConfig),
m_pListener(0)
{
}

XLClientThread::~XLClientThread(void)
{
	if(XLEventThread::IsCreated())
	{
		XLEventThread::Destroy();
	}
}


void XLClientThread::Start()
{
	if(!XLEventThread::IsCreated())
	{
		XLEventThread::Create("XLClientThread");
		if(XLEventThread::IsCreated())
		{
			XLThreadEvent e;
			e.SetType(START_THREAD);
			Put(&e);
		}
		if(m_pListener)
		{
			m_pListener->OnStart();
		}
	}
}


void XLClientThread::Stop()
{
	if(XLEventThread::IsCreated())
	{
		m_DataListenTh.Stop();
		XLEventThread::Destroy();
		if(m_pListener)
		{
			m_pListener->OnEnd();
		}
	}

}


void XLClientThread::SendSelfEvent(const XLThreadEvent* e)
{
	if(e)
	{
		XLEventQueue::Put(e);
	}
}

void XLClientThread::OnListen(const XLThreadEvent* e)
{
	SendSelfEvent(e);
}

void XLClientThread::t_OnEvent(const XLThreadEvent* e)
{
	if(!e)
	{
		return;
	}
	cout<<__FUNCTION__<<endl;
	PrintType(e->GetType());
	switch(e->GetType())
	{
	case START_THREAD:
		{
			m_DataListenTh.SetListener(this);
			m_DataListenTh.Start();
		}
		break;
	case LIFE_BEAT:
		{
		}
		break;
	case USR_MSG_BDCST:
	case USR_MSG:
	default:
		if(m_pListener)
		{
			m_pListener->OnEvent(e);
		}
		break;
	}
}

void XLClientThread::PrintType(int type)
{
	switch(type)
	{
	case START_THREAD:
		{
			cout<<"START_THREAD"<<endl;
		}
		break;
	case LIFE_BEAT:
		{
			cout<<"LIFE_BEAT"<<endl;
		}
		break;
	case USR_MSG_BDCST:
		{
			cout<<"USR_MSG_BDCST"<<endl;
		}
		break;
	case USR_MSG:
		{
			cout<<"USR_MSG"<<endl;
		}
		break;
	default:
		cout<<"NO_TYPE"<<endl;
		break;
	}
}
