#pragma once
#include "DataInterpreter.h"
#include "XLSocket.h"

class ServClientCommDI: public DataInterpreter
{
public:
	enum
	{
		SOCK_CONFIG,

	};
	ServClientCommDI(XLThreadEvent* field);
	~ServClientCommDI();

	
private:
	void WriteConfigDataFromBuffer(CommContxtData* sc);
	void ReadConfigDataToBuffer(CommContxtData* sc);


	//From DataInterpreter
	void t_GetData(CommContxtData* data);
	void t_SetData(CommContxtData* data);
	int t_DataLength(CommContxtData* data);

};