#ifndef _XL_SERVER_EVENT_HANDLER_THREAD_H_
#define _XL_SERVER_EVENT_HANDLER_THREAD_H_


#include "XLEventThread.h"
#include "XLCommonRecieverThread.h"
#include "XLSocket.h"
#include "XLClientServerMessages.h"



class XLServerClientHandler : private XLEventThread, public RecieveListener
{
	const int                   m_clientIdentifier;
	XLThreadEvent               m_EventPlace;
	XLCommonRecieverThread*     m_RecieverThread;
	RecieveListener*            m_RecieveListener;
	XLSocket*                   m_pRecieverSocket;
	SocketConfig                m_SockConfig;
public:
	

	XLServerClientHandler(const int identifier, SocketConfig& config);
	~XLServerClientHandler(void);

	void Start(const char* clientName);
	void Stop();


	//RecieveListener
	void OnRecieve(char* msg, int len);



	void SetListener(RecieveListener* listener)
	{
		m_RecieveListener = listener;
	}

private:
	void t_OnEvent(const XLThreadEvent* e);
	virtual void t_OnOtherEvents(const XLThreadEvent* e){}

	void InterpretDataAndSendEvent(char* msg, int len);

protected:
	void PutEvent(int type);
	void PutEvent(int type, char* msg, int len);
	void SendHelloToClient();
};












#endif //_XL_SERVER_EVENT_HANDLER_THREAD_H_