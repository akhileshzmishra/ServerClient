#ifndef _SERVCLIENTCOMMDI_H__
#define _SERVCLIENTCOMMDI_H__


#include "DataInterpreter.h"
#include "XLSocket.h"

class ServClientCommDI: public DataInterpreter
{
public:
	enum
	{
		IP = sizeof(int),
		IPTYPE = IP + 6,
		PORT = IPTYPE + sizeof(int),

		MAXID
	};
	ServClientCommDI(char* msg, int len);
	~ServClientCommDI();

	void GetData(char* data, int len, int dataIdentifier);
	void GetData(int& data, int dataIdentifier);

	void SetData(char* data, int len, int dataIdentifier);
	void SetData(int& data, int dataIdentifier);

};


#endif
