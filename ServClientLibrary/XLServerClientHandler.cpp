#include "XLServerClientHandler.h"
#include "DataInterpreter.h"
#include "SocketUtility.h"
#include <iostream>
#include <string>
using namespace std;

XLServerClientHandler::XLServerClientHandler(const int identifier, SocketConfig& config):
m_clientIdentifier(identifier),
m_EventPlace(0),
m_RecieverThread(0),
m_RecieveListener(0),
m_pRecieverSocket(0),
m_SockConfig(config)
{
}

XLServerClientHandler::~XLServerClientHandler(void)
{
	Stop();
}


void XLServerClientHandler::Start(const char* clientName)
{
	if(XLEventThread::Create(clientName))
	{
	}
	if(!m_pRecieverSocket)
	{
		m_SockConfig.SetPortNumber(m_SockConfig.GetPortNumber() + 1);
		m_pRecieverSocket = new XLSocket(m_SockConfig, false);
	}
	if(!m_RecieverThread && m_pRecieverSocket)
	{
		m_RecieverThread = 0;//new XLCommonRecieverThread(m_pRecieverSocket->GetListenPortID());
	}
	if(m_RecieverThread && m_pRecieverSocket)
	{
		std::string recname = clientName;
		recname += "_RECVIER_THREAD_";
		m_pRecieverSocket->Start();
		m_RecieverThread->Start(recname.c_str());
	}
	SendHelloToClient();

}

void XLServerClientHandler::SendHelloToClient()
{
	PutEvent(SERVER_CLIENT_START);
}

void XLServerClientHandler::Stop()
{
	
	if(m_RecieverThread)
	{
		m_RecieverThread->Stop();
	}

	if(XLEventThread::IsCreated())
	{
		XLEventThread::StopMainLoop();
	}

	

	if(m_RecieverThread)
	{
		m_RecieverThread->Stop();
		delete m_RecieverThread;
		m_RecieverThread = 0;		
	}

	if(m_pRecieverSocket)
	{
		delete m_pRecieverSocket;
		m_pRecieverSocket = 0;
	}

}

void XLServerClientHandler::t_OnEvent(const XLThreadEvent* e)
{
	if(!e)
	{
		return;
	}
	int type = e->GetType();
	m_EventPlace = *e;

	switch(type)
	{
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////Response Events///////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	case SERVER_CLIENT_HANDLER_MSG_ON_CONNECT:
		cout<<"SERVER_CLIENT_HANDLER_MSG_ON_CONNECT"<<endl;
		m_EventPlace.SetData((char*)(&type), sizeof(int));
		SocketUtility::Send(m_clientIdentifier, m_EventPlace.GetData(), m_EventPlace.GetDataLength());
		break;
	case SERVER_CLIENT_HANDLER_MSG_ON_DISCONNECT:
		cout<<"SERVER_CLIENT_HANDLER_MSG_ON_DISCONNECT"<<endl;
		m_EventPlace.SetData((char*)(&type), sizeof(int));
		SocketUtility::Send(m_clientIdentifier, m_EventPlace.GetData(), m_EventPlace.GetDataLength());
		break;
	case SERVER_CLIENT_HANDLER_MSG_ON_ALIVE:
		cout<<"SERVER_CLIENT_HANDLER_MSG_ON_ALIVE"<<endl;
		m_EventPlace.SetData((char*)(&type), sizeof(int));
		SocketUtility::Send(m_clientIdentifier, m_EventPlace.GetData(), m_EventPlace.GetDataLength());
		break;
	case SERVER_CLIENT_HANDLER_MSG_ON_RECIEVE:
		cout<<"SERVER_CLIENT_HANDLER_MSG_ON_RECIEVE"<<endl;
		m_EventPlace.SetData((char*)(&type), sizeof(int));
		SocketUtility::Send(m_clientIdentifier, m_EventPlace.GetData(), m_EventPlace.GetDataLength());
		
		break;
	case SERVER_CLIENT_HANDLER_MSG_ON_SEND:
		cout<<"SERVER_CLIENT_HANDLER_MSG_ON_SENDt"<<endl;
		//temporary
		PutEvent(SERVER_CLIENT_HANDLER_MSG_ON_SEND, m_EventPlace.GetDataPtr(), m_EventPlace.GetDataLength());
		m_EventPlace.SetData((char*)(&type), sizeof(int));
		SocketUtility::Send(m_clientIdentifier, m_EventPlace.GetData(), m_EventPlace.GetDataLength());
		break;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////Action Events/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	case SERVER_DATA_RECIEVED:
		PutEvent(SERVER_CLIENT_HANDLER_MSG_ON_RECIEVE, m_EventPlace.GetDataPtr(), m_EventPlace.GetDataLength());
		if(m_RecieveListener)
		{
			m_RecieveListener->OnRecieve(m_EventPlace.GetDataPtr(), m_EventPlace.GetDataLength());
		}
		InterpretDataAndSendEvent(m_EventPlace.GetDataPtr(), m_EventPlace.GetDataLength());
		break;


	case SERVER_CLIENT_START:
		{
			m_EventPlace.Reset();
			m_EventPlace.SetType(MESSAGE_HELLO_FROM_SERVER);
			m_SockConfig.WriteToSend(m_EventPlace.GetDataPtr(), m_EventPlace.GetDataLength());
			SocketUtility::Send(m_clientIdentifier, m_EventPlace.GetData(), m_EventPlace.GetDataLength());
		}
		break;




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////Default Events////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	default:
		cout<<"t_OnOtherEvents"<<endl;
		t_OnOtherEvents(e);
		break;
	}
}


void XLServerClientHandler::OnRecieve(char* msg, int len)
{
	PutEvent(SERVER_DATA_RECIEVED, msg, len);
	
}

void XLServerClientHandler::PutEvent(int type)
{
	XLThreadEvent e;
	e.SetType(type);
	Put(&e);
}

void XLServerClientHandler::PutEvent(int type, char* msg, int len)
{
	XLThreadEvent e;
	e.SetType(type);
	e.SetData(msg, len);
	Put(&e);
}

void XLServerClientHandler::InterpretDataAndSendEvent(char* msg, int len)
{
	DataInterpreterAutoPtr dip = DataInterpreterFactor::GetInterpreter(msg, len, ServerClientComm_DataInterpreterType);
	//switch(dip->GetType())
	{
		//case 
	}
}