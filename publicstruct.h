#ifndef PUBLICSTRUCT_H
#define PUBLICSTRUCT_H

#include <QString>

typedef  struct
{
    //账户信息
    QString displayName;
    QString userName;
    QString password;
    QString domain;
    //账户是否登录的配置信息
    bool enableThisSipAccount;

}SipAccountInfo;



#endif // PUBLICSTRUCT_H
