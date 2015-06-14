#ifndef _XLSERVINGTHREAD_H_
#define _XLSERVINGTHREAD_H_
#include "XLBaseSocket.h"
#include "XLEventThread.h"
#include "XLListeningThread.h"
#include "XLAcceptorThread.h"
#include <set>
#include "XLAlarm.h"
#include "LockFreeQueue.h"


class ServingThreadListener
{
public:
	virtual void OnStart(class XLServingThread* ) = 0;
	virtual void OnEnd(class XLServingThread* ) = 0;
	virtual void OnAccept(class XLServingThread*, BASE_SOCKET_PTR) = 0;
	virtual void OnEvent(const class XLThreadEvent* e) = 0;
	virtual void OnSent(const class XLThreadEvent* e) = 0;
};



class XLServingThread: private XLEventThread, public XLSocketListener, public AlarmListener, public AlarmData
{
	BASE_SOCKET_PTR                                   m_pSocket;
	std::string                                       m_Name;
	typedef std::set<BASE_SOCKET_PTR>                 CLIST ;
	typedef CLIST::iterator                           CLITR;
	CLIST                                             m_ClientIDs;
	XLAlarm                                           m_AliveAlarm;
	SocketConfig                                      m_SockConfig;
	ServingThreadListener*                            m_Listener;
	bool                                              m_bToPingClient;
	XLAcceptorThread*                                 m_AcceptorThread;
	int                                               m_iMappingType;
	XDS::XLLFQ<BASE_SOCKET_PTR>                       m_IncomingQueue;
public:
	

	enum RecieverMappingType
	{
		RMT_OneToAll,
		RMT_OneToOne,
		RMT_None
	};
	XLServingThread(SocketConfig& config, char* threadname);
	~XLServingThread(void);

//USE THESE FUNCTIONS TO SET PROPERTY BEFORE CALLING Start FUNCTION
////////////////////////////////////////////////////////////////////
	void SetListener(ServingThreadListener* listener)
	{
		m_Listener = listener;
	}
	void SetPingAlive(bool set)
	{
		m_bToPingClient = set;
	}
	void SetRecieverMappingType(RecieverMappingType type)
	{
		m_iMappingType = (int)type;
	}
/////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
	void Start();
	void Stop();

	void Send(int id, const char* msg, int len);
	void Broadcast(const char* msg, int len);

protected:
	enum
	{
		START_THREAD = XLEventThreadEventType_Max + 1,
		ACCEPT_INCOMING,
		LIFE_BEAT,
		USR_MSG_BDCST,
		USR_MSG,

		MAX_EVENTS
	};

private:

	//XLSocketListener
	virtual void OnConnect();
	virtual void OnDisConnect();
	virtual void OnListen();
	virtual void OnAccept(BASE_SOCKET_PTR uniqueSockID);
	virtual void OnSend(const char* msg, int size);

	//AlarmListener
	virtual void OnAlarm(class XLAlarm* alarm , class AlarmData* pData);

	//XLEventThread
	virtual void t_OnEvent(const XLThreadEvent* e);
	virtual void t_OnCreate();
	virtual void t_OnDestroy();
	void StartAccepting();
	void PrintType(int type);

protected:
	void SendSelfEvent(const XLThreadEvent* e);
};



#endif