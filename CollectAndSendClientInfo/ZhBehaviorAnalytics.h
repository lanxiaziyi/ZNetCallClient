#pragma once
/************************************************************************/
/* 
Ŀ��:����������������û���Ϊͳ�Ƶ�
��Ҫ�ǽ��û��Ĳ������ݼ�¼���ϴ�����̨���ɺ�̨��������ͳ��

����:robin
ʱ��:20181115
*/
/************************************************************************/
//#include <curl/curl.h>

#include <string>
#include <map>
#include "ZhHttpsClient.h"
#include "json/json.h"
using namespace std;


class ZhBehaviorAnalytics
{

public:
	static ZhBehaviorAnalytics *GetInstance();
	void setUserLoginInfo(std::string strKey,std::string strValue); //�����ε��ã������û�����Ϣ
	//��¼�¼������ú�ᷢ�����ݵ���̨
	void trackEvent(std::string eventName);//������map("seat_status":eventName)
	void trackEvent(std::map<std::string,std::string> eventMap);
	void retrackCurrentEvent();//�ٴμ�¼��ǰ�¼���Ҳ���ǽ���ǰ��״̬�ٷ���һ�ε���̨
	void setUpdateAddress(std::string updateAddr);//�����ϴ��ĵ�ַ
private:
	void sendInfoToServer(std::map<std::string, std::string> userInfo,std::map<std::string, std::string> eventInfo);
	void sendInfoToServer_2(std::map<std::string, std::string> userInfo, std::map<std::string, std::string> eventInfo);
	std::string createWillSendJson(std::map<std::string, std::string> userInfo, std::map<std::string, std::string> eventInfo);
	void appendDataInfoToJson(Json::Value &rootValue, std::map<std::string, std::string> userInfo, std::map<std::string, std::string> eventInfo);
private:
	ZhBehaviorAnalytics();
	~ZhBehaviorAnalytics();
	
	std::map<std::string, std::string> m_mapUserInfo;
	std::map<std::string, std::string> m_eventMap;//

	std::string m_strUpdateAddr;
	//ZhHttpsClient m_httpsClient;
private:

	static ZhBehaviorAnalytics *m_pInstance;
	class CGarbo // ����Ψһ��������������������ɾ��ZHReadOnlyConfigSettings��ʵ��  
	{
	public:
		~CGarbo()
		{
			if (NULL != ZhBehaviorAnalytics::m_pInstance)
			{
				delete ZhBehaviorAnalytics::m_pInstance;
				ZhBehaviorAnalytics::m_pInstance = NULL;
			}
		}
	};
	static CGarbo Garbo; // ����һ����̬��Ա���ڳ������ʱ��ϵͳ�����������������
};

