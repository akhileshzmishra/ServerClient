#ifndef _IOBSERVER_H_
#define _IOBSERVER_H_
#include <set>
#include <map>
#include <set>
#include <vector>

template<class T> 
class Subject;

template<class T>
class IObserver;

template<class T, class X, class XComp = std::less<X> > 
class ISubject2;

template<class T, class X, class XComp = std::less<X> >
class IObserver2;


template<class T>
class IObserver
{
	Subject<T>* m_Subject;
public:
	IObserver(Subject<T>* subject): m_Subject(subject)
	{
		if(m_Subject)
		{
			m_Subject->RegisterObserver(this);
		}
	}
	virtual void Notify(T data) = 0;
	virtual ~IObserver()
	{
		if(m_Subject)
		{
			m_Subject->DeRegister(this);
		}
		m_Subject = 0;
	}
	Subject<T>* GetSubject()
	{
		return m_Subject;
	}
};


template<class T> 
class Subject
{
	typedef std::set<IObserver<T>* > ObserverList;
	typedef typename ObserverList::iterator ObserverItr;
	ObserverList m_ObsList;
public:
	Subject()
	{
	}
	void RegisterObserver(IObserver<T>* o)
	{
		m_ObsList.insert(o);
	}

	virtual void Notify(T data)
	{
		ObserverItr itr = m_ObsList.begin();
		while(itr != m_ObsList.end())
		{
			(*itr)->Notify(data);
			itr++;
		}
	}
	void DeRegister(IObserver<T>* o)
	{
		ObserverItr itr = m_ObsList.find(o);
		if(itr != m_ObsList.end())
		{
			m_ObsList.erase(itr);
		}
	}
};



template<class T, class X, class XComp = std::less<X> >
class IObserver2
{
	typedef ISubject2<T, X, XComp>        SubjectType;
	typedef std::set<X, XComp>            ConditionSet;
	SubjectType*                          m_Subject;
	ConditionSet                          m_RegisteredList;
public:
	IObserver2(SubjectType* subject, std::vector<X>& conditions): m_Subject(subject)
	{
		if(m_Subject)
		{
			for(int i = 0; i < conditions.size(); i++)
			{
				Register(conditions[i]);
			}
		}
	}

	IObserver2(SubjectType* subject, X* conditions = 0, int size = 0): m_Subject(subject)
	{
		if(m_Subject)
		{
			for(int i = 0; i < size; i++)
			{
				Register(conditions[i]);
			}
		}
	}

	IObserver2(SubjectType* subject): m_Subject(subject)
	{
	}
	
	virtual ~IObserver2()
	{
		if(m_Subject)
		{
			m_Subject->DeRegister(this);
		}
	}

	void Register(X condition)
	{
		if(m_Subject)
		{
			if(m_RegisteredList.find(condition) == m_RegisteredList.end())
			{
				m_Subject->Register(this, condition);
				m_RegisteredList.insert(condition);
			}
		}
	}

	void DeRegister(X condition)
	{
		if(m_Subject)
		{
			m_Subject->DeRegister(this, condition);
		}
		m_RegisteredList.erase(condition);
	}

	void Deregister()
	{
		if(m_Subject)
		{
			m_Subject->DeRegister(this);
		}
		m_RegisteredList.clear();
		m_Subject = 0;
	}

	virtual void Notify(T* data, X* condition) = 0;

	
	SubjectType* GetSubject()
	{
		return m_Subject;
	}

	void SetSubject(SubjectType* subject)
	{
		if(m_Subject)
		{
			m_Subject->DeRegister(this);
		}
		m_RegisteredList.clear();
		m_Subject = subject;
	}
};


template<class T, class X, class XComp> 
class ISubject2 
{ 
	typedef IObserver2<T, X, XComp>                  ObserverType;
	typedef std::multimap<X, ObserverType*, XComp>   ObserverList;
	typedef std::pair<X, ObserverType* >             ObsInsPair;
	typedef typename ObserverList::iterator          ObserverItr;
	ObserverList                                     m_list;

	friend ObserverType;
public:
	ISubject2<T, X, XComp>()
	{
	}

	virtual ~ISubject2<T, X, XComp>()
	{
	}
	

	virtual void Notify(T* data, X* condition)
	{
		if(!condition)
		{
			return;
		}

		m_ListGate.Lock();
		std::pair<ObserverItr, ObserverItr> obsPair = m_list.equal_range(condition);
		ObserverItr itr = obsPair.first;
		while(itr != obsPair.second)
		{
			itr->second->Notify(data, condition);
			itr++;
		}
		m_ListGate.Unlock();
	}

protected:
	void Register(ObserverType* o, X condition)
	{
		if(!o)
		{
			return;
		}

		m_list.insert(ObsInsPair(condition, o));
	}

	void DeRegister(ObserverType* o)
	{
		if(!o)
		{
			return;
		}

		std::vector<ObserverItr> deletITr;
		ObserverItr itr = m_list.begin();
		while(itr != m_list.end())
		{
			if(itr->second == o)
			{
				deletITr.push_back(itr);
			}

			itr++;
		}
		for(int i = 0; i < (int) deletITr.size(); i++)
		{
			m_list.erase(deletITr[i]);
		}
	}
	
	void DeRegister(ObserverType* o, X condition)
	{
		if(!o)
		{
			return;
		}

		std::pair<ObserverItr, ObserverItr> obsPair = m_list.equal_range(condition);
		ObserverItr itr = obsPair.first;
		while(itr != obsPair.second)
		{
			if(o == itr->second)
			{
				m_list.erase(itr);
				break;
			}
			itr++;
		}
	}

};



#endif