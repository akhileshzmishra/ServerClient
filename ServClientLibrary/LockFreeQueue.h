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
	static const int MAX_DEF = INT_MAX;
public:
	XLLFQ():
	mMaxSize(MAX_DEF)
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

	int size()
	{
		return (int)mQ.size();
	}

};

}; //XDS

#endif
