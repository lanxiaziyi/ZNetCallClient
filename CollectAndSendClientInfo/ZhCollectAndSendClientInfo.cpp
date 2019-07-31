//#include "../stdafx.h"
#include "ZhCollectAndSendClientInfo.h"

//#include "../ZHReadOnlyConfigSettings.h"
//#include "../ZhCustomSendModule.h"
//
//#include <atlstr.h> 
//#include "../settings.h"
#include <thread>
#include <sstream>
//#include "../Rijndael.h"

//extern AccountSettings accountSettings;

ZhCollectAndSendClientInfo *ZhCollectAndSendClientInfo::m_pInstance = NULL;
ZhCollectAndSendClientInfo::CGarbo ZhCollectAndSendClientInfo::Garbo;
ZhCollectAndSendClientInfo::ZhCollectAndSendClientInfo()
{
	curl_global_init(CURL_GLOBAL_DEFAULT);//
	m_pDeviceInfo = new ZhWindowsDeviceInfo();
	

}


ZhCollectAndSendClientInfo::~ZhCollectAndSendClientInfo()
{
	delete m_pDeviceInfo;
	m_pDeviceInfo = NULL;
	curl_global_cleanup();//
}

void ZhCollectAndSendClientInfo::toSendHeartBeat()
{
	std::thread t_thread([this](){

		std::string t_outInfo = getHeartbeatJson();

		//这里将json数据发送出去
		std::string t_outStr = "";
		string t_postInfo = "";
		t_postInfo += "data=";
		t_postInfo += t_outInfo;
		m_httpsHeartbeat.request_post("https://test.k400.cc/Home/api/heart_beat", t_postInfo, t_outStr);
		//这里可以写一个处理 返回值的json解析函数

		//OutputDebugString("robin:ZhCollectAndSendClientInfo::toSendHeartBeat");
		int m = 99;
		m++;
	});
	t_thread.detach();

}
/*
void ZhCollectAndSendClientInfo::toSendHeartBeat2()
{
	std::thread t_thread([this](){
		//2018-12-30
		std::map<string, string> t_dataInfoMap = getHeartbeatInfoMap();
		std::string t_willSendData = ZhCustomSendModule::s_createWillSendData(t_dataInfoMap,
																			ZhCustomSendModule::GetInstance()->getKey());

		std::string t_strHeartBeatUrl = "https://testservice.k400.cc/v1/client/test";
		std::string t_strHeartBeatRtn;
		ZhHttpsClient t_httpClient;
		bool t_ok = t_httpClient.request_post(t_strHeartBeatUrl, t_willSendData, t_strHeartBeatRtn);
		if (!t_ok)
		{
			return ;
		}
		//这里我们不关心心跳函数的返回值
		//这里可以写一个处理 返回值的json解析函数

		//OutputDebugString("robin:ZhCollectAndSendClientInfo::toSendHeartBeat");
		int m = 99;
		m++;
	});
	t_thread.detach();
}
*/
ZhCollectAndSendClientInfo * ZhCollectAndSendClientInfo::GetInstance()
{
	if (NULL == m_pInstance)
	{
		
		m_pInstance = new ZhCollectAndSendClientInfo();
		
	}
	return m_pInstance;
}

int ZhCollectAndSendClientInfo::tryToSend(std::string regCall_Id)
{
	if (regCall_Id.length() == 0)
	{
		return -1;
	}
	if (0 == m_strRegCallId.compare(regCall_Id))
	{//如果相等，则是已经发送过了
		return 2;
	}

	m_strRegCallId = regCall_Id;
	//toSend();
	toSend2();
	return 0;
}

std::string ZhCollectAndSendClientInfo::getHeartbeatJson()
{
	Json::Value root;
	Json::StreamWriterBuilder  builder;
	root["CurrentRunId"] = m_pDeviceInfo->getCurrentRunUuid();
	root["MessageType"] = "Heartbeat";
	root["DeviceUUID"] = m_pDeviceInfo->getDeviceUuid();

	string json_str = Json::writeString(builder, root);
	return json_str;
}

