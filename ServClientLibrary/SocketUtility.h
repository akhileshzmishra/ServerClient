#ifndef SOCKET_UTILIYY__H_
#define SOCKET_UTILIYY__H_

#include "SocketDependency.h"

class SocketUtility
{
public:
	enum SocketErrCode
	{
		SocketNoError = 0,
		InvalidSocket,
		InvalidParameter,
		InvalidOperation,
		InvalidProtocol,
		MemoryError,
		RemoteHostNotFound,
		NoConnection,
		AlreadyConnected,
		NoNetwork,
		OtherError,

		SocketErrCodeMax
	};
	SocketUtility(void);

	static SocketErrCode Send(Socket_Identifier socket, const char* msg, int msglen);
	static SocketErrCode Recieve(Socket_Identifier socket, char* msg, int msglen);
	static SocketErrCode SetSocketTimeout(Socket_Identifier socket, int timeoutmill);

	static SocketErrCode GetErrorCode(int type);

	static bool IsConnected(SocketErrCode type);
	~SocketUtility(void);
};



#endif
