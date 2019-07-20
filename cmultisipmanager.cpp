#include "cmultisipmanager.h"


#include <QDebug>
#define THIS_FILE	"CMultiSipManager"

CMultiSipManager::CGarbo CMultiSipManager::Garbo;
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
    //注销所有的
    toUnRegisterAllAccounts();

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
    ua_cfg.cb.on_reg_state2 = &CMultiSipManager::on_reg_state2;//只会在注册的时候回调，注销不会执行
    ua_cfg.cb.on_reg_started2 = &CMultiSipManager::on_reg_started2;//
    ua_cfg.cb.on_incoming_call = &CMultiSipManager::on_incoming_call;
    ua_cfg.cb.on_call_media_state = &CMultiSipManager::on_call_media_state;
    ua_cfg.cb.on_call_state = &CMultiSipManager::on_call_state;



    pjsua_logging_config log_cfg;
    pjsua_logging_config_default(&log_cfg);
    log_cfg.console_level = 4;

    pjsua_media_config   media_cfg;
    pjsua_media_config_default(&media_cfg);



    pj_status_t status;
    status = pjsua_init(&ua_cfg, &log_cfg, &media_cfg);
    if (status != PJ_SUCCESS)
    {
        qDebug()<<"robin:Error in pjsua_init()";
    }



    //初始化传输方式的相关信息
    pjsua_transport_config cfg;
    pjsua_transport_config_default(&cfg);
    cfg.port = 0;//自己随便绑定个接口
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
    pjsua_acc_config acc_cfg;

    pjsua_acc_config_default(&acc_cfg);

    /*
    cfg.ice_cfg_use = PJSUA_ICE_CONFIG_USE_CUSTOM;
    cfg.ice_cfg.enable_ice = PJ_TRUE;
    cfg.allow_via_rewrite = PJ_TRUE;
    cfg.allow_sdp_nat_rewrite = PJ_TRUE;
    cfg.allow_contact_rewrite = PJ_TRUE;
    cfg.publish_enabled = PJ_TRUE;
    */

    acc_cfg.id = pj_str((char *)"sip:1031@192.168.2.215:2060");
    acc_cfg.reg_uri = pj_str((char *)"sip:192.168.2.215:2060");
    acc_cfg.cred_count = 4;//这里意思，是
    acc_cfg.cred_info[0].realm = pj_str((char *)"*");//必须是*，原因看字段注释
    acc_cfg.cred_info[0].scheme = pj_str((char *)"digest");
    acc_cfg.cred_info[0].username = pj_str((char *)"1031");
    acc_cfg.cred_info[0].data_type = PJSIP_CRED_DATA_PLAIN_PASSWD;
    acc_cfg.cred_info[0].data = pj_str((char *)"123456");

    acc_cfg.cred_info[1].realm = pj_str((char *)"*");//必须是*，原因看字段注释
    acc_cfg.cred_info[1].scheme = pj_str((char *)"digest");
    acc_cfg.cred_info[1].username = pj_str((char *)"1031");
    acc_cfg.cred_info[1].data_type = PJSIP_CRED_DATA_PLAIN_PASSWD;
    acc_cfg.cred_info[1].data = pj_str((char *)"12347777");

    acc_cfg.cred_info[2].realm = pj_str((char *)"*");//必须是*，原因看字段注释
    acc_cfg.cred_info[2].scheme = pj_str((char *)"digest");
    acc_cfg.cred_info[2].username = pj_str((char *)"1031");
    acc_cfg.cred_info[2].data_type = PJSIP_CRED_DATA_PLAIN_PASSWD;
    acc_cfg.cred_info[2].data = pj_str((char *)"12348888");

    acc_cfg.cred_info[3].realm = pj_str((char *)"*");//必须是*，原因看字段注释
    acc_cfg.cred_info[3].scheme = pj_str((char *)"digest");
    acc_cfg.cred_info[3].username = pj_str((char *)"1031");
    acc_cfg.cred_info[3].data_type = PJSIP_CRED_DATA_PLAIN_PASSWD;
    acc_cfg.cred_info[3].data = pj_str((char *)"1234");

    pjsua_acc_id acc_id;//返回当前分配的account_id
    status = pjsua_acc_add(&acc_cfg, PJ_TRUE, &acc_id);
    if (status != PJ_SUCCESS)
    {
        qDebug()<<"robin:Error in pjsua_acc_add";
    }

    }




}

void CMultiSipManager::testUnRegisterAccount()
{
    // pjsua_acc_id ,对应着每一个注册用户
    pjsua_acc_id ids[64]={0};
    unsigned t_count = 64;
    pj_status_t t_status = pjsua_enum_accs(ids, &t_count);
    if(PJ_SUCCESS != t_status)
    {
        qDebug()<<"robin:pjsua_enum_accs error";
    }


    qDebug()<<"robin:account count = "<<t_count;
    for (int i = 0;i < t_count;i++)
    {
       t_status = pjsua_acc_del(ids[i]);
       if(PJ_SUCCESS != t_status)
       {
           qDebug()<<"robin:pjsua_acc_del error";
       }

    }
    int t_i = 0;
    t_i++;


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
   // pjsua_enum_accs()





    return 0;
}

void CMultiSipManager::toUnRegisterAllAccounts()
{
    // pjsua_acc_id ,对应着每一个注册用户
    pjsua_acc_id ids[64]={0};//这里的64，我随便写的，一般这个客户端登录的坐席数量不会超过这个数的。
    unsigned t_count = 64;
    pj_status_t t_status = pjsua_enum_accs(ids, &t_count);
    if(PJ_SUCCESS != t_status)
    {
        qDebug()<<"robin:pjsua_enum_accs error";
    }


    qDebug()<<"robin:account count = "<<t_count;
    for (int i = 0;i < t_count;i++)
    {
       t_status = pjsua_acc_del(ids[i]);
       if(PJ_SUCCESS != t_status)
       {
           qDebug()<<"robin:pjsua_acc_del error";
       }

    }
}

void CMultiSipManager::on_reg_started2(pjsua_acc_id acc_id, pjsua_reg_info *info)
{
     qDebug()<<"robin:on_reg_started:acc_id:"<< acc_id <<",renew:"<< info->renew;
}

void CMultiSipManager::on_reg_state2(pjsua_acc_id acc_id, pjsua_reg_info *info)
{
    int t_code = info->cbparam->code;
    // code 为 200代表成功了
    //info->renew 如果为0代表这是注销返回的消息，非0代表是注册返回的消息
    qDebug()<<"robin:on_reg_state2:acc_id:"<< acc_id <<",renew="<<info->renew<<",cbparam_code="<<t_code;
    // <<info->cbparam->rdata->msg_info.cid->id.ptr;

    int i = 0;
    i++;

}

void CMultiSipManager::on_call_state(pjsua_call_id call_id, pjsip_event *e)
{
    qDebug()<<"robin:on_call_state:call_id:"<<call_id;

}

void CMultiSipManager::on_incoming_call(pjsua_acc_id acc_id, pjsua_call_id call_id, pjsip_rx_data *rdata)
{
    pjsua_call_info call_info;
    pjsua_call_get_info(call_id, &call_info);
    qDebug()<<"robin:on_incoming_call:acc_id="<<acc_id<<",call_id="<<call_id;




}

void CMultiSipManager::on_call_media_state(pjsua_call_id call_id)
{
    qDebug()<<"robin:on_call_media_state:call_id="<<call_id;




}
