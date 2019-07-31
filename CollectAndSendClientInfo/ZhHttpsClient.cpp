#include "ZhHttpsClient.h"
#include <iostream>
#include <thread>
#include <string>
#include <sstream>
using namespace std;

ZhHttpsClient::ZhHttpsClient()
{
	//curl_global_init(CURL_GLOBAL_DEFAULT);//不能在这里初始化，因为这个是个全局的，因此放到了上一层。
	m_pCurl = curl_easy_init();
	if (NULL == m_pCurl)
	{
		cout << "robin:curl_easy_init failed" << endl;
	}

}

static int writer(char *data, size_t size, size_t nmemb, string *writerData)
{
	unsigned long sizes = size * nmemb;
	if (writerData == NULL)
		return -1;

	writerData->append(data, sizes);
	return sizes;
}

ZhHttpsClient::~ZhHttpsClient()
{

	curl_easy_cleanup(m_pCurl);
	//curl_global_cleanup();
	if (nullptr != m_pHeaderlist)
	{
		curl_slist_free_all(m_pHeaderlist);
	}
}

void ZhHttpsClient::setHttpHeader(std::string strHeader)
{
	m_strHeaderValue = strHeader;
	if (nullptr != m_pHeaderlist)
	{
		curl_slist_free_all(m_pHeaderlist);

	}
	m_pHeaderlist = curl_slist_append(nullptr, m_strHeaderValue.c_str());
}

bool ZhHttpsClient::request(std::string urlStr, std::string &outputStr)
{
	if (NULL == m_pCurl)
	{
		cout << "robin:ZhHttpsClient::request curl is NULL" << endl;
		return false;
	}
	std::string t_outBuffer;
	curl_easy_setopt(m_pCurl, CURLOPT_TIMEOUT, 3);
	curl_easy_setopt(m_pCurl, CURLOPT_URL, urlStr.c_str());
	curl_easy_setopt(m_pCurl, CURLOPT_WRITEFUNCTION, writer);
	curl_easy_setopt(m_pCurl, CURLOPT_WRITEDATA, &t_outBuffer);

	curl_easy_setopt(m_pCurl, CURLOPT_POSTFIELDS, "");//post字段设为空
	curl_easy_setopt(m_pCurl, CURLOPT_HEADER, m_pHeaderlist);//

	CURLcode t_res;
	t_res = curl_easy_perform(m_pCurl);
	if (t_res != CURLE_OK)
	{
		fprintf(stderr, "curl_easy_perform() failed: %s\n",
			curl_easy_strerror(t_res));
		return false;
	}

	outputStr = t_outBuffer;
	return true;
}


bool ZhHttpsClient::request_post(std::string urlStr, std::string postDataStr, std::string &outputStr)
{
	if (NULL == m_pCurl)
	{
		cout << "robin:ZhHttpsClient::request curl is NULL2" << endl;
		return false;
	}
	if (!m_bAllowIn)
	{//因为发送函数那里允许多线程，所以这里加一个防止重入的限制，不然libcurl会崩溃
		return false;
	}
	m_bAllowIn = false;

	std::string t_outBuffer;
	curl_easy_setopt(m_pCurl, CURLOPT_TIMEOUT, 3);
	curl_easy_setopt(m_pCurl, CURLOPT_URL, urlStr.c_str());
	curl_easy_setopt(m_pCurl, CURLOPT_WRITEFUNCTION, writer);
	curl_easy_setopt(m_pCurl, CURLOPT_WRITEDATA, &t_outBuffer);

	curl_easy_setopt(m_pCurl, CURLOPT_POSTFIELDS, postDataStr.c_str());
	curl_easy_setopt(m_pCurl, CURLOPT_HEADER, m_pHeaderlist);//

	CURLcode t_res;
	t_res = curl_easy_perform(m_pCurl);
	if (t_res != CURLE_OK)
	{
		fprintf(stderr, "curl_easy_perform() failed: %s\n",
			curl_easy_strerror(t_res));
		return false;
	}
	
	auto this_id = std::this_thread::get_id();
	std::string t_strOutput;
	t_strOutput += "request_post id:";
	stringstream ss;
	ss << this_id;
	t_strOutput += ss.str();
	//OutputDebugString(t_strOutput.c_str());

	outputStr = t_outBuffer;

	m_bAllowIn = true;
	return true;
}
