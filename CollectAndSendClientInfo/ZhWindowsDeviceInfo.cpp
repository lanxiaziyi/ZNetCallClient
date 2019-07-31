#include "ZhWindowsDeviceInfo.h"
#include <string>
#include <atlstr.h>
#include <tchar.h>

#include <cctype>
#include <iomanip>
#include <iostream>
#include <string>
#include <stdio.h>
#include "GetRegValue.h"
//#include "RegCtrl.h"
#include <versionhelpers.h>
#include <ws2tcpip.h>

#pragma comment(lib, "Rpcrt4.lib")

//#define _WINSOCK_DEPRECATED_NO_WARNINGS
//#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "Ws2_32.lib")

#include <lm.h>
#pragma comment(lib, "netapi32.lib")

#include<shlwapi.h>

ZhWindowsDeviceInfo::ZhWindowsDeviceInfo()
{
	//initValue();//为了后面函数调用的时候能快一点，就写的麻烦一些了
}


ZhWindowsDeviceInfo::~ZhWindowsDeviceInfo()
{
	int i = 0;
	i++;
}

static std::string getAppDirPath()
{
    /*
	CString path;
	GetModuleFileName(NULL, path.GetBufferSetLength(MAX_PATH + 1), MAX_PATH);
	path.ReleaseBuffer();
	int pos = path.ReverseFind('\\');
	path = path.Left(pos);
	return path.GetBuffer();
    */

    TCHAR path[MAX_PATH] = {0};
    GetModuleFileName(NULL, path, MAX_PATH);
    std::wstring t_wstrPath = path;
    std::string t_strPath = ws2s(t_wstrPath);
    return t_strPath;


}

std::string ZhWindowsDeviceInfo::getCurrentRunUuid()
{
	if (0 == m_strCurRunUuid.length())
	{
		createUUID(m_strCurRunUuid);
	}

	return m_strCurRunUuid;
}

std::string ZhWindowsDeviceInfo::getDeviceUuid()
{
	std::string t_strOut;
	t_strOut += getMachineGUID();
	t_strOut += "||";
	t_strOut += getHardDiskSerial();
	return t_strOut;
}

std::string ZhWindowsDeviceInfo::getHardDiskSerial()
{
	if (0 == m_strHardDiskSerial.length())
	{
		initHardDiskSerial(m_strHardDiskSerial);
	}
	return m_strHardDiskSerial;
}

std::string ZhWindowsDeviceInfo::getMachineGUID()
{
	if (0 == m_strMachineGUID.length())
	{
		initMachineGUID(m_strMachineGUID);
	}

	return m_strMachineGUID;
}

bool ZhWindowsDeviceInfo::createUUID(std::string &strGUID)
{
	GUID guid;
	HRESULT hr = ::UuidCreate(&guid);
	if (HRESULT_CODE(hr) != RPC_S_OK
		|| guid == GUID_NULL
		|| HRESULT_CODE(hr) == RPC_S_UUID_NO_ADDRESS)
	{
		return FALSE;
	}
	CString strGUIDC;

	//strGUID.Format(_T("{%08X-%04X-%04x-%02X%02X-%02X%02X%02X%02X%02X%02X}"), //有大括号
	strGUIDC.Format(_T("%08X-%04X-%04x-%02X%02X-%02X%02X%02X%02X%02X%02X"), //无大括号
		guid.Data1, guid.Data2, guid.Data3,
		guid.Data4[0], guid.Data4[1], guid.Data4[2], guid.Data4[3],
		guid.Data4[4], guid.Data4[5], guid.Data4[6], guid.Data4[7]);

	strGUIDC.GetString();

	strGUID = CT2A(strGUIDC.GetBuffer());

	return TRUE;

}

int ZhWindowsDeviceInfo::initValue()
{
	if (m_strCurRunUuid.length() > 0)
	{
		return 1;
	}

	createUUID(m_strCurRunUuid);
	initMotherboardUuid(m_strMotherboardUuid);//主板的uuid
	initBaseboardSerial(m_strBaseboardSerial);//另一个序列号，我也不知道跟主板的uuid有什么不同，
	initComputerName(m_strComputerName);//设备名
	initHardDiskSerial(m_strHardDiskSerial);//硬盘序列号，只会获取第一块硬盘的
	initMachineGUID(m_strMachineGUID);//这个值重装系统才会改变。ghost有没有影响暂时未知
	initLocalIP(m_listLocalIP);//获取内网ip（可能有多个,虚拟机啥的）
	initOperatingSystemVersion(m_strOperatingSystemVersion);//获取操作系统版本
	initOperatingSystemBits(m_iOperatingSystemBits);//获取操作系统的位数.32位还是64位
	initSysteminfoOrder(m_strSysteminfoOrder);//获取本机的信息，时间有点长

	return 0;
}

