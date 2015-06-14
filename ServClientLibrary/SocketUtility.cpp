#include "SocketUtility.h"
#include <windows.h>
//#include <windowsx.h>
#include <winsock.h>
#include <iostream>
using namespace std;
#pragma comment(lib,"Ws2_32.lib") 

SocketUtility::SocketUtility(void)
{
}

SocketUtility::~SocketUtility(void)
{
}


SocketUtility::SocketErrCode SocketUtility::Send(int socket, const char* msg, int msglen)
{
	SocketUtility::SocketErrCode error = SocketNoError;
	int numbytes = send((SOCKET)socket, msg, msglen, 0);
	cout<<__FUNCTION__<<" "<<numbytes<<endl;
	if(numbytes == SOCKET_ERROR)
	{
		cout<<"SOCKET ERROR"<<endl;
		int type = WSAGetLastError();
		cout<<type<<endl;
		error = GetErrorCode(type);
	}


	return error;
}
SocketUtility::SocketErrCode SocketUtility::Recieve(int socket, char* msg, int msglen)
{
	SocketUtility::SocketErrCode error = SocketNoError;
	int numbytes = recv((SOCKET)(socket), msg, msglen, 0);

	cout<<__FUNCTION__<<" "<<numbytes<<endl;
	if(numbytes == SOCKET_ERROR)
	{
		cout<<"SOCKET ERROR"<<endl;
		int type = WSAGetLastError();
		cout<<type<<endl;
		error = GetErrorCode(type);
	}

	return error;
}

SocketUtility::SocketErrCode SocketUtility::SetSocketTimeout(int socket, int timeoutmill)
{
	SocketUtility::SocketErrCode error = SocketNoError;
	setsockopt((SOCKET)socket, SOL_SOCKET, SO_KEEPALIVE, (const char*) (&timeoutmill), sizeof(int));

	return error;
}

SocketUtility::SocketErrCode SocketUtility::GetErrorCode(int type)
{
	SocketUtility::SocketErrCode code = SocketNoError;
#ifdef _WINDOWS_
	switch(type)
	{
		/* socket invalid */
#ifdef WINVERSION_
	case WSA_INVALID_HANDLE:
#endif
	case WSAESOCKTNOSUPPORT:
		code = InvalidSocket;
		break;

		/*Memory */
#ifdef WINVERSION_
	case WSA_NOT_ENOUGH_MEMORY:
#endif
	case WSAENOBUFS:
	case WSAEMFILE:
		code = MemoryError;
		break;

		/*Not valid parameters*/
#ifdef WINVERSION_
	case WSA_INVALID_PARAMETER:
#endif
	case WSAEINVAL:
	case WSAEDESTADDRREQ:
	case WSAEMSGSIZE:
		code = InvalidParameter;
		break;

		/* Not used */
#ifdef WINVERSION_
	case WSA_OPERATION_ABORTED:
	case WSA_IO_INCOMPLETE:
	case WSA_IO_PENDING:
#endif	
	case WSAEFAULT:
	case WSAEWOULDBLOCK:
	case WSAEINPROGRESS:
	case WSAEALREADY:
	case WSAETOOMANYREFS:
	case WSAELOOP:
	case WSAENAMETOOLONG:
	case WSAVERNOTSUPPORTED:
	case WSANOTINITIALISED:
		code = OtherError;
		break;

		/* not valid operation */
	case WSAENOTSOCK:
		code = InvalidOperation;
		break;

		/* protocol */
	case WSAEPROTOTYPE:
	case WSAENOPROTOOPT:
	case WSAEPROTONOSUPPORT:
	case WSAEPFNOSUPPORT:
		code = InvalidProtocol;
		break;


		/*Network and connection */
	case WSAEADDRINUSE:
	case WSAEADDRNOTAVAIL:
	case WSAENETUNREACH:
	case WSAENETRESET:
	case WSAECONNABORTED:
	case WSAECONNRESET:
	case WSAENOTCONN:
	case WSAESHUTDOWN:
	case WSAETIMEDOUT:
	case WSAECONNREFUSED:
	case WSASYSNOTREADY:
		code = NoConnection;
		break;
		

		/* Already connected*/
	case WSAEISCONN:
		code = AlreadyConnected;
		break;

		/* Host error */
	case WSAEHOSTDOWN:
	case WSAEHOSTUNREACH:
		code = RemoteHostNotFound;
		break;

	default:
		code = SocketNoError;
		break;
	}
#endif
	return code;
}

bool SocketUtility::IsConnected(SocketErrCode type)
{
	bool bRet = true;
	switch(type)
	{
	case RemoteHostNotFound:
	case NoConnection:
	case NoNetwork:
		break;
	default:
		break;
	}

	return bRet;
}