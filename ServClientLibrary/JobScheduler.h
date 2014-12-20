#ifndef _SCHEDULER_J_O_B_H_
#define _SCHEDULER_J_O_B_H_

#include "XLEventThread.h"



class JobScheduler: private XLEventThread
{
public:
	JobScheduler(void);
	~JobScheduler(void);
private:
	virtual void t_OnEvent(const XLThreadEvent* e);
};
#endif