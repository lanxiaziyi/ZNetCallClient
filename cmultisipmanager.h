#ifndef CMULTISIPMANAGER_H
#define CMULTISIPMANAGER_H

#include <QObject>
#include <QMap>
#include <pjsua-lib/pjsua.h>
#include <pjsua-lib/pjsua_internal.h>

class CMultiSipManager : public QObject
{
    Q_OBJECT
public:
    static CMultiSipManager* GetInstance();
    virtual ~CMultiSipManager();
    void SipInit();

    void testRegisterAccount();//注册
    void testUnRegisterAccount();//注销
    void testMakeACall();

    int testFunc();

    void toHangUpCall(pjsua_call_id callId);//挂断呼叫
    void toAnswerCall(pjsua_call_id callId);//应答呼叫
    void toMakeACall(pjsua_acc_id accoundId,QString targetNumber,QMap<QString,QString> customHeaderInfoMap);// eg: accountId=0,targetUrl= sip:1000@192.168.2.215:2060
    //
    int resetAudioCodecPriority();

signals:
    void sigIncomingInfo(QString t_info);
    void sigSipInfo(QString t_info);
private:
    void toUnRegisterAllAccounts();//注销所有账户
    void customCallOutHeader(pjsua_msg_data *msg_data,QMap<QString,QString> &customHeaderInfoMap);

private:
    int actualhangUpCall(pjsua_call_id callId, pjsip_inv_session *pInvSession,unsigned st_code);
    void initMemoryPool();
    void releaseMemoryPool();
    QString createDstUri(QString t_callNumber,QString accountUri);//根据账户的uri创建被叫的uri
private:
    static void on_reg_started2(pjsua_acc_id acc_id, pjsua_reg_info *info);
    static void on_reg_state2(pjsua_acc_id acc_id, pjsua_reg_info *info);
    static void on_call_state(pjsua_call_id call_id, pjsip_event *e);
    static void on_incoming_call(pjsua_acc_id acc_id, pjsua_call_id call_id,
                  pjsip_rx_data *rdata);
    static void on_call_media_state(pjsua_call_id call_id);


private:
    explicit CMultiSipManager(QObject *parent = nullptr);
    static CMultiSipManager *m_pInstance;

    pj_caching_pool m_cp;
    pj_pool_t *m_pPool = nullptr;

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
