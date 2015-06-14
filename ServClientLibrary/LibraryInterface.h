#ifndef _CLIBRARVYYYY_INTERFACE_H___
#define _CLIBRARVYYYY_INTERFACE_H___
#include "XLAutoPtr.h"
#include <string>

typedef XLAutoPtr::XLSharedSmPtr<class CLibraryInterface> LIB_OBJECT;
class CLibraryInterface
{
	static LIB_OBJECT m_Instance;
private:
	CLibraryInterface(void);
	CLibraryInterface(const CLibraryInterface&);
	CLibraryInterface& operator = (const CLibraryInterface*);

public:
	static LIB_OBJECT GetInstance();
	~CLibraryInterface(void);

	//SSL related functions
	void SetUseSSL(bool set);
	void SetLocalVerification(bool set);
	void SetRemoteVerification(bool set);
	void SetSSLCertificates(std::string& device_cert, std::string& private_key, std::string& CA_file, std::string& pkeypassphrase);
	void SetCipherList(std::string& cipher);
	void StartSocketLibrary();
	void StopSocketLibrary();
};









#endif