static DWORD RunSilent(const char* strFunct, const char* strstrParams)
{
	STARTUPINFO StartupInfo;
	PROCESS_INFORMATION ProcessInfo;
    char Args[4096] = {0};
	char *pEnvCMD = NULL;
    const char *pDefaultCMD = "CMD.EXE";
	ULONG rc;

	memset(&StartupInfo, 0, sizeof(StartupInfo));
	StartupInfo.cb = sizeof(STARTUPINFO);
	StartupInfo.dwFlags = STARTF_USESHOWWINDOW;
	StartupInfo.wShowWindow = SW_HIDE;

	Args[0] = 0;

	//pEnvCMD = getenv("COMSPEC");
	size_t t_requiredSize;
	//errno_t t_err = _dupenv_s(&pEnvCMD,&len,"COMSPEC");
	//获取cmd的路径
	
	getenv_s(&t_requiredSize, NULL, 0, "COMSPEC");
	if (0 == t_requiredSize)
	{
		printf("robin:not found this comspec\n");
		return -1;
	}
	pEnvCMD = (char *)malloc(t_requiredSize * sizeof(char));
	getenv_s(&t_requiredSize, pEnvCMD, t_requiredSize, "COMSPEC");



	if (pEnvCMD){

		//strcpy(Args, pEnvCMD);
		strcpy_s(Args, pEnvCMD);
	}
	else{
		//strcpy(Args, pDefaultCMD);
		strcpy_s(Args, pDefaultCMD);
	}

	// "/c" option - Do the command then terminate the command window
	//strcat(Args, " /c ");
	strcat_s(Args, " /c ");
	//the application you would like to run from the command window
	//strcat(Args, strFunct);
	//strcat(Args, " ");
	strcat_s(Args, strFunct);
	strcat_s(Args, " ");
	//the parameters passed to the application being run from the command window.
	strcat_s(Args, strstrParams);

    string t_strArgs = Args;
    wstring t_wstrArgs = s2ws(t_strArgs);
    LPWSTR t_wcAprgs = (LPWSTR)t_wstrArgs.c_str();
    //if (!CreateProcess(NULL, Args, NULL, NULL, FALSE,
    if (!CreateProcess(NULL, t_wcAprgs, NULL, NULL, FALSE,
		CREATE_NEW_CONSOLE,
		NULL,
		NULL,
		&StartupInfo,
		&ProcessInfo))
	{
		return GetLastError();
	}

	WaitForSingleObject(ProcessInfo.hProcess, INFINITE);
	if (!GetExitCodeProcess(ProcessInfo.hProcess, &rc))
		rc = 0;

	CloseHandle(ProcessInfo.hThread);
	CloseHandle(ProcessInfo.hProcess);

	free(pEnvCMD);
	return rc;

}


