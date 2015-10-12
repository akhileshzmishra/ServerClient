#ifndef SCHEDULER_JBS_H_
#define SCHEDULER_JBS_H_




#include <string>

class CScheduleTime
{
	int m_h;
	int m_m;
	int m_s;
	int m_dow;
public:
	CScheduleTime():
	m_h(0),
	m_m(0),
	m_s(0),
	m_dow(0)
	{
	}

	CScheduleTime(int whole):
	m_h(0),
	m_m(0),
	m_s(0),
	m_dow(0)
	{
		SetHour(whole/3600);
		SetMinute((whole%3600)/60);
		SetSecond((whole%3600)%60);
	}

	CScheduleTime(int h, int m, int s, int dow):
	m_h(0),
	m_m(0),
	m_s(0),
	m_dow(0)
	{
		Set(h, m, s, dow);
	}

	CScheduleTime(char* buffer, int buffsize):
	m_h(0),
	m_m(0),
	m_s(0),
	m_dow(0)
	{
		ReadFrom(buffer, buffsize);
	}

	~CScheduleTime()
	{
	}

	bool operator == (const CScheduleTime& other) const
	{
		return ((m_h == other.m_h) && (m_m == other.m_m) && (m_s == other.m_s) && (m_dow == other.m_dow));
	}

	bool operator < (const CScheduleTime& other) const
	{
		return GetDaySeconds() < other.GetDaySeconds();
	}

	bool operator > (const CScheduleTime& other) const
	{
		return GetDaySeconds() > other.GetDaySeconds();
	}

	bool operator <= (const CScheduleTime& other) const
	{
		return GetDaySeconds() <= other.GetDaySeconds();
	}

	bool operator >= (const CScheduleTime& other) const
	{
		return GetDaySeconds() >= other.GetDaySeconds();
	}

	int GetDaySeconds() const
	{
		return (m_h*3600 + m_m*60 + m_s);
	}

	operator int()
	{
		return GetDaySeconds();
	}

	static int FullDay()
	{
		return (24*3600 + 60*60 + 0);
	}

	void SetHour(int h)
	{
		if((h >= 0) && (h < 24))
		{
			m_h = h;
		}
	}

	void SetMinute(int m)
	{
		if((m >= 0) && (m < 60))
		{
			m_m = m;
		}
	}

	void SetSecond(int s)
	{
		if((s >= 0) && (s < 60))
		{
			m_s = s;
		}
	}

	void SetDow(int dow)
	{
		if((dow >= 0) && (dow < 7))
		{
			m_dow = dow;
		}
	}

	void Set(int h, int m, int s, int dow)
	{
		SetHour(h);
		SetMinute(m);
		SetSecond(s);
		SetDow(dow);
	}
	static const int GetTotalWriteLength()
	{
		return sizeof(int)*4;
	}

	void WriteTo(char* output, int size) const
	{
		if((size >= GetTotalWriteLength()) && output)
		{
			int itr = 0;
			memcpy(&output[itr], &m_h, sizeof(m_h));
			itr += sizeof(m_h);
			memcpy(&output[itr], &m_m, sizeof(m_m));
			itr += sizeof(m_m);
			memcpy(&output[itr], &m_s, sizeof(m_s));
			itr += sizeof(m_s);
			memcpy(&output[itr], &m_dow, sizeof(m_dow));
			itr += sizeof(m_dow);
		}
	}

	void ReadFrom(char* input, int size)
	{
		if((size >= GetTotalWriteLength()) && input)
		{
			int itr = 0;
			memcpy(&m_h, &input[itr], sizeof(m_h));
			itr += sizeof(m_h);
			memcpy(&m_m, &input[itr], sizeof(m_m));
			itr += sizeof(m_m);
			memcpy(&m_s, &input[itr], sizeof(m_s));
			itr += sizeof(m_s);
			memcpy(&m_dow, &input[itr], sizeof(m_dow));
			itr += sizeof(m_dow);
		}
	}

	std::string String()
	{
		char input[sizeof(int) * 4];
		ReadFrom(input, CScheduleTime::GetTotalWriteLength());
		std::string str(input, sizeof(int) * 4);
		return str;
	}


	int GetHour(void)  	 const                      {return m_h;}
	int GetMinute(void)  const	                    {return m_m;}
	int GetSeconds(void)const  	                    {return m_s;}
	int GetDow()         const                      {return m_dow;}
};


enum RepeatableType
{
	RT_Once,
	RT_Daily,
	RT_Weekly,
	RT_Monthly,
	RT_Yearly,
	RT_DurationWise,
	RT_MaxType
};


class CScheduledJobs
{
	//typedef std::vector<CScheduleTime> ScheduleTimeList;
	CScheduleTime                        m_Time;
	std::string                          m_Name;
	bool                                 m_bDone;
	RepeatableType                       m_RepeatableType;
public:
	CScheduledJobs():
	m_bDone(false),
	m_RepeatableType(RT_Once)
	{
	}
	virtual ~CScheduledJobs()
	{
	}
	void SetName(const char* name)
	{
		if(name)
		{
			m_Name = name;
		}
	}
	void SetTime(int dow, CScheduleTime time)
	{
		if((dow >= 0) && (dow < 7))
		{
			m_Time = time;
			m_Time.SetDow(dow);
		}
	}

	void SetRepeatableType(RepeatableType t)                      { m_RepeatableType = t; }

	void Execute(CScheduleTime& time)
	{
		m_bDone = DoTheJob(time);
	}

	virtual bool DoTheJob(CScheduleTime& time)                    { return true; }
	std::string GetName()                                         { return m_Name; }
	CScheduleTime GetTime()                                       { return m_Time; }
	bool Done()                                                   { return m_bDone; }


};




#endif
