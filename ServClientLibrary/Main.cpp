#include "XLSocket.h"
#include <string.h>
#include <iostream>
#include "XLServer.h"
#include "XLClient.h"
#include "XLAutoPtr.h"
#include "XLThreadPool.h"
#include "XLThreadUtility.h"
using namespace std;
using namespace XLServerClientModel;
using namespace GenThreadPool;

#if 1
class JobW : public ThreadPoolJob
{
	int i;
public:
	JobW(int x): i(x){}
	void Work()
	{
		for(int k = 0; k < 1; k++)
		{
			cout<<"Job "<<i<<endl;
			XLThreadUtility::Sleep(3000);
		}
	}

};


int main()
{
	const int numThreads = 2;
	const int numJobs = 10;

	XLThreadPool pool(numThreads, false);
	pool.Create();
	cout<<"Pool created"<<endl;

	std::vector<JobW*> jobVec;
	
	for(int i = 0; i < numJobs; i++)
	{
		jobVec.push_back(new JobW(i));
	}
	for(int i = 0; i < jobVec.size(); i++)
	{
		pool.AddJob(jobVec[i]);
	}

	//while(1)
	{
		for(int i = 0; i < jobVec.size(); i++)
		{
			pool.AddJob(jobVec[i]);
		}
		//XLThreadUtility::Sleep(2000);
		//sleep(1);
	}
	//while(1);
	//XLThreadUtility::Sleep(12000);
	pool.Destroy();
	cout<<"Pool destroyed"<<endl;
}
#else

int main(int argv, char** argc)
{
	int counter = 1;
	
	SocketConfig config("127.0.0.1");
	config.SetPortNumber(8000);
	bool UseSSL = false;
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
						XLThreadUtility::Sleep(2000);
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
