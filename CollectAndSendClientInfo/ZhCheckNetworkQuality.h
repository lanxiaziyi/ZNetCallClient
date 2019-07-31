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
  这里使用ping的方式来测量网络质量，
  但是有点问题，因为ping的时候，正常情况下很快7ms左右，
  如果网络不好的时候，ping函数实际返回时间能到6000ms，为了防止这种情况，想了很多办法
  1, 实际ping的线程放到主线程中肯定不可以
  2，放到次线程中如果用while的方式，里面有sleep函数，导致程序退出的时候，需要等待这个线程退出。暂时没有找到好的强制这个线程终止的方式
  3，主线程是定时器，反复创建线程来运行，会出问题（具体表现就是内存会爆，或者函数运行出错）。因此需要只用一个线程。
  4，因此用了生产者消费者模式。我不知道有什么更好的方式
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

	void startRun();//开始任务
	void stopRun();//这个函数必须手动调用，放到析构函数里的话会报错

private:
	void toRunOrder();
	void toRunOneOrder();
private:
	ZhCheckNetworkQuality();
	~ZhCheckNetworkQuality();

	static ZhCheckNetworkQuality *m_pInstance;

	CPing* m_pObjPing;

	//ZhHttpsClient m_httpsClient;
	std::mutex m_mutex;//互斥量，保护缓冲器
	std::condition_variable m_condition_in;
	std::condition_variable m_condition_out;
	int m_iNumber = 0;
	bool m_bStopped = false;
	std::string m_strTargetIp;
	HWND m_pUserHWND = NULL;
	HANDLE m_threadHandle;
	std::thread *m_pThread = NULL;

	class CGarbo // 它的唯一工作就是在析构函数中删除ZHReadOnlyConfigSettings的实例  
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
	static CGarbo Garbo; // 定义一个静态成员，在程序结束时，系统会调用它的析构函数

};

