#include "gm_Include.h"


//-----------------------------------------------------------------------------

static MgrRes   g_sMgrRes;

//-----------------------------------------------------------------------------

static const char g_strFileName[File_Max][Size_FileName] =
{
    "..\\Resources\\String.txt",
    "..\\Resources\\Admin.dll",
    "..\\Resources\\Person.dll"
};  // 文件池

//-----------------------------------------------------------------------------

static char g_strString[str_Max][Size_String] = { 0 };     // 字符串池


//=============================================================================
// 局部函数声明
//=============================================================================
static bool SaveData();                         // 保存链表
static bool LoadData();                         // 加载链表
static bool FreeData();                         // 释放链表
static bool LoadPerson();                       // 加载人员链表
static bool LoadAdmin();                        // 加载管理员链表
static bool SavePerson();                       // 保存人员链表
static bool SaveAdmin();                        // 保存管理员链表

static bool LoadString();                       // 加载字符串
static bool ShowString(int eID, bool bHas);     // 显示字符串
static const char* GetString(int eID);          // 获取字符串
static const char* GetFileName(int eFile);      // 获取文件名


//=============================================================================
// 函数定义
//=============================================================================
//-----------------------------------------------------------------------------

static const char* GetFileName(int eFile)
{
    if ( eFile >=0 && eFile < File_Max )
    {
        return g_strFileName[eFile];
    }

    return NULL;
}

//-----------------------------------------------------------------------------

static bool LoadString()
{    
    FILE *hFile = NULL;

    const char* pName = GetFileName(File_String);   // 获取文件名

    if( !pName ) return false;
    
    bnb_fopen(hFile, pName, "r");    // 打开文件

    if ( hFile ) 
    {
        int i = 0;

        // 循环读取字符串
        while ( i < str_Max && fgets(g_strString[i], Size_String, hFile) )
        {
            char* p = strchr(g_strString[i], '\n'); // 搜索并剔除换行符
            if ( p ) *p = 0;

            i++;
        }

        fclose(hFile);

        return true;
    }

    return false;
}

//-----------------------------------------------------------------------------

static const char* GetString( int eID )
{
    if ( eID >= 0 && eID < str_Max )
    {
        return g_strString[eID];
    }

    return NULL;
}

//-----------------------------------------------------------------------------

static bool ShowString( int eID, bool bHas )
{
    if ( eID >= 0 && eID < str_Max )
    {
        printf( bHas ? "%s\n" : "%s", g_strString[eID]);
        return true;
    }

    return false;
}
//-----------------------------------------------------------------------------

static bool LoadData()
{
    bool b1 = LoadPerson();
    bool b2 = LoadAdmin();

    return b1 && b2;
}

//-----------------------------------------------------------------------------

static bool SaveData()
{
    bool b1 = SavePerson();
    bool b2 = SaveAdmin();

    return b1 && b2;
}

//-----------------------------------------------------------------------------

static bool FreeData()
{
    g_pAppMgr->m_pMgrPer->m_pfFreeAdmin(g_pAppData->m_pHeadAdmin);

    g_pAppMgr->m_pMgrPer->m_pfFreePer(g_pAppData->m_pHeadPer);

    return true;
}

//-----------------------------------------------------------------------------

static bool LoadPerson()
{
    FILE* hFile = NULL;
    const char* file = g_pAppMgr->m_pMgrRes->m_pfGetFile(File_Person);

    if ( !file ) return false;

    bnb_fopen(hFile, file, "rb");

    if ( !hFile ) return false;

    for ( ; ; )
    {
        // 定义缓冲区
        char loginID[Length_LoginID_Max+1] = { 0 };
        char password[Length_Password_Max+1] = { 0 };
        char name[Length_Name+1] = { 0 };
        char date[Length_Date+1] = { 0 };
        int eSex = 0;
        int iScore = 0;
        int iPublic = 0;

        person* pPer = NULL;

        size_t ret = 0, size = 0;

        // 开始读取文件，如果指定要读的字符数和实际读到的字符数不相等，则读取结束。
        size = sizeof(loginID) - 1;
        ret = bnb_fread(loginID, size, 1, size, hFile);
        if ( ret != size ) break;

        size = sizeof(password) - 1;
        ret = bnb_fread(password, size, 1, size, hFile);
        if ( ret != size ) break;

        size = sizeof(name) - 1;
        ret = bnb_fread(name, size, 1, size, hFile);
        if ( ret != size ) break;

        size = sizeof(date) - 1;
        ret = bnb_fread(date, size, 1, size, hFile);
        if ( ret != size ) break;

        size = sizeof(eSex);
        ret = bnb_fread(&eSex, size, 1, size, hFile);
        if ( ret != size ) break;

        size = sizeof(iScore);
        ret = bnb_fread(&iScore, size, 1, size, hFile);
        if ( ret != size ) break;

        size = sizeof(iPublic);
        ret = bnb_fread(&iPublic, size, 1, size, hFile);
        if ( ret != size ) break;

        // 分配节点内存
        pPer = (person*)malloc(sizeof(person));

        if ( pPer )
        {
            memset(pPer, 0, sizeof(person));

            bnb_strcpy(pPer->m_strLoginID, Length_LoginID_Max+1, loginID);
            bnb_strcpy(pPer->m_strPassword, Length_Password_Max+1, password);
            bnb_strcpy(pPer->m_strName, Length_Name+1, name);
            bnb_strcpy(pPer->m_strDate, Length_Date+1, date);
            pPer->m_eSex = eSex;
            pPer->m_iScore = iScore;
            pPer->m_bPublic = (bool)iPublic;
            pPer->m_pNext = NULL;
            
            // 将节点添加到链表中
            g_pAppMgr->m_pMgrPer->m_pfAddPer(&g_pAppData->m_pHeadPer, pPer);
        }
    }

    fclose(hFile);

    return true;
}