std::map<string, string> ZhCollectAndSendClientInfo::getHeartbeatInfoMap()
{
	std::map<string, string> t_outMap;
	t_outMap.insert(make_pair("CurrentRunId", m_pDeviceInfo->getCurrentRunUuid()));
	t_outMap.insert(make_pair("MessageType", "Heartbeat"));
	t_outMap.insert(make_pair("DeviceUUID", m_pDeviceInfo->getDeviceUuid()));

	return t_outMap;
}




std::string ZhCollectAndSendClientInfo::getClientInfoJson()
{
	Json::Value root;
	Json::StreamWriterBuilder  builder;
	root["CurrentRunId"] = m_pDeviceInfo->getCurrentRunUuid();
	root["MessageType"] = "ClientInfo";
	root["DeviceUUID"] = m_pDeviceInfo->getDeviceUuid();
	//添加硬件相关的信息
	appendDeviceInfoToJson(root);
	//添加软件相关的信息
	appendSoftVersionInfoToJson(root);
	//添加坐席相关的信息
	appendSeatInfoToJson(root);
	//root.toStyledString();
	string json_str = root.toStyledString(); //Json::writeString(builder, root);
	return json_str;
}

std::map<string, string> ZhCollectAndSendClientInfo::getClientInfoMap()
{
	std::map<string, string> t_outMap;
	t_outMap.insert(make_pair("CurrentRunId", m_pDeviceInfo->getCurrentRunUuid()));
	t_outMap.insert(make_pair("MessageType", "ClientInfo"));
	t_outMap.insert(make_pair("DeviceUUID", m_pDeviceInfo->getDeviceUuid()));
	//添加硬件相关的信息
	appendDeviceInfoToMap(t_outMap);
	//添加软件相关的信息
	appendSoftVersionInfoToMap(t_outMap);
	//添加坐席相关的信息
	appendSeatInfoToMap(t_outMap);

	return t_outMap;
}


std::string ZhCollectAndSendClientInfo::getUpdateOrderInfoJson()
{
	Json::Value root;
	Json::StreamWriterBuilder  builder;
	root["DeviceUUID"] = m_pDeviceInfo->getDeviceUuid();
    root["InternalNumber"] = "robin:not finished";//to_string(ZHReadOnlyConfigSettings::GetInstance()->getCurrentVersion());//
    std::cout<<"robin:not finished";
    string json_str = root.toStyledString();
	return json_str;
}

std::map<string, string> ZhCollectAndSendClientInfo::getDeviceInfoPart()
{
	std::map<std::string, std::string> t_outMap;
	t_outMap.insert(make_pair("DeviceUUID", m_pDeviceInfo->getDeviceUuid()));
    t_outMap.insert(make_pair("InternalNumber", "robin:not finished"));//to_string(ZHReadOnlyConfigSettings::GetInstance()->getCurrentVersion())));
    t_outMap.insert(make_pair("SeatNum", "robin:not finished"));//accountSettings.account.username.GetBuffer()));
	/*
	t_outMap.insert(make_pair("DeviceUUID", "bbd98544-08a7-4c19-923a-c3de8adaba26||3731383233323038323036382020202020202020"));
	t_outMap.insert(make_pair("InternalNumber", "17"));
	t_outMap.insert(make_pair("SeatNum", "1017"));
	*/
    std::cout<<"robin:not finished";

	return t_outMap;
}


