#include "XLMultiAlarm.h"

XLMultiAlarm::XLMultiAlarm(void):
m_iMaxTime(0),
m_bExit(false)
{
	m_Lock.Create();
}

XLMultiAlarm::~XLMultiAlarm(void)
{
	m_Lock.Destroy();
}


void XLMultiAlarm::RunProgram()
{
	while(!m_bExit)
	{
	}
}



void XLMultiAlarm::RanOut()
{
}


void XLMultiAlarm::SetAlarm(XLMultiAlarmListener* data, int time, int howmanytimes)
{
	if(!IsCreated())
	{
		Create("MULTIALARM");
	}
	if(IsCreated())
	{
		if(m_iMaxTime < time)
		{
			m_iMaxTime = time;
		}
		InsertIntoTempList(data, time, howmanytimes);
	}
}


void XLMultiAlarm::InsertIntoTempList(XLMultiAlarmListener* list, int time, int howmanytimes)
{
	m_Lock.Lock();
	XLMAlarmData* aldata = new XLMAlarmData();
	aldata->Listener() = list;
	m_TempList.push_back(LISTPAIR(time, aldata));
	m_Listener.insert(LISTPAIR(time, aldata));
	m_Lock.Unlock();
}

void XLMultiAlarm::TransfertoWorkingList()
{
}