//-----------------------------------------------------------------------------

static bool LoadAdmin()
{
    FILE* hFile = NULL;
    const char* file = g_pAppMgr->m_pMgrRes->m_pfGetFile(File_Admin);

    if ( !file ) return false;

    bnb_fopen(hFile, file, "rb");

    if ( !hFile ) return false;

    for ( ; ; )
    {
        // 定义缓冲区
        char loginID[Length_LoginID_Max+1] = { 0 };
        char password[Length_Password_Max+1] = { 0 };

        admin* item = NULL;

        size_t ret = 0, size = 0;

        size = sizeof(loginID) - 1;
        ret = bnb_fread(loginID, size, 1, size, hFile);
        if ( ret != size ) break;

        size = sizeof(password) - 1;
        ret = bnb_fread(password, size, 1, size, hFile);
        if ( ret != size ) break;

        item = (admin*)malloc(sizeof(admin));

        if ( item )
        {
            memset(item, 0, sizeof(admin));

            bnb_strcpy(item->m_strLoginID, Length_LoginID_Max+1, loginID);
            bnb_strcpy(item->m_strPassword, Length_Password_Max+1, password);
            item->m_pNext = NULL;
           
            // 将节点添加到链表中
            g_pAppMgr->m_pMgrPer->m_pfAddAdmin(&g_pAppData->m_pHeadAdmin, item);
        }
    }

    fclose(hFile);

    return true;
}

//-----------------------------------------------------------------------------

static bool SavePerson()
{
    FILE* hFile = NULL;

    const char* file = g_pAppMgr->m_pMgrRes->m_pfGetFile(File_Person);
    const person* pPer = g_pAppData->m_pHeadPer;

    if ( !file ) return false;

    bnb_fopen(hFile, file, "wb");

    if ( !hFile ) return false;

    for ( ; pPer; pPer = pPer->m_pNext)
    {
        int iPublic = pPer->m_bPublic;

        fwrite(pPer->m_strLoginID, 1, Length_LoginID_Max, hFile);
        fwrite(pPer->m_strPassword, 1, Length_Password_Max, hFile);
        fwrite(pPer->m_strName, 1, Length_Name, hFile);
        fwrite(pPer->m_strDate, 1, Length_Date, hFile);
        fwrite(&pPer->m_eSex, 1, sizeof(int), hFile);
        fwrite(&pPer->m_iScore, 1, sizeof(int), hFile);
        fwrite(&iPublic, 1, sizeof(int), hFile);
    }

    fclose(hFile);

    return true;
}

//-----------------------------------------------------------------------------

static bool SaveAdmin()
{
    FILE* hFile = NULL;

    const char* file = g_pAppMgr->m_pMgrRes->m_pfGetFile(File_Admin);
    const admin* item = g_pAppData->m_pHeadAdmin;

    if ( !file ) return false;

    bnb_fopen(hFile, file, "wb");

    if ( !hFile ) return false;
    
    for ( ; item; item = item->m_pNext)
    {
        fwrite(item->m_strLoginID, 1, Length_LoginID_Max, hFile);
        fwrite(item->m_strPassword, 1, Length_Password_Max, hFile);
    }

    fclose(hFile);

    return true;
}

//-----------------------------------------------------------------------------

bool InitMgrRes()
{
    g_sMgrRes.m_pfLoadData      = &LoadData;
    g_sMgrRes.m_pfSaveData      = &SaveData;
    g_sMgrRes.m_pfFreeData      = &FreeData;

    g_sMgrRes.m_pfGetFile       = &GetFileName;
    g_sMgrRes.m_pfGetString     = &GetString;
    g_sMgrRes.m_pfLoadString    = &LoadString;
    g_sMgrRes.m_pfShowString    = &ShowString;

    g_pAppMgr->m_pMgrRes = &g_sMgrRes;
    
    return true;
}


