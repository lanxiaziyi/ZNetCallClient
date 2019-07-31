#pragma once

#include <curl/curl.h>
#include <iostream>
using namespace std;

class ZhHttpsClient
{
public:
	ZhHttpsClient();
	~ZhHttpsClient();
	void setHttpHeader(std::string strHeader);
public:
	bool request(std::string urlStr, std::string &outputStr);
	bool request_post(std::string urlStr, std::string postDataStr, std::string &outputStr);
private:

	CURL *m_pCurl = NULL;
	bool m_bAllowIn = true;//����������ڷ�ֹ��������
	std::string m_strHeaderValue;
	curl_slist *m_pHeaderlist = NULL;
};

