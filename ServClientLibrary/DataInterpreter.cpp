#include "DataInterpreter.h"

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


ServClientCommDI::ServClientCommDI(char* msg, int len):
DataInterpreter(msg, len)
{
}
ServClientCommDI::~ServClientCommDI()
{
}

void ServClientCommDI::GetData(char* data, int len, int dataIdentifier)
{
	switch(dataIdentifier)
	{
	case IP:
		memcpy(data, &m_Message[IP], len > (IPTYPE - IP) ? (IPTYPE - IP): len);
		break;
	case IPTYPE:
		memcpy(data, &m_Message[IPTYPE],len > (PORT - IPTYPE) ? (PORT - IPTYPE): len);
		break;
	case PORT:
		memcpy(data, &m_Message[PORT], len > (MAXID - PORT) ? (MAXID - PORT): len);
		break;
	default:
		break;
	}
}
void ServClientCommDI::GetData(int& data, int dataIdentifier)
{
	GetData((char*)(&data), (int)sizeof(int), dataIdentifier);
}

void ServClientCommDI::SetData(char* data, int len, int dataIdentifier)
{
	switch(dataIdentifier)
	{
	case IP:
		memcpy(&m_Message[IP], data, len > (IPTYPE - IP) ? (IPTYPE - IP): len);
		break;
	case IPTYPE:
		memcpy(&m_Message[IPTYPE], data, len > (PORT - IPTYPE) ? (PORT - IPTYPE): len);
		break;
	case PORT:
		memcpy(&m_Message[PORT], data, len > (MAXID - PORT) ? (MAXID - PORT): len);
		break;
	default:
		break;
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