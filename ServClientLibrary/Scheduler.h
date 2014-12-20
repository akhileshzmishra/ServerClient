#ifndef _C_SCHEDULER_H_
#define _C_SCHEDULER_H_

#include "XLEventThread.h"
#include "SchedulerJobs.h"
#include "XLAlarm.h"

class CScheduler: private XLEventThread, public AlarmListener
{
	typedef std::multimap<CScheduleTime, CScheduledJobs*> JobSet; 
	typedef JobSet::iterator                              JobItr;
	typedef std::pair<CScheduleTime, CScheduledJobs*>     JobInsPair; 
	typedef std::pair<JobItr, JobItr>                     JobFindPair; 
	JobSet                                                m_JobSet[7];
	XLAlarm                                               m_Alarm;
	int                                                   m_CurrentDay;
	int                                                   m_AlarmPeriodicity;
	CScheduleTime                                         m_CurrentTime;
	XLSemaPhore                                           m_AlarmSem;
public:
	enum Inputs
	{
		START_PROCESSING = XLEventThreadEventType_Max + 1,
		RESET_INPUTS,
		ADD_A_JOB,
		NEXT_DAY_INPUT,
		ALARM_INPUT,
		MAX_INPUTS
	};
	CScheduler(void);
	~CScheduler(void);

	void Start();
	void Stop();

	void SetJob(CScheduleTime t, CScheduledJobs* job);

	void SetAlarmPeriodicity(int p)
	{
		m_AlarmPeriodicity = p;
	}


private:
	virtual void t_OnEvent(const XLThreadEvent* e);
	void StartProcessing(CScheduleTime& t);
	void AddAJob(CScheduleTime time, CScheduledJobs* job);

	void OnAlarm(XLAlarm* alarm, AlarmData* pData);

	void SetCurrentTime();
	void SetCurrentDay();
	void SetCurrentTime(CScheduleTime& t);
	void AlarmInput();
};



#endif