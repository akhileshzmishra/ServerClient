#include "XLSocketFactory.h"
#include "XLSocket.h"
#include "XLSSLSocket.h"
#include "SocketSession.h"

SOCKET_FACTORY  XLSocketFactory::m_Instance = 0;


XLSocketFactory::XLSocketFactory(void):
m_bSSLSocket(false)
{
}

XLSocketFactory::~XLSocketFactory(void)
{
}


SOCKET_FACTORY XLSocketFactory::GetInstance()
{
	if(!m_Instance)
	{
		m_Instance = new XLSocketFactory();
	}

	return m_Instance;
}


XLBaseSocket* XLSocketFactory::GetDefaultSocket(SocketConfig& config, bool IsServerMode)
{
	if(m_bSSLSocket)
	{
		return GetSocket(SST_OpenSSL, config, IsServerMode);
	}

	return GetSocket(SST_Normalsock, config, IsServerMode);
}


XLBaseSocket* XLSocketFactory::GetSocket(SocketType type, SocketConfig& config, bool IsServerMode)
{
	XLBaseSocket* socket = 0;
	switch(type)
	{
	case SST_Normalsock:
		{
			GetWinsockSession();
			socket = new XLSocket(config, IsServerMode);
		}
		break;
#ifdef _OPENSSL_ACTIVE_
	case SST_OpenSSL:
		{
			GetWinsockSession();
			GetOpenSSLSession();
			socket = new XLSSLSocket(config, IsServerMode);
		}
		break;
#endif
	default:
		break;
	}

	return socket;
}

void XLSocketFactory::StartSession()
{
	if(!m_bSSLSocket)
	{
		GetWinsockSession()->Start();;
	}
	else
	{
		GetWinsockSession()->Start();
		GetOpenSSLSession()->Start();
	}
}


void XLSocketFactory::StopSession()
{
	if(!m_bSSLSocket)
	{
		GetWinsockSession()->Stop();;
	}
	else
	{
		GetWinsockSession()->Stop();
		GetOpenSSLSession()->Stop();
	}
}


XLBaseSocket* GetNormalSocket(SocketConfig& config, bool IsServerMode)
{
	return XLSocketFactory::GetInstance()->GetSocket(SST_Normalsock, config, IsServerMode);
}

XLBaseSocket* GetSSLSocket(SocketConfig& config, bool IsServerMode)
{
	return XLSocketFactory::GetInstance()->GetSocket(SST_OpenSSL, config, IsServerMode);
}


