#ifndef _XLACCEPTOR_THREAD_H_
#define _XLACCEPTOR_THREAD_H_
#include "XLSimpleThreads.h"
#include "XLBaseSocket.h"
#include "XLSemaPhore.h"
#include "XLEvents.h"




class XLAcceptorThread :	private XLSimpleThreads
{
	BASE_SOCKET_PTR                      m_pSocket;
	bool                                 m_bExit;
	XLSemaPhore                          m_ExitKeeper;
	int                                  m_iNConnection;
public:
	XLAcceptorThread(XLBaseSocket* socket, int numberAcception);
	virtual ~XLAcceptorThread(void);

	void Start();
	void Stop();

private:

	void RunProgram() ;
	void RanOut();


};


#endif