void ZhCollectAndSendClientInfo::appendDeviceInfoToJson(Json::Value &rootValue)
{
	Json::Value t_dataMap;
	
	t_dataMap["MachineGUID"] = m_pDeviceInfo->getMachineGUID();
	t_dataMap["MotherboardUuid"] = m_pDeviceInfo->getMotherboardUuid();
	t_dataMap["BaseboardSerial"] = m_pDeviceInfo->getBaseboardSerial();
	t_dataMap["ComputerName"] = m_pDeviceInfo->getComputerName();
	t_dataMap["OperatingSystem"] = m_pDeviceInfo->getOperatingSystemVersion();
	//t_dataMap["SysteminfoOrder"] = m_pDeviceInfo->getSysteminfoOrder();
	t_dataMap["HardDiskSerial"] = m_pDeviceInfo->getHardDiskSerial();

	Json::Value t_localIpValue;
	{
		std::list<std::string>t_localIps = m_pDeviceInfo->getLocalIP();
		for (auto it = t_localIps.begin(); it != t_localIps.end(); it++)
		{
			t_localIpValue.append(*it);
		}
	}

	t_dataMap["LocalIP"] = t_localIpValue;
	
	rootValue["DeviceInfo"] = t_dataMap;
}

static bool GetVersionInfo(
	LPCTSTR filename,
	int &major,
	int &minor,
	int &build,
	int &revision)
{
	DWORD   verBufferSize;
	char    verBuffer[2048];

	//  Get the size of the version info block in the file
	verBufferSize = GetFileVersionInfoSize(filename, NULL);
	if (verBufferSize > 0 && verBufferSize <= sizeof(verBuffer))
	{
		//  get the version block from the file
		if (TRUE == GetFileVersionInfo(filename, NULL, verBufferSize, verBuffer))
		{
			UINT length;
			VS_FIXEDFILEINFO *verInfo = NULL;

			//  Query the version information for neutral language
			if (TRUE == VerQueryValue(
				verBuffer,
                L"\\",
				reinterpret_cast<LPVOID*>(&verInfo),
				&length))
			{
				//  Pull the version values.
				major = HIWORD(verInfo->dwProductVersionMS);
				minor = LOWORD(verInfo->dwProductVersionMS);
				build = HIWORD(verInfo->dwProductVersionLS);
				revision = LOWORD(verInfo->dwProductVersionLS);
				return true;
			}
		}
	}

	return false;
}

static std::string getFileVersion()
{
	bool t_bOk;
	int major = 0;
	int minor = 0;
	int build = 0;
	int revision = 0;

//	CString path;
//	GetModuleFileName(NULL, path.GetBufferSetLength(MAX_PATH + 1), MAX_PATH);
//	path.ReleaseBuffer();

    TCHAR path[MAX_PATH] = {0};
    GetModuleFileName(NULL, path, MAX_PATH);



	t_bOk = GetVersionInfo(path, major, minor, build, revision);

//	CString t_cstringOut;
//	t_cstringOut.Format(_T("%d.%d.%d.%d"), major, minor, build, revision);

    std::ostringstream t_stringOut;
    t_stringOut<<to_string(major)<<"."<<to_string(minor)<<"."<<to_string(build)<<"."<<to_string(revision);


    //std::string t_out = t_cstringOut.GetBuffer();
    std::string t_out = t_stringOut.str();
	return t_out;
}

/*
static CString GetProductName()
{
	DWORD   dwSize, dwHandle;
	BYTE   *byData, *lpName;
	char   sAppName[MAX_PATH] = {0};
	CString   strName;
	unsigned   int   uLen;

	//获得版本号 
	CString path;
	GetModuleFileName(NULL, path.GetBufferSetLength(MAX_PATH + 1), MAX_PATH);
	//sprintf(sAppName, "%s.exe", AfxGetApp()->m_pszExeName);//若为MFC程序,     可用此句获得当前程序(或DLL)文件所在目录  

	dwSize = GetFileVersionInfoSize(path, &dwHandle);
	byData = new BYTE[dwSize + 10];
	memset(byData, 0, dwSize + 10);
	GetFileVersionInfo(path, NULL, dwSize, byData);

	VerQueryValue(byData, TEXT("\\StringFileInfo\\080404b0\\ProductName"), (void **)&lpName, &uLen);//这里面的路径就是rc文件中的路径

	//去掉版本号的最后一位,并将','改为'.'
	strName = lpName;
	strName.Replace(",", ".");
	strName.Replace("   ", "");

	delete[] byData;

	return strName;
}
*/

