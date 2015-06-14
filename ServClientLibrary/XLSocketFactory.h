#ifndef XLSOCKET_FACTORY_H_
#define XLSOCKET_FACTORY_H_
#include "XLAutoPtr.h"
#include "XLBaseSocket.h"
#include "SocketSession.h"


class XLSocketFactory;

enum SocketType
{
	SST_Normalsock = 0,
	SST_OpenSSL = 1
};

typedef XLAutoPtr::XLSharedSmPtr<XLSocketFactory>  SOCKET_FACTORY;
class XLSocketFactory
{
private:
	XLSocketFactory(void);
	XLSocketFactory(const XLSocketFactory&);
	XLSocketFactory& operator = (const XLSocketFactory&);
	bool                               m_bSSLSocket;
	static SOCKET_FACTORY              m_Instance;
public:
	static SOCKET_FACTORY GetInstance();

	XLBaseSocket* GetSocket(SocketType type, SocketConfig& config, bool IsServerMode = true);

	XLBaseSocket* GetDefaultSocket(SocketConfig& config, bool IsServerMode = true);

	void SetSecureSocketSetting(bool set)                       {m_bSSLSocket = set;}
	bool GetSecureSocketSetting()                               {return m_bSSLSocket;}

	void StartSession();
	void StopSession();

	~XLSocketFactory(void);
};



XLBaseSocket* GetNormalSocket(SocketConfig& config, bool IsServerMode = true);

XLBaseSocket* GetSSLSocket(SocketConfig& config, bool IsServerMode = true);



#endif