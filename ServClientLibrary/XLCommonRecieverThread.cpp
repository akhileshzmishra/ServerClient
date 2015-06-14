#include "XLCommonRecieverThread.h"
#include "SocketUtility.h"

XLCommonRecieverThread::XLCommonRecieverThread(const int socket):
m_SockIdentifier(socket),
m_Listener(0)
{
}

XLCommonRecieverThread::~XLCommonRecieverThread(void)
{
}

void XLCommonRecieverThread::Start(const char* name)
{
	if(XLEventThread::Create(name))
	{
		SocketUtility::SetSocketTimeout(m_SockIdentifier, 2000);
	}
}
void XLCommonRecieverThread::Stop()
{
	if(XLEventThread::IsCreated())
	{
		XLEventThread::StopMainLoop();
	}
}
void XLCommonRecieverThread::t_OnEvent(const XLThreadEvent* e)
{
	SocketUtility::Recieve(m_SockIdentifier, m_Event.GetDataPtr(), m_Event.GetDataLength());
	if(m_Listener)
	{
		m_Listener->OnRecieve(m_Event.GetDataPtr(), m_Event.GetDataLength());
	}
}