static string&   replace_all(string&   str,const   string&   old_value,const   string&   new_value)
{
   while(true)   {
       string::size_type   pos(0);
       if((pos=str.find(old_value))!=string::npos)
           str.replace(pos,old_value.length(),new_value);
       else   break;
   }
   return   str;
}

//可移植版本 wstring => string
static std::string ws2s(const std::wstring& wstr)
{
    LPCWSTR pwszSrc = wstr.c_str();
    int nLen = WideCharToMultiByte(CP_ACP, 0, pwszSrc, -1, NULL, 0, NULL, NULL);
    if (nLen == 0)
        return std::string("");

    char* pszDst = new char[nLen];
    if (!pszDst)
        return std::string("");

    WideCharToMultiByte(CP_ACP, 0, pwszSrc, -1, pszDst, nLen, NULL, NULL);
    std::string str(pszDst);
    delete[] pszDst;
    pszDst = NULL;

    return str;
}
static string GetProductName()
{
    DWORD   dwSize, dwHandle;
    BYTE   *byData;
    char   sAppName[MAX_PATH] = {0};
    //CString   strName;
    string strName;
    unsigned   int   uLen;

    //获得版本号
    //CString path;
    //GetModuleFileName(NULL, path.GetBufferSetLength(MAX_PATH + 1), MAX_PATH);
    TCHAR path[MAX_PATH] = {0};
    GetModuleFileName(NULL, path, MAX_PATH);
    //sprintf(sAppName, "%s.exe", AfxGetApp()->m_pszExeName);//若为MFC程序,     可用此句获得当前程序(或DLL)文件所在目录

    dwSize = GetFileVersionInfoSize(path, &dwHandle);
    byData = new BYTE[dwSize + 10];
    memset(byData, 0, dwSize + 10);
    GetFileVersionInfo(path, NULL, dwSize, byData);

    LPVOID lpName;

    VerQueryValue(byData, TEXT("\\StringFileInfo\\080404b0\\ProductName"), (void **)&lpName, &uLen);//这里面的路径就是rc文件中的路径

    //去掉版本号的最后一位,并将','改为'.'
    std::wstring wstrName = ((LPTSTR)lpName);

    strName = ws2s(wstrName);//lpName;

    strName = replace_all(strName,",", ".");
    strName = replace_all(strName,"   ", "");

//    strName.Replace(",", ".");
//    strName.Replace("   ", "");


    delete[] byData;

    return strName;
}





void ZhCollectAndSendClientInfo::appendSoftVersionInfoToJson(Json::Value &rootValue)
{
	Json::Value t_dataMap;
	t_dataMap["Version"] = getFileVersion();//文件显示的版本
    t_dataMap["InternalVersion"] =  "robin:not finished";//to_string(ZHReadOnlyConfigSettings::GetInstance()->getCurrentVersion());//内部版本号
	t_dataMap["SoftwareName"] = "qytx";// GetProductName().GetBuffer();;//软件名称
    t_dataMap["Edition"] = "robin:not finished";//ZHReadOnlyConfigSettings::GetInstance()->getEdition();//版本类型//是普通版本，还是定制版本
    cout<<"robin:not finished";

	rootValue["SoftwareInfo"] = t_dataMap;
}

void ZhCollectAndSendClientInfo::appendSeatInfoToJson(Json::Value &rootValue)
{
	Json::Value t_dataMap;
	t_dataMap["Reg-Call-ID"] = m_strRegCallId;//本次回话信息，签出之前都不会变
    t_dataMap["UserName"] =  "robin:not finished";//accountSettings.account.username.GetBuffer();//用户名
    t_dataMap["Site"] =  "robin:not finished";//accountSettings.account.proxy.GetBuffer();//连接的网络地址
    t_dataMap["Number"] =  "robin:not finished";//accountSettings.account.outNumber.GetBuffer();//外显号码
    t_dataMap["Usercode"] =  "robin:not finished";//accountSettings.account.usercode.GetBuffer();//id识别码
//	accountSettings.account.
    cout<<"robin:not finished";
	rootValue["SeatInfo"] = t_dataMap;
}

