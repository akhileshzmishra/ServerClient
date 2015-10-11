#ifdef _OPENSSL_ACTIVE_
#include "SocketDependency.h"
#include "XLSSLSocket.h"
#include "CommunicationHeader.h"
#include "SocketSession.h"
#include "bio.h"
#include "ssl.h"
#include "err.h"
#include <iostream>
using namespace std;

XLSSLSocket::XLSSLSocket(SocketConfig& config, bool IsServerMode):
XLBaseSocket(config, IsServerMode),
m_ConnectionCtx(0),
m_pSSLBio(0),
m_SecureStruct(0),
m_bSecure(true),
m_bConnected(false),
m_Socket(INVALID_SOCKET)
{
	try
	{
		m_Session = GetOpenSSLSession();
		OpenSSLSession* sslSession = dynamic_cast<OpenSSLSession*>(m_Session.GetData());
		if(sslSession)
		{
			m_ConnectionCtx = sslSession->GetContext();	
		}
	}
	catch(...)
	{
	}

}

XLSSLSocket::XLSSLSocket(Socket_Identifier sock, SocketConfig& config):
XLBaseSocket(config, true),
m_ConnectionCtx(0),
m_pSSLBio(0),
m_SecureStruct(0),
m_bSecure(true),
m_bConnected(false),
m_Socket(sock)
{
	try
	{
		m_Session = GetOpenSSLSession();
		OpenSSLSession* sslSession = dynamic_cast<OpenSSLSession*>(m_Session.GetData());
		if(sslSession && (m_Socket != INVALID_SOCKET))
		{
			m_ConnectionCtx = sslSession->GetContext();
			if(m_ConnectionCtx)
			{
				m_SecureStruct = SSL_new(m_ConnectionCtx);
			}
			if(m_SecureStruct)
			{
				 SSL_set_fd(m_SecureStruct, m_Socket);
				 //m_pSSLBio = BIO_new(BIO_s_socket());
				 if(m_pSSLBio)
				 {
					 BIO_set_fd(m_pSSLBio, m_Socket, BIO_NOCLOSE);
					 SSL_set_bio(m_SecureStruct, m_pSSLBio, m_pSSLBio);
				 }
				 
				 int error = SSL_accept(m_SecureStruct);
				 if(error > 0)
				 {
					 SetConnected(true);
					 m_bActive = true;
				 }
				 else
				 {
					 ERR_print_errors_fp(stderr);
					 SetConnected(false);
					 m_bActive = false;
				 }
			}			

		}
	}
	catch(...)
	{
		SetConnected(false);
		m_bActive = false;
	}
}


XLSSLSocket::~XLSSLSocket(void)
{
	Stop();
}



int XLSSLSocket::Start()
{
	if(!m_bServerMode)
	{
		return StartAsClient();
	}
	else
	{
		return StartAsServer();
	}
	return RESULT_OK;
}



bool XLSSLSocket::CreateSocket()
{
	GetWinsockSession();
	m_Socket = socket (m_Config.GetIPFormat(), m_Config.GetConnectionType(), m_Config.GetIPProtocol()); // Create socket
    if (m_Socket == INVALID_SOCKET)
    {
        return false;
    }
	return true;
}




int XLSSLSocket::StartAsServer()
{
	OpenSSLSession* sslSession = dynamic_cast<OpenSSLSession*>(m_Session.GetData());
	if(sslSession)
	{
		if(!CreateSocket())
		{
			return RESULT_NO_SOCKET;
		}

		SOCKADDR_IN addr; // the address structure for a TCP socket
		memset(&addr, 0, sizeof(SOCKADDR_IN));
	   
		addr.sin_family = m_Config.GetIPFormat();           // address family Internet
		addr.sin_port = htons (m_Config.GetPortNumber());        // set server�s port number
		//target.sin_addr.s_addr = m_Config.HostNumber;  // set server�s IP
		addr.sin_addr.s_addr = inet_addr(m_Config.GetHostNumber());

		int retryCount = 0;
#ifdef __WINDOWS_OS_
		while(bind(m_Socket, (LPSOCKADDR)&addr, sizeof(addr)) == SOCKET_ERROR) //Try binding
#else
		//while(bind(m_Socket, (struct sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR) //Try binding
#endif
		{ 
			// error
			retryCount++;
			if(retryCount < m_Config.GetConnectionTries())
			{
#ifdef __WINDOWS_OS_
				::Sleep(BIND_SLEEP);
#else
				sleep(BIND_SLEEP/1000);
#endif
				//continue;
			}
			return RESULT_NO_BIND;
		}
	    
		int listenRes = listen(m_Socket, m_Config.GetNumberofClients()); //Start listening
		if(m_Config.GetEventListener())
		{
			m_Config.GetEventListener()->OnListen();
		}
		else
		{
		}
	}
	else // if(!sslSession)
	{
		Stop();
		SetConnected(false);
		return RESULT_FAIL;
	}
	return RESULT_OK;
}