bool ZhWindowsDeviceInfo::initMotherboardUuid(std::string &strUuid)
{
	strUuid = "FFFFFFFF-FFFF-FFFF-FFFF-FFFFFFFFFFFF";//当获取失败时，就会得到这个值
	// 改写的下面的例子:http://www.cplusplus.com/forum/windows/210905/
	//这里会有一个问题，会生成一个文件
	
	//if (std::system("wmic csproduct get uuid > test.txt") != 0)
	//{
	//	return false;
	//}

	std::string t_outputFilePath;
	t_outputFilePath += getAppDirPath();
	t_outputFilePath += "\\res1\\test_mother.txt";
	std::string t_infoOrder;
	t_infoOrder += "csproduct get uuid > ";
	t_infoOrder += t_outputFilePath;
	//DWORD t_drtn = RunSilent("wmic", "csproduct get uuid > test.txt");
	DWORD t_drtn = RunSilent("wmic", t_infoOrder.c_str());
	if (0 != t_drtn)
	{
        OutputDebugString(L"robin:RunSilent failed initMotherboardUuid");
		return false;
	}


	//auto file = ::fopen_s("test.txt", "rt, ccs=UNICODE"); // open the file for unicode input
	FILE *file = NULL;
	//errno_t t_rtn = ::fopen_s(&file, "test.txt", "rt, ccs=UNICODE");
	errno_t t_rtn = ::fopen_s(&file, t_outputFilePath.c_str(), "rt, ccs=UNICODE");
	if (0!= t_rtn)
	{
        OutputDebugString(L"robin:fopen_s failed initMotherboardUuid");
		return false;
	}

	enum { BUFFSZ = 1000, UUID_SZ = 36 };
	wchar_t wbuffer[BUFFSZ]; // buffer to hold unicode characters

	if (file && // file was succesffully opened
		::fgetws(wbuffer, BUFFSZ, file) && // successfully read (and discarded) the first line
		::fgetws(wbuffer, BUFFSZ, file)) // successfully read the second line
	{
		char cstr[BUFFSZ]; // buffer to hold the converted c-style string
		size_t   i = 0;
		size_t t_max = _TRUNCATE;
		errno_t t_rtn = ::wcstombs_s(&i, cstr, BUFFSZ, wbuffer, _TRUNCATE);
		//if (::wcstombs(cstr, wbuffer, BUFFSZ) > UUID_SZ) // convert unicode to utf-8
		if (0 == t_rtn)
		{
			std::string uuid = cstr;
			while (!uuid.empty() && std::isspace(uuid.back()))
			{
				uuid.pop_back(); // discard trailing white space
			}
			strUuid = uuid;
			fclose(file);
			return true;
		}
	}
	else
	{
        OutputDebugString(L"robin:open MotherboardUuid file failed");
	}
	fclose(file);
	

	return false;
}

bool ZhWindowsDeviceInfo::initBaseboardSerial(std::string &strSerial)
{
	strSerial = "FFFFFFFFFFFFF";//当获取失败时，就会得到这个值
	// 改写的下面的例子:http://www.cplusplus.com/forum/windows/210905/
	//这里会有一个问题，会生成一个文件
	//if (std::system("wmic baseboard get SerialNumber > test.txt") != 0)
	//{
	//	return false;
	//}

	std::string t_outputFilePath;
	t_outputFilePath += getAppDirPath();
	t_outputFilePath += "\\res1\\test_baseboard.txt";
	std::string t_infoOrder;
	t_infoOrder += "baseboard get SerialNumber > ";
	t_infoOrder += t_outputFilePath;


	//DWORD t_drtn = RunSilent("wmic", "baseboard get SerialNumber > test.txt");
	DWORD t_drtn = RunSilent("wmic", t_infoOrder.c_str());
	if (0 != t_drtn)
	{
        OutputDebugString(L"robin:RunSilent failed initBaseboardSerial");
		return false;
	}

	//auto file = ::fopen_s("test.txt", "rt, ccs=UNICODE"); // open the file for unicode input
	FILE *file = NULL;
	//errno_t t_rtn = ::fopen_s(&file, "test.txt", "rt, ccs=UNICODE");
	errno_t t_rtn = ::fopen_s(&file, t_outputFilePath.c_str(), "rt, ccs=UNICODE");
	if (0 != t_rtn)
	{
        OutputDebugString(L"robin:fopen_s failed initBaseboardSerial");
		return false;
	}

	enum { BUFFSZ = 1000, UUID_SZ = 36 };
	wchar_t wbuffer[BUFFSZ]; // buffer to hold unicode characters

	if (file && // file was succesffully opened
		::fgetws(wbuffer, BUFFSZ, file) && // successfully read (and discarded) the first line
		::fgetws(wbuffer, BUFFSZ, file)) // successfully read the second line
	{
		char cstr[BUFFSZ]; // buffer to hold the converted c-style string
		size_t   i;
		errno_t t_rtn = ::wcstombs_s(&i, cstr, BUFFSZ, wbuffer, _TRUNCATE);
		//if (::wcstombs(cstr, wbuffer, BUFFSZ) > UUID_SZ) // convert unicode to utf-8
		if (0 == t_rtn)
		{
			std::string uuid = cstr;
			while (!uuid.empty() && std::isspace(uuid.back()))
			{
				uuid.pop_back(); // discard trailing white space
			}
			strSerial = uuid;
			fclose(file);
			return true;
		}
	}
	fclose(file);
	

	return false;
}

