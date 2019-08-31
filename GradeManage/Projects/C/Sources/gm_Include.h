//=============================================================================
// 文件：gm_Include.h
// 描述：包含程序所需所有头文件，程序过程方法定义，声明程序全局变量。
// 说明：
//      其他源文件只需包含本头文件即可。
//=============================================================================
#ifndef _GM_INCLUDE_H
#define _GM_INCLUDE_H

//=============================================================================
// 包含程序所需头文件
//=============================================================================
// 标准库头文件  
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 判断编译器版本
#if _MSC_VER

#define bnb_strcpy(buf, size, dsc)               strcpy_s(buf, size, dsc)
#define bnb_fopen(file, name, mode)              fopen_s(&(file), name, mode)
#define bnb_fread(buf, size, _byte, count, file) fread_s(buf, size, _byte, count, file)
    
#else

#define bnb_strcpy(buf, size, dsc)               strcpy(buf, dsc)
#define bnb_fopen(file, name, mode)              file = fopen(name, mode)
#define bnb_fread(buf, size, _byte, count, file) fread(buf, _byte, count, file)

#endif


// 自定义头文件
#include "gm_Utility.h"
#include "gm_Resource.h"
#include "gm_Person.h"
#include "gm_Command.h"
#include "gm_Manager.h"


//-----------------------------------------------------------------------------

typedef struct _appdata
{
    admin*      m_pHeadAdmin;                   // 管理员信息链表头节点
    person*     m_pHeadPer;                     // 人员信息链表头节点

    admin*      m_pCurAdmin;                    // 当前登陆的管理员
    person*     m_pCurPer;                      // 当前登陆的用户

    int         m_eState;                       // 当前登陆状态

}AppData;   // 程序数据结构


//=============================================================================
// 程序执行过程
//=============================================================================
bool    Init();             // 初始化

bool    Loop();             // 命令循环

bool    Exit();             // 退出


//=============================================================================
// 声明全局变量
//=============================================================================
extern  AppMgr*     g_pAppMgr;  // 全局应用程序管理者
extern  AppData*    g_pAppData; // 全局应用程序数据


#endif
