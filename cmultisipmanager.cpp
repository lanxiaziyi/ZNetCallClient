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

    //内存池的释放

    releaseMemoryPool();

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



    //初始化传输方式的相关信息 ，我们创建三种,至于使用哪一种，由程序自己决定
    pjsua_transport_config udp_cfg;
    pjsua_transport_id udp_id;
    pjsua_transport_config_default(&udp_cfg);
    udp_cfg.port = 0;//自己随便绑定个接口
    status = pjsua_transport_create(PJSIP_TRANSPORT_UDP, &udp_cfg, &udp_id);
    if (status != PJ_SUCCESS)
    {
        qDebug()<<"robin:Error in pjsua_transport_create UDP";
    }

    pjsua_transport_config tcp_cfg;
    pjsua_transport_id tcp_id;
    pjsua_transport_config_default(&tcp_cfg);
    tcp_cfg.port = 0;//自己随便绑定个接口
    status = pjsua_transport_create(PJSIP_TRANSPORT_TCP, &tcp_cfg, &tcp_id);
    if (status != PJ_SUCCESS)
    {
        qDebug()<<"robin:Error in pjsua_transport_create tcp";
    }

    pjsua_transport_config tls_cfg;
    pjsua_transport_id tls_id;
    pjsua_transport_config_default(&tls_cfg);
    tls_cfg.port = 0;//自己随便绑定个接口
    status = pjsua_transport_create(PJSIP_TRANSPORT_TLS, &tls_cfg, &tls_id);
    if (status != PJ_SUCCESS)
    {//TLS 这个需要freeswitch 那里的配合;而且一旦使用TLS，用wireshark就不能分析抓到的包了
     //TLS 支持需要pjsip编译的时候添加，我写在文档中了。其实不支持也没关系，至少我们客户端没用到
        qDebug()<<"robin:Error in pjsua_transport_create tls";
    }



    status = pjsua_start();
    if (status != PJ_SUCCESS)
    {
        qDebug()<<"robin:Error in pjsua_start";
    }

    //创建内存池;目前的用处就是自定义呼叫请求头部的时候会用到
    initMemoryPool();

    //设置音频编码优先级，就是pcma、g729、iLbc的顺序
    resetAudioCodecPriority();



}

