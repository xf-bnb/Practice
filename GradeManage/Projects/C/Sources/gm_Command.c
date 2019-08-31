#include "gm_Include.h"


//-----------------------------------------------------------------------------

static MgrCmd   g_sMgrCmd;


//=============================================================================
// �ֲ���������
//=============================================================================
static void MenuMain();                 // �������˵��߼�
static void MenuPerson();               // �����Ӳ˵��߼�
static void MenuAdmin();                // �������Ա�˵�
static void MenuLook();                 // ����鿴�˵��߼�
static void MenuSort();                 // ��������˵��߼�
static void MenuSift();                 // ����ɸѡ�˵��߼�

static int  CmdLogin();                 // ������Ա��½�߼�
static int  CmdRegister();              // ������Աע���߼�
static int  CmdCancel();                // ����ע��
static int  CmdDelete();                // ����ɾ��
static int  ModifyPassword();           // �����޸�����
static int  ModifyPublic();             // �����޸Ĺ���Ȩ��
static int  GetCommand();               // ��ȡ��������

static int  InputSex();                 // ¼���Ա�
static int  InputScore();               // ¼�����
static void InputLoginID(char* buf);    // ¼���û���
static void InputName(char* buf);       // ¼������
static void InputDate(char* buf);       // ¼������
static bool InputPublic();              // ¼�빫��Ȩ��
static void InputPassword(char* buf, int eID);  // ¼������

static void ShowTableHead();            // ��ʾ�б�ͷ
static void ShowScreen(int eID);        // ��ʾ����
static void ShowMenu(int eMenu);        // ��ʾ�˵�
static void ShowSeparator(bool bHas);   // ��ʾ�ָ���
static bool ShowErrorCode(int eCode);   // ��ʾ������Ϣ
static void ShowPerBySort(int eSort);   // ������ʾ�ڵ�
static void ShowPerBySift(const siftdata* data); //ɸѡ��ʾ�ڵ�

static bool CheckLoginID(const char* str);                          // ����û����Ϸ���
static bool VerifyLogin(const char* name, const char* pwd);         // �����½ƥ���߼�
static bool LoginIdIsExist(const char* strlogin);                   // �ж��û����Ƿ����
static void ShowPersonList(const person* head);                     // ��ʾһ����Ա����


//=============================================================================
// ��������
//=============================================================================
//-----------------------------------------------------------------------------

static void MenuMain()
{
    int key = 0;
    int err = err_None;

    ShowMenu(menu_Main);

    key = GetCommand();

    switch ( key )
    {
    case cmdMain_Exit:
        g_pAppData->m_eState = State_None;
        break;

    case cmdMain_Login:
        err = CmdLogin();
        break;

    case cmdMain_Register:
        err = CmdRegister();
        break;

    default:
        err = err_InvalidCommand;
        break;
    }

    ShowErrorCode(err);
}

//-----------------------------------------------------------------------------

static void MenuPerson()
{
    int key = 0;
    int err = err_None;

    ShowMenu(menu_Person);

    key = GetCommand();

    switch ( key )
    {
    case cmdCh_Back:
        g_pAppData->m_pCurPer = NULL;
        g_pAppData->m_eState = State_LoginOut;
        return;

    case cmdCh_Look:
        MenuLook();
        return;

    case cmdCh_Password:
        err = ModifyPassword();
        break; 

    case cmdCh_Public:
        err = ModifyPublic();
        break;

    case cmdCh_Cancel:
        err = CmdCancel();
        break;

    default:
        err = str_InvalidCommand;
        break;
    }

    ShowErrorCode(err);
}

//-----------------------------------------------------------------------------

static void MenuAdmin()
{
    char key = 0;
    int err = err_None;

    ShowMenu(menu_Admin);

    key = GetCommand();

    switch ( key )
    {
    case cmdAd_Back:
        g_pAppData->m_pCurAdmin = NULL;
        g_pAppData->m_eState = State_LoginOut;
        return;

    case cmdAd_Look:
        MenuLook();
        return;

    case cmdAd_Password:
        err = ModifyPassword();
        break; 

    case cmdAd_Delete:
        err = CmdDelete();
        break;

    default:
        err = str_InvalidCommand;
        break;
    }

    ShowErrorCode(err);
}

//-----------------------------------------------------------------------------

