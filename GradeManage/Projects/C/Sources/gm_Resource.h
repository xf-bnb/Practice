//=============================================================================
// 文件：gm_Resource.h
// 描述：定义资源文件类型、字符串ID，资源管理者。
// 说明：
//      资源管理者(MgrPer)提供对资源的所有操作。
//      包括：字符串的加载、获取、显示，数据的加载、保存、释放。
//=============================================================================
#ifndef _GM_RESOURCE_H
#define _GM_RESOURCE_H

//-----------------------------------------------------------------------------

typedef enum
{
    File_String,                        // 字符串文件
    File_Admin,                         // 管理员数据文件
    File_Person,                        // 人员信息文件

    File_Max
}AppFile;   // 文件类型枚举


//-----------------------------------------------------------------------------

typedef enum
{
    str_Welcom,                         // 欢迎进入信息管理系统！
    str_ThankYou,                       // 谢谢使用，再见！
    str_VerifyCancel,                   // (提示：注销后用户将被删除)您确定要注销吗？(y/n)：
    str_RecordCount,                    // 共%d条记录。

    str_InputCmd,                       // 请输入一个命令：
    str_InputLoginID,                   // 请输入用户名：
    str_InputPassword,                  // 请输入密码：
    str_AgainPassword,                  // 请确认密码:
    str_InputName,                      // 请输入姓名：
    str_InputBirth,                     // 请输入出生日期(yyyymmdd)：
    str_InputSex,                       // 请输入性别(男：M，女：W)：
    str_InputScore,                     // 请输入分数(0-%d)：
    str_InputPublic,                    // 请选择资料是否公开(y/n)：
    str_InputSort,                      // 请输入排序方式(a：升序，d：降序)：
    str_InputOldPwd,                    // 请输入旧密码：
    str_InputNewPwd,                    // 请输入新密码：

    str_MemoryIsLess,                   // 内存不够！
    str_StateExp,                       // 状态异常！
    str_SuccessLogin,                   // 登陆成功！
    str_SuccessRegister,                // 注册成功！
    str_SuccessCancel,                  // 注销成功！
    str_SuccessDelete,                  // 删除成功！
    str_SuccessModifyPwd,               // 修改密码成功！
    str_SuccessModifyPub,               // 修改公开权限成功！
    str_FailCancel,                     // 注销失败！
    str_FailDelete,                     // 删除失败！
    str_CancelCancel,                   // 注销操作已取消！
    str_ErrorOldPwd,                    // 您的旧密码输入不正确！
    str_InvalidCommand,                 // 您输入的命令无效！
    str_InvalidLoginID,                 // 您输入的用户名无效！
    str_LoginIsExist,                   // 用户名已经存在！
    str_PasswordLess,                   // 您输入的密码长度不够！
    str_PasswordNoSame,                 // 您两次输入的密码不一致！
    str_ErrorLogin,                     // 用户名或密码不正确！
    str_InvalidDate,                    // 您输入的出生日期无效！

    str_Exit,                           // 退出
    str_Login,                          // 登陆
    str_Register,                       // 注册
    str_Back,                           // 返回
    str_Look,                           // 查看
    str_ModifyPassword,                 // 修改密码
    str_ModifyPublic,                   // 修改公开权限
    str_Cancel,                         // 注销
    str_Delete,                         // 删除
    str_Sort,                           // 排序
    str_Sift,                           // 筛选
    str_ByLoginID,                      // 按用户名
    str_ByName,                         // 按姓名
    str_ByDate,                         // 按出生日期
    str_ByScore,                        // 按分数
    str_BySex,                          // 按性别
    str_ByPublic,                       // 按公开权限

    str_Max
}AppString; // 字符串ID


//=============================================================================
// 声明资源管理者结构
//=============================================================================
typedef bool    (*fpLoadData)();
typedef bool    (*fpSaveData)();
typedef bool    (*fpFreeData)();
typedef bool    (*fpLoadString)();
typedef bool    (*fpShowString)(int, bool);
typedef const char* (*fpGetString)(int);
typedef const char* (*fpGetFile)(int);

typedef struct
{
    fpLoadData      m_pfLoadData;       // 加载数据
    fpSaveData      m_pfSaveData;       // 保存数据
    fpFreeData      m_pfFreeData;       // 释放数据

    fpGetFile       m_pfGetFile;        // 获取文件名
    fpGetString     m_pfGetString;      // 获取字符串
    fpLoadString    m_pfLoadString;     // 加载字符串
    fpShowString    m_pfShowString;     // 显示字符穿
}MgrRes;    // 资源管理者结构


//-----------------------------------------------------------------------------

bool    InitMgrRes();   // 初始化资源管理者


#endif