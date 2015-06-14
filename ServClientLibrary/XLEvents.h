#ifndef _XLEVENTS_H____
#define _XLEVENTS_H____

#include <memory.h>
#include <iostream>
#include "CommunicationHeader.h"




class XLThreadEvent
{
	CHARSTORAGE m_pData;
	int         m_iLength;
	int         m_iType;

public:
	XLThreadEvent():m_pData(),m_iLength(0), m_iType(0)
	{
	}
	XLThreadEvent(int size):m_pData(),m_iLength(0), m_iType(0)
	{
		if(m_iLength > 0)
		{
			m_pData = new char[m_iLength];
		}
	}
	XLThreadEvent(const char* pData, int size):m_pData(),m_iLength(0), m_iType(0)
	{
		ReadFrom(pData, size);
	}

	XLThreadEvent(const XLThreadEvent& other):m_pData(),m_iLength(0), m_iType(0)
	{
		m_iType = other.m_iType;
		m_pData = other.m_pData;
		m_iLength = other.m_iLength;
		
	}
	XLThreadEvent& operator = (const XLThreadEvent& other)
	{
		if(this == &other)
		{
			return *this;
		}
		m_iType = other.m_iType;
		m_pData = other.m_pData;
		m_iLength = other.m_iLength;
		return *this;
	}

	~XLThreadEvent()
	{
		Reset();
	}

	bool IsValid() const
	{
		return (m_pData && (m_iLength > 0));
	}

	bool IsData()    const                                  {return m_iLength > 0;} 
	void SetType(int iType)                                 {m_iType = iType;}
	void SetData(const char* Data, int Length)
	{
		Reset();
		if((Length > 0) && Data)
		{
			m_iLength = Length;
			m_pData = new char[m_iLength];
			memcpy(m_pData, Data, m_iLength);
		}
	}

	template<class T>
	bool Read(int place, T& output) const
	{
		if((place >= 0) && ((int)(place + sizeof(T)) <= m_iLength))
		{
			memcpy(&output, &m_pData[place], sizeof(T));
			return true;
		}
		return false;
	}


	bool Read(int place, const char*& output) const
	{
		if((place >= 0) && (place < m_iLength))
		{
			output = &m_pData[place];
			return true;
		}
		return false;
	}

	template<class T>
	bool Write(int place, T& input)
	{
		if((place >= 0) && ((int)(place + sizeof(T)) <= m_iLength))
		{
			memcpy(&m_pData[place], &input, sizeof(T));
			return true;
		}
		else if(place == m_iLength)
		{
			AddData((char*)&input, sizeof(T));
		}
		return false;
	}

	bool Write(int place, const char* input, int size)
	{
		if((place >= 0) && (place + size <= m_iLength) && input)
		{
			memcpy(&m_pData[place], input, size);
			return true;
		}
		else if(place == m_iLength)
		{
			AddData(input, size);
		}
		return false;
	}

	void AddData(const char* Data, int Length)
	{
		if(IsValid() && Data && (Length > 0))
		{
			int beforelength = m_iLength;
			CHARSTORAGE newplace = new char[m_iLength + Length];
			memcpy(newplace, m_pData, m_iLength);
			memcpy(&newplace[m_iLength], Data, Length);
			m_pData = newplace;
			m_iLength = beforelength + Length;
		}
		else
		{
			SetData(Data, Length);
		}
	}

	const int  GetType()  const                           {return m_iType;} 
	const int  GetDataLength()    const                   {return m_iLength;} 
	const char* GetData()const                            {return m_pData;}	
	CHARSTORAGE GetDataPtr()                              {return m_pData;}

	void CopyData(char* dest, int Length) const
	{
		if((Length > 0) && dest)
		{
			memcpy(dest, m_pData, Length > m_iLength? m_iLength: Length);
		}
	}

	int WritableSize() const
	{
		return (m_iLength + MetaDataLength());
	}

	int MetaDataLength() const
	{
		return (int)(sizeof(m_iLength) + sizeof(m_iType));
	}

	void WriteTo(char* destination, int length) const
	{
		if(destination && (length >= WritableSize()))
		{
			int itr = 0;
			memcpy(&destination[itr], &m_iType, sizeof(m_iType));
			itr += sizeof(m_iType);

			memcpy(&destination[itr], &m_iLength, sizeof(m_iLength));
			itr += sizeof(m_iLength);

			if(m_iLength > 0)
			{
				memcpy(&destination[itr], m_pData, m_iLength);
				itr += m_iLength;
			}
		}
	}

	void ReadFrom(const char* source, int length)
	{
		Reset();
		if(source && (length >= WritableSize()))
		{
			int itr = 0;
			memcpy(&m_iType, &source[itr],sizeof(m_iType));
			itr += sizeof(m_iType);

			memcpy(&m_iLength, &source[itr], sizeof(m_iLength));
			itr += sizeof(m_iLength);

			if((m_iLength > 0) && (m_iLength < (length - itr)))
			{
				SetData(&source[itr], m_iLength);
				itr += m_iLength;
			}
			else
			{
				m_pData = 0;
				m_iLength = 0;
			}
		}
	}

	CHARSTORAGE ConvertIntoCharBuffer() const
	{
		CHARSTORAGE s( new char[WritableSize()]);
		WriteTo(s, WritableSize());
		return s;
	}

	void Reset()
	{
		if(m_pData)
		{
			//delete []m_pData;
			m_pData = 0;
		}
		m_iLength = 0;
	}


	void Print() const
	{
		std::cout<<std::endl<<"TYPE : "<<m_iType<<std::endl;
		std::cout<<"LENGTH : "<<m_iLength<<std::endl;
		for(int i = 0; i < m_iLength; i++)
		{
			std::cout<<m_pData[i]<<" ";
		}
		std::cout<<std::endl<<std::endl;
	}
};



#endif