/*
//////////////////////////////////////////////////(UNICODE 环境)
static std::string TCHAR2STRING(TCHAR *STR)
{

	int iLen = WideCharToMultiByte(CP_ACP, 0, STR, -1, NULL, 0, NULL, NULL);

	char* chRtn = new char[iLen*sizeof(char)];

	WideCharToMultiByte(CP_ACP, 0, STR, -1, chRtn, iLen, NULL, NULL);

	std::string str(chRtn);

	delete chRtn;

	return str;
}
*/


//////////////////////////////////////////////////(UNICODE 环境)
static std::string TCHAR2STRING(TCHAR *STR)
{

    int iLen = WideCharToMultiByte(CP_ACP, 0, STR, -1, NULL, 0, NULL, NULL);

    char* chRtn = new char[iLen*sizeof(char)];

    WideCharToMultiByte(CP_ACP, 0, STR, -1, chRtn, iLen, NULL, NULL);

    std::string str(chRtn);

    delete chRtn;

    return str;
}


bool ZhWindowsDeviceInfo::initComputerName(std::string &strName)
{
	TCHAR buffer[256] = TEXT("");
	DWORD dwSize = sizeof(buffer);
	if (!GetComputerNameEx((COMPUTER_NAME_FORMAT)1, buffer, &dwSize))
	{
		return false;
	}
    strName = TCHAR2STRING(buffer);
    //strName = buffer;
	//注意 printf里是大写的S。。
	//printf("buffer= %S \n",buffer);
	//sprintf(ComputerName, "%s", buffer);
	//printf("ComputerName==%s\n",ComputerName);
	dwSize = _countof(buffer);
	ZeroMemory(buffer, dwSize);

	return true;
}



bool ZhWindowsDeviceInfo::initHardDiskSerial(std::string &strFirstHDSerial)
{

	strFirstHDSerial = "FFFFFFFFFFFFFFF";//当获取失败时，就会得到这个值
	// 改写的下面的例子:http://www.cplusplus.com/forum/windows/210905/
	//这里会有一个问题，会生成一个文件
	//if (std::system("wmic DISKDRIVE get SerialNumber > test.txt") != 0)
	//{
	//	return false;
	//}
	std::string t_outputFilePath;
	t_outputFilePath += getAppDirPath();
	t_outputFilePath += "\\res1\\test_harddisk.txt";
	std::string t_infoOrder;
	t_infoOrder += "DISKDRIVE get SerialNumber > ";
	t_infoOrder += t_outputFilePath;
	//DWORD t_drtn = RunSilent("wmic", "DISKDRIVE get SerialNumber > test.txt");
	DWORD t_drtn = RunSilent("wmic", t_infoOrder.c_str());
	if (0 != t_drtn)
	{
		return false;
	}

	//auto file = ::fopen_s("test.txt", "rt, ccs=UNICODE"); // open the file for unicode input
	FILE *file = NULL;
	//errno_t t_rtn = ::fopen_s(&file, "test.txt", "rt, ccs=UNICODE");
	errno_t t_rtn = ::fopen_s(&file, t_outputFilePath.c_str(), "rt, ccs=UNICODE");
	if (0 != t_rtn)
	{
		return false;
	}

	enum { BUFFSZ = 1000, UUID_SZ = 36 };
	wchar_t wbuffer[BUFFSZ]; // buffer to hold unicode characters

	if (file && // file was succesffully opened
		::fgetws(wbuffer, BUFFSZ, file) && // successfully read (and discarded) the first line
		::fgetws(wbuffer, BUFFSZ, file)) // successfully read the second line
	{
		char cstr[BUFFSZ]; // buffer to hold the converted c-style string
		size_t   i;
		errno_t t_rtn = ::wcstombs_s(&i, cstr, BUFFSZ, wbuffer, _TRUNCATE);
		//if (::wcstombs(cstr, wbuffer, BUFFSZ) > UUID_SZ) // convert unicode to utf-8
		if (0 == t_rtn)
		{
			std::string uuid = cstr;
			while (!uuid.empty() && std::isspace(uuid.back()))
			{
				uuid.pop_back(); // discard trailing white space
			}
			strFirstHDSerial = uuid;
			fclose(file);
			return true;
		}
	}
	fclose(file);



	return false;
}

