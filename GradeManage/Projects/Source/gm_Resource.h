//=============================================================================
// �ļ���gm_Resource.h
// ������������Դ�ļ����͡��ַ���ID����Դ�����ߡ�
// ˵����
//      ��Դ������(MgrPer)�ṩ����Դ�����в�����
//      �������ַ����ļ��ء���ȡ����ʾ�����ݵļ��ء����桢�ͷš�
//=============================================================================
#ifndef _GM_RESOURCE_H
#define _GM_RESOURCE_H

//-----------------------------------------------------------------------------

typedef enum
{
    File_String,                        // �ַ����ļ�
    File_Admin,                         // ����Ա�����ļ�
    File_Person,                        // ��Ա��Ϣ�ļ�

    File_Max
}AppFile;   // �ļ�����ö��


//-----------------------------------------------------------------------------

typedef enum
{
    str_Welcom,                         // ��ӭ������Ϣ����ϵͳ��
    str_ThankYou,                       // ллʹ�ã��ټ���
    str_VerifyCancel,                   // (��ʾ��ע�����û�����ɾ��)��ȷ��Ҫע����(y/n)��
    str_RecordCount,                    // ��%d����¼��

    str_InputCmd,                       // ������һ�����
    str_InputLoginID,                   // �������û�����
    str_InputPassword,                  // ���������룺
    str_AgainPassword,                  // ��ȷ������:
    str_InputName,                      // ������������
    str_InputBirth,                     // �������������(yyyymmdd)��
    str_InputSex,                       // �������Ա�(�У�M��Ů��W)��
    str_InputScore,                     // ���������(0-%d)��
    str_InputPublic,                    // ��ѡ�������Ƿ񹫿�(y/n)��
    str_InputSort,                      // ����������ʽ(a������d������)��
    str_InputOldPwd,                    // ����������룺
    str_InputNewPwd,                    // �����������룺

    str_MemoryIsLess,                   // �ڴ治����
    str_StateExp,                       // ״̬�쳣��
    str_SuccessLogin,                   // ��½�ɹ���
    str_SuccessRegister,                // ע��ɹ���
    str_SuccessCancel,                  // ע���ɹ���
    str_SuccessDelete,                  // ɾ���ɹ���
    str_SuccessModifyPwd,               // �޸�����ɹ���
    str_SuccessModifyPub,               // �޸Ĺ���Ȩ�޳ɹ���
    str_FailCancel,                     // ע��ʧ�ܣ�
    str_FailDelete,                     // ɾ��ʧ�ܣ�
    str_CancelCancel,                   // ע��������ȡ����
    str_ErrorOldPwd,                    // ���ľ��������벻��ȷ��
    str_InvalidCommand,                 // �������������Ч��
    str_InvalidLoginID,                 // ��������û�����Ч��
    str_LoginIsExist,                   // �û����Ѿ����ڣ�
    str_PasswordLess,                   // ����������볤�Ȳ�����
    str_PasswordNoSame,                 // ��������������벻һ�£�
    str_ErrorLogin,                     // �û��������벻��ȷ��
    str_InvalidDate,                    // ������ĳ���������Ч��

    str_Exit,                           // �˳�
    str_Login,                          // ��½
    str_Register,                       // ע��
    str_Back,                           // ����
    str_Look,                           // �鿴
    str_ModifyPassword,                 // �޸�����
    str_ModifyPublic,                   // �޸Ĺ���Ȩ��
    str_Cancel,                         // ע��
    str_Delete,                         // ɾ��
    str_Sort,                           // ����
    str_Sift,                           // ɸѡ
    str_ByLoginID,                      // ���û���
    str_ByName,                         // ������
    str_ByDate,                         // ����������
    str_ByScore,                        // ������
    str_BySex,                          // ���Ա�
    str_ByPublic,                       // ������Ȩ��

    str_Max
}AppString; // �ַ���ID


//=============================================================================
// ������Դ�����߽ṹ
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
    fpLoadData      m_pfLoadData;       // ��������
    fpSaveData      m_pfSaveData;       // ��������
    fpFreeData      m_pfFreeData;       // �ͷ�����

    fpGetFile       m_pfGetFile;        // ��ȡ�ļ���
    fpGetString     m_pfGetString;      // ��ȡ�ַ���
    fpLoadString    m_pfLoadString;     // �����ַ���
    fpShowString    m_pfShowString;     // ��ʾ�ַ���
}MgrRes;    // ��Դ�����߽ṹ


//-----------------------------------------------------------------------------

bool    InitMgrRes();   // ��ʼ����Դ������


#endif