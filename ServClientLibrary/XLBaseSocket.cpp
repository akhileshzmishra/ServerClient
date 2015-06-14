#include "XLBaseSocket.h"
#include <windows.h>
#include <windowsx.h>
#include <winsock.h>
#include <vector>
#include <set>
#include <memory.h>
#include <iostream>
#include "CommunicationHeader.h"

using namespace std;

SocketConfig::SocketConfig():
m_HostNumber(""),
m_PortNumber(0),
m_ConnectionType(SOCK_STREAM),
m_IPFormat(AF_INET),
m_IPProtocol(IPPROTO_TCP),
m_ConnectionTries(MAX_NUM_CONN_TRIES),
m_NumberofClients(DEFAULT_SERVER_ACCEPTORS),
m_EventListener(0)
{
}

SocketConfig::SocketConfig(const char* hostNumber):
m_HostNumber(""),
m_PortNumber(0),
m_ConnectionType(SOCK_STREAM),
m_IPFormat(AF_INET),
m_IPProtocol(IPPROTO_TCP),
m_ConnectionTries(MAX_NUM_CONN_TRIES),
m_NumberofClients(DEFAULT_SERVER_ACCEPTORS),
m_EventListener(0)
{
	SetHostNumber(hostNumber);
}

SocketConfig::SocketConfig(char* buffer, int length):
m_HostNumber(""),
m_PortNumber(0),
m_ConnectionType(SOCK_STREAM),
m_IPFormat(AF_INET),
m_IPProtocol(IPPROTO_TCP),
m_ConnectionTries(MAX_NUM_CONN_TRIES),
m_NumberofClients(DEFAULT_SERVER_ACCEPTORS),
m_EventListener(0)
{
	int itr = 0;
	if(length >= GetTotalWriteLength())
	{		
		SetHostNumber(&buffer[itr]);
		itr += 20;

		memcpy( &m_PortNumber, &buffer[itr], sizeof(int));
		itr += sizeof(int);

		memcpy( &m_ConnectionType, &buffer[itr], sizeof(int));
		itr += sizeof(int);

		memcpy( &m_IPFormat, &buffer[itr], sizeof(int));
		itr += sizeof(int);

		memcpy( &m_IPProtocol, &buffer[itr], sizeof(int));
		itr += sizeof(int);

		memcpy( &m_ConnectionTries, &buffer[itr], sizeof(int));
		itr += sizeof(int);

		memcpy( &m_NumberofClients, &buffer[itr], sizeof(int));
		itr += sizeof(int);
	}
	
}



SocketConfig::~SocketConfig()
{
}

SocketConfig::SocketConfig(const SocketConfig& another):
m_HostNumber(""),
m_PortNumber(another.m_PortNumber),
m_ConnectionType(another.m_ConnectionType),
m_IPFormat(another.m_IPFormat),
m_IPProtocol(another.m_IPProtocol),
m_ConnectionTries(another.m_ConnectionTries),
m_NumberofClients(another.m_NumberofClients),
m_EventListener(another.m_EventListener)
{
	SetHostNumber(another.m_HostNumber.c_str());
}


SocketConfig& SocketConfig::operator = (const SocketConfig& another)
{
	if(this == &another)
	{
		return *this;
	}
	SetHostNumber(another.m_HostNumber.c_str());

	m_PortNumber = another.m_PortNumber;
	m_ConnectionType = another.m_ConnectionType;
	m_IPFormat = another.m_IPFormat;
	m_IPProtocol = another.m_IPProtocol;
	m_ConnectionTries = another.m_ConnectionTries;
	m_NumberofClients = another.m_NumberofClients;
	m_EventListener = another.m_EventListener;

	return *this;
}


void SocketConfig::SetHostNumber(const char* hostnumber)
{
	if(hostnumber)
	{
		int strLength = strlen(hostnumber);
		if(strLength > 0)
		{
			m_HostNumber = hostnumber;
		}
	}
}

const int SocketConfig::GetTotalWriteLength()
{
	return (20 + sizeof(int)*6);
}

std::string SocketConfig::GetHostWithPort()
{
	std::string str;
	str += GetHostNumber();
	str += ":";


	CHARSTORAGE buff = new char[100];
	memset(buff, 0 , 100);


	str += itoa(GetPortNumber(), buff, 10);
	return str;
}


void SocketConfig::SetHostWithPort(const char* hostport)
{
	if(!hostport)
	{
		return;
	}
	int lenHostPort = strlen(hostport);
	if(lenHostPort > 0 )
	{
		CHARSTORAGE storage = new char[lenHostPort+ 2];
		memset((char*)storage, 0, lenHostPort + 2);
		char* findResult = strtok((char*)hostport, ":");
		if(findResult)
		{
			SetHostNumber(findResult);
		}
		findResult = strtok((char*)hostport, " \n\t");
		if(findResult)
		{
			int port = atoi(findResult);
			SetPortNumber(port);
		}
	}
}

void SocketConfig::WriteToSend(char* buffer, int length)
{
	if(length < GetTotalWriteLength())
	{
		return;
	}
	int itr = 0;
	memset(buffer, 0, 20);
	memcpy(&buffer[itr], GetHostNumber(), strlen(GetHostNumber()));
	itr += 20;

	memcpy(&buffer[itr], &m_PortNumber, sizeof(int));
	itr += sizeof(int);

	memcpy(&buffer[itr], &m_ConnectionType, sizeof(int));
	itr += sizeof(int);

	memcpy(&buffer[itr], &m_IPFormat, sizeof(int));
	itr += sizeof(int);

	memcpy(&buffer[itr], &m_IPProtocol, sizeof(int));
	itr += sizeof(int);

	memcpy(&buffer[itr], &m_ConnectionTries, sizeof(int));
	itr += sizeof(int);

	memcpy(&buffer[itr], &m_NumberofClients, sizeof(int));
	itr += sizeof(int);
}


void SocketConfig::Print()
{
	cout<<"Host: "<<GetHostNumber()<<endl;
	cout<<"Port: "<<m_PortNumber<<endl;
}



XLBaseSocket::XLBaseSocket(SocketConfig& config, bool IsServerMode):
m_bServerMode(IsServerMode),
m_Config(config),
m_bActive(false)
{
}



XLBaseSocket::XLBaseSocket():
m_bServerMode(true),
m_bActive(false)
{
}


XLBaseSocket::~XLBaseSocket(void)
{
}