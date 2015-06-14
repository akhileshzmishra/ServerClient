#ifndef XLCOMMCONTXT_DATA___H_
#define XLCOMMCONTXT_DATA___H_






class CommContxtData
{
public:
	virtual void WriteTo(char* buffer, int length) const = 0;
	virtual void ReadFrom(char* buffer, int length) = 0;
	virtual int Length() = 0;
	virtual void ResetDataStorage(int newLength)  = 0;
};









#endif