static void MenuLook()
{
    for ( ; ; )
    {
        char key = 0;

        ShowMenu(menu_Look);

        key = GetCommand();

        switch ( key )
        {
        case cmdLook_Back:
            return;

        case cmdLook_Sort:
            MenuSort();
            break;

        case cmdLook_Sift:
            MenuSift();
            break; 
            
        default:
            g_pAppMgr->m_pMgrRes->m_pfShowString(str_InvalidCommand, true);
            break;
        }
    }
}

//-----------------------------------------------------------------------------

static void MenuSort()
{
    for ( ; ; )
    {
        char cmd = 0;

        ShowMenu(menu_Sort);

        cmd = GetCommand();

        if ( cmdSort_Back == cmd )
        {
            return;
        }
        else if ( 0 < cmd && cmd < cmdSort_Max )
        {
            int eType = 0;
            char key = 0;

            g_pAppMgr->m_pMgrRes->m_pfShowString(str_InputSort, false);

            for (key = 'a'; key != '\r'; key = GetChar())
            {
                if ('a' == key)
                {
                    eType = Sort_Asce;
                    printf("%c\b", key);
                }

                if ('d' == key)
                {
                    eType = Sort_Desc;
                    printf("%c\b", key);
                }
            }
            
            ShowPerBySort((cmd-1) | eType);
        }
        else
        {
            g_pAppMgr->m_pMgrRes->m_pfShowString(str_InvalidCommand, true);
        }
    }
}

//-----------------------------------------------------------------------------

static void MenuSift()
{
    for ( ; ; )
    {
        char key = 0;
        siftdata data = { 0 };

        ShowMenu(menu_Sift);

        key = GetCommand();

        switch ( key )
        {
        case cmdSift_Back:
            return;

        case cmdSift_LoginID:
            {
                InputLoginID(data.m_unData.m_strLoginID);
                if ( CheckLoginID(data.m_unData.m_strLoginID) )
                {
                    data.m_eReason = Filter_Account;
                } 
                else
                {
                    ShowErrorCode(err_InvalidLoginID);
                }

                break;
            }

        case cmdSift_ByName:
            data.m_eReason = Filter_ByName;
            InputName(data.m_unData.m_srtName);
            break; 

        case cmdSift_ByDate:
            data.m_eReason = Filter_ByDate;
            InputDate(data.m_unData.m_strDate);
            break;

        case cmdSift_BySex:
            data.m_eReason = Filter_BySex;
            data.m_unData.m_eSex = InputSex();
            break;

        case cmdSift_ByScore:
            data.m_eReason = Filter_ByScore;
            data.m_unData.m_iScore = InputScore();
            break;

        case cmdSift_ByPublic:
            {
                if ( State_LoginAdmin == g_pAppData->m_eState )
                {
                    data.m_eReason = Filter_ByRight;
                    data.m_unData.m_bPublic = InputPublic();
                    break;
                }
            }

        default:
            g_pAppMgr->m_pMgrRes->m_pfShowString(str_InvalidCommand, true);
            break;
        }

        if ( data.m_eReason & Filter_Mask )
        {
            ShowPerBySift(&data);
        }
    }
}

//-----------------------------------------------------------------------------

static int GetCommand()
{
    char key = g_cSymbolSpace;
    char last = 0;

    g_pAppMgr->m_pMgrRes->m_pfShowString(str_InputCmd, false);

    for ( last = GetChar(); '\r' != last; last = GetChar())
    {
        key = last;

        if ( '\b' == key ) key = g_cSymbolSpace;

        if ( g_cSymbolSpace == key || isdigit(key) )
        {
            printf("%c\b", key);
        }
    }

#ifdef _MSC_VER
    system("cls");
#else
    system("clear");
#endif

    return (key - '0');
}

//-----------------------------------------------------------------------------

static int CmdLogin()
{
    char loginID[Length_LoginID_Max+1] = { 0 };
    char password[Length_Password_Max+1] = { 0 };

    InputLoginID(loginID);

    InputPassword(password, str_InputPassword);

    if ( !CheckLoginID(loginID) ) return err_InvalidLoginID;

    if ( !VerifyLogin(loginID, password) ) return err_ErrorLogin;

    return err_SuccessLogin;
}

//-----------------------------------------------------------------------------

