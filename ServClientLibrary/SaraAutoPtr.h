#ifndef _SARA_AUTO_PTR__H___
#define _SARA_AUTO_PTR__H___
#include <set>
#include <map>
#include <vector>

namespace Sara_Smart_Objects
{



template<class T>
class RefCounterObject
{
	int        m_iCount;
	T*         m_pData;

	//not to be exposed
	RefCounterObject(const RefCounterObject&);
	RefCounterObject& operator = (const RefCounterObject&);
public:
	RefCounterObject(T* data):
	m_iCount(0),
	m_pData(data)
	{
		AddRef();
	}

	~RefCounterObject()
	{
		Release();
	}

	void AddRef()
	{
		if(m_pData)
		{
			m_iCount++;
		}
	}

	int RefCount()
	{
		return m_iCount;
	}

	void Release()
	{
		if(m_pData)
		{
			m_iCount--;
			if(m_iCount == 0)
			{
				delete m_pData;
				m_pData = 0;
			}
		}
	}

	T* GetData()  const                  {return m_pData;}
};


template<class T>
class SaraSharableSmartPtr
{
	typedef RefCounterObject<T>           RefCounter;
	RefCounter*                           m_pData;
public:
	SaraSharableSmartPtr(T* data):
	m_pData(0)
	{
		SetData(data);
	}

	SaraSharableSmartPtr():
	m_pData(0)
	{
	}

	void SetData(T* data)
	{
		ReleaseData();
		m_pData = new RefCounter(data);
	}

	virtual ~SaraSharableSmartPtr()
	{
		ReleaseData();
	}

	SaraSharableSmartPtr(const SaraSharableSmartPtr& other)
	{
		m_pData = other.m_pData;
		if(m_pData)
		{
			m_pData->AddRef();
		}
	}

	SaraSharableSmartPtr<T>& operator = (const SaraSharableSmartPtr& other)
	{
		if(this == &other)
		{
			return *this;
		}

		ReleaseData();

		m_pData = other.m_pData;

		if(m_pData)
		{
			m_pData->AddRef();
		}

		return *this;

	}

	SaraSharableSmartPtr<T>& operator = (T* data)
	{
		SetData(data);
		return *this;
	}

	

	T* GetData()                                   
	{
		if(m_pData)
		{
			return m_pData->GetData();
		}
		return 0;
	}

	T& operator*()                                
	{
		return *(m_pData->GetData());
	}

	T* operator->()                                
	{
		if(m_pData)
		{
			return m_pData->GetData();
		}
		return 0;
	}


	operator const T*() const
	{
		if(m_pData)
		{
			return m_pData->GetData();
		}
		return 0;
	}


	operator T*()	
	{
		if(m_pData)
		{
			return m_pData->GetData();
		}
		return 0;
	}

	bool operator !()  const                            
	{
		if(m_pData)
		{
			return (m_pData->GetData() == 0);
		}
		return true;
	}

	operator bool() const                           
	{
		if(m_pData)
		{
			return (m_pData->GetData() != 0);
		}
		return false;
	}

private:

	void ReleaseData()
	{
		if(m_pData)
		{
			m_pData->Release();
			if(m_pData->GetData() == 0)
			{
				delete m_pData;
			}
			m_pData = 0;
		}
	}

};

};
#endif