void ZhCollectAndSendClientInfo::appendDeviceInfoToMap(std::map<string, string> &rootMap)
{
	rootMap.insert(make_pair("DeviceInfo[MachineGUID]", m_pDeviceInfo->getMachineGUID()));
	rootMap.insert(make_pair("DeviceInfo[MotherboardUuid]", m_pDeviceInfo->getMotherboardUuid()));
	rootMap.insert(make_pair("DeviceInfo[BaseboardSerial]", m_pDeviceInfo->getBaseboardSerial()));
	rootMap.insert(make_pair("DeviceInfo[ComputerName]", m_pDeviceInfo->getComputerName()));
	rootMap.insert(make_pair("DeviceInfo[OperatingSystem]", m_pDeviceInfo->getOperatingSystemVersion()));
	rootMap.insert(make_pair("DeviceInfo[HardDiskSerial]", m_pDeviceInfo->getHardDiskSerial()));

	//下面添加 DeviceInfo[LocalIP][] 的值
	std::list<std::string>t_localIps = m_pDeviceInfo->getLocalIP();
	int i = 0;
	for (auto it = t_localIps.begin(); it != t_localIps.end(); it++)
	{
		std::string t_strKey;
		t_strKey.append("DeviceInfo[LocalIP][");
		t_strKey.append(to_string(i));
		t_strKey.append("]");
		rootMap.insert(make_pair(t_strKey, *it));
		i++;
	}
}

void ZhCollectAndSendClientInfo::appendSoftVersionInfoToMap(std::map<string, string> &rootMap)
{
	rootMap.insert(make_pair("SoftwareInfo[Version]", getFileVersion()));
    rootMap.insert(make_pair("SoftwareInfo[InternalVersion]", "robin:not finished"));//to_string(ZHReadOnlyConfigSettings::GetInstance()->getCurrentVersion())));
	rootMap.insert(make_pair("SoftwareInfo[SoftwareName]", "qytx"));
    rootMap.insert(make_pair("SoftwareInfo[Edition]","robin:not finished"));// ZHReadOnlyConfigSettings::GetInstance()->getEdition()));
}

void ZhCollectAndSendClientInfo::appendSeatInfoToMap(std::map<string, string> &rootMap)
{
	rootMap.insert(make_pair("SeatInfo[Reg-Call-ID]", m_strRegCallId));
    rootMap.insert(make_pair("SeatInfo[UserName]","robin:not finished"));// accountSettings.account.username.GetBuffer()));
    rootMap.insert(make_pair("SeatInfo[Site]","robin:not finished"));// accountSettings.account.proxy.GetBuffer()));
    rootMap.insert(make_pair("SeatInfo[Number]", "robin:not finished"));//accountSettings.account.outNumber.GetBuffer()));
    rootMap.insert(make_pair("SeatInfo[Usercode]","robin:not finished"));// accountSettings.account.usercode.GetBuffer()));
}

static int str_replace(string &str, const string &src, const string &dest)
{
	int counter = 0;
	string::size_type pos = 0;
	while ((pos = str.find(src, pos)) != string::npos) {
		str.replace(pos, src.size(), dest);
		++counter;
		pos += dest.size();
	}
	return counter;
}

static string&  replace_all_distinct(string&  str, const string& old_value, const  string&  new_value)
{
	for (string::size_type pos(0); pos != string::npos; pos += new_value.length())
	{
		if ((pos = str.find(old_value, pos)) != string::npos)
		{
			str.replace(pos, old_value.length(), new_value);
		}
		else { break; }
	}
	return  str;
}

