#pragma once
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
using namespace std;
/************************************************************************/
/* 这里会获取当前：软件的版本号，内部版本号，账户名（保存）。
*/
/************************************************************************/
class ZhClientSoftwareInfo
{
public:
	ZhClientSoftwareInfo();
	~ZhClientSoftwareInfo();


private:
	std::string m_str;
};