bool ZhWindowsDeviceInfo::initMachineGUID(std::string &strGuid)
{
	//读取 HKEY_MACHINE\SOFTWARE\Microsoft\Cryptography 中的 MachineGuid字段值


	std::string t_strValue = GetRegValue(2, "SOFTWARE\\Microsoft\\Cryptography", "MachineGuid");

	if ( 0 == t_strValue.length())
	{
		return false;
	}

	strGuid = t_strValue;

	return true;
}
 
void Wchar_tToString(std::string& szDst, wchar_t *wchar)
{
	wchar_t * wText = wchar;
	DWORD dwNum = WideCharToMultiByte(CP_OEMCP, NULL, wText, -1, NULL, 0, NULL, FALSE);// WideCharToMultiByte的运用
	char *psText; // psText为char*的临时数组，作为赋值给std::string的中间变量
	psText = new char[dwNum];
	WideCharToMultiByte(CP_OEMCP, NULL, wText, -1, psText, dwNum, NULL, FALSE);// WideCharToMultiByte的再次运用
	szDst = psText;// std::string赋值
	delete[]psText;// psText的清除
}
bool ZhWindowsDeviceInfo::initLocalIP(std::list<std::string> &ipList)
{
	//1.初始化wsa
	WSADATA wsaData;
	int ret = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (ret != 0)
	{
		return false;
	}

	struct addrinfo *result = NULL;
	struct addrinfo *ptr = NULL;
	struct addrinfo hints;

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	char hostname[256] = { 0 };
	int dwRetval = getaddrinfo(hostname, 0, &hints, &result);
	if (dwRetval != 0)
	{
		printf("getaddrinfo response error:%d \n", dwRetval);
		return false;
	}
	else
	{
		for (ptr = result; ptr != NULL; ptr = ptr->ai_next)
		{

			//printf("getaddrinfo response %d\n", dwRetval);
			//printf("\tFlags: 0x%x\n", ptr->ai_flags);
			//printf("\tFamily: ");
			switch (ptr->ai_family) {
			case AF_UNSPEC:
				//printf("Unspecified\n");
				break;
			case AF_INET:
			{
				//printf("AF_INET (IPv4)\n");
				struct sockaddr_in *sockaddr_ipv4 = (struct sockaddr_in *) ptr->ai_addr;
				//printf("\tIPv4 address %s\n",
				//	inet_ntoa(sockaddr_ipv4->sin_addr));
				char str[INET_ADDRSTRLEN] = { 0 };
				inet_ntop(AF_INET, &sockaddr_ipv4->sin_addr, str, sizeof(str));
				//printf("\tIPv4 address %s\n", str);
				ipList.push_back(str);
			}
			break;
			case AF_INET6:
			{
				//printf("AF_INET6 (IPv6)\n");
				// the InetNtop function is available on Windows Vista and later
				// sockaddr_ipv6 = (struct sockaddr_in6 *) ptr->ai_addr;
				// printf("\tIPv6 address %s\n",
				//    InetNtop(AF_INET6, &sockaddr_ipv6->sin6_addr, ipstringbuffer, 46) );

				// We use WSAAddressToString since it is supported on Windows XP and later
				LPSOCKADDR sockaddr_ip = (LPSOCKADDR)ptr->ai_addr;
				// The buffer length is changed by each call to WSAAddresstoString
				// So we need to set it for each iteration through the loop for safety
				DWORD ipbufferlength = 100;
				//TCHAR ipstringbuffer[46] = { 0 };
				/*int iRetval = WSAAddressToString(sockaddr_ip, (DWORD)ptr->ai_addrlen, NULL,
					ipstringbuffer, &ipbufferlength);*/
				wchar_t ipstringbuffer[100] = { 0 };
				int t_rtn = WSAAddressToStringW(sockaddr_ip, (DWORD)ptr->ai_addrlen, NULL,
					ipstringbuffer, &ipbufferlength);
				int t_specError = WSAGetLastError();
				//if (iRetval)
				//	printf("WSAAddressToString failed with %u\n", WSAGetLastError());
				//else
				//	printf("\tIPv6 address %s\n", TCHAR2STRING(ipstringbuffer).c_str());
				std::string t_ipStr;
				Wchar_tToString(t_ipStr, ipstringbuffer);
				//ipList.push_back(TCHAR2STRING(ipstringbuffer));
				//ipList.push_back(t_ipStr);
				


				//CString t_ipBuffer;
				//LPSTR lpStr = t_ipBuffer.GetBuffer(100);
				//char t_ipBuffer[100] = { 0 };
				//DWORD t_trueLength = 100;
				//INT t_error = WSAAddressToString(sockaddr_ip, (DWORD)ptr->ai_addrlen, NULL, t_ipBuffer, &t_trueLength);
				//int t_specError =  WSAGetLastError();
				std::string t_strIpv6 = t_ipStr;// t_ipBuffer;
				std::size_t t_found = t_strIpv6.find("%");
				if (t_found != std::string::npos)
				{//这里的意思是去掉后面以%开头的字符串（%13，%7 ），不知道问什么，会带着这个字符
					t_strIpv6 = t_strIpv6.substr(0, t_found);
				}

				ipList.push_back(t_strIpv6);
				//t_ipBuffer.ReleaseBuffer();
				//CString t_newAddr = t_ipBuffer.Left()

				int i = 0;
				i++;


			}
			break;
			case AF_NETBIOS:
				printf("AF_NETBIOS (NetBIOS)\n");
				break;
			default:
				printf("Other %ld\n", ptr->ai_family);
				break;
			}
		}

	}

	int t_rtn = WSACleanup();



	return true;

}

