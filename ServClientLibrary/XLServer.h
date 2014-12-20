#ifndef _XL_SERVER_H__
#define _XL_SERVER_H__


#include "XLServingThread.h"
#include "LibraryInterface.h"
#include <map>

namespace XLServerClientModel
{
class XLCService;

class XLServer: public ServingThreadListener
{
	
	typedef std::map<int, XLCService*>               SERVTLIST;
	typedef SERVTLIST::iterator                      SERVTITR;
	typedef std::pair<int, XLCService*>              SERVTPAIR;
	SERVTLIST                                        m_ClientServices;
	int                                              m_iMaxServices;
public:
	XLServer(int maxServices = 1);
	virtual ~XLServer(void);

	void Start();
	void Stop();
	int StartAService(SocketConfig& coinfig);
	void StopAService(int id);

	void Broadcast(const char* msg, int msglen, int serviceID);
	void Broadcast(const char* msg, int msglen); // to all the services

	LIB_OBJECT GetSettingLibrary();

private:

	//ServingThreadListener
	virtual void OnStart(class XLServingThread* );
	virtual void OnEnd(class XLServingThread* );
	virtual void OnAccept(class XLServingThread*, BASE_SOCKET_PTR);
	virtual void OnEvent(const class XLThreadEvent* e);
	virtual void OnSent(const class XLThreadEvent* e);

};





}; //namespace XLServerClientModel





#endif