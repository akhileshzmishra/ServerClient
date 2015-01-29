#include "XLSocket.h"
#include <string.h>
#include <iostream>
#include "XLServer.h"
#include "XLClient.h"
#include "XLAutoPtr.h"
using namespace std;
using namespace XLServerClientModel;

#if 0
int main()
{
	typedef XLAutoPtr::XLSharedSmPtr<int> SM;
	SM a = new int (10);
	SM b = a;
	SM c;
	c = b;
	c = new int (15);
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
		if(strcmp(argc[counter],  "-x") == 0)
		{
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