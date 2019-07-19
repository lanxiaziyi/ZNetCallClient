#include "cmultisipmanager.h"


#include <QDebug>
#define THIS_FILE	"CMultiSipManager"


CMultiSipManager *CMultiSipManager::m_pInstance = nullptr;
CMultiSipManager::CMultiSipManager(QObject *parent) : QObject(parent)
{

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

}

int CMultiSipManager::testFunc()
{
    pjsua_acc_id acc_id;
    pj_status_t status;

    /* Create pjsua first! */
    status = pjsua_create();
    if (status != PJ_SUCCESS)
    {
        //error_exit("Error in pjsua_create()", status);
        qDebug()<<"robin:pjsua_create failed";
    }




    return 0;
}
