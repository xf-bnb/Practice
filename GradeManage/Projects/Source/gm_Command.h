//=============================================================================
// 文件：gm_Command.h
// 描述：定义界面参数、菜单标识、程序状态、错误码、命令管理者。
// 说明：
//      系统的状态分为：
//      未登陆(LoginOut)、普通用户登陆(LoginPerson)、管理员登陆(LoginAdmin)。
//      每一种状态都有对应的菜单和指令。
//      用户和管理员的区别主要体现在对人员的操作和查看功能。
//      管理员登陆可以查看到所有用户信息，以及根据用户名删除指定用户。
//      普通用户登陆只能查看到资料权限公开的其他用户信息，以及注销自己。
//=============================================================================
#ifndef _GM_COMMAND_H
#define _GM_COMMAND_H


//-----------------------------------------------------------------------------

#define ScreenWidth     73          // 屏幕宽度
#define ScreenHeight    15          // 屏幕高度


//-----------------------------------------------------------------------------

typedef enum _cmdMain
{
    cmdMain_Exit,       // 退出
    cmdMain_Login,      // 登陆
    cmdMain_Register,   // 注册

    cmdMain_Max
}cmdMain;   // 主菜单


//-----------------------------------------------------------------------------

typedef enum _cmdChild
{
    cmdCh_Back,         // 返回
    cmdCh_Look,         // 查看
    cmdCh_Password,     // 修改密码
    cmdCh_Public,       // 修改公开权限
    cmdCh_Cancel,       // 注销

    cmdCh_Max
}cmdChild;  // 用户子菜单


//-----------------------------------------------------------------------------

typedef enum _cmdAdmin
{
    cmdAd_Back,         // 返回
    cmdAd_Look,         // 查看
    cmdAd_Password,     // 修改密码
    cmdAd_Delete,       // 删除

    cmdAd_Max
}cmdAdmin;  // 管理员菜单


//-----------------------------------------------------------------------------

typedef enum _cmdLook
{
    cmdLook_Back,       // 返回
    cmdLook_Sort,       // 排序
    cmdLook_Sift,       // 筛选

    cmdLook_Max
}cmdLook;  // 查询菜单

//-----------------------------------------------------------------------------

typedef enum _cmdSort
{
    cmdSort_Back,       // 返回
    cmdSort_LoginID,    // 按用户名
    cmdSort_Name,       // 按姓名
    cmdSort_Date,       // 按出生日期
    cmdSort_Score,      // 按分数

    cmdSort_Max
}cmdSort;   // 排序菜单

//-----------------------------------------------------------------------------

typedef enum _cmdSift
{
    cmdSift_Back,       // 返回
    cmdSift_LoginID,    // 按用户名
    cmdSift_ByName,     // 按姓名
    cmdSift_ByDate,     // 按出生日期
    cmdSift_ByScore,    // 按分数
    cmdSift_BySex,      // 按性别
    cmdSift_ByPublic,   // 按公开权限

    cmdSift_Max
}cmdSift;   // 筛选菜单

//-----------------------------------------------------------------------------

typedef enum _LoginState
{
    State_None  = -1,

    State_LoginOut,     // 未登陆
    State_LoginPerson,  // 人员登陆
    State_LoginAdmin,   // 管理员登陆

    State_Max
}LoginState;  // 登陆状态

//-----------------------------------------------------------------------------

typedef enum _ErrorCode
{
    err_None    = -1,
    err_MemoryIsLess,       // 内存不够
    err_StateExp,           // 状态异常
    err_SuccessLogin,       // 登陆成功
    err_SuccessRegister,    // 注册成功
    err_SuccessCancel,      // 注销成功
    err_SuccessDelete,      // 删除成功
    err_SuccessModifyPwd,   // 修改密码成功
    err_SuccessModifyPub,   // 修改公开权限成功
    err_FailCancel,         // 注销失败
    err_FailDelete,         // 删除失败
    err_CancelCancel,       // 取消注销
    err_ErrorOldPwd,        // 旧密码输入不正确
    err_InvalidCommand,     // 命令无效
    err_InvalidLoginID,     // 用户名无效
    err_LoginIsExist,       // 用户名已经存在
    err_PasswordLess,       // 密码长度不够
    err_PasswordNoSame,     // 密码不一致
    err_ErrorLogin,         // 用户名或密码不正确
    err_InvalidDate,        // 出生日期无效

    err_Max
}ErrorCode; // 错误码

//-----------------------------------------------------------------------------

typedef enum _MenuType
{
    menu_Main,              // 主菜单
    menu_Person,            // 普通用户菜单
    menu_Admin,             // 管理员菜单
    menu_Look,              // 查看菜单
    menu_Sort,              // 排序菜单
    menu_Sift               // 筛选菜单
}menuType;  // 菜单类型


//=============================================================================
// 声明指令管理者
//=============================================================================
typedef void    (*fpCommand)();
typedef void    (*fpShowScreen)(int);
typedef void    (*fpShowMenu)(int);

typedef struct
{
    fpCommand       m_pfCommand[State_Max];     // 指令函数指针数组
    fpShowMenu      m_pfShowMenu;               // 显示菜单
    fpShowScreen    m_pfShowScreen;             // 显示界面
}MgrCmd;    // 指令管理者


//-----------------------------------------------------------------------------

bool InitMerCmd();      // 初始化命令管理者


#endif