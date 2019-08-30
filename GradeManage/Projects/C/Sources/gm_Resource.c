#include "gm_Include.h"


//-----------------------------------------------------------------------------

static MgrRes   g_sMgrRes;

//-----------------------------------------------------------------------------

static const char g_strFileName[File_Max][Size_FileName] =
{
    "..\\Resources\\String.txt",
    "..\\Resources\\Admin.dll",
    "..\\Resources\\Person.dll"
};  // �ļ���

//-----------------------------------------------------------------------------

static char g_strString[str_Max][Size_String] = { 0 };     // �ַ�����


//=============================================================================
// �ֲ���������
//=============================================================================
static bool SaveData();                         // ��������
static bool LoadData();                         // ��������
static bool FreeData();                         // �ͷ�����
static bool LoadPerson();                       // ������Ա����
static bool LoadAdmin();                        // ���ع���Ա����
static bool SavePerson();                       // ������Ա����
static bool SaveAdmin();                        // �������Ա����

static bool LoadString();                       // �����ַ���
static bool ShowString(int eID, bool bHas);     // ��ʾ�ַ���
static const char* GetString(int eID);          // ��ȡ�ַ���
static const char* GetFileName(int eFile);      // ��ȡ�ļ���


//=============================================================================
// ��������
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

    const char* pName = GetFileName(File_String);   // ��ȡ�ļ���

    if( !pName ) return false;
    
    bnb_fopen(hFile, pName, "r");    // ���ļ�

    if ( hFile ) 
    {
        int i = 0;

        // ѭ����ȡ�ַ���
        while ( i < str_Max && fgets(g_strString[i], Size_String, hFile) )
        {
            char* p = strchr(g_strString[i], '\n'); // �������޳����з�
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
        // ���建����
        char loginID[Length_LoginID_Max+1] = { 0 };
        char password[Length_Password_Max+1] = { 0 };
        char name[Length_Name+1] = { 0 };
        char date[Length_Date+1] = { 0 };
        int eSex = 0;
        int iScore = 0;
        int iPublic = 0;

        person* pPer = NULL;

        size_t ret = 0, size = 0;

        // ��ʼ��ȡ�ļ������ָ��Ҫ�����ַ�����ʵ�ʶ������ַ�������ȣ����ȡ������
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

        // ����ڵ��ڴ�
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
            
            // ���ڵ���ӵ�������
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
        // ���建����
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
           
            // ���ڵ���ӵ�������
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