void ZhCollectAndSendClientInfo::toSend()
{
	std::thread t_thread([this](){
		m_pDeviceInfo->initValue();
		std::string t_outInfo = getClientInfoJson();

		//这里将json数据发送出去
		std::string t_outStr = "";
		string t_postInfo = "";
		t_postInfo += "data=";
		t_postInfo += t_outInfo;
		//这里替换一下，是因为我们这边的jsoncpp将特殊字符"\\"解析成了"\\\\",而接收端那里的php json解析，不能解析"\\\\"；因此只能我们这边做出改变了
		t_postInfo = replace_all_distinct(t_postInfo, "\\\\", "\\/"); //t_postInfo.replace("\\\\", "\\/");
		//bool ok = m_httpsClientInfo.request_post("https://test.k400.cc/Home/api/receive_client_info", t_postInfo, t_outStr);
		bool ok = m_httpsClientInfo.request_post("https://mirror.k400.cc/Home/api/receive_client_info", t_postInfo, t_outStr);
		//这里可以写一个处理 返回值的json解析函数


		int m = 99;
		m++;
	});
	t_thread.detach();
	
	//std::string t_outInfo = getClientInfoJson();

	int i = 0;
	i++;

}

static std::string string_To_UTF8(std::string & str)
{
    int nwLen = ::MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, NULL, 0);

    wchar_t * pwBuf = new wchar_t[nwLen + 1];//一定要加1，不然会出现尾巴
    ZeroMemory(pwBuf, nwLen * 2 + 2);

    ::MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.length(), pwBuf, nwLen);

    int nLen = ::WideCharToMultiByte(CP_UTF8, 0, pwBuf, -1, NULL, NULL, NULL, NULL);

    char * pBuf = new char[nLen + 1];
    ZeroMemory(pBuf, nLen + 1);

    ::WideCharToMultiByte(CP_UTF8, 0, pwBuf, nwLen, pBuf, nLen, NULL, NULL);

    std::string retStr(pBuf);

    delete []pwBuf;
    delete []pBuf;

    pwBuf = NULL;
    pBuf = NULL;
    return retStr;

}
static std::string UTF8_To_string(std::string & str)
{
    int nwLen = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, NULL, 0);

    wchar_t * pwBuf = new wchar_t[nwLen + 1];//一定要加1，不然会出现尾巴
    memset(pwBuf, 0, nwLen * 2 + 2);

    MultiByteToWideChar(CP_UTF8, 0, str.c_str(), str.length(), pwBuf, nwLen);

    int nLen = WideCharToMultiByte(CP_ACP, 0, pwBuf, -1, NULL, NULL, NULL, NULL);

    char * pBuf = new char[nLen + 1];
    memset(pBuf, 0, nLen + 1);

    WideCharToMultiByte(CP_ACP, 0, pwBuf, nwLen, pBuf, nLen, NULL, NULL);

    std::string retStr = pBuf;
    delete []pBuf;
    delete []pwBuf;
    pBuf = NULL;
    pwBuf = NULL;
    return retStr;

}

void ZhCollectAndSendClientInfo::toSend2()
{
	std::thread t_thread([this](){
		m_pDeviceInfo->initValue();
		//std::string t_outInfo = getClientInfoJson();
		//2018-1230
		std::map<string, string> t_clientInfoMap = getClientInfoMap();
        std::string t_willSendData = "robin:not finished";//ZhCustomSendModule::s_createWillSendData(t_clientInfoMap,
            //ZhCustomSendModule::GetInstance()->getKey());


        cout<<"robin:not finished";

        std::string t_strReceiveClientInfoUrl = "robin:not finished";//ZHReadOnlyConfigSettings::GetInstance()->getReceiveClientInfoUrl2();//"https://testservice.k400.cc/v1/client/receive-client-info";//
		std::string t_strClientInfoRtn;
		ZhHttpsClient t_httpClient;
		bool t_ok = t_httpClient.request_post(t_strReceiveClientInfoUrl, t_willSendData, t_strClientInfoRtn);
		if (!t_ok)
		{
			return ;
		}

		t_strClientInfoRtn = UTF8_To_string(t_strClientInfoRtn);//传过来的编码变一下

		//这里可以写一个处理 返回值的json解析函数
		//我们不关心返回值
		

		int m = 99;
		m++;
	});
	t_thread.detach();

}
