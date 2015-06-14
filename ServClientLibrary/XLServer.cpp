#include "XLServer.h"
#include "XLSemaphore.h"
#include "XLServingThread.h"
#include "XLSocketFactory.h"
using namespace XLServerClientModel;



//////////////////////////++++++++++++++++++//////////////////////////////////////////////////////////////////////////////
//////////////////////////+ CLASS XLCService +/////////////////////////////////////////////////////////////////////////////
//////////////////////////++++++++++++++++++//////////////////////////////////////////////////////////////////////////////
namespace XLServerClientModel
{
class XLCService
{
	XLServingThread*           Service;
	XLSemaPhore                Gate;
public:
	XLCService(XLServingThread* service = 0):
	Service(service)
	{
		Gate.Create(1,1);
	}

	~XLCService()
	{
		if(Service)
		{
			Service->Stop();
		}
		delete Service;
		Gate.Destroy();
	}


	XLServingThread*& GetService()                   {return Service;}
	XLSemaPhore& GetGate()                           {return Gate;}
};


}// namespace XLServerClientModel

//////////////////////////++++++++++++++++++/////////////////////////////////////////////////////////////////////////////
//////////////////////////++CLASS XLServer++/////////////////////////////////////////////////////////////////////////////
//////////////////////////++++++++++++++++++/////////////////////////////////////////////////////////////////////////////
XLServer::XLServer(int maxServices):
m_iMaxServices(maxServices)
{
}

XLServer::~XLServer(void)
{
	SERVTITR itr = m_ClientServices.begin();
	while(itr != m_ClientServices.end())
	{
		itr->second->GetGate().Take();
		itr++;
	}
	Stop();
}

void XLServer::Start()
{
	GetSettingLibrary()->StartSocketLibrary();
}


void XLServer::Stop()
{
	GetSettingLibrary()->StopSocketLibrary();
}

int XLServer::StartAService(SocketConfig& coinfig)
{
	XLServingThread* t = new XLServingThread(coinfig,  "HANDLER_TH");
	t->SetListener(this);
	
	XLCService* newService = new XLCService(t);
	m_ClientServices.insert(SERVTPAIR((int)t, newService));
	t->Start();
	newService->GetGate().Take();

	return (int) t;
}

void XLServer::StopAService(int id)
{
	SERVTITR itr = m_ClientServices.find(id);
	if(itr != m_ClientServices.end())
	{
		itr->second->GetService()->Stop();
		delete itr->second;
		itr->second->GetGate().Give();
	}
	
}

void XLServer::Broadcast(const char* msg, int msglen, int serviceID)
{
	SERVTITR itr = m_ClientServices.find(serviceID);
	if(itr != m_ClientServices.end())
	{
		itr->second->GetService()->Broadcast(msg, msglen);
	}
}


void XLServer::Broadcast(const char* msg, int msglen)
{
	SERVTITR itr = m_ClientServices.begin();
	while(itr != m_ClientServices.end())
	{
		itr->second->GetService()->Broadcast(msg, msglen);
		itr++;
	}
}


void XLServer::OnStart(class XLServingThread* )
{
}

void XLServer::OnEnd(class XLServingThread* )
{
}

void XLServer::OnAccept(class XLServingThread*, BASE_SOCKET_PTR)
{
}


void XLServer::OnEvent(const class XLThreadEvent* e)
{
	if(e)
	{
		e->Print();
	}
}
void XLServer::OnSent(const XLThreadEvent* e)
{
	if(e)
	{
		e->Print();
	}
}

LIB_OBJECT XLServer::GetSettingLibrary()
{
	return CLibraryInterface::GetInstance();
}
