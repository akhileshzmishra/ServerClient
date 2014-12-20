#include "LibraryInterface.h"
#include "SocketSession.h"
#include "XLSocketFactory.h"


LIB_OBJECT CLibraryInterface::m_Instance = 0;



CLibraryInterface::CLibraryInterface(void)
{
}

CLibraryInterface::~CLibraryInterface(void)
{
}

LIB_OBJECT CLibraryInterface::GetInstance()
{
	if(!m_Instance)
	{
		m_Instance = new  CLibraryInterface();
	}
	return m_Instance;
}


void CLibraryInterface::SetSSLCertificates(std::string& device_cert, std::string& private_key, std::string& CA_file, std::string& pkeypassphrase)
{
	OpenSSLSession* session = dynamic_cast<OpenSSLSession*>((SocketSession*)GetOpenSSLSession());
	if(session)
	{
		session->SetCAFile(CA_file);
		session->SetCertificate(device_cert);
		session->SetPrivateKey(private_key);
		session->SetPrivateKeyPassword(pkeypassphrase);
	}
}

void CLibraryInterface::SetCipherList(std::string& cipher)
{
	OpenSSLSession* session = dynamic_cast<OpenSSLSession*>((SocketSession*)GetOpenSSLSession());
	if(session)
	{
		session->SetCipherList(cipher);
	}
}

void CLibraryInterface::SetUseSSL(bool set)
{
	XLSocketFactory::GetInstance()->SetSecureSocketSetting(set);
}

void CLibraryInterface::SetLocalVerification(bool set)
{
	OpenSSLSession* session = dynamic_cast<OpenSSLSession*>((SocketSession*)GetOpenSSLSession());
	if(session)
	{
		//certification would happen remotely. This setting enables to load and send certificate on demand
		session->SetRemoteCertification(set);
	}
}
void CLibraryInterface::SetRemoteVerification(bool set)
{
	OpenSSLSession* session = dynamic_cast<OpenSSLSession*>((SocketSession*)GetOpenSSLSession());
	if(session)
	{
		//certification would happen locally. This setting enables to load and send certificate on demand
		session->SetLocalCertification(set);
	}
}

void CLibraryInterface::StartSocketLibrary()
{
	XLSocketFactory::GetInstance()->StartSession();
}

void CLibraryInterface::StopSocketLibrary()
{
	XLSocketFactory::GetInstance()->StopSession();
}