//bool ZhWindowsDeviceInfo::getInternetIP(std::string &strIP)
//{
//
//	return true;
//}


//static string GetSystemName()
//{
//
//		
//}

bool ZhWindowsDeviceInfo::initOperatingSystemVersion(std::string &strVersion)
{
	//我知道可以用 IsWindows8Point1OrGreater 这种函数，但是限制条件太多了。

	DWORD dwVersion = 0;
	WKSTA_INFO_100 *wkstaInfo = NULL;
	DWORD dwMajVer = 0;
	DWORD dwMinVer = 0;
	NET_API_STATUS netStatus = NetWkstaGetInfo(NULL, 100, (BYTE **)&wkstaInfo);
	if (netStatus == NERR_Success)
	{
		dwMajVer = wkstaInfo->wki100_ver_major;
		dwMinVer = wkstaInfo->wki100_ver_minor;
		dwVersion = (DWORD)MAKELONG(dwMinVer, dwMajVer);
		NetApiBufferFree(wkstaInfo);
	}
	if (0 == dwMajVer
		&& 0 == dwMinVer)
	{
		return false;
	}
	std::string t_internalVersion;
	t_internalVersion += to_string(dwMajVer);
	t_internalVersion += ".";
	t_internalVersion += to_string(dwMinVer);

	if ("5.0" == t_internalVersion)
	{
		strVersion = "Windows 2000";
	}
	else if ("5.1" == t_internalVersion)
	{
		strVersion = "Windows XP (x86)";
	}
	else if ("5.2" == t_internalVersion)
	{
		strVersion = "Windows XP (x64)";
	}
	else if ("6.0" == t_internalVersion)
	{
		strVersion = "Windows Vista";
	}
	else if ("6.1" == t_internalVersion)
	{
		strVersion = "Windows 7";
	}
	else if ("6.2" == t_internalVersion)
	{
		strVersion = "Windows 8";
	}
	else if ("6.3" == t_internalVersion)
	{
		strVersion = "Windows 8.1";
	}
	else if ("10.0" == t_internalVersion)
	{
		strVersion = "Windows 10";
	}
	else
	{
		strVersion = "";
	}

	double t_fVersion = atof(t_internalVersion.c_str());
	if (t_fVersion > 10.0)
	{
		strVersion = "Windows 10 or greater";
	}

	return true;
}

// 获取系统信息
static void SafeGetNativeSystemInfo(__out LPSYSTEM_INFO lpSystemInfo)
{
	if (NULL == lpSystemInfo)
	{
		return;
	}
	typedef VOID(WINAPI *FuncGetSystemInfo)(LPSYSTEM_INFO lpSystemInfo);
	FuncGetSystemInfo funcGetNativeSystemInfo = (FuncGetSystemInfo)GetProcAddress(GetModuleHandle(_T("kernel32")), "GetNativeSystemInfo");
	// 优先使用 "GetNativeSystemInfo" 函数来获取系统信息
	// 函数 "GetSystemInfo" 存在系统位数兼容性问题
	if (NULL != funcGetNativeSystemInfo)
	{
		funcGetNativeSystemInfo(lpSystemInfo);
	}
	else
	{
		GetSystemInfo(lpSystemInfo);
	}
}


