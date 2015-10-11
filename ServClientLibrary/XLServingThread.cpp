#include "XLServingThread.h"
#include "SocketUtility.h"
#include "CommunicationHeader.h"
#include "XLSocketFactory.h"
#include <iostream>
using namespace std;



XLServingThread::XLServingThread(SocketConfig& config, char* threadname):
XLEventThread(),
m_pSocket(0),
m_Name(threadname),
m_AliveAlarm(),
m_SockConfig(config),
m_Listener(0),
m_bToPingClient(true),
m_AcceptorThread(0),
m_iMappingType((int)RMT_OneToOne)
{
}

XLServingThread::~XLServingThread(void)
{
}



void XLServingThread::Start()
{
	if(!IsCreated())
	{
		if(XLEventThread::Create(m_Name.c_str()))
		{
			if(!m_pSocket)
			{
				m_SockConfig.SetEventListener(this);
				m_pSocket = XLSocketFactory::GetInstance()->GetDefaultSocket(m_SockConfig, true);
				XLThreadEvent e;
				e.SetType(START_THREAD);
				Put(&e);
			}
		}
	}
}

void XLServingThread::Broadcast(const char* msg, int len)
{
	XLThreadEvent e;
	e.SetData(msg, len);
	e.SetType(USR_MSG_BDCST);
	SendSelfEvent(&e);
}

void XLServingThread::t_OnCreate()
{
	if(m_Listener)
	{
		m_Listener->OnStart(this);
	}
}


void XLServingThread::t_OnDestroy()
{
	if(m_Listener)
	{
		m_Listener->OnEnd(this);
	}
}


void XLServingThread::Stop()
{
	if(IsCreated())
	{
		if(m_AcceptorThread)
		{
			m_AcceptorThread->Stop();
			delete m_AcceptorThread;
			m_AcceptorThread = 0;
		}

		XLEventThread::StopMainLoop();
	}
}

void XLServingThread::Send(int id, const char* msg, int len)
{
	XLThreadEvent e;
	e.Write(0, id);
	e.Write(sizeof(int), msg, len);
	e.SetType(USR_MSG);
	SendSelfEvent(&e);
}

void XLServingThread::t_OnEvent(const XLThreadEvent* e)
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
		if(!m_AcceptorThread && m_pSocket)
		{
			m_AcceptorThread = new XLAcceptorThread(m_pSocket, m_SockConfig.GetNumberofClients());
			m_AcceptorThread->Start();
		}
		if(m_bToPingClient)
		{
			m_AliveAlarm.SetListener(this);
			m_AliveAlarm.SetAlarm(CLIENT_HEART_BEAT_ALARM_TIME);
		}
		break;
	case ACCEPT_INCOMING:
		if(m_pSocket)
		{
			CHARSTORAGE s = e->ConvertIntoCharBuffer();
			BASE_SOCKET_PTR id;
			if(m_IncomingQueue.pop(id))
			{
				if(id)
				{
					m_ClientIDs.insert(id);
					id->Send( s, e->WritableSize());
				}
			}			
		}
		break;
	case LIFE_BEAT:
		if(m_pSocket)
		{
			CHARSTORAGE s = e->ConvertIntoCharBuffer();
			CLITR itr = m_ClientIDs.begin();
			while(itr != m_ClientIDs.end())
			{
				(*itr).GetConstData()->Send( s, e->WritableSize());
				itr++;

			}

		}
		break;
	case USR_MSG_BDCST:
		{
			CHARSTORAGE s = e->ConvertIntoCharBuffer();
			CLITR itr = m_ClientIDs.begin();
			while(itr != m_ClientIDs.end())
			{
				(*itr).GetConstData()->Send(s, e->WritableSize());
				itr++;
				if(m_Listener)
				{
					m_Listener->OnSent(e);
				}
			}
			
		}
		break;
	case USR_MSG:
		{
			CHARSTORAGE s = e->ConvertIntoCharBuffer();
			XLBaseSocket* id = 0;
			if(e->Read(0, id))
			{
				if(id)
				{
					CLITR itr = m_ClientIDs.find(id);
					if(itr != m_ClientIDs.end())
					{
						(*itr).GetConstData()->Send(s, e->WritableSize());
					}
					if(m_Listener)
					{
						m_Listener->OnSent(e);
					}
				}
			}
		}
		break;
	default:
		if(m_Listener)
		{
			m_Listener->OnEvent(e);
		}
		break;
	}
}


void XLServingThread::StartAccepting()
{
	XLThreadEvent e;
	e.SetType(START_THREAD);
	SendSelfEvent(&e);
}

void XLServingThread::SendSelfEvent(const XLThreadEvent* e)
{
	if(e)
	{
		XLEventQueue::Put(e);
	}
}


void XLServingThread::OnConnect()
{
	cout<<__FUNCTION__<<endl;
}


void XLServingThread::OnDisConnect()
{
	cout<<__FUNCTION__<<endl;
}


void XLServingThread::OnListen()
{
	cout<<__FUNCTION__<<endl;
}


void XLServingThread::OnAccept(BASE_SOCKET_PTR socket)
{
	m_IncomingQueue.push(socket);
	cout<<__FUNCTION__<<endl;
	XLThreadEvent e;
	e.SetType(ACCEPT_INCOMING);
	SendSelfEvent(&e);
	if(m_Listener)
	{
		m_Listener->OnAccept(this, socket);
	}
}

void XLServingThread::PrintType(int type)
{
	switch(type)
	{
	case START_THREAD:
		{
			cout<<"START_THREAD"<<endl;
		}
		break;
	case ACCEPT_INCOMING:
		{
			cout<<"ACCEPT_INCOMING"<<endl;
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
	default:
		cout<<"NO_TYPE"<<endl;
		break;
	}
}


void XLServingThread::OnAlarm(XLAlarm* alarm, AlarmData* pData)
{
	static int i = 0;
	cout<<__FUNCTION__<<endl;
	cout<<i<<endl;
	i++;

	XLThreadEvent e;
	e.SetType(LIFE_BEAT);
	SendSelfEvent(&e);
}

void XLServingThread::OnSend(const char* msg, int size)
{
	cout<<__FUNCTION__<<endl;
	cout<<endl<<endl;
	for(int i = 0; i < size; i++)
	{
		cout<<(int)msg[i] <<" ";
	}
	cout<<endl<<endl;
}
