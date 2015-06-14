#ifndef _XLClientThread_H_
#define _XLClientThread_H_
#include "XLEventThread.h"
#include "XLListeningThread.h"


class XLClientThreadListener
{
public:
	virtual void OnStart() = 0;
	virtual void OnEnd() = 0;
	virtual void OnEvent(const XLThreadEvent* e) = 0;
};


class XLClientThread: private XLEventThread, public XLListenThreadListener
{
	SocketConfig                m_SocketConfig;
	XLListeningThread           m_DataListenTh;
	XLClientThreadListener*     m_pListener;
public:
	enum
	{
		START_THREAD = XLEventThreadEventType_Max + 1,
		ACCEPT_INCOMING, // Not used
		LIFE_BEAT,
		USR_MSG_BDCST,
		USR_MSG,

		MAX_EVENTS
	};
		
	XLClientThread(SocketConfig& config);
	virtual ~XLClientThread(void);

	void Start();
	void Stop();

	void SetListener(XLClientThreadListener* listener)
	{
		m_pListener = listener;
	}

	
private:
	//XLEventThread
	virtual void t_OnEvent(const XLThreadEvent* e);
	
	//XLListenThreadListener
	virtual void OnListen(const XLThreadEvent* e);

	void PrintType(int type);
protected:
	void SendSelfEvent(const XLThreadEvent* e);
};




#endif