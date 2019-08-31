//=============================================================================
// 文件：gm_Person.h
// 描述：定义用户结构参数、常量，用户管理者。
// 说明：
//      用户类型分为普通用户(person)和管理员(admin)。
//      两种类型均支持链表结构。
//      用户信息管理者(MgrPer)提供对链表结构的所有操作。
//      包括：添加、查找、删除、释放、插入、比较、复制。
//=============================================================================
#ifndef _GM_PERSON_H
#define _GM_PERSON_H


//-----------------------------------------------------------------------------

#define Length_LoginID_Min  3                   // 帐户名最小长度
#define Length_LoginID_Max  13                  // 账户名最大长度
#define Length_Password_Min 6                   // 密码最小长度
#define Length_Password_Max 16                  // 密码最大长度
#define Length_Name         12                  // 姓名最大长度
#define Length_Date         8                   // 日期长度
#define Score_Max           150                 // 分数最大值

//-----------------------------------------------------------------------------

typedef enum
{
    Per_Admin,      // 管理员
    Per_Person,     // 普通用户

    Per_Max
}UserType;   // 用户类型


//-----------------------------------------------------------------------------

typedef enum
{
    Sex_Male,        // 男
    Sex_Female,      // 女

    Sex_Max
} E_Sex;    // 性别

//-----------------------------------------------------------------------------

typedef enum
{
    Sort_Account    = 0x0,  // 按用户名
    Sort_Name       = 0x1,  // 按姓名
    Sort_Date       = 0x2,  // 按出生日期
    Sort_Score      = 0x3,  // 按分数
    Sort_Right      = 0x3,  // 排序条件

    Sort_Asce       = 0x0,  // 升序
    Sort_Desc       = 0x4,  // 降序
    Sort_Type       = 0x4   // 排序类型

}E_SortType;    // 排序方式(排序条件和排序类型组成排序方式)


//-----------------------------------------------------------------------------

typedef enum
{
    Filter_Account  = 0x01, // 按用户名
    Filter_ByName   = 0x02, // 按姓名
    Filter_ByDate   = 0x04, // 按出生日期
    Filter_BySex    = 0x08, // 按性别
    Filter_ByScore  = 0x10, // 按分数
    Filter_ByRight  = 0x20, // 按公开权限
    Filter_Mask     = 0x3f  // 筛选条件
}E_SiftType;    // 筛选方式



//-----------------------------------------------------------------------------

typedef struct
{
    int         m_eReason;  // 指定有效数据标识

    union{
        int     m_eSex;
        int     m_iScore;
        bool    m_bPublic;
        char    m_srtName[Length_Name+1];
        char    m_strDate[Length_Date+1];
        char    m_strLoginID[Length_LoginID_Max+1];
    }m_unData;

}siftdata;  // 筛选数据结构


//-----------------------------------------------------------------------------

typedef struct _person
{
    char    m_strLoginID[Length_LoginID_Max+1];     // 账户名
    char    m_strPassword[Length_Password_Max+1];   // 密码
    char    m_strName[Length_Name+1];               // 姓名
    char    m_strDate[Length_Date+1];               // 日期
    int     m_eSex;                                 // 性别
    int     m_iScore;                               // 分数
    bool    m_bPublic;                              // 是否公开

    struct _person* m_pNext;                        // 指向下一个节点指针

} person;   // 人员信息结构


//-----------------------------------------------------------------------------

typedef struct _admin 
{
    char    m_strLoginID[Length_LoginID_Max+1];     // 账户名
    char    m_strPassword[Length_Password_Max+1];   // 密码

    struct  _admin* m_pNext;                        // 指向下一个节点指针

}admin; // 管理员信息结构




//=============================================================================
// 声明人员信息管理者
//=============================================================================
typedef bool    (*fpAddAdmin)(admin**, admin*);
typedef void    (*fpFreeAdmin)(admin*);
typedef bool    (*fpDelPer)(person**, const char*);
typedef bool    (*fpAddPer)(person**, person*);
typedef void    (*fpFreePer)(person*);
typedef bool    (*fpShowPer)(const person*);
typedef bool    (*fpCopyPer)(person*, const person*);
typedef bool    (*fpInsertPer)(person**, person*, int);
typedef const person* (*fpFindPer)(const person*, const siftdata*);

typedef struct
{
    fpAddAdmin      m_pfAddAdmin;       // 添加管理员节点
    fpFreeAdmin     m_pfFreeAdmin;      // 释放管理员链表
        
    fpDelPer        m_pfDelPer;         // 删除节点
    fpAddPer        m_pfAddPer;         // 添加人员节点
    fpFreePer       m_pfFreePer;        // 释放人员链表
    fpFindPer       m_pfFindPer;        // 查找节点
    fpShowPer       m_pfShowPer;        // 显示节点
    fpCopyPer       m_pfCopyPer;        // 复制结构
    fpInsertPer     m_pfInsertPer;      // 插入节点

}MgrPer;    // 人员信息管理者


//-----------------------------------------------------------------------------

bool InitMgrPer();      // 初始化人员管理者

#endif