static int CmdRegister()
{
    // ���建����
    char loginID[Length_LoginID_Max+1] = { 0 };
    char password[Length_Password_Max+1] = { 0 };
    char password2[Length_Password_Max+1] = { 0 };
    char name[Length_Name+1] = { 0 };
    char date[Length_Date+1] = { 0 };
    int eSex = 0;
    int iScore = 0;
    bool bPublic = 0;

    person* pPer = NULL;
    
    // ¼����Ϣ
    InputLoginID(loginID);

    InputPassword(password, str_InputPassword);
    InputPassword(password2, str_AgainPassword);

    InputName(name);

    InputDate(date);

    eSex = InputSex();

    iScore = InputScore();

    bPublic = InputPublic();

    // У���û���
    if ( !CheckLoginID(loginID) ) return err_InvalidLoginID;
    if ( LoginIdIsExist(loginID) ) return err_LoginIsExist;

    // У������
    if ( strlen(password) < Length_Password_Min ) return err_PasswordLess;
    if ( StringCompare(password, password2, true) ) return err_PasswordNoSame;
    
    // �����ڴ�
    pPer = (person*)malloc(sizeof(person));

    // ��ʼ���ڵ�
    if ( !pPer ) return err_MemoryIsLess;

    memset(pPer, 0, sizeof(person));

    bnb_strcpy(pPer->m_strLoginID, Length_LoginID_Max+1, loginID);
    bnb_strcpy(pPer->m_strPassword, Length_Password_Max+1, password);
    bnb_strcpy(pPer->m_strName, Length_Name+1, name);
    bnb_strcpy(pPer->m_strDate, Length_Date+1, date);
    pPer->m_eSex = eSex;
    pPer->m_iScore = iScore;
    pPer->m_bPublic = bPublic;
    pPer->m_pNext = NULL;

    g_pAppMgr->m_pMgrPer->m_pfAddPer(&g_pAppData->m_pHeadPer, pPer); // ���ڵ���ӵ�������

    g_pAppData->m_eState = State_LoginPerson;
    
    return err_SuccessRegister;    
}

//-----------------------------------------------------------------------------

static int CmdCancel()
{
    char key = 0;
    int err = err_StateExp;

    if ( State_LoginPerson != g_pAppData->m_eState ) return err;

    g_pAppMgr->m_pMgrRes->m_pfShowString(str_VerifyCancel, false);

    for (key = 'Y'; key != '\r'; key = GetChar())
    {
        if ('Y' == key || 'y' == key)
        {
            err = err_SuccessCancel;
            printf("%c\b", key);
        }

        if ('N' == key || 'n' == key)
        {
            err = err_CancelCancel;
            printf("%c\b", key);
        }
    }

    printf("\n");

    if ( err_CancelCancel == err ) return err;

    if ( g_pAppMgr->m_pMgrPer->m_pfDelPer(&g_pAppData->m_pHeadPer, g_pAppData->m_pCurPer->m_strLoginID) )
    {
        g_pAppData->m_pCurPer = NULL;
        g_pAppData->m_eState = State_LoginOut;
        err = err_SuccessCancel;
    } 
    else
    {
        err = err_FailCancel;
    }

    return err;
}

//-----------------------------------------------------------------------------

static int CmdDelete()
{
    if ( State_LoginAdmin == g_pAppData->m_eState )
    {
        char loginID[Length_LoginID_Max+1] = { 0 };
        InputLoginID(loginID);

        if ( !CheckLoginID(loginID) )
        {
            return err_InvalidLoginID;
        }

        if ( g_pAppMgr->m_pMgrPer->m_pfDelPer(&g_pAppData->m_pHeadPer, loginID) )
        {
            return err_SuccessDelete;
        }
        else
        {
            return err_FailDelete;
        }
    }

    return err_StateExp;
}

//-----------------------------------------------------------------------------

static int ModifyPassword()
{
    if ( State_LoginAdmin == g_pAppData->m_eState || State_LoginPerson == g_pAppData->m_eState )
    {
        // ���建����
        char oldpwd[Length_Password_Max+1] = { 0 }; // ������
        char newpwd[Length_Password_Max+1] = { 0 }; // ������
        char twopwd[Length_Password_Max+1] = { 0 }; // ȷ������

        // ¼������
        InputPassword(oldpwd, str_InputOldPwd);
        InputPassword(newpwd, str_InputNewPwd);
        InputPassword(twopwd, str_AgainPassword);

        // ���ݵ�½״̬У���������ȷ��
        if ( State_LoginPerson == g_pAppData->m_eState && StringCompare(oldpwd, g_pAppData->m_pCurPer->m_strPassword, true)
            ||State_LoginAdmin == g_pAppData->m_eState && StringCompare(oldpwd, g_pAppData->m_pCurAdmin->m_strPassword, true) )
        {
            return err_ErrorOldPwd;
        }
        
        // �����볤���Ƿ�Ϸ�
        if ( strlen(newpwd) < Length_Password_Min )
        {
            return err_PasswordLess;
        }

        // �������ȷ�������Ƿ�һ��
        if ( StringCompare(newpwd, twopwd, true) )
        {
            return err_PasswordNoSame;
        }

        // ���ݵ�½״̬���������֮ǰ�����븲��
        if ( State_LoginAdmin == g_pAppData->m_eState )
        {
            bnb_strcpy(g_pAppData->m_pCurAdmin->m_strPassword, Length_Password_Max+1, newpwd); 
        } 
        else
        {
            bnb_strcpy(g_pAppData->m_pCurPer->m_strPassword, Length_Password_Max+1, newpwd);
        }

        return err_SuccessModifyPwd;
    }

    return err_StateExp;
}

