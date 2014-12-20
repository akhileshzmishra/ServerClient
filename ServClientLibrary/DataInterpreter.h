#ifndef DATA_INTERPRETER_H_
#define DATA_INTERPRETER_H_
#include <set>


enum DataInterpreterType
{
	Normal_DataInterpreterType,

	ServerClientComm_DataInterpreterType,

	Max_DataInterpreterType
};


class DataInterpreter
{
protected:
	char*  m_Message;
	int    m_MessLength;

public:
	DataInterpreter(char* msg, int len);
	virtual ~DataInterpreter(void);

	int GetType();
	void SetType(int type);

	virtual void GetData(char* data, int len, int dataIdentifier){}
	virtual void GetData(int& data, int dataIdentifier){}
	virtual void GetData(double& data, int dataIdentifier){}

	virtual void SetData(char* data, int len, int dataIdentifier){}
	virtual void SetData(int& data, int dataIdentifier){}
	virtual void SetData(double& data, int dataIdentifier){}

	
};

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





typedef std::auto_ptr<DataInterpreter> DataInterpreterAutoPtr;
class DataInterpreterFactor
{
public:
	DataInterpreterFactor();
	~DataInterpreterFactor();
	static DataInterpreterAutoPtr GetInterpreter(char* msg, int len, int type = Normal_DataInterpreterType);

	static int GetInterpreterType(char* msg, int len);
};






#endif