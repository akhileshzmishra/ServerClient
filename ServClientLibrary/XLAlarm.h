#ifndef _XLALARM__H_
#define _XLALARM__H_
#include "XLSimpleThreads.h"
#include "XLSemaPhore.h"
#include <map>
class XLAlarm;
class AlarmListener
{
public:
	virtual void OnAlarm(class XLAlarm* alarm, class AlarmData* pData) = 0;
};

class AlarmData
{
public:
	AlarmData(){}
	virtual ~AlarmData(){}
};

class XLAlarm :	private XLSimpleThreads
{
	AlarmListener*                       m_Listener;
	int                                  m_iMillSec;
	unsigned int                         m_iRepeatCount;
	bool                                 m_bExit;
	XLSemaPhore                          m_ExitKeeper;
	AlarmData*                           m_pData;

public:
	XLAlarm(AlarmListener* listener = 0, int numberOfTimes = -1);
	~XLAlarm(void);

	void SetAlarm(int millisec);

	void Reset();

	void SetListener(AlarmListener* listener, AlarmData* pData = 0)
	{
		m_Listener = listener;
		m_pData = pData;
	}

private:
	void RunProgram();
	void RanOut();
};




#endif