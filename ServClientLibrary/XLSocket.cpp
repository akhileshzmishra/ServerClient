
#include "SocketDependency.h"
#include "XLSocket.h"
#include "SocketUtility.h"
#include <iostream>
#include "CommunicationHeader.h"
#include "SocketSession.h"
//using namespace std;

#ifdef __WINDOWS_OS_
#pragma comment(lib,"Ws2_32.lib") 
#endif



//-----------------------------------------------------------------------------------------------------
//-----------------------------------------XLSocket----------------------------------------------------
//-----------------------------------------------------------------------------------------------------

XLSocket::XLSocket(SocketConfig& config, bool IsServerMode):
XLBaseSocket(config, IsServerMode),
m_ListenSocket(INVALID_SOCKET)
{
	m_Session = GetWinsockSession();
}

XLSocket::XLSocket(Socket_Identifier sock):
XLBaseSocket(),
m_ListenSocket(sock)
{
	m_bActive = true;
	m_bServerMode = true;
	m_Session = GetWinsockSession();
}

XLSocket::~XLSocket(void)
{
	OnCleanUp();
}


int XLSocket::TryConnect()
{
	if(m_bServerMode || m_bActive)
	{
		return RESULT_FAIL;
	}

    SOCKADDR_IN target; //Information about host
	memset(&target, 0, sizeof(SOCKADDR_IN));
    
    
    m_ListenSocket = socket (m_Config.GetIPFormat(), m_Config.GetConnectionType(), m_Config.GetIPProtocol()); // Create socket
    if (m_ListenSocket == INVALID_SOCKET)
    {
        return RESULT_NO_SOCKET;
    }
    target.sin_family = m_Config.GetIPFormat();           // address family Internet
    target.sin_port = htons (m_Config.GetPortNumber());        // set server�s port number
    //target.sin_addr.s_addr = m_Config.HostNumber;  // set server�s IP
	target.sin_addr.s_addr = inet_addr(m_Config.GetHostNumber());
     
    //Try connecting...
    if (connect(m_ListenSocket, (SOCKADDR *)&target, sizeof(target)) == SOCKET_ERROR) //Try binding
    { // error
          return RESULT_NO_CONNECT;
    }
	if(m_Config.GetEventListener())
	{
		m_Config.GetEventListener()->OnConnect();
	}
    //WSAAsyncSelect (m_ListenSocket, hwnd, 1045, FD_READ | FD_CONNECT | FD_CLOSE); 
    //Switch to Non-Blocking mode
	m_bActive = true;
    return RESULT_OK; //OK
}



int XLSocket::ListenOnPort()
{
	if(!m_bServerMode || m_bActive)
	{
		return RESULT_FAIL;
	}

	m_ListenSocket = socket (m_Config.GetIPFormat(), m_Config.GetConnectionType(), m_Config.GetIPProtocol()); // Create socket
    
    if (m_ListenSocket == INVALID_SOCKET)
    {
#ifdef _WINDOWS_OS_
		WSACleanup();
#endif
        return RESULT_NO_SOCKET;
    }
    
    SOCKADDR_IN addr; // the address structure for a TCP socket
	memset(&addr, 0, sizeof(SOCKADDR_IN));
   
    addr.sin_family = m_Config.GetIPFormat();           // address family Internet
    addr.sin_port = htons (m_Config.GetPortNumber());        // set server�s port number
    //target.sin_addr.s_addr = m_Config.HostNumber;  // set server�s IP
	addr.sin_addr.s_addr = inet_addr(m_Config.GetHostNumber());

    int retryCount = 0;
    while((bind(m_ListenSocket, (PSOCKADDR)&addr, sizeof(addr)) == SOCKET_ERROR))//Try binding
    { 
		// error
		retryCount++;
		if(retryCount < m_Config.GetConnectionTries())
		{
#ifdef __WINDOWS_OS_
			::Sleep(BIND_SLEEP);
#else
			sleep(BIND_SLEEP/1000);
#endif
			continue;
		}
        return RESULT_NO_BIND;
    }
    
    listen(m_ListenSocket, m_Config.GetNumberofClients()); //Start listening
	if(m_Config.GetEventListener())
	{
		m_Config.GetEventListener()->OnListen();
	}
	else
	{
	}
	m_bActive = true;
    return RESULT_OK;
}

void XLSocket::OnCleanUp()
{
#ifdef __WINDOWS_OS_
	closesocket(m_ListenSocket);
	WSACleanup();
#else
	close(m_ListenSocket);
#endif
	m_bActive = false;
}

void XLSocket::Stop()
{
	OnCleanUp();
}

int XLSocket::Start()
{
	m_Config.Print();
	int ErrorCode = RESULT_OK;
	if(m_bServerMode)
	{
		if((ErrorCode = ListenOnPort()) != RESULT_OK)
		{
			return ErrorCode;
		}
		if(!AcceptConnection())
		{
			return RESULT_FAIL;
		}
		m_bActive = true;
	}
	else
	{
		if((ErrorCode = TryConnect()) != RESULT_OK)
		{
			return ErrorCode;
		}
		m_bActive = true;
	}
	return ErrorCode;
}


bool XLSocket::AcceptConnection()
{
	if(!m_bServerMode)
	{
		return false;
	}


#ifdef __WINDOWS_OS_
	struct sockaddr_in saClient;
	int iClientSize = sizeof(saClient);
	Socket_Identifier AcceptSocket = accept(m_ListenSocket, (SOCKADDR*) &saClient, &iClientSize);
#else
	struct sockaddr saddr;
	socklen_t iClientSize = sizeof(saddr);
	Socket_Identifier AcceptSocket = accept(m_ListenSocket, &saddr, &iClientSize);
#endif

	Socket_Identifier uniqueSockID = AcceptSocket;

	if(m_Config.GetEventListener())
	{
		XLSocket* clientSock = new XLSocket(uniqueSockID);
		clientSock->SetConfig(m_Config);
		clientSock->SetServerMode(m_bServerMode);
		m_Config.GetEventListener()->OnAccept(clientSock);
	}

	return true;

}

SocketUtility::SocketErrCode XLSocket::Send(const char* msg, int msglen) const
{
	SocketUtility::SocketErrCode error = SocketUtility::SocketNoError;
	error = SocketUtility::Send(m_ListenSocket, msg, msglen);
	if(m_Config.GetEventListener() && (error == SocketUtility::SocketNoError))
	{
		m_Config.GetEventListener()->OnSend(msg, msglen);
	}
	else if(m_Config.GetEventListener() && SocketUtility::IsConnected(error))
	{
		m_Config.GetEventListener()->OnDisConnect();
	}
	return error;
	
}



SocketUtility::SocketErrCode XLSocket::Recieve(char* buff, int bufflen) const
{
	SocketUtility::SocketErrCode error = SocketUtility::SocketNoError;
	error = SocketUtility::Recieve(m_ListenSocket, buff, bufflen);
	if(m_Config.GetEventListener() && (error == SocketUtility::SocketNoError))
	{
		m_Config.GetEventListener()->OnRecieve(buff, bufflen);
	}
	else if(m_Config.GetEventListener() && SocketUtility::IsConnected(error))
	{
		m_Config.GetEventListener()->OnDisConnect();
	}
	return error;
}


void XLSocket::SetTimeOut(int timeout)
{
	SocketUtility::SetSocketTimeout(m_ListenSocket, timeout);
}
