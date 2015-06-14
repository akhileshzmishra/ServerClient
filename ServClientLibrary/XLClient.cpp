#include "XLClient.h"
#include "XLSocketFactory.h"
#include <iostream>
using namespace std;
using namespace XLServerClientModel;

XLClient::XLClient(SocketConfig config):
m_Client(0),
m_Config(config)
{
	m_Gatekeeper.Create(1, 1);
}

XLClient::~XLClient(void)
{
	m_Gatekeeper.Take();
	m_Gatekeeper.Destroy();
}



void XLClient::Start()
{	
	if(!m_Client)
	{
		GetSettingLibrary()->StartSocketLibrary();
		m_Client = new XLClientThread(m_Config);
		m_Client->SetListener(this);
		m_Client->Start();
	}
}


void XLClient::Stop()
{
	if(m_Client)
	{
		GetSettingLibrary()->StopSocketLibrary();
		m_Client->Stop();
		delete m_Client;
		m_Client = 0;
	}
}

void XLClient::OnStart()
{
	m_Gatekeeper.Take();
}


void XLClient::OnEnd()
{
	m_Gatekeeper.Give();
}


void XLClient::OnEvent(const XLThreadEvent* e)
{
	cout<<__FUNCTION__<<endl;
	if(e)
	{
		e->Print();
	}
}



LIB_OBJECT XLClient::GetSettingLibrary()
{
	return CLibraryInterface::GetInstance();
}