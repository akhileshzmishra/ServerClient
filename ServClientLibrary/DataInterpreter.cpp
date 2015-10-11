#include "DataInterpreter.h"
#include "ServClientCommDI.h"

DataInterpreter::DataInterpreter(char* msg, int len):
m_Message(msg),
m_MessLength(len)
{
}

DataInterpreter::~DataInterpreter(void)
{
}

int DataInterpreter::GetType()
{
	int type = 0;
	if(m_Message && (m_MessLength > 0))
	{
		memcpy((char*)(&type), m_Message, m_MessLength > sizeof(int)?   sizeof(int):m_MessLength);
	}
	return type;
}

void DataInterpreter::SetType(int type)
{
	if(m_Message && (m_MessLength > 0))
	{
		memcpy(m_Message, (char*)(&type), m_MessLength > sizeof(int)?   sizeof(int):m_MessLength);
	}
}

void ServClientCommDI::SetData(int& data, int dataIdentifier)
{
	SetData((char*)data, (int)sizeof(int),dataIdentifier);
}



//FACTORY
DataInterpreterFactor::DataInterpreterFactor()
{
}
DataInterpreterFactor::~DataInterpreterFactor()
{
}
DataInterpreterAutoPtr DataInterpreterFactor::GetInterpreter(char* msg, int len, int type)
{
	DataInterpreter* iptr = 0;
	switch(type)
	{
	case Normal_DataInterpreterType:
		iptr = new DataInterpreter(msg, len);
		break;
	case ServerClientComm_DataInterpreterType:
		iptr = new ServClientCommDI(msg, len);
		break;
	default:
		iptr = new DataInterpreter(msg, len);
		break;
	}

	return DataInterpreterAutoPtr(iptr);
}

int DataInterpreterFactor::GetInterpreterType(char* msg, int len)
{
	//do not use it for now
	int type = 0;
	if(msg && (len > 0))
	{
		memcpy((char*)(&type), msg, len > sizeof(int)?   sizeof(int):len);
	}
	return type;
}
