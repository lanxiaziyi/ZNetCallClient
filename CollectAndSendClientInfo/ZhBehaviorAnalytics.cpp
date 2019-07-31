//#include "../stdafx.h"
#include "ZhBehaviorAnalytics.h"
#include "../../jsoncpp/include/json/json.h"
//#include "../ZhCustomSendModule.h"
//#include "../ZHReadOnlyConfigSettings.h"
#include <thread>

ZhBehaviorAnalytics *ZhBehaviorAnalytics::m_pInstance = NULL;
ZhBehaviorAnalytics::CGarbo ZhBehaviorAnalytics::Garbo;
ZhBehaviorAnalytics * ZhBehaviorAnalytics::GetInstance()
{
	if (NULL == m_pInstance)
	{

		m_pInstance = new ZhBehaviorAnalytics();

	}
	return m_pInstance;
}

void ZhBehaviorAnalytics::setUserLoginInfo(std::string strKey, std::string strValue)
{
	if (m_mapUserInfo.find(strKey) != m_mapUserInfo.end())
	{//如果没找到就插入
		m_mapUserInfo.insert(make_pair(strKey, strValue));
	}
	else
	{//如果找到就更新相应的值
		m_mapUserInfo[strKey] = strValue;
	}
}

void ZhBehaviorAnalytics::trackEvent(std::string eventName)
{
	map<string, string> t_mapEvent;
	t_mapEvent.insert(make_pair("seat_status", eventName));

	m_eventMap = t_mapEvent;
	//sendInfoToServer(m_mapUserInfo,t_mapEvent);
	sendInfoToServer_2(m_mapUserInfo, t_mapEvent);
}

void ZhBehaviorAnalytics::trackEvent(std::map<std::string, std::string> eventMap)
{
	m_eventMap = eventMap;
	//sendInfoToServer(m_mapUserInfo, eventMap);
	sendInfoToServer_2(m_mapUserInfo, eventMap);
}

void ZhBehaviorAnalytics::retrackCurrentEvent()
{
	//sendInfoToServer(m_mapUserInfo, m_eventMap);
	sendInfoToServer_2(m_mapUserInfo, m_eventMap);

}

void ZhBehaviorAnalytics::setUpdateAddress(std::string updateAddr)
{
	m_strUpdateAddr = updateAddr;
}

void ZhBehaviorAnalytics::sendInfoToServer(std::map<std::string, std::string> userInfo, std::map<std::string, std::string> eventInfo)
{
	//
	std::thread t_thread([this, userInfo, eventInfo](){

		std::string t_outInfo = createWillSendJson(userInfo, eventInfo);
		ZhHttpsClient t_httpsClient;
		//这里将json数据发送出去
		std::string t_outStr = "";
		string t_postInfo = "";
		//t_postInfo += "data=";
		t_postInfo += t_outInfo;
		
		string t_outDebugStr = "robin:willSend:";
		t_outDebugStr += t_postInfo;
        //OutputDebugString(t_outDebugStr.c_str());
        cout<<t_outDebugStr;
		bool ok = t_httpsClient.request_post(m_strUpdateAddr, t_postInfo, t_outStr);
		if (!ok)
		{
            //OutputDebugString("robin:post event failed");
            cout<<"robin:post event failed";
        }

	});
	//t_thread.join();
	t_thread.detach();

	int i = 0;
	i++;
	
}

void ZhBehaviorAnalytics::sendInfoToServer_2(std::map<std::string, std::string> userInfo, std::map<std::string, std::string> eventInfo)
{
	std::thread t_thread([this, userInfo, eventInfo](){
	
		//2019-1-8
		std::map<string, string> t_dataInfoMap;// = userInfo + eventInfo;//getHeartbeatInfoMap();
		t_dataInfoMap.insert(make_pair("action", "Event"));
		for (auto i = userInfo.begin(); i != userInfo.end();i++)
		{
			t_dataInfoMap.insert(make_pair(i->first, i->second));
		}
		for (auto i = eventInfo.begin(); i != eventInfo.end(); i++)
		{
			t_dataInfoMap.insert(make_pair(i->first, i->second));
		}

        /*
		std::string t_willSendData = ZhCustomSendModule::s_createWillSendData(t_dataInfoMap,
			ZhCustomSendModule::GetInstance()->getKey());

		std::string t_strSeatChangeStatusUrl = ZHReadOnlyConfigSettings::GetInstance()->getBehaviorServer3();//"https://testservice.linkallchina.com/v1/client/seat-change-status";
		std::string t_strSeatChangeStatusRtn;
		ZhHttpsClient t_httpClient;
		bool t_ok = t_httpClient.request_post(t_strSeatChangeStatusUrl, t_willSendData, t_strSeatChangeStatusRtn);
		if (!t_ok)
		{
			return;
		}
        */
        cout<<"robin:not finished";


	});
	t_thread.detach();
}

std::string ZhBehaviorAnalytics::createWillSendJson(std::map<std::string, std::string> userInfo, std::map<std::string, std::string> eventInfo)
{
	Json::Value root;
	Json::StreamWriterBuilder  builder;
	root["action"] = "Event";
	appendDataInfoToJson(root, userInfo, eventInfo);
	string json_str = root.toStyledString(); //Json::writeString(builder, root);
	return json_str;
}

static string sLocaltimetoStr(){
	char tmp[64];
	time_t t = time(NULL);
	tm *_tm = localtime(&t);
	int year = _tm->tm_year + 1900;
	int month = _tm->tm_mon + 1;
	int date = _tm->tm_mday;
	int hh = _tm->tm_hour;
	int mm = _tm->tm_min;
	int ss = _tm->tm_sec;
	sprintf_s(tmp, 64, "%04d_%02d_%02d:%02d_%02d_%02d",
		year, month, date, hh, mm, ss);
	return string(tmp);
}


void ZhBehaviorAnalytics::appendDataInfoToJson(Json::Value &rootValue, std::map<std::string, std::string> userInfo, std::map<std::string, std::string> eventInfo)
{
	Json::Value t_dataMap;

	for (auto i = userInfo.begin(); i != userInfo.end(); i++)
	{
		//i->first; i->second;
		t_dataMap[i->first] = i->second;
	}

	for (auto i = eventInfo.begin(); i != eventInfo.end(); i++)
	{
		t_dataMap[i->first] = i->second;
	}
	//添加本机时间

	t_dataMap["local_time"] = sLocaltimetoStr();

	rootValue["data"] = t_dataMap;
}

ZhBehaviorAnalytics::ZhBehaviorAnalytics()
{

}


ZhBehaviorAnalytics::~ZhBehaviorAnalytics()
{
}
