#ifndef ZCONFIGSETTINGS_H
#define ZCONFIGSETTINGS_H

#include <QObject>
//这里保存着所有的配置信息
#include "publicstruct.h"
#include <QSettings>
#include <QList>

class ZConfigSettings : public QObject
{
    Q_OBJECT
public:
    static ZConfigSettings *GetInstance();
    static void Release();

    ~ZConfigSettings();
    QList<SipAccountInfo>getSipAccountInfos();
    void setSipAccountInfos(QList<SipAccountInfo> t_accountListInfo);

signals:

public slots:

private:
    explicit ZConfigSettings(QObject *parent = nullptr);
    static ZConfigSettings *m_pInsance;

    QSettings *m_pSetting = nullptr;
};

#endif // ZCONFIGSETTINGS_H
