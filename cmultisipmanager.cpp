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

    pjsua_config ua_cfg;//账号的各种状态通过这里返回
    pjsua_config_default(&ua_cfg);//
    ua_cfg.cb.on_reg_state2 = &CMultiSipManager::on_reg_state2;//只会在注册的时候回调，注销不会执行
    ua_cfg.cb.on_reg_started2 = &CMultiSipManager::on_reg_started2;//
    ua_cfg.cb.on_incoming_call = &CMultiSipManager::on_incoming_call;//来电话时的通知
    ua_cfg.cb.on_call_media_state = &CMultiSipManager::on_call_media_state;//媒体状态发生改变时，具体的，要看使用的
    ua_cfg.cb.on_call_state = &CMultiSipManager::on_call_state;//呼叫状态发生改变时

    pjsua_logging_config log_cfg;
    pjsua_logging_config_default(&log_cfg);
    log_cfg.console_level = 4;

    pjsua_media_config   media_cfg;
    pjsua_media_config_default(&media_cfg);

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

    //设置音频编码优先级
    resetAudioCodecPriority();
}

void CMultiSipManager::testRegisterAccount()
{
    pj_status_t status;
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

void CMultiSipManager::testUnRegisterAccount()
{
    // pjsua_acc_id ,对应着每一个注册用户
    pjsua_acc_id ids[64]={0};
    unsigned t_count = PJ_ARRAY_SIZE(ids);
    pj_status_t t_status = pjsua_enum_accs(ids, &t_count);
    if(PJ_SUCCESS != t_status)
    {
        qDebug()<<"robin:pjsua_enum_accs error";
    }


    qDebug()<<"robin:account count = "<<t_count;
    for (unsigned i = 0;i < t_count;i++)
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
    unsigned t_count = PJ_ARRAY_SIZE(ids);
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

void CMultiSipManager::toHangUpCall(pjsua_call_id callId)
{
    //这里挂断分两类；自己作为主叫挂断；自己作为被叫挂断
    //这里操作的是call_id
   pjsua_call_info ci;
   pj_status_t t_status = pjsua_call_get_info(callId, &ci);
   if(PJ_SUCCESS != t_status)
   {
       qDebug()<<"robin:toHangUpCall:pjsua_call_get_info failed,maybe callId is error";
       return;
   }
   //分成两个是因为自己作为主叫的时候主动挂断，发送487信号告诉fs，是自己主动挂断的
   //用户代理客户端-UAC（User Agent Client），用于发起请求
   //用户代理服务器-UAS（User Agent Server），用于接收请求
   //UAC/UAS的划分是针对一个事务的。在一个呼叫中的多个事务里，UAC和UAS的角色是可以互换的。
   // if(PJSIP_INV_STATE_CALLING == ci.state || (PJSIP_ROLE_UAC == ci.role && PJSIP_INV_STATE_CONNECTING == ci.state))



    //列举所有的calls
   pjsua_call_id t_callIds[PJSUA_MAX_CALLS] = {0};
   unsigned t_iCallCount = PJ_ARRAY_SIZE(t_callIds);
   t_status = pjsua_enum_calls(t_callIds,&t_iCallCount);
//   if(callId > t_iCallCount)
//   {
//       qDebug()<<"robin:callId maybe error."<<"Call Count is:"<< t_iCallCount<<",input index is:" << callId;
//       return;
//   }




//   unsigned t_iIndex = 0;
//   for(unsigned i = 0; i < t_iCallCount; i++)
//   {
//        if(t_callIds[i] == callId)
//        {
//            t_iIndex = i;
//            break;
//        }
//   }





   //总感觉这里写的不自然
//   pjsua_call *t_pCall = &pjsua_var.calls[t_iIndex];
//   pjsip_inv_session *t_pInvSession = t_pCall->inv;
//   if(nullptr == t_pInvSession)
//   {
//        qDebug()<<"robin:error:inv sesssion ptr is NULL";
//        return;
//   }

   if(PJSIP_ROLE_UAC == ci.role)
   {//客户端作为主叫

        pjsua_call_hangup(callId, 0, nullptr, nullptr);
   }
   else if(PJSIP_ROLE_UAS == ci.role)
   {//客户端作为被叫
       PJSIP_SC_REQUEST_TERMINATED;

        if(PJSIP_INV_STATE_INCOMING == ci.state )
        {//来电振铃，被叫挂断。这里参考“软电话”，返回的480，然后fs就认为用户busy。
            pjsua_call_hangup(callId, PJSIP_SC_TEMPORARILY_UNAVAILABLE , nullptr, nullptr);
        }
        else if(PJSIP_INV_STATE_CONFIRMED == ci.state)
        {
            //通话中，执行挂断，不论传入什么参数,抓到的包都是正常say bye
            pjsua_call_hangup(callId,0 , nullptr, nullptr);
        }
        else
        {
            pjsua_call_hangup(callId,0 , nullptr, nullptr);
        }

   }
   else
   {
        //不会执行到这里
        pjsua_call_hangup(callId, 0, nullptr, nullptr);
   }

}

void CMultiSipManager::toAnswerCall(pjsua_call_id callId)
{
    if(-1 == callId)
    {
        return;
    }

    pjsua_call_info call_info;
    pjsua_call_get_info(callId, &call_info);

    pjsua_call_setting call_setting;
    pjsua_call_setting_default(&call_setting);

    pjsua_call_answer2(callId,&call_setting,200,nullptr,nullptr);
}

void CMultiSipManager::toMakeACall(pjsua_acc_id accoundId, QString targetUri)
{
    if(-1 == accoundId
            || 0 == targetUri.length())
    {
        qDebug()<<"robin:toMakeACall input error:"<<accoundId<<targetUri;
        return;
    }
    pjsua_call_id call_id = -1;
    pjsua_call_setting call_setting;
    pjsua_call_setting_default(&call_setting);
    call_setting.flag = PJSUA_CALL_INCLUDE_DISABLED_MEDIA;
    call_setting.vid_cnt = 0; //

    char uri[256] = {0};
    std::string t_targetStr = targetUri.toStdString();

    strncpy_s(uri,sizeof(uri),t_targetStr.c_str(),t_targetStr.length());
    pj_str_t uri2 = pj_str(uri);
    pjsua_msg_data msg_data;
    pjsua_msg_data_init(&msg_data);

    pj_status_t t_status;
    t_status = pjsua_call_make_call(accoundId, &uri2 ,&call_setting,nullptr,&msg_data,&call_id);
    if(PJ_SUCCESS != t_status)
    {
        qDebug()<<"robin:pjsua_call_make_call failed,"<<t_status;
    }

}

int CMultiSipManager::actualhangUpCall(pjsua_call_id callId, pjsip_inv_session *pInvSession, unsigned st_code)
{
    if(nullptr == pInvSession)
    {
         return -1;
    }

   pjsip_tx_data *tdata;
   pj_status_t t_status = pjsip_inv_end_session(pInvSession, st_code, nullptr, &tdata);//PJSIP_SC_REQUEST_TERMINATED:487
   if(PJ_SUCCESS == t_status && nullptr != tdata)
   {
     t_status =  pjsip_inv_send_msg(pInvSession,tdata);
   }
   else
   {//如果失败了就在执行另一个。
     t_status =  pjsip_inv_terminate( pInvSession, st_code, PJ_TRUE);
   }

   if(PJ_SUCCESS == t_status)
   {
       return 0;
   }

   t_status = pjsua_call_hangup(callId,0,nullptr,nullptr);//这里实际用的是 603
   if(PJ_SUCCESS == t_status)
   {
       return 0;
   }
   else
   {
       return -1;
   }


   return 0;
}
//线程注册
static pj_status_t pjcall_thread_register(void)
{
    pj_thread_desc  desc;
    pj_thread_t*    thread = 0;

    if (!pj_thread_is_registered())
    {
        return pj_thread_register(NULL, desc, &thread);
    }

    return PJ_SUCCESS;
}
int CMultiSipManager::resetAudioCodecPriority()
{
   // pjcall_thread_register();

    //先将所有的编码设置为禁用
    pjsua_codec_info codec_info[64] = {0};//这里64个空间够用了
    unsigned count = 64;
    pjsua_enum_codecs(codec_info, &count);
    for (unsigned i = 0; i < count; i++)
    {
        pjsua_codec_set_priority(&codec_info[i].codec_id, 0);
    }


    pj_str_t codec_id;
    codec_id.ptr = (char *)"PCMA";
    codec_id.slen = 4;
    pjsua_codec_set_priority(&codec_id, 255);




    return 0;
}

void CMultiSipManager::on_reg_started2(pjsua_acc_id acc_id, pjsua_reg_info *info)
{
     qDebug()<<"robin:on_reg_started:acc_id:"<< acc_id <<",renew:"<< info->renew;

     QString t_info = QString("robin:on_reg_started2:acc_id:%1,renew=%2").arg(acc_id).arg(info->renew);
     emit CMultiSipManager::GetInstance()->sigSipInfo(t_info);

}

void CMultiSipManager::on_reg_state2(pjsua_acc_id acc_id, pjsua_reg_info *info)
{
    int t_code = info->cbparam->code;
    // code 为 200代表成功了
    //info->renew 如果为0代表这是注销返回的消息，非0代表是注册返回的消息
    qDebug()<<"robin:on_reg_state2:acc_id:"<< acc_id <<",renew="<<info->renew<<",cbparam_code="<<t_code;
    // <<info->cbparam->rdata->msg_info.cid->id.ptr;
    QString t_info = QString("robin:on_reg_state2:acc_id:%1,renew=%2,cbparam_code=%3").arg(acc_id).arg(info->renew).arg(t_code);
    emit CMultiSipManager::GetInstance()->sigSipInfo(t_info);
    int i = 0;
    i++;

}

void CMultiSipManager::on_call_state(pjsua_call_id call_id, pjsip_event *e)
{
    pjsua_call_info ci;
    pjsua_call_get_info(call_id, &ci);
    qDebug()<<"robin:on_call_state:call_id:"<<call_id<<",state_text:"<<ci.state_text.ptr;
    QString t_info = QString("robin:on_call_state:call_id:%1,state_text:%2").arg(call_id).arg(ci.state_text.ptr);
    emit CMultiSipManager::GetInstance()->sigSipInfo(t_info);




}

void CMultiSipManager::on_incoming_call(pjsua_acc_id acc_id, pjsua_call_id call_id, pjsip_rx_data *rdata)
{
    pjsua_call_info call_info;
    pjsua_call_get_info(call_id, &call_info);

    QString t_outInfo = QString("on_incoming_call:acc_id=%1,call_id=%2").arg(acc_id).arg(call_id);
    qDebug()<<"robin:on_incoming_call:acc_id="<<acc_id<<",call_id="<<call_id;

    emit CMultiSipManager::GetInstance()->sigIncomingInfo(t_outInfo);


}

void CMultiSipManager::on_call_media_state(pjsua_call_id call_id)
{

    pjsua_call_info ci;
    pjsua_call_get_info(call_id, &ci);
    qDebug()<<"robin:on_call_media_state:call_id="<<call_id;
    pjsua_conf_port_id t_iPort = pjsua_call_get_conf_port(call_id);

    if (ci.media_status == PJSUA_CALL_MEDIA_ACTIVE)
    {
        // When media is active, connect call to sound device.
        pjsua_conf_connect(ci.conf_slot, 0); //这里的0貌似是客户端自己，出处我没查到
        pjsua_conf_connect(0, ci.conf_slot);
    }
    else if(PJSUA_CALL_MEDIA_LOCAL_HOLD == ci.media_status)
    {//

    }
    else if (PJSUA_CALL_MEDIA_REMOTE_HOLD == ci.media_status)
    {//

    }
    else
    {//一般这里不会有

    }


}
