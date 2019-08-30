//=============================================================================
// �ļ���gm_Person.h
// �����������û��ṹ�������������û������ߡ�
// ˵����
//      �û����ͷ�Ϊ��ͨ�û�(person)�͹���Ա(admin)��
//      �������;�֧������ṹ��
//      �û���Ϣ������(MgrPer)�ṩ������ṹ�����в�����
//      ��������ӡ����ҡ�ɾ�����ͷš����롢�Ƚϡ����ơ�
//=============================================================================
#ifndef _GM_PERSON_H
#define _GM_PERSON_H


//-----------------------------------------------------------------------------

#define Length_LoginID_Min  3                   // �ʻ�����С����
#define Length_LoginID_Max  13                  // �˻�����󳤶�
#define Length_Password_Min 6                   // ������С����
#define Length_Password_Max 16                  // ������󳤶�
#define Length_Name         12                  // ������󳤶�
#define Length_Date         8                   // ���ڳ���
#define Score_Max           150                 // �������ֵ

//-----------------------------------------------------------------------------

typedef enum
{
    Per_Admin,      // ����Ա
    Per_Person,     // ��ͨ�û�

    Per_Max
}UserType;   // �û�����


//-----------------------------------------------------------------------------

typedef enum
{
    Sex_Male,        // ��
    Sex_Female,      // Ů

    Sex_Max
} E_Sex;    // �Ա�

//-----------------------------------------------------------------------------

typedef enum
{
    Sort_Account    = 0x0,  // ���û���
    Sort_Name       = 0x1,  // ������
    Sort_Date       = 0x2,  // ����������
    Sort_Score      = 0x3,  // ������
    Sort_Right      = 0x3,  // ��������

    Sort_Asce       = 0x0,  // ����
    Sort_Desc       = 0x4,  // ����
    Sort_Type       = 0x4   // ��������

}E_SortType;    // ����ʽ(�������������������������ʽ)


//-----------------------------------------------------------------------------

typedef enum
{
    Filter_Account  = 0x01, // ���û���
    Filter_ByName   = 0x02, // ������
    Filter_ByDate   = 0x04, // ����������
    Filter_BySex    = 0x08, // ���Ա�
    Filter_ByScore  = 0x10, // ������
    Filter_ByRight  = 0x20, // ������Ȩ��
    Filter_Mask     = 0x3f  // ɸѡ����
}E_SiftType;    // ɸѡ��ʽ



//-----------------------------------------------------------------------------

typedef struct
{
    int         m_eReason;  // ָ����Ч���ݱ�ʶ

    union{
        int     m_eSex;
        int     m_iScore;
        bool    m_bPublic;
        char    m_srtName[Length_Name+1];
        char    m_strDate[Length_Date+1];
        char    m_strLoginID[Length_LoginID_Max+1];
    }m_unData;

}siftdata;  // ɸѡ���ݽṹ


//-----------------------------------------------------------------------------

typedef struct _person
{
    char    m_strLoginID[Length_LoginID_Max+1];     // �˻���
    char    m_strPassword[Length_Password_Max+1];   // ����
    char    m_strName[Length_Name+1];               // ����
    char    m_strDate[Length_Date+1];               // ����
    int     m_eSex;                                 // �Ա�
    int     m_iScore;                               // ����
    bool    m_bPublic;                              // �Ƿ񹫿�

    struct _person* m_pNext;                        // ָ����һ���ڵ�ָ��

} person;   // ��Ա��Ϣ�ṹ


//-----------------------------------------------------------------------------

typedef struct _admin 
{
    char    m_strLoginID[Length_LoginID_Max+1];     // �˻���
    char    m_strPassword[Length_Password_Max+1];   // ����

    struct  _admin* m_pNext;                        // ָ����һ���ڵ�ָ��

}admin; // ����Ա��Ϣ�ṹ




//=============================================================================
// ������Ա��Ϣ������
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
    fpAddAdmin      m_pfAddAdmin;       // ��ӹ���Ա�ڵ�
    fpFreeAdmin     m_pfFreeAdmin;      // �ͷŹ���Ա����
        
    fpDelPer        m_pfDelPer;         // ɾ���ڵ�
    fpAddPer        m_pfAddPer;         // �����Ա�ڵ�
    fpFreePer       m_pfFreePer;        // �ͷ���Ա����
    fpFindPer       m_pfFindPer;        // ���ҽڵ�
    fpShowPer       m_pfShowPer;        // ��ʾ�ڵ�
    fpCopyPer       m_pfCopyPer;        // ���ƽṹ
    fpInsertPer     m_pfInsertPer;      // ����ڵ�

}MgrPer;    // ��Ա��Ϣ������


//-----------------------------------------------------------------------------

bool InitMgrPer();      // ��ʼ����Ա������

#endif