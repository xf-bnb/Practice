#include "gm_Include.h"


//=============================================================================
// ����ȫ�־�̬����
//=============================================================================
static const char g_strSex[Sex_Max][Size_Word]  = { "�� ", " ��" };
static const char g_strPublic[2][Size_Word]     = { "��", "��" };

//-----------------------------------------------------------------------------

static MgrPer   g_sMgrPer;


//=============================================================================
// �ֲ���������
//=============================================================================
static bool AddAdmin(admin** head, admin* item);            // ��ӹ���Ա�ڵ�
static void FreeAdmin(admin* head);                         // �ͷŹ���Ա����
static bool AddPerson(person** head, person* item);         // �����Ա�ڵ�
static bool DelPerson(person** head, const char* strLogin); // �����û���ɾ����Ա�ڵ�
static void FreePerson(person* head);                       // �ͷ���Ա����
static bool ShowPerson(const person* item);                 // ��ʾһ����Ա
static bool CopyPerson(person* item1, const person* item2); // �� item2 ������ item1 �С�

/*
 *	������FindPerson
 *  ���ܣ��������в��ҽڵ�
 *  ������head ָ������ͷ�ڵ�ָ��ĵ�ַ��data ָ�����Ƚ����ݡ�
 *  ���أ��ҵ��Ľڵ㣬�Ҳ������� NULL��
 */
static const person* FindPerson(const person* head, const siftdata* data);

/*
 *	������InsertPerson
 *  ���ܣ��������ڵ�
 *  ������head ָ������ͷ�ڵ�ָ��ĵ�ַ��item ָ������ڵ㣬eSort ָ������ʽ��
 *  ���أ��ɹ����� true�����򷵻� false��
 */
static bool InsertPerson(person** head, person* item, int eSort);   // �������ڵ�

/*
 *	������IsEqualSiftData
 *  ���ܣ��жϽڵ��ɸѡ�����Ƿ����
 *  ������item ָ�����ȽϽڵ㣬data ָ�����Ƚ����ݡ�
 *  ���أ���ȷ��� true�����򷵻� false��
 */
static bool IsEqualSiftData(const person* item, const siftdata* data);

/*
 *	������ComparePerson
 *  ���ܣ��Ƚ�2���ڵ�Ĵ�С��(ע���ú����ٶ����Ƚ�����ָ�벻Ϊ�գ�����ָ����Ч�Լ�顣)
 *  ������item1��item2 ָ�����Ƚϵ�2���ڵ㣬eType ָ���Ƚ�����(�μ�ö�٣�E_SortType)��
 *  ���أ�item1>item2 ���� >0��item1==item2 ���� =0��item1<item2 ���� <0��
 */
static int ComparePerson(const person* item1, const person* item2, int eType);


//=============================================================================
// ��������
//=============================================================================
//-----------------------------------------------------------------------------

static bool AddAdmin( admin** head, admin* item )
{
    if ( !head || !item ) return false;

    if ( *head ) // ���ͷ�ڵ㲻Ϊ��
    {
        admin* p = *head;

        while ( p->m_pNext ) p = p->m_pNext;    // ����������β

        p->m_pNext = item;      // ���ڵ���ӵ�β��
    }
    else
    {
        *head = item;
    }

    return true;
}

//-----------------------------------------------------------------------------

static void FreeAdmin( admin* head )
{
    while ( head )
    {
        admin* item = head;
        head = head->m_pNext;
        free(item);
    }
}

//-----------------------------------------------------------------------------

static bool AddPerson( person** head, person* item )
{
    if ( !head || !item ) return false;

    if ( *head ) // ���ͷ�ڵ㲻Ϊ��
    {
        person* p = *head;

        while ( p->m_pNext ) p = p->m_pNext;    // ����������β

        p->m_pNext = item;      // ���ڵ���ӵ�β��
    }
    else
    {
        *head = item;
    }

    return true;
}

//-----------------------------------------------------------------------------

static void FreePerson( person* head )
{
    while ( head )
    {
        person* item = head->m_pNext;
        free(head);
        head = item;
    }
}

//-----------------------------------------------------------------------------

static bool DelPerson( person** head, const char* strLogin )
{
    person* pLast = *head;
    person* pNext = *head;

    while ( pNext )
    {
        if ( !StringCompare(strLogin, pNext->m_strLoginID, false) )
        {
            if ( pNext == *head )
            {
                *head = pNext->m_pNext;
            } 
            else
            {
                pLast->m_pNext = pNext->m_pNext;
            }

            free(pNext);
            return true;
        }

        pLast = pNext;
        pNext = pNext->m_pNext;
    }
    
    return false;
}

//-----------------------------------------------------------------------------

static const person* FindPerson( const person* head, const siftdata* data )
{
    while ( head )
    {
        if ( IsEqualSiftData(head, data) ) return head;
        
        head = head->m_pNext;
    }

    return NULL;
}

//-----------------------------------------------------------------------------

