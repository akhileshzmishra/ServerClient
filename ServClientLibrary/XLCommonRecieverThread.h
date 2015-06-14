#ifndef _XL_COMMONRECIEVERTHREAD_H_
#define _XL_COMMONRECIEVERTHREAD_H_
#include "XLEventThread.h"
#include "XLSocket.h"


class RecieveListener
{
public:
	virtual void OnRecieve(char* msg, int len) = 0;
};

class XLCommonRecieverThread: private XLEventThread
{
	const int                     m_SockIdentifier;
	RecieveListener*              m_Listener;
	XLThreadEvent                 m_Event;
public:
	XLCommonRecieverThread(const int socket);
	~XLCommonRecieverThread(void);
	void Start(const char* name);
	void Stop();
	void SetListener(RecieveListener* listener)
	{
		m_Listener = listener;
	}
private:
	void t_OnEvent(const XLThreadEvent* e);

};



#endif