#include "XLAlarm.h"

XLAlarm::XLAlarm(AlarmListener* listener, int numberOfTimes):
m_Listener(listener),
m_iMillSec(0),
m_iRepeatCount(unsigned int(numberOfTimes)),
m_bExit(false),
m_pData(0)
{
	m_ExitKeeper.Create(1,1);
}

XLAlarm::~XLAlarm(void)
{
	Reset();
	m_ExitKeeper.Destroy();
}

void XLAlarm::SetAlarm(int millisec)
{
	if(XLSimpleThreads::IsCreated())
	{
		Reset();
		m_ExitKeeper.Take();
		m_ExitKeeper.Give();
	}

	m_iMillSec = millisec;

	m_bExit = false;

	XLSimpleThreads::Create("ALARM_THREAD");
}

void XLAlarm::RunProgram()
{
	unsigned int itr = 0;
	m_ExitKeeper.Take();
	while((!m_bExit) && (itr < m_iRepeatCount))
	{
		XLSimpleThreads::Sleep(m_iMillSec);
		if(m_Listener)
		{
			m_Listener->OnAlarm(this, m_pData);
		}
		itr++;
	}
}

void XLAlarm::Reset()
{
	m_bExit = true;
}

void XLAlarm::RanOut()
{
	XLSimpleThreads::Destroy();
	m_ExitKeeper.Give();
}