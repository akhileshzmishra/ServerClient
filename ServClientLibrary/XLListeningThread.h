#ifndef _XL_LISTENING_THREADS_H_
#define _XL_LISTENING_THREADS_H_
#include "XLSimpleThreads.h"
#include "XLSocket.h"
#include "XLSemaPhore.h"
#include "XLEvents.h"
#include "CommunicationHeader.h"

class XLListenThreadListener
{
public:
	virtual void OnListen(const class XLThreadEvent* e) = 0;
};

class XLListeningThread :	private XLSimpleThreads, public XLSocketListener
{
	BASE_SOCKET_PTR                            m_pSocket;
	bool                                       m_bExit;
	XLSemaPhore                                m_ExitKeeper;
	XLThreadEvent                              m_Event;
	XLListenThreadListener*                    m_Listener;
	SocketConfig                               m_SocketConfig;
	CHARSTORAGE                                m_DataStorage;
	bool                                       m_bOwnership;
public:
	XLListeningThread(SocketConfig config);
	XLListeningThread(XLBaseSocket* socket);
	virtual ~XLListeningThread(void);

	void Start();
	void Stop();
	

	void SetListener(XLListenThreadListener* s)
	{
		m_Listener = s;
	}

	void OnRecieve(char* msg, int size);
private:
	void RunProgram();
	void RanOut();
};






#endif