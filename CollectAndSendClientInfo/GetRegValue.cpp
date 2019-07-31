
#include "GetRegValue.h"
#include <Windows.h>
//可移植版本 wstring => string
std::string ws2s(const std::wstring& wstr)
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

//可移植版本 string => wstring
std::wstring s2ws(const std::string& str)
{
	LPCSTR pszSrc = str.c_str();
	int nLen = MultiByteToWideChar(CP_ACP, 0, pszSrc, -1, NULL, 0);
	if (nLen == 0)
		return std::wstring(L"");

	wchar_t* pwszDst = new wchar_t[nLen];
	if (!pwszDst)
		return std::wstring(L"");

	MultiByteToWideChar(CP_ACP, 0, pszSrc, -1, pwszDst, nLen);
	std::wstring wstr(pwszDst);
	delete[] pwszDst;
	pwszDst = NULL;

	return wstr;
}

std::string GetRegValue(int nKeyType, const std::string& strUrl, const std::string& strKey)
{
	std::string strValue("");
	HKEY hKey = NULL;
	HKEY  hKeyResult = NULL;
	DWORD dwSize = 0;
	DWORD dwDataType = 0;
    std::wstring wstrUrl = s2ws(strUrl);
    std::wstring wstrKey = s2ws(strKey);

	switch (nKeyType)
	{
	case 0:
	{
		hKey = HKEY_CLASSES_ROOT;
		break;
	}
	case 1:
	{
		hKey = HKEY_CURRENT_USER;
		break;
	}
	case 2:
	{
		hKey = HKEY_LOCAL_MACHINE;
		break;
	}
	case 3:
	{
		hKey = HKEY_USERS;
		break;
	}
	case 4:
	{
		hKey = HKEY_PERFORMANCE_DATA;
		break;
	}
	case 5:
	{
		hKey = HKEY_CURRENT_CONFIG;
		break;
	}
	case 6:
	{
		hKey = HKEY_DYN_DATA;
		break;
	}
	case 7:
	{
		hKey = HKEY_CURRENT_USER_LOCAL_SETTINGS;
		break;
	}
	case 8:
	{
		hKey = HKEY_PERFORMANCE_TEXT;
		break;
	}
	case 9:
	{
		hKey = HKEY_PERFORMANCE_NLSTEXT;
		break;
	}
	default:
	{
		return strValue;
	}
	}


	char data[256] = { 0 };

	//打开注册表
	if (ERROR_SUCCESS == ::RegOpenKeyEx(hKey, wstrUrl.c_str(), 0, KEY_READ | KEY_WOW64_64KEY, &hKeyResult))
	{
		// 获取缓存的长度dwSize及类型dwDataType
		long ret1 = RegQueryValueEx(hKeyResult, wstrKey.c_str(), 0, &dwDataType, NULL, &dwSize);
		if (ERROR_SUCCESS != ret1)
		{
			printf("robin:can not open");
			return "";
		}
		switch (dwDataType)
		{
		case REG_MULTI_SZ:
		{
			//分配内存大小
			BYTE* lpValue = new BYTE[dwSize];
			//获取注册表中指定的键所对应的值
			LONG lRet = ::RegQueryValueEx(hKeyResult, wstrKey.c_str(), 0, &dwDataType, lpValue, &dwSize);
			delete[] lpValue;
			break;
		}
		case REG_SZ:
		{
			//分配内存大小
			//wchar_t* lpValue = new wchar_t[dwSize];
			//memset(lpValue, 0, dwSize * sizeof(wchar_t));
			
			char* lpValue = new char[dwSize];
			memset(lpValue, 0, dwSize * sizeof(char));
			//获取注册表中指定的键所对应的值
			if (ERROR_SUCCESS == ::RegQueryValueEx(hKeyResult, wstrKey.c_str(), 0, &dwDataType, (LPBYTE)lpValue, &dwSize))
			{
				//std::wstring wstrValue(lpValue);
				//strValue = ws2s(wstrValue);
				strValue = lpValue;
			}
			delete[] lpValue;
			break;
		}
		default:
			break;
		}
	}

	//关闭注册表
	::RegCloseKey(hKeyResult);


	return strValue;
}