// 获取操作系统位数
static int GetSystemBits()
{
	SYSTEM_INFO si;
	SafeGetNativeSystemInfo(&si);
	if (si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64 ||
		si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_IA64)
	{
		return 64;
	}
	return 32;
}
bool ZhWindowsDeviceInfo::initOperatingSystemBits(int &bits)
{
	bits = GetSystemBits();
	if (bits != 32 
		&& bits != 64)
	{
		return false;
	}
	return true;
}

static string wideCharToString(wchar_t* pWCStrKey)
{
	//第一次调用确认转换后单字节字符串的长度，用于开辟空间
	int pSize = WideCharToMultiByte(CP_OEMCP, 0, pWCStrKey, wcslen(pWCStrKey), NULL, 0, NULL, NULL);
	char* pCStrKey = new char[pSize + 1];
	//第二次调用将双字节字符串转换成单字节字符串
	WideCharToMultiByte(CP_OEMCP, 0, pWCStrKey, wcslen(pWCStrKey), pCStrKey, pSize, NULL, NULL);
	pCStrKey[pSize] = '\0';
	//return pCStrKey;

	//如果想要转换成string，直接赋值即可
	string t_strRtn = pCStrKey;
	delete pCStrKey;

	return t_strRtn;
}

bool ZhWindowsDeviceInfo::initSysteminfoOrder(std::string &strInfo)
{

	std::string t_outputFilePath;
	t_outputFilePath += getAppDirPath();
	t_outputFilePath += "\\res1\\systeminfo.txt";
	std::string t_infoOrder;
	t_infoOrder += "-c \"systeminfo | Out-File -Encoding UTF8 ";
	t_infoOrder += t_outputFilePath;
	t_infoOrder += "\"";


	//DWORD t_drtn = RunSilent("powershell", "-c \"systeminfo | Out-File -Encoding UTF8 E:\\TempDir\\TempDir\\systeminfo.txt\"");
	DWORD t_drtn = RunSilent("powershell", t_infoOrder.c_str());
	//DWORD t_drtn = RunSilent("systeminfo", "> systeminfo.txt");
	if (0 != t_drtn)
	{
		return false;
	}




	FILE * pFile;
	long lSize;
	//LPTSTR  buffer;
	size_t result;

	// 若要一个byte不漏地读入整个文件，只能采用二进制方式打开 
	//pFile = fopen("systeminfo.txt", "rb");
//	errno_t t_ertn = fopen_s(&pFile, "systeminfo.txt", "rb,ccs=UTF-8");//以utf8方式打开文件，因为我们的文件保存成了utf8方式
	errno_t t_ertn = fopen_s(&pFile, t_outputFilePath.c_str(), "rb,ccs=UTF-8");//以utf8方式打开文件，因为我们的文件保存成了utf8方式

	//errno_t t_ertn = fopen_s(&pFile, "systeminfo.txt", "rb");//这里以系统默认的方式打开了
	if (pFile == NULL)
	{
		fputs("File error", stderr);
		exit(1);
	}

	//获取文件大小 
	fseek(pFile, 0, SEEK_END);
	lSize = ftell(pFile);
	rewind(pFile);

	//分配内存存储整个文件
	//buffer = (LPTSTR)malloc(sizeof(LPTSTR)*lSize);
	//if (buffer == NULL)
	//{
	//	fputs("Memory error", stderr);
	//	exit(2);
	//}

	//wchar_t *wBuffer = (wchar_t*)malloc(lSize * sizeof(wchar_t) + 1);
	//memset(wBuffer, 0, lSize* sizeof(wchar_t) + 1);
	char *t_buffer = (char *)malloc(lSize * sizeof(char)+1);
	memset(t_buffer, 0, lSize * sizeof(char) + 1);
	
	fseek(pFile,sizeof(char)*3,0);//windows下的utf8的话需要跳过前面三个字符

	//result = fread(wBuffer, sizeof(wchar_t), lSize, pFile);
	result = fread(t_buffer, sizeof(char), lSize, pFile);
	strInfo = t_buffer;
	//strInfo = wideCharToString(wBuffer);

	// 结束演示，关闭文件并释放内存 
	fclose(pFile);
	//free(buffer);
	//free(wBuffer);
	free(t_buffer);

	//SYSTEM_INFO info;
	//GetSystemInfo(&info);
	int i = 0;
	i++;
	
	return true;
}
