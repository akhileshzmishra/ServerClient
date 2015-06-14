#ifndef _XLCLIENT____H____
#define _XLCLIENT____H____

#include "XLClientThread.h"
#include "LibraryInterface.h"
namespace XLServerClientModel
{


class XLClient: public XLClientThreadListener
{
	XLClientThread*  m_Client;
	SocketConfig     m_Config;
	XLSemaPhore      m_Gatekeeper;
public:
	XLClient(SocketConfig config);
	virtual ~XLClient(void);

	void Start();
	void Stop();

	LIB_OBJECT GetSettingLibrary();
private:
	//XLClientThreadListener

	virtual void OnStart();
	virtual void OnEnd();
	virtual void OnEvent(const XLThreadEvent* e);
};


}; //namespace XLServerClientModel












#endif