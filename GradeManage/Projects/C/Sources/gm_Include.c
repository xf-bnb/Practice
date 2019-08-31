#include "gm_Include.h"


//=============================================================================
// 定义全部变量
//=============================================================================
AppMgr*     g_pAppMgr   = NULL;
AppData*    g_pAppData  = NULL;

static AppMgr   g_sAppMgr;
static AppData  g_sAppData;


//=============================================================================
// 定义全局函数
//=============================================================================
//-----------------------------------------------------------------------------

bool Init()
{
    // 初始化全局应用程序数据
    g_sAppData.m_pHeadPer   = NULL;
    g_sAppData.m_pHeadAdmin = NULL;
    g_sAppData.m_pCurPer    = NULL;
    g_sAppData.m_pCurAdmin  = NULL;
    g_sAppData.m_eState     = State_LoginOut;

    // 初始化全局指针变量
    g_pAppMgr   = &g_sAppMgr;
    g_pAppData  = &g_sAppData;
    
    // 初始化管理者
    InitManager();
    
    // 加载资源
    g_pAppMgr->m_pMgrRes->m_pfLoadString();

    // 加载数据(链表)
    g_pAppMgr->m_pMgrRes->m_pfLoadData();

    // 显示欢迎界面
    g_pAppMgr->m_pMgrCmd->m_pfShowScreen(str_Welcom);
    
    return true;
}

//-----------------------------------------------------------------------------

bool Loop()
{
    // 菜单命令循环
    while ( g_pAppData->m_eState > State_None && g_pAppData->m_eState < State_Max )
    {
        g_pAppMgr->m_pMgrCmd->m_pfCommand[g_pAppData->m_eState]();
    }

    return true;
}

//-----------------------------------------------------------------------------

bool Exit()
{
    // 保存链表数据
    g_pAppMgr->m_pMgrRes->m_pfSaveData();

    // 释放链表内存
    g_pAppMgr->m_pMgrRes->m_pfFreeData();

    // 显示退出界面
    g_pAppMgr->m_pMgrCmd->m_pfShowScreen(str_ThankYou);

    return true;
}
