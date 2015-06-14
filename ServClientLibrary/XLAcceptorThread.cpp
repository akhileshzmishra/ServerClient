#include "XLAcceptorThread.h"

XLAcceptorThread::XLAcceptorThread(XLBaseSocket* socket, int num):
m_pSocket(socket),
m_bExit(false),
m_iNConnection(num)
{
}

XLAcceptorThread::~XLAcceptorThread(void)
{
	Stop();
}



void XLAcceptorThread::Start()
{
	if(!IsCreated())
	{
		m_bExit = false;
		XLSimpleThreads::Create("ACCEPTORTHREAD");
	}
}


void XLAcceptorThread::Stop()
{
	if(IsCreated())
	{
		m_bExit = true;
		XLSimpleThreads::Destroy();
	}
}




void XLAcceptorThread::RunProgram()
{
	if(!m_pSocket)
	{
		return;	
	}
	m_pSocket->Start();
	int itr  = 1;
	while((!m_bExit) && (itr < m_iNConnection))
	{
		if(!m_pSocket->AcceptConnection())
		{
			m_bExit = true;
		}
		itr++;
	}	

}


void XLAcceptorThread::RanOut()
{
}

