#ifndef XLSIMPLETHREADS_H_
#define XLSIMPLETHREADS_H_

//#include "afxwin.h"
//#include "CommonHeader.h"
#include "XLSemaPhore.h"
//#include "XLThreadDependency.h"
#include <string>
//#include "CommunicationHeader.h"
//#include "IProjectInterface.h"

enum ThreadConfig
{
	ThreadConfig_StackSize = 8192
};

class IThreadFunc
{
public:
	IThreadFunc()               {}
	virtual ~IThreadFunc()      {}
	virtual void RunProgram() = 0;
};



class XLSimpleThreads: public IThreadFunc
{
	XLSemaPhore            m_Sync1;
	XLSemaPhore            m_Sync2;
	ThreadHandle           m_handle;
	std::string            mName;
	Thread_Identifier      mThreadID;
	friend THFUN_RETVAL OTHER_EXPANSION RunSimpleThread(LPVOID data);
public:
	XLSimpleThreads(void);
    virtual ~XLSimpleThreads(void);
	virtual bool Create(const char *threadname, unsigned long stackSize = ThreadConfig_StackSize);
	void Destroy();
	bool IsCreated()
    {
#ifndef _C_11_COMPILER
        return (mThreadID != 0);
#else
        return (m_handle != 0);
#endif
    }
	void RunProgram()                      {}
	Thread_Identifier ThreadID() const     {return mThreadID;}
	const char* GetName()                  {return mName.c_str();}

	void Sleep(int millsec);

protected:
	virtual void RanOut(){}

	void GetThreadID();

};







#endif