//-----------------------------------------------------------------------------

static int ModifyPublic()
{
    if ( State_LoginPerson == g_pAppData->m_eState )
    {
        g_pAppData->m_pCurPer->m_bPublic = InputPublic();

        return err_SuccessModifyPub;
    }

    return err_StateExp;
}

//-----------------------------------------------------------------------------

static void ShowPerBySort( int eSort )
{
    person* newh = NULL;
    person* head = g_pAppData->m_pHeadPer;

    while ( head )
    {
        // �������Ȩ��Ϊ����߹���Ա��½����������򣬷���������
        if ( head->m_bPublic || State_LoginAdmin == g_pAppData->m_eState )
        {
            person* item = (person*)malloc(sizeof(person));

            if ( g_pAppMgr->m_pMgrPer->m_pfCopyPer(item, head) )
            {
                g_pAppMgr->m_pMgrPer->m_pfInsertPer(&newh, item, eSort);
            }
        }

        head = head->m_pNext;
    }

    // ��ʾ������
    ShowPersonList(newh);

    // �ͷ�������
    g_pAppMgr->m_pMgrPer->m_pfFreePer(newh);
}

//-----------------------------------------------------------------------------

static void ShowPerBySift( const siftdata* data )
{
    person* newh = NULL, *item = NULL;
    person* head = g_pAppData->m_pHeadPer;

    while ( head )
    {
        const person* find = g_pAppMgr->m_pMgrPer->m_pfFindPer(head, data);

        if ( !find ) break; // ����Ҳ�����ɸѡ����

        item = (person*)malloc(sizeof(person));

        if ( g_pAppMgr->m_pMgrPer->m_pfCopyPer(item, find) )
        {
            g_pAppMgr->m_pMgrPer->m_pfAddPer(&newh, item);
        }
        
        // ���ҵ��Ľڵ����һ���ڵ���Ϊ��һ�������Ŀ�ʼλ��
        head = find->m_pNext;
    }

    // ��ʾɸѡ���
    ShowPersonList(newh);

    // �ͷ�������
    g_pAppMgr->m_pMgrPer->m_pfFreePer(newh);
}

//-----------------------------------------------------------------------------

static void ShowScreen( int eID )
{
    int x = 0, y = 0;
    const char* str = g_pAppMgr->m_pMgrRes->m_pfGetString(eID);
    
    for (x=0; x<ScreenHeight; ++x)
    {
        if ( 0 == x || ScreenHeight-1 == x )
        {
            for (y=0; y<ScreenWidth; ++y) printf("%c", g_cSymbolFill);
        } 
        else
        {
            for (y=0; y<ScreenWidth; ++y)
            {
                if (0 == y || ScreenWidth-1 == y)
                {
                    printf("%c", g_cSymbolFill);
                } 
                else
                {
                    if (str && ScreenHeight/2 == x && 1 == y )
                    {
                        size_t len = strlen(str);
                        printf(" %s", str);
                        y += (int)(len+1);
                    }

                    printf("%c", g_cSymbolSpace);
                }
            }
        }

        printf("\n");
    }
}

//-----------------------------------------------------------------------------

