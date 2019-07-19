#include "cmultisipmanager.h"


#include <QDebug>
#define THIS_FILE	"CMultiSipManager"


CMultiSipManager *CMultiSipManager::m_pInstance = nullptr;
CMultiSipManager::CMultiSipManager(QObject *parent) : QObject(parent)
{
    SipInit();
}

CMultiSipManager *CMultiSipManager::GetInstance()
{
    if(nullptr == m_pInstance)
    {
        m_pInstance = new CMultiSipManager();
    }
    return m_pInstance;
}

CMultiSipManager::~CMultiSipManager()
{
    pj_status_t status;
    status = pjsua_destroy();


}

/* Display error and exit application */
static void error_exit(const char *title, pj_status_t status)
{
    qDebug()<<"robin:app error_exit";

    pjsua_perror(THIS_FILE, title, status);
    pjsua_destroy();
    exit(1);
}


void CMultiSipManager::SipInit()
{
    //pjsua_acc_id acc_id;
    pj_status_t status;

    /* Create pjsua first! */
    status = pjsua_create(); //初始化的时候用的，程序退出的时候必须调用 pjsua_destroy
    if (status != PJ_SUCCESS)
    {
        //error_exit("Error in pjsua_create()", status);
        qDebug()<<"robin:pjsua_create failed";
    }
}

void CMultiSipManager::testRegisterAccount()
{
    pjsua_config ua_cfg;//账号的各种状态通过这里返回
    pjsua_config_default(&ua_cfg);//
    ua_cfg.cb.on_reg_state2 = &CMultiSipManager::on_reg_state2;

    pjsua_logging_config log_cfg;
    pjsua_logging_config_default(&log_cfg);
    log_cfg.console_level = 4;

    pj_status_t status;
    status = pjsua_init(&ua_cfg, &log_cfg, nullptr);
    if (status != PJ_SUCCESS)
    {
        qDebug()<<"robin:Error in pjsua_init()";
    }

    //初始化传输方式的相关信息
    pjsua_transport_config cfg;
    pjsua_transport_config_default(&cfg);
    cfg.port = 5060;
    status = pjsua_transport_create(PJSIP_TRANSPORT_UDP, &cfg, nullptr);
    if (status != PJ_SUCCESS)
    {
        qDebug()<<"robin:Error in pjsua_transport_create";
    }

    status = pjsua_start();
    if (status != PJ_SUCCESS)
    {
        qDebug()<<"robin:Error in pjsua_start";
    }


    {
    pjsua_acc_config cfg;

    pjsua_acc_config_default(&cfg);
    cfg.id = pj_str("sip:1031@192.168.2.215:2060");
    cfg.reg_uri = pj_str("sip:192.168.2.215:2060");
    cfg.cred_count = 1;
    cfg.cred_info[0].realm = pj_str("192.168.2.215:2060");
    cfg.cred_info[0].scheme = pj_str("digest");
    cfg.cred_info[0].username = pj_str("1031");
    cfg.cred_info[0].data_type = PJSIP_CRED_DATA_PLAIN_PASSWD;
    cfg.cred_info[0].data = pj_str("123456");
    pjsua_acc_id acc_id;
    status = pjsua_acc_add(&cfg, PJ_TRUE, &acc_id);
    if (status != PJ_SUCCESS)
    {
        qDebug()<<"robin:Error in pjsua_acc_add";
    }

    }




}

void CMultiSipManager::testMakeACall()
{
    pj_status_t status;
    status = pjsua_verify_url("sip:1000@192.168.2.215:2060");
    if(PJ_SUCCESS != status)
    {
        qDebug()<<"robin:sip url is wrong";
    }




}

int CMultiSipManager::testFunc()
{






    return 0;
}

void CMultiSipManager::on_reg_state2(pjsua_acc_id acc_id, pjsua_reg_info *info)
{
    qDebug()<<"robin:on_reg_state2:"<<acc_id <<"="<<info->renew;

}
