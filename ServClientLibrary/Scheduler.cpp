#include "Scheduler.h"
#include <ctime>

CScheduler::CScheduler(void):
m_CurrentDay(0),
m_AlarmPeriodicity(3000)
{
	SetCurrentTime();
}

CScheduler::~CScheduler(void)
{
}

void CScheduler::Start()
{
	if(!IsCreated())
	{
		SetCurrentTime();
		SetCurrentDay();
		XLEventThread::Create("SCHDEDULER");
		m_AlarmSem.Create(0,1);
		m_Alarm.SetListener(this);
		m_Alarm.SetAlarm(m_AlarmPeriodicity);
	}
}
void CScheduler::Stop()
{
	if(IsCreated())
	{
		m_Alarm.Reset();
		m_AlarmSem.Destroy();
		XLEventThread::StopMainLoop();
		XLEventThread::Destroy();
	}
}

void CScheduler::t_OnEvent(const XLThreadEvent* e)
{
	switch(e->GetType())
	{
	case START_PROCESSING:
		{
			CScheduleTime time;
			e->Read(0, time);
			StartProcessing(time);
		}
		break;
	case RESET_INPUTS:
		break;
	case ADD_A_JOB:
		{
			CScheduleTime time;
			CScheduledJobs* job = 0;
			e->Read(0, time);
			e->Read(CScheduleTime::GetTotalWriteLength(), job);
			AddAJob(time, job);
		}
		break;
	case NEXT_DAY_INPUT:
		m_CurrentDay = (m_CurrentDay + 1)%7;
		SetCurrentTime();
		break;
	case ALARM_INPUT:
		AlarmInput();
		break;
	default:
		break;
	}
}

void CScheduler::SetJob(CScheduleTime t, CScheduledJobs* job)
{
	XLThreadEvent e;
	CScheduleTime time;
	e.SetType(ADD_A_JOB);
	e.Write(0, time);
	e.Write(CScheduleTime::GetTotalWriteLength(), job);
	Put(&e);
}

void CScheduler::AddAJob(CScheduleTime time, CScheduledJobs* job)
{
	if(job)
	{
		m_JobSet[time.GetDow()].insert(JobInsPair(time, job));
	}
}
void CScheduler::StartProcessing(CScheduleTime& t)
{
	JobFindPair fp = m_JobSet[m_CurrentDay].equal_range(t);
	JobItr itr = fp.first;
	while(itr != fp.second)
	{
		itr->second->DoTheJob(t);
		itr++;
	}
}

void CScheduler::OnAlarm(XLAlarm* alarm, AlarmData* pData)
{
	XLThreadEvent e;
	m_AlarmSem.Take();
	e.SetType(ALARM_INPUT);
	Put(&e);
}

void CScheduler::AlarmInput()
{
	CScheduleTime nowcsT;
	SetCurrentTime(nowcsT);
	JobItr itr = m_JobSet[m_CurrentDay].begin();
	if(((int)nowcsT >= CScheduleTime::FullDay()) && ((int)m_CurrentTime < CScheduleTime::FullDay()))
	{
		XLThreadEvent e;
		e.SetType(NEXT_DAY_INPUT);
		Put(&e);
		m_AlarmSem.Give();
		return;
	}
	while(itr != m_JobSet[m_CurrentDay].end())
	{
		if((itr->first > m_CurrentTime) && (itr->first < nowcsT))
		{
			CScheduleTime bTime = itr->first;
			itr->second->DoTheJob(bTime);
		}
		itr++;
	}
	SetCurrentTime();
	m_AlarmSem.Give();
}

void CScheduler::SetCurrentTime()
{
	time_t t = time(0);   // get time now
    struct tm * now = localtime( & t );
	if(!now)
	{
		return;
	}
	m_CurrentTime = now->tm_hour*3600 + now->tm_min*60 + now->tm_sec + (now->tm_wday - m_CurrentDay)*3600*24 ;
}

void CScheduler::SetCurrentTime(CScheduleTime& ct)
{
	time_t t = time(0);   // get time now
    struct tm * now = localtime( & t );
	if(!now)
	{
		return;
	}
	ct = now->tm_hour*3600 + now->tm_min*60 + now->tm_sec + (now->tm_wday - m_CurrentDay)*3600*24;
}

void CScheduler::SetCurrentDay()
{
	time_t t = time(0);   // get time now
    struct tm * now = localtime( & t );
	if(!now)
	{
		return;
	}
	m_CurrentDay = now->tm_wday;
}