int XLSSLSocket::StartAsClient()
{
	OpenSSLSession* sslSession = dynamic_cast<OpenSSLSession*>(m_Session.GetData());
	if(sslSession)
	{                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                               
		if(!CreateSocket())
		{
			return RESULT_NO_SOCKET;
		}

		SOCKADDR_IN target; //Information about host
		memset(&target, 0, sizeof(SOCKADDR_IN));

		target.sin_family = m_Config.GetIPFormat();           // address family Internet
		target.sin_port = htons (m_Config.GetPortNumber());        // set server�s port number
		//target.sin_addr.s_addr = m_Config.HostNumber;  // set server�s IP
		target.sin_addr.s_addr = inet_addr(m_Config.GetHostNumber());
	
		//Try connecting...
		if (connect(m_Socket, (SOCKADDR *)&target, sizeof(target)) == SOCKET_ERROR) //Try binding
		{ // error
			  return RESULT_NO_CONNECT;
		}

		m_SecureStruct = SSL_new (m_ConnectionCtx);

		if(m_SecureStruct)
		{
			SSL_set_fd(m_SecureStruct, m_Socket);
			//m_pSSLBio = BIO_new(BIO_s_socket());
			if(m_pSSLBio)
			{
				BIO_set_fd(m_pSSLBio, m_Socket, BIO_NOCLOSE);
				SSL_set_bio(m_SecureStruct, m_pSSLBio, m_pSSLBio);
			}
		}
		else
		{
			return RESULT_NO_CONNECT;
		}

		int error = SSL_connect(m_SecureStruct);

		if(error < 0)
		{
			return RESULT_NO_CONNECT;
		}

		if(sslSession->CanPeerCertify())
		{
			if(!CertifyPeer())
			{
				return RESULT_NO_CONNECT;
			}
			if(m_Config.GetEventListener())
			{
				m_Config.GetEventListener()->OnConnect();
			}
		}

		if(m_Config.GetEventListener())
		{
			m_Config.GetEventListener()->OnConnect();
		}

		SetConnected(true);
		return RESULT_OK;
	}
	else // if(!sslSession)
	{
		Stop();
		SetConnected(false);
		return RESULT_FAIL;
	}

	return RESULT_OK;
}




void XLSSLSocket::Stop()
{
	try
	{
		if(m_SecureStruct)
		{
			SSL_free(m_SecureStruct);
			m_SecureStruct = 0;
			m_pSSLBio = 0;
		}
		else if(m_pSSLBio)
		{
			BIO_free(m_pSSLBio);
			m_pSSLBio = 0;
		}

		if(m_Socket != INVALID_SOCKET)
		{
#ifdef __WINDOWS_OS_
			closesocket(m_Socket);
			WSACleanup();
#else
			close(m_Socket);
#endif
			m_Socket = INVALID_SOCKET; 
		}

		m_ConnectionCtx = 0;

		SetConnected(false);
	}
	catch(...)
	{
		SetConnected(false);
	}
}





SocketUtility::SocketErrCode XLSSLSocket::Send(const char* msg, int msglen) const // for client
{
	if(!IsConnected())
	{
		return SocketUtility::NoConnection;
	}

	try
	{
		int n = SSL_write( m_SecureStruct, msg, msglen);
		if(n <= 0)
		{
			ERR_print_errors_fp(stderr);
			return SocketUtility::NoConnection;
		}
		else if(m_Config.GetEventListener())
		{
			m_Config.GetEventListener()->OnSend(msg, msglen);
		}
		return SocketUtility::SocketNoError;
	}
	catch(...)
	{
	}


	return SocketUtility::SocketNoError;
}




SocketUtility::SocketErrCode XLSSLSocket::Recieve(char* buff, int bufflen) const
{
	if(!IsConnected())
	{
		return SocketUtility::NoConnection;
	}

	try
	{
		int bufRead = SSL_read(m_SecureStruct, buff, bufflen);
		if(bufRead == 0)
		{
			ERR_print_errors_fp(stderr);
			return SocketUtility::NoConnection;
		}
		else if((bufRead < 0) && m_pSSLBio)
		{
		    if(!BIO_should_retry(m_pSSLBio))
			{
				ERR_print_errors_fp(stderr);
				return SocketUtility::NoConnection;
			}
		}
		else
		{
			return SocketUtility::SocketNoError;
			if(m_Config.GetEventListener())
			{
				m_Config.GetEventListener()->OnRecieve(buff, bufflen);
			}
		}
	}
	catch(...)
	{
	}
	return SocketUtility::SocketNoError;
}