void CMultiSipManager::testRegisterAccount()
{
    pj_status_t t_status;
    pjsua_acc_config acc_cfg;//每个用户一个acc_cfg

    pjsua_acc_config_default(&acc_cfg);

    /*
    cfg.ice_cfg_use = PJSUA_ICE_CONFIG_USE_CUSTOM;
    cfg.ice_cfg.enable_ice = PJ_TRUE;
    cfg.allow_via_rewrite = PJ_TRUE;
    cfg.allow_sdp_nat_rewrite = PJ_TRUE;
    cfg.allow_contact_rewrite = PJ_TRUE;
    cfg.publish_enabled = PJ_TRUE;
    */
    // acc_cfg.id 有两种形式:"sip:account@serviceprovider" or "\"Display Name\" <sip:account@provider>"
  //  acc_cfg.id = pj_str((char *)"\"SID2018082382631479\" sip:1031@192.168.2.215:2060");
  //  acc_cfg.reg_uri = pj_str((char *)"sip:192.168.2.215:2060");//客户端是这种形式；pj_str((char *)"sip:1031");但是貌似不对//
    acc_cfg.id = pj_str((char *)"sip:1031@192.168.2.215:2060");
    acc_cfg.reg_uri = pj_str((char *)"sip:192.168.2.215:2060");//pj_str((char *)"sip:192.168.2.215:2060");//客户端是这种形式；pj_str((char *)"sip:1031");但是貌似不对//
    acc_cfg.cred_count = 1;//这里意思，是认证信息的数量.一个就行，但是我试了下多个，反而不行
    acc_cfg.cred_info[0].realm = pj_str((char *)"*");//必须是*，原因看字段注释
    acc_cfg.cred_info[0].scheme = pj_str((char *)"digest");
    acc_cfg.cred_info[0].username = pj_str((char *)"1031");
    acc_cfg.cred_info[0].data_type = PJSIP_CRED_DATA_PLAIN_PASSWD;
    acc_cfg.cred_info[0].data = pj_str((char *)"1234");//密码

/*
    //我们自己还添加了一些定制的字段
    //acc_cfg.id 添加了一个SID字段.这里添加的字段，本来应该是“显示名称”的，后来，我们用这个作为"账号拆分"的id识别码用了。
    QString t_strUriParams = ";sid=";
    t_strUriParams += "SID2018082382631479";
    std::string t_uriParams = t_strUriParams.toStdString();
    acc_cfg.contact_uri_params =  pj_str((char *)t_uriParams.c_str());
    //我觉得proxy这个不用改，这个值跟domain一样
    //acc_cfg.proxy[0];
    // use_srtp ,是否使用加密传输。原先客户端是用了PJMEDIA_SRTP_DISABLED
    acc_cfg.use_srtp = PJMEDIA_SRTP_DISABLED;
    acc_cfg.ice_cfg_use = PJSUA_ICE_CONFIG_USE_CUSTOM;
    acc_cfg.ice_cfg.enable_ice = PJ_FALSE;
    //acc_cfg.rtp_cfg.public_addr;//这个字段原先客户端设置了空
    //acc_cfg.allow_via_rewrite;//这个字段使用默认值就好 //sip头请求中,via里面的地址是否允许修改，是本机的ip还是经过nat映射后公网中的ip https://blog.csdn.net/gyy_2046/article/details/50071019
    acc_cfg.allow_sdp_nat_rewrite = acc_cfg.allow_via_rewrite;//主要都是nat穿网相关的
    acc_cfg.allow_contact_rewrite = PJ_TRUE;
    acc_cfg.publish_enabled = PJ_FALSE;
    //acc_cfg.transport_id;//这个参数客户端原先的也不用设置


    //这里必须要要写成这样才能设置tcp传输模式//https://trac.pjsip.org/repos/wiki/Using_SIP_TCP
    //这里需要配置文件中添加一个变量，根据变量来
    acc_cfg.proxy[acc_cfg.proxy_cnt++] =  pj_str((char *)"sip:192.168.2.215:2060;transport=tcp");

*/


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
    acc_cfg.cred_info[3].data = pj_str((char *)"123456");

    pjsua_acc_id acc_id;//返回当前分配的account_id
    t_status = pjsua_acc_add(&acc_cfg, PJ_TRUE, &acc_id);
    if (t_status != PJ_SUCCESS)
    {
        qDebug()<<"robin:Error in pjsua_acc_add:"<<t_status;
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

void CMultiSipManager::customCallOutHeader(pjsua_msg_data *msg_data, QMap<QString, QString> &customHeaderInfoMap)
{

    for (auto i = customHeaderInfoMap.begin();i != customHeaderInfoMap.end();i++)
    {
        std::string t_keyStr = i.key().toStdString();
        std::string t_valueStr = i.value().toStdString();
        pj_str_t hname, hvalue;

        char cKey[256] = {0};
        strncpy_s(cKey,sizeof(cKey),t_keyStr.c_str(),t_keyStr.length());
        char cValue[256] = {0};
        strncpy_s(cValue,sizeof(cValue),t_valueStr.c_str(),t_valueStr.length());
        hname  = pj_str(cKey);
        hvalue = pj_str(cValue);

        pjsip_generic_string_hdr *t_oneHeaderItem = nullptr;
        t_oneHeaderItem = pjsip_generic_string_hdr_create(m_pPool, &hname, &hvalue);
        pj_list_push_back(&msg_data->hdr_list,t_oneHeaderItem);
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

void CMultiSipManager::toMakeACall(pjsua_acc_id accoundId, QString targetNumber, QMap<QString,QString> customHeaderInfoMap)
{
    if(-1 == accoundId
            || 0 == targetNumber.length())
    {
        qDebug()<<"robin:toMakeACall input error:"<<accoundId<<targetNumber;
        return;
    }

    //这里要
    pj_bool_t t_bAccValid = pjsua_acc_is_valid(accoundId);
    if(!t_bAccValid)
    {
        qDebug()<<"robin:acc id is not valid";
        return;
    }
    //组装sip的呼叫url。也就是
    //1,从:  18612345678 转化成 类型 sip:18612345678@192.168.2.215:2060 的样子
    //2,从: 1234 转化成 类型 sip:1234@192.168.2.215:2060 的样子
    pj_status_t t_status;
    pjsua_acc_info t_accInfo;
    t_status = pjsua_acc_get_info(accoundId,&t_accInfo);

    QString t_accountUrl = t_accInfo.acc_uri.ptr; //
    QString t_dstUri = createDstUri(targetNumber,t_accountUrl);



    pjsua_call_id call_id = -1;
    pjsua_call_setting call_setting;
    pjsua_call_setting_default(&call_setting);
    call_setting.flag = PJSUA_CALL_INCLUDE_DISABLED_MEDIA;
    call_setting.vid_cnt = 0; //

    char uri[256] = {0};
    std::string t_targetStr = t_dstUri.toStdString();

    strncpy_s(uri,sizeof(uri),t_targetStr.c_str(),t_targetStr.length());
    pj_str_t uri2 = pj_str(uri);
    pjsua_msg_data msg_data; //主要在这里添加自定义的头部
    pjsua_msg_data_init(&msg_data);//
    customCallOutHeader(&msg_data,customHeaderInfoMap);//添加


    t_status = pjsua_call_make_call(accoundId, &uri2 ,&call_setting,nullptr,&msg_data,&call_id);
    if(PJ_SUCCESS != t_status)
    {//这里返回的错误码的意义，需要到pjsip的源码中去搜（我没找到对应的接口）
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

void CMultiSipManager::initMemoryPool()
{
    pj_caching_pool_init(&m_cp, &pj_pool_factory_default_policy, 0);
    m_pPool = pj_pool_create(&m_cp.factory, "header", 1000,
                          1000, nullptr);
}

void CMultiSipManager::releaseMemoryPool()
{
    pj_pool_safe_release(&m_pPool);
    pj_caching_pool_destroy(&m_cp);
}

QString CMultiSipManager::createDstUri(QString t_callNumber, QString accountUri)
{
    if(t_callNumber.isEmpty() || accountUri.isEmpty())
    {
        qDebug() << "robin:createDstUri inputs have one empty:" << t_callNumber << accountUri;
        return "";
    }
    // accountUri 有两种形式: "sip:account@serviceprovider" or "\"Display Name\" <sip:account@provider>"
    //我们组装返回的结果是: sip:callNumber@serviceprovider
    int t_beginIndex = accountUri.indexOf("@");
    int t_endIndex = -1;
    t_endIndex = accountUri.indexOf(">");
    if(-1 == t_endIndex)
    {
       t_endIndex = accountUri.length();
    }
    //取出serviceprovider
    QString t_serviceprovider = accountUri.mid(t_beginIndex+1, t_endIndex-t_beginIndex);
    QString t_outStr = QString("sip:%1@%2").arg(t_callNumber).arg(t_serviceprovider);
    return t_outStr;
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
    if(200 == t_code)
    {
       t_info += " registration success";
    }
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
