#pragma once
//这里将
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
	int tryToSend(std::string regCall_Id);//尝试发送自己的信息到后台，如果发送过就不会再发了。

	void toSendHeartBeat();
	//void toSendHeartBeat2();//发送心跳的第二种方式
	std::string getUpdateOrderInfoJson();//这个json中包含了软件相关的信息
	std::map<string, string> getDeviceInfoPart();//
	std::map<string, string> getHeartbeatInfoMap();//
	std::map<string, string> getClientInfoMap();//
public:
	
private:
	ZhCollectAndSendClientInfo();
	~ZhCollectAndSendClientInfo();

	
private:
	//这里一共构造了这么些json包
	std::string getHeartbeatJson();//心跳包的json

	std::string getClientInfoJson();//这个json中包含了硬件设备信息，软件版本信息和登录坐席相关的信息。这里放到一起，是因为统计人员要求这样的

	

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

	class CGarbo // 它的唯一工作就是在析构函数中删除ZHReadOnlyConfigSettings的实例  
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
	static CGarbo Garbo; // 定义一个静态成员，在程序结束时，系统会调用它的析构函数
};

