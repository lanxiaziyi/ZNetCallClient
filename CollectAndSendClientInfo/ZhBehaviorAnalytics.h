#pragma once
/************************************************************************/
/* 
目的:这个类是用来进行用户行为统计的
主要是将用户的操作内容记录并上传到后台，由后台进行数据统计

作者:robin
时间:20181115
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
	void setUserLoginInfo(std::string strKey,std::string strValue); //这个多次调用，传入用户的信息
	//记录事件，调用后会发送数据到后台
	void trackEvent(std::string eventName);//插入了map("seat_status":eventName)
	void trackEvent(std::map<std::string,std::string> eventMap);
	void retrackCurrentEvent();//再次记录当前事件，也就是将当前的状态再发送一次到后台
	void setUpdateAddress(std::string updateAddr);//设置上传的地址
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
	class CGarbo // 它的唯一工作就是在析构函数中删除ZHReadOnlyConfigSettings的实例  
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
	static CGarbo Garbo; // 定义一个静态成员，在程序结束时，系统会调用它的析构函数
};

