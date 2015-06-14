#ifndef LOCK_FREE_QUEUE_H_
#define LOCK_FREE_QUEUE_H_
#include <list>

namespace XDS
{
//single reader single writer lock free queue
template <class T>
class XLLFQ
{
	typedef std::list<T>                IQ;
	typedef typename IQ::iterator       IOItr;
	IQ                                  mQ;
	IOItr                               mHead;
	IOItr                               mTail;
	unsigned int                        mMaxSize;
public:
	XLLFQ():
	mMaxSize(unsigned int(-1))
	{
		mQ.push_back(T());
		mHead = mQ.begin();
		mTail = mQ.end();
	}

	void SetMaxSize(int size)
	{
		mMaxSize = (unsigned int)size;
	}

	~XLLFQ()
	{
	}


	bool push(T& data)
	{
		mQ.erase(mQ.begin(), mHead);
		if(mMaxSize < mQ.size())
		{			
			return false;
		}
		mQ.push_back(data);
		mTail = mQ.end();	
		return true;
	}

	bool pop(T& data)
	{
		IOItr next = mHead;
		++next;
		if(next != mTail)
		{
			mHead = next;
			data = *mHead;
			return true;
		}
		return false;
	}

};

}; //XDS

#endif