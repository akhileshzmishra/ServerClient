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
		m_Alarm.SetFrequency(INT_MAX);
		m_Alarm.SetAlarm(m_AlarmPeriodicity);
		//m_AlarmSem.Give();
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
			std::cout << "START_PROCESSING" << std::endl;
			CScheduleTime time;
			e->Read(0, time);
			StartProcessing(time);
		}
		break;
	case RESET_INPUTS:
		break;
	case ADD_A_JOB:
		{
			std::cout << "ADD_A_JOB" << std::endl;
			CScheduleTime time;
			CScheduledJobs* job = 0;
			e->Read(0, time);
			e->Read(CScheduleTime::GetTotalWriteLength(), job);
			AddAJob(time, job);
		}
		break;
	case REMOVE_A_JOB:
		{
			std::cout << "ADD_A_JOB" << std::endl;
			CScheduleTime time;
			CScheduledJobs* job = 0;
			e->Read(0, time);
			e->Read(CScheduleTime::GetTotalWriteLength(), job);
			DeleteAJob(time, job);
		}
	case NEXT_DAY_INPUT:
		SetCurrentDay();
		SetCurrentTime();
		break;
	case ALARM_INPUT:
		std::cout << "ALARM_INPUT" << std::endl;
		AlarmInput();
		break;
	default:
		break;
	}
}

void CScheduler::SetJob(CScheduleTime t, CScheduledJobs* job)
{
	XLThreadEvent e;
	e.SetType(ADD_A_JOB);
	e.Write(0, t);
	e.Write(CScheduleTime::GetTotalWriteLength(), job);
	Put(&e);
}

void CScheduler::DeleteJob(CScheduleTime t, CScheduledJobs* job)
{
	XLThreadEvent e;
	e.SetType(REMOVE_A_JOB);
	e.Write(0, t);
	e.Write(CScheduleTime::GetTotalWriteLength(), job);
	Put(&e);
}


void CScheduler::DeleteAJob(CScheduleTime time, CScheduledJobs* job)
{
	if (job)
	{
		JobFindPair jfp = m_JobSet[time.GetDow()].equal_range(time);
		JobItr itr = jfp.first;
		while (itr != jfp.second)
		{
			if (itr->second == job)
			{
				break;
			}
			itr++;
		}
		if (itr != jfp.second)
		{
			m_JobSet[time.GetDow()].erase(itr);
		}

	}
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
		itr->second->Execute(t);
		itr++;
	}
}

void CScheduler::OnAlarm(XLAlarm* alarm, AlarmData* pData)
{
	XLThreadEvent e;
	//m_AlarmSem.Take();
	e.SetType(ALARM_INPUT);
	Put(&e);
}

void CScheduler::AlarmInput()
{
	CScheduleTime nowcsT;
	SetCurrentTime(nowcsT);
	JobItr itr = m_JobSet[m_CurrentDay].begin();
	int CurrentDay = m_CurrentDay;
	SetCurrentDay();
	if (CurrentDay != m_CurrentDay)
	{
		XLThreadEvent e;
		e.SetType(NEXT_DAY_INPUT);
		Put(&e);
		//m_AlarmSem.Give();
	}
	else
	{
		while (itr != m_JobSet[m_CurrentDay].end())
		{
			if ((itr->first > m_CurrentTime) && (itr->first < nowcsT) && !itr->second->Done())
			{
				CScheduleTime bTime = itr->first;
				itr->second->Execute(bTime);
			}
			itr++;
		}
		SetCurrentTime();
	}
	//m_AlarmSem.Give();
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
