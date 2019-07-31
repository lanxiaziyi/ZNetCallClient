#pragma once
//����ౣ����windowsϵͳ�������Ϣ��
//������ȡֵʱ�����ŵ��߳��У���Ϊ�еĽӿڿ��ܻῨһ�£���Լ3s��
//û�л�ȡcpuid����Ϊͬһ���ε�cpu�����ǵ�id��һ����
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
	
	int initValue();//����дһ�������ʼ������������Ϊ����Щ���������ٶ��е�����������ʹ��֮ǰ��ʼ��һ��
	
	std::string getCurrentRunUuid();
	std::string getDeviceUuid();//��ȡ�豸uuid�����ֵ��ͨ�� MachineGUID �� HardDiskSerial ��������ġ�
	std::string getMotherboardUuid(){ return m_strMotherboardUuid; }
	std::string getBaseboardSerial(){ return m_strBaseboardSerial; }//��һ�����кţ���Ҳ��֪���������uuid��ʲô��ͬ��
	std::string getComputerName(){ return m_strComputerName; }//�豸��
	std::string getHardDiskSerial();
	std::string getMachineGUID();
	std::list<std::string> getLocalIP(){ return m_listLocalIP; }//��ȡ����ip�������ж��,�����ɶ�ģ�
	std::string getOperatingSystemVersion(){ return m_strOperatingSystemVersion; }//��ȡ����ϵͳ�汾
	int getOperatingSystemBits(){ return m_iOperatingSystemBits; }//��ȡ����ϵͳ��λ��.32λ����64λ
	std::string getSysteminfoOrder(){ return m_strSysteminfoOrder; }//��ȡ��������Ϣ��ʱ���е㳤

private:
	//���ܻ�ʧ��
	bool initMotherboardUuid(std::string &strUuid);//�����uuid
	bool initBaseboardSerial(std::string &strSerial);//��һ�����кţ���Ҳ��֪���������uuid��ʲô��ͬ��
	bool initComputerName(std::string &strName);//�豸��
	bool initHardDiskSerial(std::string &strFirstHDSerial);//Ӳ�����кţ�ֻ���ȡ��һ��Ӳ�̵�
	bool initMachineGUID(std::string &strGuid);//���ֵ��װϵͳ�Ż�ı䡣ghost��û��Ӱ����ʱδ֪
	bool initLocalIP(std::list<std::string> &ipList);//��ȡ����ip�������ж��,�����ɶ�ģ�
	bool initOperatingSystemVersion(std::string &strVersion);//��ȡ����ϵͳ�汾
	bool initOperatingSystemBits(int &bits);//��ȡ����ϵͳ��λ��.32λ����64λ
	bool initSysteminfoOrder(std::string &strInfo);//��ȡ��������Ϣ��ʱ���е㳤

private:
	bool createUUID(std::string &strUUID);//����һ��uuid��ÿ�ζ��᲻ͬ��//eg��00000000-0000-0000-0000-000000000000 
	
private:
	std::string m_strCurRunUuid;//��ǰ���е�uuid
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