static void ShowMenu( int eMenu )
{
    int i = 0;
    int begin = 0, end = 0;     // ����˵��ַ����Ŀ�ʼ�ͽ���λ��

    ShowSeparator(true);        // ��ӡ�ָ���

    switch ( eMenu )
    {
    case menu_Main:
        begin = str_Login;
        end = str_Register;
        break;

    case menu_Person:
        begin = str_Look;
        end = str_Cancel;
        break;

    case menu_Admin:
        begin = str_Look;
        end = str_ModifyPassword;
        break;

    case menu_Look:
        begin = str_Sort;
        end = str_Sift;
        break;

    case menu_Sort:
        begin = str_ByLoginID;
        end = str_ByScore;
        break;

    case menu_Sift:
        {
            begin = str_ByLoginID;

            if ( State_LoginPerson == g_pAppData->m_eState )
            {
                end = str_BySex;
            } 
            else
            {
                end = str_ByPublic;
            }

            break;
        }

    default:
        return;
    }

    // ����ʼ�ͽ���λ�ü�����ִ�ӡ����
    for (i = begin; i<=end; ++i)
    {
        const char* str = g_pAppMgr->m_pMgrRes->m_pfGetString(i);
        printf("%d��%s\n", i-begin+1, str);
    }
    
    if ( menu_Admin == eMenu )  // ����ǹ���Ա�˵������һ��
    {
        printf("%d��%s\n", cmdAd_Delete, g_pAppMgr->m_pMgrRes->m_pfGetString(str_Delete));
    }

    if ( menu_Main == eMenu )   // �����ػ����˳���ӡ��������
    {
        printf("%d��%s\n", 0, g_pAppMgr->m_pMgrRes->m_pfGetString(str_Exit));
    } 
    else
    {
        printf("%d��%s\n", 0, g_pAppMgr->m_pMgrRes->m_pfGetString(str_Back));
    }
        
    ShowSeparator(false);       // ��ӡ�ָ���
}

//-----------------------------------------------------------------------------

static void ShowSeparator( bool bHas )
{
    int i=0;

    if ( bHas ) printf("\n");

    for (i=0; i<ScreenWidth; ++i) printf("%c", g_cSeparator);

    printf("\n");
}

//-----------------------------------------------------------------------------

static bool ShowErrorCode( int eCode )
{
    if ( eCode > err_None && eCode < err_Max )
    {
        ShowSeparator(false);
        g_pAppMgr->m_pMgrRes->m_pfShowString(eCode + str_MemoryIsLess, true);
        return true;
    }

    return false;
}

//-----------------------------------------------------------------------------

static void ShowTableHead()
{
    ShowSeparator(true);

    // ��ӡ�б�ͷ����(��ʱ��ӡ����)
    printf("%c LoginID\t%c Name\t\t%c Date\t\t%c Sex\t%c Score\t%c Public%c\n"
        , g_cSymbolBlank
        , g_cSymbolBlank
        , g_cSymbolBlank
        , g_cSymbolBlank
        , g_cSymbolBlank
        , g_cSymbolBlank
        , g_cSymbolBlank);
}

//-----------------------------------------------------------------------------

static void InputLoginID( char* buf )
{
    ShowSeparator(true);

    g_pAppMgr->m_pMgrRes->m_pfShowString(str_InputLoginID, false);
    RuleInputString(buf, Length_LoginID_Max, 0);
}

//-----------------------------------------------------------------------------

static void InputPassword( char* buf, int eID )
{
    ShowSeparator(true);

    g_pAppMgr->m_pMgrRes->m_pfShowString(eID, false);
    RuleInputString(buf, Length_Password_Max, strTy_Password);
}

//-----------------------------------------------------------------------------

static void InputName( char* buf )
{
    ShowSeparator(true);

    g_pAppMgr->m_pMgrRes->m_pfShowString(str_InputName, false);
    RuleInputString(buf, Length_Name, strTy_HasAlpha);
}

//-----------------------------------------------------------------------------

static void InputDate( char* buf )
{
    ShowSeparator(true);

    g_pAppMgr->m_pMgrRes->m_pfShowString(str_InputBirth, false);
    RuleInputDate(buf);
}

//-----------------------------------------------------------------------------

static int InputSex()
{
    char key = 0;
    int sex = Sex_Male;

    ShowSeparator(true);

    g_pAppMgr->m_pMgrRes->m_pfShowString(str_InputSex, false);

    for (key = 'M'; key != '\r'; key = GetChar())
    {
        if ('M' == key || 'm' == key)
        {
            sex = Sex_Male;
            printf("%c\b", key);
        }

        if ('W' == key || 'w' == key)
        {
            sex = Sex_Female;
            printf("%c\b", key);
        }
    }

    printf("\n");

    return sex;
}

//-----------------------------------------------------------------------------

static int InputScore()
{
    ShowSeparator(true);

    printf(g_pAppMgr->m_pMgrRes->m_pfGetString(str_InputScore), Score_Max);

    return (int)RuleInputDouble(Score_Max, 0);
}

//-----------------------------------------------------------------------------

