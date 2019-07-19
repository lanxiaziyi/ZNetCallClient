#ifndef CMULTISIPMANAGER_H
#define CMULTISIPMANAGER_H

#include <QObject>

#include <pjsua-lib/pjsua.h>
#include <pjsua-lib/pjsua_internal.h>

class CMultiSipManager : public QObject
{
    Q_OBJECT
public:
    static CMultiSipManager* GetInstance();
    virtual ~CMultiSipManager();
    void SipInit();

    void testRegisterAccount();
    void testMakeACall();

    int testFunc();

private:
    static void on_reg_state2(pjsua_acc_id acc_id, pjsua_reg_info *info);

private:
    explicit CMultiSipManager(QObject *parent = nullptr);
    static CMultiSipManager *m_pInstance;



signals:

public slots:


private:
    class CGarbo // 它的唯一工作就是在析构函数中删除CSipManager的实例
    {
    public:
        ~CGarbo()
        {
            if (CMultiSipManager::m_pInstance)
                delete CMultiSipManager::m_pInstance;
        }
    };
    static CGarbo Garbo; // 定义一个静态成员，在程序结束时，系统会调用它的析构函数
};

#endif // CMULTISIPMANAGER_H
