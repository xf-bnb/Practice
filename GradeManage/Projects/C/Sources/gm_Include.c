#include "gm_Include.h"


//=============================================================================
// ����ȫ������
//=============================================================================
AppMgr*     g_pAppMgr   = NULL;
AppData*    g_pAppData  = NULL;

static AppMgr   g_sAppMgr;
static AppData  g_sAppData;


//=============================================================================
// ����ȫ�ֺ���
//=============================================================================
//-----------------------------------------------------------------------------

bool Init()
{
    // ��ʼ��ȫ��Ӧ�ó�������
    g_sAppData.m_pHeadPer   = NULL;
    g_sAppData.m_pHeadAdmin = NULL;
    g_sAppData.m_pCurPer    = NULL;
    g_sAppData.m_pCurAdmin  = NULL;
    g_sAppData.m_eState     = State_LoginOut;

    // ��ʼ��ȫ��ָ�����
    g_pAppMgr   = &g_sAppMgr;
    g_pAppData  = &g_sAppData;
    
    // ��ʼ��������
    InitManager();
    
    // ������Դ
    g_pAppMgr->m_pMgrRes->m_pfLoadString();

    // ��������(����)
    g_pAppMgr->m_pMgrRes->m_pfLoadData();

    // ��ʾ��ӭ����
    g_pAppMgr->m_pMgrCmd->m_pfShowScreen(str_Welcom);
    
    return true;
}

//-----------------------------------------------------------------------------

bool Loop()
{
    // �˵�����ѭ��
    while ( g_pAppData->m_eState > State_None && g_pAppData->m_eState < State_Max )
    {
        g_pAppMgr->m_pMgrCmd->m_pfCommand[g_pAppData->m_eState]();
    }

    return true;
}

//-----------------------------------------------------------------------------

bool Exit()
{
    // ������������
    g_pAppMgr->m_pMgrRes->m_pfSaveData();

    // �ͷ������ڴ�
    g_pAppMgr->m_pMgrRes->m_pfFreeData();

    // ��ʾ�˳�����
    g_pAppMgr->m_pMgrCmd->m_pfShowScreen(str_ThankYou);

    return true;
}
