#include "ServClientCommDI.h"

ServClientCommDI::ServClientCommDI(XLThreadEvent* field):
DataInterpreter(field, Comm_DTI)
{
}
ServClientCommDI::~ServClientCommDI()
{
}

void ServClientCommDI::t_GetData(CommContxtData* data)
{
	ReadConfigDataToBuffer(data);
}


void ServClientCommDI::t_SetData(CommContxtData* data)
{
	WriteConfigDataFromBuffer(data);
	
}

int ServClientCommDI::t_DataLength(CommContxtData* data)
{
	if(data)
	{
		return SocketConfig::GetTotalWriteLength();
	}
	return 0;
}

void ServClientCommDI::WriteConfigDataFromBuffer(CommContxtData* sc)
{
	SocketConfig* conf = dynamic_cast<SocketConfig*>(sc);
	if(conf)
	{
		//conf->WriteTo(Message(), Length());
	}
}
void ServClientCommDI::ReadConfigDataToBuffer(CommContxtData* sc)
{
	SocketConfig* conf = dynamic_cast<SocketConfig*>(sc);
	if(conf)
	{
		//conf->ReadFrom(Message(), Length());
	}
}