#ifndef ____XLBASESOCK_H___
#define ____XLBASESOCK_H___


#include <string>
#include <vector>
#include <set>
#include <memory.h>
#include "SocketUtility.h"
#include "XLAutoPtr.h"
#include "CommunicationHeader.h"
#include "SocketSession.h"


typedef XLAutoPtr::XLSharedSmPtr<class XLBaseSocket> BASE_SOCKET_PTR;
class XLBaseSocket;

class XLSocketListener
{
public:
	XLSocketListener()                                  {}
	virtual ~XLSocketListener()                         {}
	virtual void OnConnect()                            {}
	virtual void OnDisConnect()                         {}
	virtual void OnListen()                             {}
	virtual void OnAccept(BASE_SOCKET_PTR clientSock)   {}
	virtual void OnSend(const char* msg, int size)      {}
	virtual void OnRecieve(char* msg, int size)         {}
};


class SocketConfig
{
private:
	std::string                 m_HostNumber;
	int                         m_PortNumber;	
	int                         m_ConnectionType;
	int                         m_IPFormat;
	int                         m_IPProtocol;
	int                         m_ConnectionTries;
	int                         m_NumberofClients;
	mutable XLSocketListener*   m_EventListener;

public:
	enum Operation
	{
		CONNECT,
		DISCONNECT,
		SEND,
		RECIEVE
	};


	SocketConfig();
	SocketConfig(char* valuesFromBuff, int bufflen);
	SocketConfig(const SocketConfig& another);
	SocketConfig& operator = (const SocketConfig& another);
	SocketConfig(const char* hostNumber);
	~SocketConfig();
	
	const char* GetHostNumber()                                     {return m_HostNumber.c_str();}
	std::string GetHostWithPort();
	int GetPortNumber()const                                        {return m_PortNumber;}
	int GetConnectionType()  const                                  {return m_ConnectionType;}
	int GetIPFormat()   const                                       {return m_IPFormat;}
	int GetIPProtocol()    const                                    {return m_IPProtocol;}
	int GetConnectionTries()  const                                 {return m_ConnectionTries;}
	int GetNumberofClients() const                                  {return m_NumberofClients;}
	XLSocketListener* GetEventListener()                            {return m_EventListener;}

	void SetHostNumber(const char* hostnumber);
	void SetHostWithPort(const char* hostport);
	void SetPortNumber(int Value)                                   {m_PortNumber = Value;}
	void SetConnectionType(int Value)                               {m_ConnectionType = Value;}
	void SetIPFormat(int Value)                                     {m_IPFormat = Value;}
	void SetIPProtocol(int Value)                                   {m_IPProtocol = Value;}
	void SetConnectionTries(int Value)                              {m_ConnectionTries = Value;}
	void SetNumberofClients(int Value)                              {m_NumberofClients = Value;}
	void SetEventListener(XLSocketListener* Value)                  {m_EventListener = Value;}

	void WriteToSend(char* buffer, int length);

	static const int GetTotalWriteLength();

	void Print();

	
};


class XLBaseSocket
{
protected:
	bool                            m_bServerMode;
	mutable SocketConfig            m_Config;
	bool                            m_bActive;
	SESSION_PTR                     m_Session;
public:
	enum 
	{
		RESULT_OK,
		RESULT_NO_STARTUP,
		RESULT_NO_SOCKET,
		RESULT_NO_CONNECT,
		RESULT_NO_BIND,
		RESULT_WRONG_VERSION,
		RESULT_FAIL,
	};
	XLBaseSocket(SocketConfig& config, bool IsServerMode = true);
	
	virtual ~XLBaseSocket(void);
	virtual int Start() = 0;

	virtual void Stop() = 0;

	virtual SocketUtility::SocketErrCode Send(const char* msg, int msglen) const = 0;
	virtual SocketUtility::SocketErrCode Recieve(char* buff, int bufflen) const = 0;

	virtual void SetTimeOut(int timeout) = 0;

	virtual bool AcceptConnection() = 0;

	SocketConfig* GetConfig()                            {return &m_Config;}
	bool IsActive()    const                             {return m_bActive;}
	
protected:
	XLBaseSocket();
	void SetServerMode(bool set)                         {m_bServerMode = set;}
	void SetConfig(SocketConfig& conf)                   {m_Config = conf;}
	bool GetMode()                                       {return m_bServerMode;}

};









#endif
