#ifndef XL_MULTI_ALARM_H_
#define XL_MULTI_ALARM_H_

#include "XLSimpleThreads.h"
#include "XLMutex.h"
#include <map>
#include <list>

class XLMultiAlarmListener
{
public:
	virtual void OnAlarm() = 0;
};


class XLMAlarmData
{
	XLMultiAlarmListener*  mListener;
	int                    mCount;
public:
	XLMAlarmData() {}

	XLMultiAlarmListener*& Listener()           {return mListener;}
	int Count()                                 {return mCount;}
	void SetCount(int s)                        {mCount = s;}
	void Decrease()    
	{
		if(mCount > 0)
		{
			mCount--;
		}
	}
	void Fire()
	{
		if((mCount > 0) && mListener)
		{
			mListener->OnAlarm();
		}
	}
};

class XLMultiAlarm : public XLSimpleThreads
{
	typedef std::map<int, XLMAlarmData*>                  LISTSET;
	LISTSET::iterator                                     LISTITR;
	typedef std::pair<int, XLMAlarmData*>                 LISTPAIR;
	typedef std::list<LISTPAIR>                           WORKINGLIST;
	typedef WORKINGLIST::iterator                         WITR;
	LISTSET                                               m_Listener;
	int                                                   m_iMaxTime;
	WORKINGLIST                                           m_WorkList;
	WORKINGLIST                                           m_TempList;
	bool                                                  m_bExit;
	XLMutex                                               m_Lock;
	                                   
public:
	XLMultiAlarm(void);
	~XLMultiAlarm(void);

	void SetAlarm(XLMultiAlarmListener* data, int time, int howmanytimes);

private:
	void RunProgram();
	void RanOut();

	void InsertIntoTempList(XLMultiAlarmListener* data, int time, int howmanytimes);
	void TransfertoWorkingList();
};










#endif