#include "ZhCheckNetworkQuality.h"

#include <string>
#include <iostream>
//#include "../PublicMacro.h"

ZhCheckNetworkQuality *ZhCheckNetworkQuality::m_pInstance = NULL;
ZhCheckNetworkQuality::CGarbo ZhCheckNetworkQuality::Garbo;
ZhCheckNetworkQuality * ZhCheckNetworkQuality::GetInstance()
{
	if (NULL == m_pInstance)
	{

		m_pInstance = new ZhCheckNetworkQuality();

	}
	return m_pInstance;
}

void ZhCheckNetworkQuality::toCheckNetworkQuality(HWND hWnd, std::string targetIp)
{
	std::thread t_thread([this,hWnd, targetIp](){

		std::string t_outStr;

		LARGE_INTEGER t_cpuHz;
		QueryPerformanceFrequency(&t_cpuHz);
		LARGE_INTEGER t_beginTime;
		LARGE_INTEGER t_endTime;
		QueryPerformanceCounter(&t_beginTime);

		//m_httpsClient.request("https://61.135.169.125/", t_outStr);
	//	CPing objPing;
		char *szDestIP = (char *)targetIp.c_str();//"61.135.169.125";
		PingReply reply;
		if (NULL != this->m_pObjPing)
		{
			this->m_pObjPing->Ping(szDestIP, &reply);
		}
		//objPing.Ping(szDestIP, &reply);
		//objPing.Ping(szDestIP, &reply);
		//objPing.Ping(szDestIP, &reply);
		QueryPerformanceCounter(&t_endTime);
		//这里我本来想ping三次的，但是为了给服务器减小点压力，就ping一次算了



		//printf("Reply from %s: bytes=%d time=%ldms TTL=%ld\n", szDestIP, reply.m_dwBytes, reply.m_dwRoundTripTime, reply.m_dwTTL);
		/*
		std::string t_outString0;
		t_outString0 += "Reply from ";
		t_outString0 += szDestIP;
		t_outString0 += ": ";
		t_outString0 += "bytes=";
		t_outString0 += to_string(reply.m_dwBytes);
		t_outString0 += " time=";
		t_outString0 += to_string(reply.m_dwRoundTripTime);
		t_outString0 += "ms TTL=";
		t_outString0 += to_string(reply.m_dwTTL);
		OutputDebugString(t_outString0.c_str());
		*/

		
		double t_dInterval = ((double)t_endTime.QuadPart - (double)t_beginTime.QuadPart) / (double)t_cpuHz.QuadPart;
		//cout << "robin:hello:" << t_interval * 1000 << endl;
		//std::string t_ourStr = "robin:hello:";
		//t_ourStr += to_string(t_interval * 1000);
		//OutputDebugString(t_ourStr.c_str());

		//这里需要通知界面当前的网络状况
		int t_iInterval = (int)(t_dInterval * 1000);
		t_iInterval = t_iInterval;
        //PostMessage(hWnd, IDT_TO_SHOW_NETWORK_QUALITY, (WPARAM)t_iInterval, NULL);
        std::cout<<"robin:not finished";
    });
	t_thread.detach();
}

void ZhCheckNetworkQuality::toAddOneOrder()
{//这里是生产者

	if (m_bStopped)
	{
		return;
	}

	std::unique_lock<std::mutex> in(m_mutex);
	//m_condition_in.wait(in, [this](){
	//	return m_iNumber < 10;//
	//});

	m_iNumber++;

	if (m_iNumber > 10)
	{
		m_iNumber = 10;
	}

	m_condition_out.notify_one();
}





void ZhCheckNetworkQuality::toRunOrder()
{
	while (!m_bStopped)
	{
		std::unique_lock<std::mutex> in(m_mutex);
		m_condition_out.wait(in, [this](){
			return m_iNumber > 0;
		});
		m_iNumber--;

		if (m_bStopped)
		{
			break;
		}

		toRunOneOrder();
		//m_condition_in.notify_one();//这一句基本没什么作用
	}

	//std::chrono::milliseconds dura(5000);
	//std::this_thread::sleep_for(dura);
}

void ZhCheckNetworkQuality::setTargetIp(std::string targetIp)
{
	m_strTargetIp = targetIp;
}

void ZhCheckNetworkQuality::setUserPtr(HWND hWnd)
{
	m_pUserHWND = hWnd;
}

void ZhCheckNetworkQuality::startRun()
{
	
	m_pThread = new std::thread([this]{
		m_pObjPing = new CPing();
		toRunOrder();
		delete m_pObjPing;
		m_pObjPing = NULL;
	});

	//m_threadHandle = m_pThread->native_handle();

	//t_thread.detach();
}

void ZhCheckNetworkQuality::stopRun()
{
	m_bStopped = true;
	m_iNumber++;
	m_condition_out.notify_one();
	m_pUserHWND = NULL;
	if (m_pThread->joinable())
	{
		try
		{
			m_pThread->join();
		}
		catch (std::exception& e)
		{
			std::string t_outStr;
			t_outStr += "robin:";
			t_outStr += e.what();
            //OutputDebugString(t_outStr.c_str());
            std::cout<<t_outStr;
		}

	}
}

void ZhCheckNetworkQuality::toRunOneOrder()
{
	if (m_strTargetIp.empty())
	{
		return;
	}

	if (NULL == m_pUserHWND)
	{
		return;
	}
	//这里发送ping命令
	//LARGE_INTEGER t_cpuHz;
	//QueryPerformanceFrequency(&t_cpuHz);
	//LARGE_INTEGER t_beginTime;
	//LARGE_INTEGER t_endTime;
	//QueryPerformanceCounter(&t_beginTime);

	//	CPing objPing;
	//我们的服务器有:139.129.208.166  112.253.8.146 
	//
	char *szDestIP = (char *)m_strTargetIp.c_str();//"61.135.169.125";
	PingReply reply;
	BOOL t_bReachable = false;//是否可以ping通
	if (NULL != m_pObjPing)
	{
		t_bReachable = this->m_pObjPing->Ping(szDestIP, &reply);
	}
	
	//objPing.Ping(szDestIP, &reply);
	//QueryPerformanceCounter(&t_endTime);


	//OutputDebugString("");

	//double t_dInterval = ((double)t_endTime.QuadPart - (double)t_beginTime.QuadPart) / (double)t_cpuHz.QuadPart;
	//(int)(t_dInterval * 1000);
	//这里需要通知界面当前的网络状况
	int t_iInterval = t_bReachable ? reply.m_dwRoundTripTime : 3000;//如果ping不通，默认就传3000
    //PostMessage(m_pUserHWND, IDT_TO_SHOW_NETWORK_QUALITY, (WPARAM)t_iInterval, NULL);
    std::cout<<"robin:not finished";

}

ZhCheckNetworkQuality::ZhCheckNetworkQuality()
{
}


ZhCheckNetworkQuality::~ZhCheckNetworkQuality()
{
	stopRun();

	delete m_pThread;
	m_pThread = NULL;

}
