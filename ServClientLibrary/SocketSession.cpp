#include "SocketSession.h"
#include "bio.h"
#include "ssl.h"
#include "err.h"
#include "engine.h"
#include "conf.h"
#include <string>

#pragma comment(lib,"Ws2_32.lib") 

SESS_MANAGER_PTR  SocketSessionManager::m_pInstance = 0;
#define CERTIFICATE_DIR "../openssl/cert"
#define SARA_CERTIFICATE_FILE "selfsign_cert.PEM"
#define SARA_PVT_KEY_FILE "saraPvtKeyFile.PEM"
#define REMOTE_SARA_CERTIFICATE_FILE "remotesign_cert.PEM"


WinsockSession::WinsockSession():
m_pWSAData(0)
{
}


void WinsockSession::Start()
{
	m_pWSAData = new WSADATA;
	if( ::WSAStartup( 0x0002, m_pWSAData) )
	{
		delete m_pWSAData;
		m_pWSAData  = 0;
	}
}



WinsockSession::~WinsockSession()
{
	Stop();
}


void WinsockSession::Stop()
{
	if(m_pWSAData )
	{
		// Wait for all sockets to sort themselves out
		while( WSAEINPROGRESS == ::WSACleanup() );

		// Clean up the rest of it
		delete m_pWSAData;
		m_pWSAData  = 0;
	}
}


WinsockSession::operator const WSADATA *()
{
	return m_pWSAData;
}






// Construction / destruction
OpenSSLSession::OpenSSLSession():
m_ConnectionCtx(0),
m_SARACertificate(CERTIFICATE_DIR"/"SARA_CERTIFICATE_FILE),
m_SARAPvtKeyFile(CERTIFICATE_DIR"/"SARA_PVT_KEY_FILE),
m_TrustedCACertificate(CERTIFICATE_DIR),
m_AuthPassword("Password"),
m_CipherList("ALL"),
m_bLocalVerification(false),
m_bRemoteVerification(false),
m_b2ndTypeRemoteVerification(false),
m_iDepth(1),
m_bStarted(false)
{
}

void OpenSSLSession::Start()
{
	if(m_bStarted)
	{
		return;
	}

	SSL_load_error_strings();
	SSL_library_init();	
	ERR_load_BIO_strings();
	//OpenSSL_add_all_algorithms();

	//SSL protocol method
	const SSL_METHOD *meth = SSLv3_method();
	//Connection context using SSL protocol method
	m_ConnectionCtx = SSL_CTX_new(meth);
	if(!m_ConnectionCtx)
	{		
		ERR_print_errors_fp(stderr);
		return;
	}	

	if(SSL_CTX_set_cipher_list(m_ConnectionCtx, (char*)m_CipherList.c_str()) == 0)
	{
		ERR_print_errors_fp(stderr);
	}

	//Send SARA certificate to remote location
	if(m_bLocalVerification)
	{
		/* Load our keys and certificates*/
		//1. Load the certificates in to "m_ConnectionCtx"
		if (!SSL_CTX_use_certificate_file(m_ConnectionCtx, m_SARACertificate.c_str(), SSL_FILETYPE_PEM))
		{	
			ERR_print_errors_fp(stderr);
			return;
		}

		//2. Set the default password callback when loading/storing a PEM certificate with encryption
		SSL_CTX_set_default_passwd_cb(m_ConnectionCtx, GetPassword_CB);

		//3. Add the first private key found in file "m_SARACertificate" to "m_ConnectionCtx"
		if (!SSL_CTX_use_PrivateKey_file(m_ConnectionCtx, m_SARAPvtKeyFile.c_str(), SSL_FILETYPE_PEM))
		{			
			ERR_print_errors_fp(stderr);
			return;
		}

		//3_A. Check the private key is correct
		if(SSL_CTX_check_private_key(m_ConnectionCtx) == -1)
		{
			ERR_print_errors_fp(stderr);
			return;
		}
	}

	//Evaluation of remote certificate by SARA
	if(m_bRemoteVerification)
    {
		//4. Specify the location ("m_TrustedCACertificate") where CA certificates for verification purposes are located
		//   Used to authenticate the host to which we are connected. Loading all the CAs that we trust.
		if(!(SSL_CTX_load_verify_locations(m_ConnectionCtx, 0, m_TrustedCACertificate.c_str())))
		{			
			ERR_print_errors_fp(stderr);
			return;
		}

#if (OPENSSL_VERSION_NUMBER < 0x00905100L)
		SSL_CTX_set_verify_depth(m_ConnectionCtx,1);
#else
		SSL_CTX_set_verify_depth(m_ConnectionCtx, m_iDepth);
#endif
		//5. Set verification for host.
		SSL_CTX_set_verify(m_ConnectionCtx, SSL_VERIFY_PEER, OpenSSLSession::SSL_VerifyCallback);
	}

	m_bStarted = true;
}

OpenSSLSession::~OpenSSLSession()
{
	Stop();
}


void OpenSSLSession::Stop()
{
	if(!m_bStarted)
	{
		return;
	}

	if(m_ConnectionCtx)
	{
		SSL_CTX_free(m_ConnectionCtx);
		m_ConnectionCtx = 0;
	}

	ERR_remove_state(0);
	ENGINE_cleanup();
	CONF_modules_unload(1);
	ERR_free_strings();
	EVP_cleanup();
	sk_SSL_COMP_free(SSL_COMP_get_compression_methods());
	CRYPTO_cleanup_all_ex_data();

	m_bStarted = false;
}


int OpenSSLSession::SSL_VerifyCallback(int preverify_ok, X509_STORE_CTX *x509_ctx)
{
	//dummy
	return 1;
}


int OpenSSLSession::GetPassword_CB(char *out, int num, int rwflag, void *userdata)
{
	OpenSSLSession* sslSession = dynamic_cast<OpenSSLSession*>(GetOpenSSLSession().GetData());
	if(sslSession)
	{
		if(num >= (sslSession->GetAuthPassword().size() + 1))
		{
			strcpy(out, sslSession->GetAuthPassword().c_str());
			return sslSession->GetAuthPassword().size();
		}
	}
	return 0;
}









SocketSessionManager::SocketSessionManager():
m_pWinSession(), 
m_pSSLSession()
{
}

SocketSessionManager::~SocketSessionManager()
{
}

SESS_MANAGER_PTR SocketSessionManager::GetInstance()
{
	if(!m_pInstance)
	{
		m_pInstance = (new SocketSessionManager());
	}
	return m_pInstance;
}


SESSION_PTR SocketSessionManager::GetSession(SocketSessionType type)
{
	SESSION_PTR ptr;

	switch(type)
	{
	case SST_WinsockSession:
		if(!m_pWinSession)
		{
			m_pWinSession = (new WinsockSession());
		}
		ptr = m_pWinSession;
		break;

	case SST_OpenSSLSession:
		if(!m_pSSLSession)
		{
			m_pSSLSession = (new OpenSSLSession());
		}
		ptr = m_pSSLSession;
		break;
		
	default:
		break;
	}

	return ptr;
}


SESSION_PTR GetWinsockSession()
{
	SESSION_PTR session( SocketSessionManager::GetInstance()->GetSession(SST_WinsockSession));
	return session;
}

SESSION_PTR GetOpenSSLSession()
{
	SESSION_PTR session( SocketSessionManager::GetInstance()->GetSession(SST_OpenSSLSession));
	return session;
}