static bool InputPublic()
{
    char key = 0;
    bool bRet = true;

    ShowSeparator(true);

    g_pAppMgr->m_pMgrRes->m_pfShowString(str_InputPublic, false);

    for (key = 'Y'; key != '\r'; key = GetChar())
    {
        if ('Y' == key || 'y' == key)
        {
            bRet = true;
            printf("%c\b", key);
        }

        if ('N' == key || 'n' == key)
        {
            bRet = false;
            printf("%c\b", key);
        }
    }

    printf("\n");

    return bRet;
}

//-----------------------------------------------------------------------------

static bool CheckLoginID( const char* str )
{
    /*
     *	�û�������
     *  1�����ȱ�����3-16֮��
     *  2�����ַ�����Ϊ��ĸ����Ϊ�»���
     *  3��ֻ�����֡���ĸ���»���Ϊ�Ϸ��ַ�
     *  4�������ִ�Сд
     */
    size_t len = strlen(str);

    if ( len < Length_LoginID_Min || len > Length_LoginID_Max ) return false;
    
    if ( !isalpha(*str) && '_' != *str ) return false;

    for (++str; *str; ++str)
    {
        if ( !isalnum(*str) && '_' != *str ) return false;
    }

    return true;
}

//-----------------------------------------------------------------------------

static bool VerifyLogin( const char* strlogin, const char* pwd )
{
    person* pPer    = g_pAppData->m_pHeadPer;
    admin* pAdmin   = g_pAppData->m_pHeadAdmin;

    // ����Ա������ƥ����û���������
    for ( ; pPer; pPer = pPer->m_pNext)
    {
        if ( !StringCompare(strlogin, pPer->m_strLoginID, false) && !StringCompare(pwd, pPer->m_strPassword, true) )
        {
            g_pAppData->m_eState = State_LoginPerson;
            g_pAppData->m_pCurPer = pPer;

            return true;
        }
    }

    // �ڹ���Ա������ƥ����û���������
    for ( ; pAdmin; pAdmin = pAdmin->m_pNext)
    {
        if ( !StringCompare(strlogin, pAdmin->m_strLoginID, false) && !StringCompare(pwd, pAdmin->m_strPassword, true) )
        {
            g_pAppData->m_eState = State_LoginAdmin;
            g_pAppData->m_pCurAdmin = pAdmin;

            return true;
        }
    }

    return false;
}

//-----------------------------------------------------------------------------

static bool LoginIdIsExist( const char* strlogin )
{
    person* pPer    = g_pAppData->m_pHeadPer;
    admin* pAdmin   = g_pAppData->m_pHeadAdmin;

    // ����Ա�����в��Ҹ��û���
    for ( ; pPer; pPer = pPer->m_pNext)
    {
        if ( !StringCompare(strlogin, pPer->m_strLoginID, false) ) return true;
    }

    // �ڹ���Ա�����в��Ҹ��û���
    for ( ; pAdmin; pAdmin = pAdmin->m_pNext)
    {
        if ( !StringCompare(strlogin, pAdmin->m_strLoginID, false) ) return true;
    }

    return false;
}

//-----------------------------------------------------------------------------

static void ShowPersonList( const person* head )
{
    int count = 0;

    ShowTableHead();        // ��ӡ�б�ͷ

    // ��ӡ�ν���
    for (count = 0; count<ScreenWidth; ++count)
    {
        if (0 == count || ScreenWidth-1 == count)
        {
            printf("%c", g_cSymbolBlank);
        } 
        else
        {
            printf("%c", g_cSeparator);
        }
    }

    printf("\n");

    // ��ӡ��Ϣ�б�
    for ( count = 0; head; ++count)
    {
        g_pAppMgr->m_pMgrPer->m_pfShowPer(head);

        head = head->m_pNext;
    }

    ShowSeparator(false);   // ��ӡ�ָ���

    printf(g_pAppMgr->m_pMgrRes->m_pfGetString(str_RecordCount), count);
    printf("\n");
}

//-----------------------------------------------------------------------------

bool InitMerCmd()
{
    g_sMgrCmd.m_pfCommand[State_LoginOut]       = &MenuMain;
    g_sMgrCmd.m_pfCommand[State_LoginPerson]    = &MenuPerson;
    g_sMgrCmd.m_pfCommand[State_LoginAdmin]     = &MenuAdmin;

    g_sMgrCmd.m_pfShowMenu  = &ShowMenu;
    g_sMgrCmd.m_pfShowScreen= &ShowScreen;

    g_pAppMgr->m_pMgrCmd = &g_sMgrCmd;

    return true;
}



