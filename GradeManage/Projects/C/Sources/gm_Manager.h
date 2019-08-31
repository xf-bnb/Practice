﻿//=============================================================================
// 文件：gm_Manager.h
// 描述：定义程序管理者。
// 说明：
//      程序管理者提供对程序的所有操作，汇总所有操作管理者，从而将任务分层。
//      程序中需要的任何通用操作均可调用程序管理者，也应该调用它。
//      例如需要加载数据：先用程序管理者调用资源管理者，再调用加载方法。
//      g_pAppMgr->m_pMgrRes->m_pfLoadData();
//=============================================================================
#ifndef _GM_MANAGER_H
#define _GM_MANAGER_H


//=============================================================================
// 程序管理者
//=============================================================================
typedef struct
{
    MgrRes*     m_pMgrRes;      // 资源管理者
    MgrPer*     m_pMgrPer;      // 人员管理者
    MgrCmd*     m_pMgrCmd;      // 命令管理者
}AppMgr;

//-----------------------------------------------------------------------------

bool    InitManager();

#endif

