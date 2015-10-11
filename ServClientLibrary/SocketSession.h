#ifndef _SOCKSESSION_H_
#define _SOCKSESSION_H_
#include "XLAutoPtr.h"
#include <vector>
#include <string>
#include "SocketDependency.h"


class SocketSession
{
public:
	SocketSession()                              {}
	virtual void Start() = 0;
	virtual void Stop() = 0;
	virtual ~SocketSession()                     {}

};


class WinsockSession: public SocketSession
{
	WSADATA*  m_pWSAData;
public:
	// Construction / destruction
	WinsockSession();
	void Start();
	void Stop();
	~WinsockSession();

	// The one and only access function
	operator const WSADATA *();
private:
};




class OpenSSLSession: public SocketSession
{
#ifdef _OPENSSL_ACTIVE_
	SSL_CTX*               m_ConnectionCtx;
#endif
	std::string            m_SARACertificate;
	std::string            m_SARAPvtKeyFile;
	std::string            m_TrustedCACertificate;
	std::string            m_AuthPassword;
	std::string            m_CipherList;
	bool                   m_bLocalVerification;
	bool                   m_bRemoteVerification;
	bool                   m_b2ndTypeRemoteVerification;
	int                    m_iDepth;
	bool                   m_bStarted;
public:
	// Construction / destruction
	OpenSSLSession();
	~OpenSSLSession();
	void Start();
	void Stop();

#ifdef _OPENSSL_ACTIVE_
	SSL_CTX* GetContext()                           {return m_ConnectionCtx;}
#endif

	bool CanPeerCertify()                           {return m_b2ndTypeRemoteVerification;}
	void SetDepth(int d)                            {m_iDepth = (d > 0 ? d : 1);}
	int GetDepth()                                  {return m_iDepth;}
	void SetCAFile(std::string& CAFile)             {m_TrustedCACertificate = CAFile;}
	void SetCertificate(std::string& certificate)   {m_SARACertificate = certificate;}
	void SetPrivateKey(std::string& pkey)           {m_SARAPvtKeyFile = pkey;}
	void SetPrivateKeyPassword(std::string& pass)   {m_AuthPassword = pass;}
	void SetLocalCertification(bool set)            {m_bLocalVerification = set;}
	void SetRemoteCertification(bool set)           {m_bRemoteVerification = set;}
	void SetCipherList(std::string& clist)          {m_CipherList = clist;}
private:
#ifdef _OPENSSL_ACTIVE_
	static int SSL_VerifyCallback(int preverify_ok, X509_STORE_CTX *x509_ctx);
#endif
	static int GetPassword_CB(char *out, int num, int rwflag, void *userdata);

	std::string GetAuthPassword()                   {return m_AuthPassword;}
	std::string GetCACertFile()                     {return m_TrustedCACertificate;}
};



enum SocketSessionType
{
	SST_WinsockSession = 0,
	SST_OpenSSLSession = 1
};

class SocketSessionManager;

typedef XLAutoPtr::XLSharedSmPtr<SocketSession> SESSION_PTR;
typedef XLAutoPtr::XLSharedSmPtr<SocketSessionManager> SESS_MANAGER_PTR;

class SocketSessionManager
{
	
	SESSION_PTR                         m_pWinSession;
	SESSION_PTR                         m_pSSLSession;
	static SESS_MANAGER_PTR             m_pInstance;

	SocketSessionManager();
	SocketSessionManager(const SocketSessionManager&);
	SocketSessionManager& operator = (const SocketSessionManager&);
public:

	static SESS_MANAGER_PTR GetInstance();
	~SocketSessionManager();

	SESSION_PTR GetSession(SocketSessionType type);
};


SESSION_PTR GetWinsockSession();
SESSION_PTR GetOpenSSLSession();



#endif //!_WINSOCKSESSION_H_
