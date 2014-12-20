#ifndef _XL_SOCKET___
#define _XL_SOCKET___


#include <windows.h>
#include <windowsx.h>
#include <winsock.h>
#include <vector>
#include <set>
#include <memory.h>
#include "SocketUtility.h"
#include "XLBaseSocket.h"


class XLSocket: public XLBaseSocket
{
	SOCKET                          m_ListenSocket;
public:
	enum 
	{
		RESULT_OK = XLBaseSocket::RESULT_OK,
		RESULT_NO_STARTUP = XLBaseSocket::RESULT_NO_STARTUP,
		RESULT_NO_SOCKET = XLBaseSocket::RESULT_NO_SOCKET,
		RESULT_NO_CONNECT = XLBaseSocket::RESULT_NO_CONNECT,
		RESULT_NO_BIND = XLBaseSocket::RESULT_NO_BIND,
		RESULT_WRONG_VERSION = XLBaseSocket::RESULT_WRONG_VERSION,
		RESULT_FAIL = XLBaseSocket::RESULT_FAIL,
	};
	XLSocket(SocketConfig& config, bool IsServerMode = true);
	virtual ~XLSocket(void);
	int Start();

	void Stop();

	SocketUtility::SocketErrCode Send(const char* msg, int msglen) const; // for client
	SocketUtility::SocketErrCode Recieve(char* buff, int bufflen) const;

	void SetTimeOut(int timeout);
	bool AcceptConnection();

private:
	XLSocket(int sock);
	int PASCAL TryConnect();
	int PASCAL ListenOnPort();
	void OnCleanUp();
};


#endif