static bool ShowPerson( const person* pPer )
{
    int ymd = 0, y = 0, m = 0, d = 0;
    char format[Size_Buffer] = { 0 };

    if ( !pPer ) return false;

    // ��������
    ymd = atoi(pPer->m_strDate);
    d = ymd % 100;
    m = (ymd / 100) % 100;
    y = ymd / 10000;

    // ��������ʽ����
#if _MSC_VER >= 1500
	sprintf_s(format, Size_Buffer
#else
	sprintf(format
#endif
        , "| %%-%ds\t| %%-%ds\t| %%4d-%%02d-%%02d\t| %%s\t| %%4d\t| %%s\t|\n"
        , Length_LoginID_Max
        , Length_Name);
        
    // ��ӡ�ڵ�
    printf(format
        , pPer->m_strLoginID
        , pPer->m_strName
        , y, m, d
        , g_strSex[pPer->m_eSex]
        , pPer->m_iScore
        , g_strPublic[(int)pPer->m_bPublic]);

    return true;
}

//-----------------------------------------------------------------------------

static bool CopyPerson( person* item1, const person* item2 )
{
    if ( !item1 || !item2 ) return false;

    memset(item1, 0, sizeof(person));   // ����ڴ�

    bnb_strcpy(item1->m_strLoginID, Length_LoginID_Max+1, item2->m_strLoginID);
    bnb_strcpy(item1->m_strPassword, Length_Password_Max+1, item2->m_strPassword);
    bnb_strcpy(item1->m_strName, Length_Name+1, item2->m_strName);
    bnb_strcpy(item1->m_strDate, Length_Date+1, item2->m_strDate);
    item1->m_eSex       = item2->m_eSex;
    item1->m_iScore     = item2->m_iScore;
    item1->m_bPublic    = item2->m_bPublic;
    item1->m_pNext      = NULL;

    return true;
}

//-----------------------------------------------------------------------------

static bool IsEqualSiftData( const person* item, const siftdata* data )
{
    if ( !item ) return false;

    if ( !data ) return true;

    switch ( data->m_eReason & Filter_Mask )
    {
    case Filter_Account:
        return !StringCompare(item->m_strLoginID, data->m_unData.m_strLoginID, false);

    case Filter_ByName:
        return !StringCompare(item->m_strName, data->m_unData.m_srtName, true);

    case Filter_ByDate:
        return !StringCompare(item->m_strDate, data->m_unData.m_strDate, true);

    case Filter_BySex:
        return (item->m_eSex == data->m_unData.m_eSex);

    case Filter_ByScore:
        return (item->m_iScore == data->m_unData.m_iScore);

    case Filter_ByRight:
        return (item->m_bPublic == data->m_unData.m_bPublic);

    default:
        return true;
    }
}

//-----------------------------------------------------------------------------

static int ComparePerson( const person* item1, const person* item2, int eType )
{
    switch ( eType & Sort_Right )
    {
    case Sort_Account:
        return StringCompare(item1->m_strLoginID, item2->m_strLoginID, false);

    case Sort_Name:
        return StringCompare(item1->m_strName, item2->m_strName, true);

    case Sort_Date:
        return (atoi(item2->m_strDate) - atoi(item1->m_strDate));

    case Sort_Score:
        return (item1->m_iScore - item2->m_iScore);

    default:
        return 0;
    }
}

//-----------------------------------------------------------------------------

static bool InsertPerson( person** head, person* item, int eSort )
{
    if ( !head ) return false;
    if ( !item ) return false;

    if ( *head )
    {
        person* last = *head;
        person* next = *head;
        int type = eSort & Sort_Type;

        while ( next )
        {
            if ( Sort_Asce == type && ComparePerson(next, item, eSort) >= 0
                || Sort_Desc == type && ComparePerson(next, item, eSort) <= 0 )
            {
                if ( next == *head )
                {
                    item->m_pNext = *head;
                    *head = item;
                } 
                else
                {
                    last->m_pNext = item;
                    item->m_pNext = next;
                }

                return true;
            }

            last = next;
            next = next->m_pNext;
        }

        last->m_pNext = item;
    } 
    else
    {
        *head = item;
    }

    return true;
}

//-----------------------------------------------------------------------------

bool InitMgrPer()
{
    g_sMgrPer.m_pfAddAdmin  = &AddAdmin;
    g_sMgrPer.m_pfFreeAdmin = &FreeAdmin;

    g_sMgrPer.m_pfDelPer    = &DelPerson;
    g_sMgrPer.m_pfAddPer    = &AddPerson;
    g_sMgrPer.m_pfFreePer   = &FreePerson;
    g_sMgrPer.m_pfFindPer   = &FindPerson;
    g_sMgrPer.m_pfShowPer   = &ShowPerson;
    g_sMgrPer.m_pfCopyPer   = &CopyPerson;
    g_sMgrPer.m_pfInsertPer = &InsertPerson;

    g_pAppMgr->m_pMgrPer = &g_sMgrPer;

    return true;
}

