#pragma once
//这个类保存着windows系统的相关信息。
//这个类获取值时候尽量放到线程中，因为有的接口可能会卡一下（大约3s）
//没有获取cpuid，因为同一批次的cpu，它们的id是一样的
//
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <list>

using namespace std;

class ZhWindowsDeviceInfo
{
public:
	ZhWindowsDeviceInfo();
	~ZhWindowsDeviceInfo();
	
	int initValue();//单独写一个这个初始化函数，是因为，这些函数运行速度有点慢，所以在使用之前初始化一下
	
	std::string getCurrentRunUuid();
	std::string getDeviceUuid();//获取设备uuid，这个值是通过 MachineGUID 和 HardDiskSerial 组合起来的。
	std::string getMotherboardUuid(){ return m_strMotherboardUuid; }
	std::string getBaseboardSerial(){ return m_strBaseboardSerial; }//另一个序列号，我也不知道跟主板的uuid有什么不同，
	std::string getComputerName(){ return m_strComputerName; }//设备名
	std::string getHardDiskSerial();
	std::string getMachineGUID();
	std::list<std::string> getLocalIP(){ return m_listLocalIP; }//获取内网ip（可能有多个,虚拟机啥的）
	std::string getOperatingSystemVersion(){ return m_strOperatingSystemVersion; }//获取操作系统版本
	int getOperatingSystemBits(){ return m_iOperatingSystemBits; }//获取操作系统的位数.32位还是64位
	std::string getSysteminfoOrder(){ return m_strSysteminfoOrder; }//获取本机的信息，时间有点长

private:
	//可能会失败
	bool initMotherboardUuid(std::string &strUuid);//主板的uuid
	bool initBaseboardSerial(std::string &strSerial);//另一个序列号，我也不知道跟主板的uuid有什么不同，
	bool initComputerName(std::string &strName);//设备名
	bool initHardDiskSerial(std::string &strFirstHDSerial);//硬盘序列号，只会获取第一块硬盘的
	bool initMachineGUID(std::string &strGuid);//这个值重装系统才会改变。ghost有没有影响暂时未知
	bool initLocalIP(std::list<std::string> &ipList);//获取内网ip（可能有多个,虚拟机啥的）
	bool initOperatingSystemVersion(std::string &strVersion);//获取操作系统版本
	bool initOperatingSystemBits(int &bits);//获取操作系统的位数.32位还是64位
	bool initSysteminfoOrder(std::string &strInfo);//获取本机的信息，时间有点长

private:
	bool createUUID(std::string &strUUID);//创建一个uuid，每次都会不同。//eg：00000000-0000-0000-0000-000000000000 
	
private:
	std::string m_strCurRunUuid;//当前运行的uuid
	std::string m_strMotherboardUuid;
	std::string m_strBaseboardSerial;
	std::string m_strComputerName;
	std::string m_strHardDiskSerial;
	std::string m_strMachineGUID;
	std::list<std::string> m_listLocalIP;
	std::string m_strOperatingSystemVersion;
	int m_iOperatingSystemBits = 0;
	std::string m_strSysteminfoOrder;
};

