#ifndef SOCKET_UTILIYY__H_
#define SOCKET_UTILIYY__H_


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

	static SocketErrCode Send(int socket, const char* msg, int msglen);
	static SocketErrCode Recieve(int socket, char* msg, int msglen);
	static SocketErrCode SetSocketTimeout(int socket, int timeoutmill);

	static SocketErrCode GetErrorCode(int type);

	static bool IsConnected(SocketErrCode type);
	~SocketUtility(void);
};



#endif