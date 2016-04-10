#include "XLListeningThread.h"
#include "CommunicationHeader.h"
#include <iostream>
#include "XLSocketFactory.h"
using namespace std;

XLListeningThread::XLListeningThread(SocketConfig conf):
m_pSocket(0),
m_bExit(false),
m_Event(MAX_MESS_LENGTH),
m_Listener(0),
m_SocketConfig(conf),
m_DataStorage(new char[MAX_MESS_LENGTH]),
m_bOwnership(true)
{
	m_ExitKeeper.Create(1,1);
}

XLListeningThread::XLListeningThread(XLBaseSocket* conf):
m_pSocket(conf? conf: 0),
m_bExit(false),
m_Event(MAX_MESS_LENGTH),
m_Listener(0),
m_SocketConfig(),
m_DataStorage(new char[MAX_MESS_LENGTH]),
m_bOwnership(false)
{
	if(conf)
	{
		m_SocketConfig = *(conf->GetConfig());
	}
}

XLListeningThread::~XLListeningThread(void)
{
}


void XLListeningThread::Start()
{
	if(!IsCreated())
	{
		m_SocketConfig.SetEventListener(this);
		XLSimpleThreads::Create("LISTENER_CLIENT");
	}
	
}


void XLListeningThread::Stop()
{
	m_bExit = true;
	XLSimpleThreads::Destroy();
}




void XLListeningThread::RunProgram()
{
	if(!m_pSocket && m_bOwnership)
	{
		m_pSocket = XLSocketFactory::GetInstance()->GetDefaultSocket(m_SocketConfig, false);
		m_pSocket->Start();
		//m_pSocket->SetTimeOut(MAX_RECIEVE_TIME_OUT);
	}
	else if(m_pSocket)
	{
		if(!m_pSocket->IsActive() && !m_bOwnership)
		{
			m_pSocket->Start();
			//m_pSocket->SetTimeOut(MAX_RECIEVE_TIME_OUT);
		}
	}
	if(!m_pSocket)
	{
		return;
	}
   // m_Event.
	while(!m_bExit)
	{
        //m_DataStorage = new char[MAX_MESS_LENGTH];
		if(m_pSocket->Recieve(m_DataStorage, MAX_MESS_LENGTH) == SocketUtility::SocketNoError)
		{
			if(m_Listener)
			{
				m_Event.ReadFrom(m_DataStorage, MAX_MESS_LENGTH);
				m_Listener->OnListen(&m_Event);
			}
		}
	}
	if(m_pSocket)
	{
		m_pSocket->Stop();
	}
}




void XLListeningThread::OnRecieve(char* msg, int size)
{
	cout<<__FUNCTION__<<endl;
	cout<<endl<<endl;
	for(int i = 0; i < size; i++)
	{
		cout<<(int)msg[i] <<" ";
	}
	cout<<endl<<endl;
}


void XLListeningThread::RanOut()
{	
	m_ExitKeeper.Give();
}