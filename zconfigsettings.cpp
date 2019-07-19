#include "zconfigsettings.h"
#include <QApplication>


ZConfigSettings *ZConfigSettings::m_pInsance = nullptr;
ZConfigSettings *ZConfigSettings::GetInstance()
{
    if(nullptr == m_pInsance)
    {
        m_pInsance = new ZConfigSettings();
    }

    return m_pInsance;
}

void ZConfigSettings::Release()
{
    if(nullptr != NULL)
    {
        delete m_pInsance;
        m_pInsance = nullptr;
    }
}

ZConfigSettings::~ZConfigSettings()
{
    m_pSetting->sync();

}

QList<SipAccountInfo> ZConfigSettings::getSipAccountInfos()
{
    QList<SipAccountInfo> t_outListInfo;
    int t_size = m_pSetting->beginReadArray("UserInfo");
    for (int i = 0;i < t_size;i++)
    {
        m_pSetting->setArrayIndex(i);
        SipAccountInfo t_accountInfo;
        t_accountInfo.displayName = m_pSetting->value("displayName").toString();
        t_accountInfo.userName = m_pSetting->value("userName").toString();
        t_accountInfo.password = m_pSetting->value("password").toString();
        t_accountInfo.domain = m_pSetting->value("domain").toString();
        t_accountInfo.enableThisSipAccount = m_pSetting->value("enableThisSipAccount").toBool();
        t_outListInfo.append(t_accountInfo);
    }
    m_pSetting->endArray();

    return t_outListInfo;
}

void ZConfigSettings::setSipAccountInfos(QList<SipAccountInfo> t_accountListInfo)
{
   m_pSetting->beginWriteArray("UserInfo");
   for (int i = 0;i < t_accountListInfo.size();i++)
   {
       SipAccountInfo t_accountInfo = t_accountListInfo.at(i);
        m_pSetting->setArrayIndex(i);
        m_pSetting->setValue("displayName",t_accountInfo.displayName);
        m_pSetting->setValue("userName",t_accountInfo.userName);
        m_pSetting->setValue("password",t_accountInfo.password);
        m_pSetting->setValue("domain",t_accountInfo.domain);
        m_pSetting->setValue("enableThisSipAccount",t_accountInfo.enableThisSipAccount);
   }
    m_pSetting->endArray();
}

ZConfigSettings::ZConfigSettings(QObject *parent) : QObject(parent)
{
    QString t_iniFileName = qApp->applicationDirPath() + "/NetCallClient_config.ini";
    m_pSetting = new QSettings("t_iniFileName",QSettings::IniFormat,this);

}
