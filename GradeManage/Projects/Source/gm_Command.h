//=============================================================================
// �ļ���gm_Command.h
// �������������������˵���ʶ������״̬�������롢��������ߡ�
// ˵����
//      ϵͳ��״̬��Ϊ��
//      δ��½(LoginOut)����ͨ�û���½(LoginPerson)������Ա��½(LoginAdmin)��
//      ÿһ��״̬���ж�Ӧ�Ĳ˵���ָ�
//      �û��͹���Ա��������Ҫ�����ڶ���Ա�Ĳ����Ͳ鿴���ܡ�
//      ����Ա��½���Բ鿴�������û���Ϣ���Լ������û���ɾ��ָ���û���
//      ��ͨ�û���½ֻ�ܲ鿴������Ȩ�޹����������û���Ϣ���Լ�ע���Լ���
//=============================================================================
#ifndef _GM_COMMAND_H
#define _GM_COMMAND_H


//-----------------------------------------------------------------------------

#define ScreenWidth     73          // ��Ļ���
#define ScreenHeight    15          // ��Ļ�߶�


//-----------------------------------------------------------------------------

typedef enum _cmdMain
{
    cmdMain_Exit,       // �˳�
    cmdMain_Login,      // ��½
    cmdMain_Register,   // ע��

    cmdMain_Max
}cmdMain;   // ���˵�


//-----------------------------------------------------------------------------

typedef enum _cmdChild
{
    cmdCh_Back,         // ����
    cmdCh_Look,         // �鿴
    cmdCh_Password,     // �޸�����
    cmdCh_Public,       // �޸Ĺ���Ȩ��
    cmdCh_Cancel,       // ע��

    cmdCh_Max
}cmdChild;  // �û��Ӳ˵�


//-----------------------------------------------------------------------------

typedef enum _cmdAdmin
{
    cmdAd_Back,         // ����
    cmdAd_Look,         // �鿴
    cmdAd_Password,     // �޸�����
    cmdAd_Delete,       // ɾ��

    cmdAd_Max
}cmdAdmin;  // ����Ա�˵�


//-----------------------------------------------------------------------------

typedef enum _cmdLook
{
    cmdLook_Back,       // ����
    cmdLook_Sort,       // ����
    cmdLook_Sift,       // ɸѡ

    cmdLook_Max
}cmdLook;  // ��ѯ�˵�

//-----------------------------------------------------------------------------

typedef enum _cmdSort
{
    cmdSort_Back,       // ����
    cmdSort_LoginID,    // ���û���
    cmdSort_Name,       // ������
    cmdSort_Date,       // ����������
    cmdSort_Score,      // ������

    cmdSort_Max
}cmdSort;   // ����˵�

//-----------------------------------------------------------------------------

typedef enum _cmdSift
{
    cmdSift_Back,       // ����
    cmdSift_LoginID,    // ���û���
    cmdSift_ByName,     // ������
    cmdSift_ByDate,     // ����������
    cmdSift_ByScore,    // ������
    cmdSift_BySex,      // ���Ա�
    cmdSift_ByPublic,   // ������Ȩ��

    cmdSift_Max
}cmdSift;   // ɸѡ�˵�

//-----------------------------------------------------------------------------

typedef enum _LoginState
{
    State_None  = -1,

    State_LoginOut,     // δ��½
    State_LoginPerson,  // ��Ա��½
    State_LoginAdmin,   // ����Ա��½

    State_Max
}LoginState;  // ��½״̬

//-----------------------------------------------------------------------------

typedef enum _ErrorCode
{
    err_None    = -1,
    err_MemoryIsLess,       // �ڴ治��
    err_StateExp,           // ״̬�쳣
    err_SuccessLogin,       // ��½�ɹ�
    err_SuccessRegister,    // ע��ɹ�
    err_SuccessCancel,      // ע���ɹ�
    err_SuccessDelete,      // ɾ���ɹ�
    err_SuccessModifyPwd,   // �޸�����ɹ�
    err_SuccessModifyPub,   // �޸Ĺ���Ȩ�޳ɹ�
    err_FailCancel,         // ע��ʧ��
    err_FailDelete,         // ɾ��ʧ��
    err_CancelCancel,       // ȡ��ע��
    err_ErrorOldPwd,        // ���������벻��ȷ
    err_InvalidCommand,     // ������Ч
    err_InvalidLoginID,     // �û�����Ч
    err_LoginIsExist,       // �û����Ѿ�����
    err_PasswordLess,       // ���볤�Ȳ���
    err_PasswordNoSame,     // ���벻һ��
    err_ErrorLogin,         // �û��������벻��ȷ
    err_InvalidDate,        // ����������Ч

    err_Max
}ErrorCode; // ������

//-----------------------------------------------------------------------------

typedef enum _MenuType
{
    menu_Main,              // ���˵�
    menu_Person,            // ��ͨ�û��˵�
    menu_Admin,             // ����Ա�˵�
    menu_Look,              // �鿴�˵�
    menu_Sort,              // ����˵�
    menu_Sift               // ɸѡ�˵�
}menuType;  // �˵�����


//=============================================================================
// ����ָ�������
//=============================================================================
typedef void    (*fpCommand)();
typedef void    (*fpShowScreen)(int);
typedef void    (*fpShowMenu)(int);

typedef struct
{
    fpCommand       m_pfCommand[State_Max];     // ָ���ָ������
    fpShowMenu      m_pfShowMenu;               // ��ʾ�˵�
    fpShowScreen    m_pfShowScreen;             // ��ʾ����
}MgrCmd;    // ָ�������


//-----------------------------------------------------------------------------

bool InitMerCmd();      // ��ʼ�����������


#endif