void XLSSLSocket::SetTimeOut(int timeout)
{
	try
	{
		if(IsConnected() && (timeout > 0))
		{
			SSL_CTX_set_timeout(m_ConnectionCtx, timeout);
		}
	}
	catch(...)
	{
	}
}




bool XLSSLSocket::AcceptConnection()
{
	try
	{
		if(!m_bServerMode)
		{
			return false;
		}

#ifdef __WINDOWS_OS_
		struct sockaddr_in saClient;
		unsigned int iClientSize = sizeof(saClient);
		Socket_Identifier AcceptSocket = accept(m_Socket, &saClient, &iClientSize);
#else
		struct sockaddr saClient;
		socklen_t iClientSize = sizeof(saClient);
		Socket_Identifier AcceptSocket = accept(m_Socket, &saClient, &iClientSize);
#endif

		int uniqueSockID = (int)AcceptSocket;

		BASE_SOCKET_PTR clientSock = new XLSSLSocket(uniqueSockID, m_Config);

		if(clientSock->IsActive())
		{
			OpenSSLSession* sslSession = dynamic_cast<OpenSSLSession*>(m_Session.GetData());
			if(sslSession)
			{
				if(sslSession->CanPeerCertify())
				{
					if(!CertifyPeer())
					{
						return false;
					}
					else if(m_Config.GetEventListener())
					{
						m_Config.GetEventListener()->OnAccept(clientSock);
					}
				}
				else
				{
					if(m_Config.GetEventListener())
					{
						m_Config.GetEventListener()->OnAccept(clientSock);
					}
				}
				
			}
		}
		else
		{
			return false;
		}
	}
	catch(...)
	{
	}

	return true;
}



void XLSSLSocket::GetCertificateName(SSL* ssl, std::string& certSubjectName, std::string& certCommonName ) const
{
    // get a pointer to the X509 certificate
    X509* cert;
    if ( !(cert = SSL_get_peer_certificate(m_SecureStruct)) )
    {
       // EventLog::Trace( EventLog::Error, "Failed to obtain certificate info. from the connection object");
        certSubjectName = "";
        certCommonName = "";
        return;
    }

    // get a handle to the certificate's DN
    X509_NAME *subj;
    if ( !(subj = X509_get_subject_name(cert)) )
    {
        certSubjectName = "<Unknown>";
        certCommonName = "<Unknown>";
        X509_free(cert);
        return;
    }

    // obtain & log the certificate's DN
    BIO *out = BIO_new(BIO_s_mem());
    if ( X509_NAME_print_ex(out, subj, 0, XN_FLAG_ONELINE) )
    {
        int size = (int)BIO_number_written(out);
		BIO_read(out, (char*) certSubjectName.c_str(), size);
        certSubjectName.resize(size);
    }
    else
    {
        certSubjectName = "<Unknown>";
        certCommonName = "<Unknown>";
    }
    BIO_free(out);

    // Get the common name string from the subject name 
    int cnLen = X509_NAME_get_text_by_NID( subj, NID_commonName, NULL, 0 );

    if ( cnLen != -1 )
    {
		certCommonName.resize(cnLen+1);
		certCommonName[cnLen] = 0;
        cnLen = X509_NAME_get_text_by_NID( subj, NID_commonName, (char*)certCommonName.c_str(), cnLen+1 );

        if ( cnLen != -1 )
        {
            certCommonName.resize(cnLen+1);
			certCommonName[cnLen] = 0;
        }
    }
    
    if ( cnLen == -1 )
    {
        certCommonName = "<Unknown>";
        X509_free(cert);
        return;
    }

    X509_free(cert);

    return;
}


bool XLSSLSocket::CertifyPeer()
{
	 X509* server_cert = SSL_get_peer_certificate (m_SecureStruct); 
	 if(server_cert == 0)
	 {
		 return false;
	 }

	 CHARSTORAGE str = X509_NAME_oneline(X509_get_subject_name(server_cert),0,0);

	 if(!str)
	 {
		 return false;
	 }

	 str = X509_NAME_oneline(X509_get_issuer_name(server_cert),0,0);

	 if(!str)
	 {
		 return false;
	 }

	 X509_free (server_cert);
	 return true;
}


#endif
