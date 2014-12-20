#include "XLSocket.h"
#include <string.h>
#include <iostream>
#include "XLServer.h"
#include "XLClient.h"
using namespace std;
using namespace XLServerClientModel;

#if 0
static int uqcliedid = -1;
static XLSocket* sock = 0;
char buff[100];
static DWORD WINAPI myServerRecThread(LPVOID param);
class mylistener: public XLSocketListener
{
public:
	void OnRecieve(char* msg, int size, int& sockID)
	{
		cout<<"RECIEVED"<<endl;
		for(int i = 0; i < size; i++)
		{
			cout<<(int)msg[i];
		}
	}

	void OnAccept(int id)
	{
		uqcliedid = id;
		cout<<"ACCEPTED id = "<<uqcliedid<<endl;
		CreateThread(NULL, 0, myServerRecThread, this, 0, NULL);
	}

	void OnConnect()
	{
		cout<<"CONNECTED"<<endl;
	}



	void OnSend(const char* msg, int size)
	{
		cout<<"SENDING ... "<<endl;
		for(int i = 0; i < size; i++)
		{
			cout<<(int)msg[i];
		}
	}
};

DWORD WINAPI myServerRecThread(LPVOID param)
{
	if(sock)
	{
		while(true)
		{
			int id = uqcliedid;
			sock->Recieve(buff, sizeof(buff), &id);
			cout<<endl<<endl<<endl<<"Recieved on server side"<<endl<<endl<<endl;
		}
	}

	return 0;
}


int main(int argv, char** argc)
{
	int counter = 1;
	
	SocketConfig config("127.0.0.1");
	config.SetPortNumber(8000);
	mylistener listener;
	config.SetEventListener(&listener);
	
	int uniqueid = 0;
	int clientpos = 20;
	int errorcode = XLSocket::RESULT_OK;
	if(argv > 1)
	{
		if(strcmp(argc[counter],  "-x") == 0)
		{
			counter++;
			if(counter < argv)
			{
				if(strcmp(argc[counter],  "server") == 0)
				{
					cout<<"Starting ...."<<endl;
					sock = new XLSocket(config, true);
					if((errorcode = sock->Start()) != XLSocket::RESULT_OK)
					{
						cout<<"Server failed to start "<<errorcode<<endl;
					}
					else
					{
						cout<<"Started ...."<<endl;
						memset(buff, 1, sizeof(buff));
						int counter = 1;
						while(true)
						{
							cout<<"Sending...."<<endl;
							memcpy(buff, (char*)(&counter), sizeof(int));
							counter++;
							sock->Send(buff, sizeof(buff), uqcliedid);
							::Sleep(2000);
						}
					}
					delete sock;
				}
				else if(strcmp(argc[counter],  "client") == 0)
				{
					cout<<"Starting ...."<<endl;
					sock = new XLSocket(config, false);
					if((errorcode = sock->Start()) != XLSocket::RESULT_OK)
					{
						cout<<"Client failed to start "<<errorcode<<endl;
					}
					else
					{
						cout<<"Revieving ...."<<endl;
						int counter = 1;
						while(true)
						{
							int uniquebuffid = 0;						
							sock->Recieve(buff, sizeof(buff), &uniquebuffid);
							memcpy(&buff[clientpos], &counter, sizeof(int));
							sock->Send(buff, sizeof(buff));
							counter += 2;
						}
					}
					delete sock;
				}
				else
				{
					cout<<"Bad output"<<endl;
				}
			}
			else
			{
				cout<<"-x should be followed by (server/client)"<<endl;
			}
		}
		else
		{
			cout<<"Please specify the type via -x"<<endl;
		}
	}

	return 0;
}


#else

int main(int argv, char** argc)
{
	int counter = 1;
	
	SocketConfig config("127.0.0.1");
	config.SetPortNumber(8000);
	bool UseSSL = true;
	std::string Certi = "cert/www.iontrading.com.crt";
	std::string pkey = "cert/www.iontrading.com.pem";
	std::string CA = "cert/root.pem";
	std::string pass = "akhilesh";
	std::string cipher = " ALL ";

	if(argv > 1)
	{
		if(strcmp(argc[counter],  "-x") == 0){
			counter++;
			if(counter < argv)
			{
				if(strcmp(argc[counter],  "server") == 0)
				{
					cout<<"Starting ...."<<endl;
					XLServer server;
					if(UseSSL)
					{
						server.GetSettingLibrary()->SetUseSSL(UseSSL);
						server.GetSettingLibrary()->SetLocalVerification(true);
						server.GetSettingLibrary()->SetRemoteVerification(false);
						server.GetSettingLibrary()->SetSSLCertificates(Certi, pkey, CA, pass);
						server.GetSettingLibrary()->SetCipherList(cipher);
					}
					server.Start();
					int id = server.StartAService(config);
					while(true)
					{
						server.Broadcast("TOM & JERRY", sizeof("TOM & JERRY"));
						::Sleep(2000);
					}
					server.Stop();
				}
				else if(strcmp(argc[counter],  "client") == 0)
				{
					cout<<"Starting ...."<<endl;
					XLClient client(config);
					if(UseSSL)
					{
						client.GetSettingLibrary()->SetUseSSL(UseSSL);
						client.GetSettingLibrary()->SetLocalVerification(false);
						client.GetSettingLibrary()->SetRemoteVerification(true);
						client.GetSettingLibrary()->SetSSLCertificates(Certi, pkey, CA, pass);
						client.GetSettingLibrary()->SetCipherList(cipher);
					}
					client.Start();
				}
				else
				{
					cout<<"Bad output"<<endl;
				}
			}
			else
			{
				cout<<"-x should be followed by (server/client)"<<endl;
			}
		}
		else
		{
			cout<<"Please specify the type via -x"<<endl;
		}
	}

	return 0;
}


#endif