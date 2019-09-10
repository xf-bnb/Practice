#pragma once

enum _ResType {
    lang_chinese, lang_english, ResTypeMax
};

enum _ResID
{
    str_Welcome,                        // 欢迎进入信息管理系统！
    str_ThankYou,                       // 谢谢使用，再见！
    str_VerifyLogout,                   // (提示：注销后不可恢复)您确定要注销吗？(y/n)：
    str_RecordCount,                    // 共%d条记录。

    str_InputCmd,                       // 请输入一个命令：
    str_InputAccount,                   // 请输入用户名：
    str_InputPassword,                  // 请输入密码：
    str_AgainPassword,                  // 请确认密码:
    str_InputName,                      // 请输入姓名：
    str_InputBirthday,                  // 请输入出生日期(yyyy-mm-dd)：
    str_InputSex,                       // 请输入性别(男：M，女：W)：
    str_InputScore,                     // 请输入分数(0-%d)：
    str_InputPublic,                    // 请选择资料是否公开(y/n)：
    // str_InputSort,                      // 请输入排序方式(a：升序，d：降序)：
    str_InputOldPwd,                    // 请输入旧密码：
    str_InputNewPwd,                    // 请输入新密码：

    // str_MemoryIsLess,                   // 内存不够！
    // str_StateExp,                       // 状态异常！
    str_SuccessLogin,                   // 登陆成功！
    str_SuccessRegister,                // 注册成功！
    str_SuccessLogout,                  // 注销成功！
    str_SuccessDelete,                  // 删除成功！
    str_SuccessModifyPwd,               // 修改密码成功！
    str_SuccessModifyPub,               // 修改公开权限成功！
    str_FailCancel,                     // 注销失败！
    str_FailDelete,                     // 删除失败！
    str_CancelLogout,                   // 注销操作已取消！
    str_ErrorOldPwd,                    // 您输入的旧密码不正确！
    str_InvalidCommand,                 // 您输入的命令无效！
    str_InvalidAccount,                 // 您输入的用户名无效！
    str_InvalidDate,                    // 您输入的出生日期无效！
    str_Existing,                       // 用户名已经存在！
    str_PasswordLess,                   // 您输入的密码长度不够！
    str_PasswordNoSame,                 // 您两次输入的密码不一致！
    str_ErrorLogin,                     // 用户名或密码不正确！

    str_Exit,                           // 退出
    str_Login,                          // 登陆
    str_Register,                       // 注册
    str_Switch,                         // 切换语言
    str_Back,                           // 返回
    str_View,                           // 查看
    str_ModifyPassword,                 // 修改密码
    str_ModifyRight,                    // 修改公开权限
    str_Logout,                         // 注销
    str_Delete,                         // 删除
    str_Sort,                           // 排序
    str_Filter,                         // 筛选
    str_ByAccount,                      // 按账户名
    str_ByName,                         // 按姓名
    str_ByBirthday,                     // 按出生日期
    str_ByScore,                        // 按分数
    str_BySex,                          // 按性别
    str_ByRight,                        // 按公开权限
    str_Ascending,                      // 升序
    str_Dscending,                      // 降序
    str_Chinese,                        // 中文
    str_English,                        // English

    ResMax
};


class Resource final
{
    using string_type = std::string;

    Resource() = default;
    Resource(const Resource&) = delete;
    Resource& operator = (const Resource&) = delete;

public:

    static Resource& GetInstance() { static Resource res; return res; }

    using string_list = std::vector<string_type>;

    bool Init();
    unsigned int Load();

    const string_type& GetRes(unsigned int resType, unsigned int resId) const { return m_Res[resType][resId]; }

private:

    string_list m_Res[ResTypeMax];

};

#define _Res_ Resource::GetInstance()
