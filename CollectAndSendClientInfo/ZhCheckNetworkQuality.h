#pragma once

#include <curl/curl.h>
//#include "ZhHttpsClient.h"
#include <string>
#include "../ping.h"
#include <thread>
#include <mutex>
#include <condition_variable>
#include <exception>
/*
  ����ʹ��ping�ķ�ʽ����������������
  �����е����⣬��Ϊping��ʱ����������ºܿ�7ms���ң�
  ������粻�õ�ʱ��ping����ʵ�ʷ���ʱ���ܵ�6000ms��Ϊ�˷�ֹ������������˺ܶ�취
  1, ʵ��ping���̷߳ŵ����߳��п϶�������
  2���ŵ����߳��������while�ķ�ʽ��������sleep���������³����˳���ʱ����Ҫ�ȴ�����߳��˳�����ʱû���ҵ��õ�ǿ������߳���ֹ�ķ�ʽ
  3�����߳��Ƕ�ʱ�������������߳������У�������⣨������־����ڴ�ᱬ�����ߺ������г����������Ҫֻ��һ���̡߳�
  4���������������������ģʽ���Ҳ�֪����ʲô���õķ�ʽ
*/

//#include "../globle.h"
class ZhCheckNetworkQuality
{
public:

	static ZhCheckNetworkQuality *GetInstance();

	void toCheckNetworkQuality(HWND hWnd,std::string targetIp = "127.0.0.1");
	
	void toAddOneOrder();

	void setTargetIp(std::string targetIp);
	void setUserPtr(HWND hWnd);

	void startRun();//��ʼ����
	void stopRun();//������������ֶ����ã��ŵ�����������Ļ��ᱨ��

private:
	void toRunOrder();
	void toRunOneOrder();
private:
	ZhCheckNetworkQuality();
	~ZhCheckNetworkQuality();

	static ZhCheckNetworkQuality *m_pInstance;

	CPing* m_pObjPing;

	//ZhHttpsClient m_httpsClient;
	std::mutex m_mutex;//������������������
	std::condition_variable m_condition_in;
	std::condition_variable m_condition_out;
	int m_iNumber = 0;
	bool m_bStopped = false;
	std::string m_strTargetIp;
	HWND m_pUserHWND = NULL;
	HANDLE m_threadHandle;
	std::thread *m_pThread = NULL;

	class CGarbo // ����Ψһ��������������������ɾ��ZHReadOnlyConfigSettings��ʵ��  
	{
	public:
		~CGarbo()
		{
			if (NULL != ZhCheckNetworkQuality::m_pInstance)
			{
				delete ZhCheckNetworkQuality::m_pInstance;
				ZhCheckNetworkQuality::m_pInstance = NULL;
			}
		}
	};
	static CGarbo Garbo; // ����һ����̬��Ա���ڳ������ʱ��ϵͳ�����������������

};

