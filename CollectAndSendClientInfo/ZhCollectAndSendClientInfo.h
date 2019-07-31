#pragma once
//���ｫ
#include <iostream>
#include <string>
using namespace std;
#include "../../jsoncpp/include/json/json.h"
#include "ZhWindowsDeviceInfo.h"
#include <string>
#include "ZhHttpsClient.h"

class ZhCollectAndSendClientInfo
{
public:
	static ZhCollectAndSendClientInfo *GetInstance();
	//
	int tryToSend(std::string regCall_Id);//���Է����Լ�����Ϣ����̨��������͹��Ͳ����ٷ��ˡ�

	void toSendHeartBeat();
	//void toSendHeartBeat2();//���������ĵڶ��ַ�ʽ
	std::string getUpdateOrderInfoJson();//���json�а����������ص���Ϣ
	std::map<string, string> getDeviceInfoPart();//
	std::map<string, string> getHeartbeatInfoMap();//
	std::map<string, string> getClientInfoMap();//
public:
	
private:
	ZhCollectAndSendClientInfo();
	~ZhCollectAndSendClientInfo();

	
private:
	//����һ����������ôЩjson��
	std::string getHeartbeatJson();//��������json

	std::string getClientInfoJson();//���json�а�����Ӳ���豸��Ϣ������汾��Ϣ�͵�¼��ϯ��ص���Ϣ������ŵ�һ������Ϊͳ����ԱҪ��������

	

private:
	void appendDeviceInfoToJson(Json::Value &rootValue);
	void appendSoftVersionInfoToJson(Json::Value &rootValue);
	void appendSeatInfoToJson(Json::Value &rootValue);

	void appendDeviceInfoToMap(std::map<string, string> &rootMap);
	void appendSoftVersionInfoToMap(std::map<string, string> &rootMap);
	void appendSeatInfoToMap(std::map<string, string> &rootMap);

	void toSend();
	void toSend2();
public:


private:
	ZhWindowsDeviceInfo* m_pDeviceInfo = NULL;
	std::string m_strRegCallId;//
	ZhHttpsClient m_httpsClientInfo;
	ZhHttpsClient m_httpsHeartbeat;

	static ZhCollectAndSendClientInfo *m_pInstance;

	class CGarbo // ����Ψһ��������������������ɾ��ZHReadOnlyConfigSettings��ʵ��  
	{
	public:
		~CGarbo()
		{
			if (NULL != ZhCollectAndSendClientInfo::m_pInstance)
			{
				delete ZhCollectAndSendClientInfo::m_pInstance;
				ZhCollectAndSendClientInfo::m_pInstance = NULL;
			}
		}
	};
	static CGarbo Garbo; // ����һ����̬��Ա���ڳ������ʱ��ϵͳ�����������������
};

