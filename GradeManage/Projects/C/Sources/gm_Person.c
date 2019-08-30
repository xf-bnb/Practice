#include "gm_Include.h"


//=============================================================================
// 定义全局静态变量
//=============================================================================
static const char g_strSex[Sex_Max][Size_Word]  = { "♂ ", " ♀" };
static const char g_strPublic[2][Size_Word]     = { "×", "√" };

//-----------------------------------------------------------------------------

static MgrPer   g_sMgrPer;


//=============================================================================
// 局部函数声明
//=============================================================================
static bool AddAdmin(admin** head, admin* item);            // 添加管理员节点
static void FreeAdmin(admin* head);                         // 释放管理员链表
static bool AddPerson(person** head, person* item);         // 添加人员节点
static bool DelPerson(person** head, const char* strLogin); // 根据用户名删除人员节点
static void FreePerson(person* head);                       // 释放人员链表
static bool ShowPerson(const person* item);                 // 显示一个人员
static bool CopyPerson(person* item1, const person* item2); // 将 item2 拷贝到 item1 中。

/*
 *	函数：FindPerson
 *  功能：在链表中查找节点
 *  参数：head 指定链表头节点指针的地址，data 指定待比较数据。
 *  返回：找到的节点，找不到返回 NULL。
 */
static const person* FindPerson(const person* head, const siftdata* data);

/*
 *	函数：InsertPerson
 *  功能：有序插入节点
 *  参数：head 指定链表头节点指针的地址，item 指定待插节点，eSort 指定排序方式。
 *  返回：成功返回 true，否则返回 false。
 */
static bool InsertPerson(person** head, person* item, int eSort);   // 有序插入节点

/*
 *	函数：IsEqualSiftData
 *  功能：判断节点和筛选数据是否相等
 *  参数：item 指定待比较节点，data 指定待比较数据。
 *  返回：相等返回 true，否则返回 false。
 */
static bool IsEqualSiftData(const person* item, const siftdata* data);

/*
 *	函数：ComparePerson
 *  功能：比较2个节点的大小。(注：该函数假定待比较数据指针不为空，不做指针有效性检查。)
 *  参数：item1、item2 指定待比较的2个节点，eType 指定比较类型(参见枚举：E_SortType)。
 *  返回：item1>item2 返回 >0，item1==item2 返回 =0，item1<item2 返回 <0。
 */
static int ComparePerson(const person* item1, const person* item2, int eType);


//=============================================================================
// 函数定义
//=============================================================================
//-----------------------------------------------------------------------------

static bool AddAdmin( admin** head, admin* item )
{
    if ( !head || !item ) return false;

    if ( *head ) // 如果头节点不为空
    {
        admin* p = *head;

        while ( p->m_pNext ) p = p->m_pNext;    // 遍历到链表尾

        p->m_pNext = item;      // 将节点添加到尾部
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

    if ( *head ) // 如果头节点不为空
    {
        person* p = *head;

        while ( p->m_pNext ) p = p->m_pNext;    // 遍历到链表尾

        p->m_pNext = item;      // 将节点添加到尾部
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

    // 解析日期
    ymd = atoi(pPer->m_strDate);
    d = ymd % 100;
    m = (ymd / 100) % 100;
    y = ymd / 10000;

    // 组合输出格式化串
#if _MSC_VER >= 1500
	sprintf_s(format, Size_Buffer
#else
	sprintf(format
#endif
        , "| %%-%ds\t| %%-%ds\t| %%4d-%%02d-%%02d\t| %%s\t| %%4d\t| %%s\t|\n"
        , Length_LoginID_Max
        , Length_Name);
        
    // 打印节点
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

    memset(item1, 0, sizeof(person));   // 清空内存

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

