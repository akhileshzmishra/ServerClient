#ifndef _XLSSL_SOCKET_H_
#define _XLSSL_SOCKET_H_
#include "SocketUtility.h"
#include "XLBaseSocket.h"
#include "XLAutoPtr.h"
#include "bio.h"


class XLSSLSocket: public XLBaseSocket
{
	SSL_CTX *   m_ConnectionCtx; 
	BIO*        m_pSSLBio;
	SSL*        m_SecureStruct;
	bool        m_bSecure;
	bool        m_bConnected;
	SOCKET      m_Socket;

	typedef XLAutoPtr::XLSharedSmPtr<XLSSLSocket> SSLSockPtr;
public:	
	enum 
	{
		RESULT_OK = XLBaseSocket::RESULT_OK,
		RESULT_NO_STARTUP = XLBaseSocket::RESULT_NO_STARTUP,
		RESULT_NO_SOCKET = XLBaseSocket::RESULT_NO_SOCKET,
		RESULT_NO_CONNECT = XLBaseSocket::RESULT_NO_CONNECT,
		RESULT_NO_BIND = XLBaseSocket::RESULT_NO_BIND,
		RESULT_WRONG_VERSION = XLBaseSocket::RESULT_WRONG_VERSION,
		RESULT_FAIL = XLBaseSocket::RESULT_FAIL,
	};

	XLSSLSocket(class SocketConfig& config, bool IsServerMode = true);
	~XLSSLSocket(void);
	int Start();

	void Stop();

	SocketUtility::SocketErrCode Send(const char* msg, int msglen) const; // for client
	SocketUtility::SocketErrCode Recieve(char* buff, int bufflen) const;

	void SetTimeOut(int timeout);
	bool IsConnected()const                                   {return m_bConnected;}

	bool AcceptConnection();
private:
	
	void SetConnected(bool set)                               {m_bConnected = set;}

	int StartAsClient();
	int StartAsServer();

	void GetCertificateName(SSL* ssl, std::string& certSubjectName, std::string& certCommonName ) const;

	bool CertifyPeer();

	XLSSLSocket(SOCKET sock, class SocketConfig& config);

	bool CreateSocket();


};




#endif