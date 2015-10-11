#include "ServClientCommDI.h"



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
