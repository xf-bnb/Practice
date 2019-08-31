//=============================================================================
// �ļ���gm_Include.h
// ����������������������ͷ�ļ���������̷������壬��������ȫ�ֱ�����
// ˵����
//      ����Դ�ļ�ֻ�������ͷ�ļ����ɡ�
//=============================================================================
#ifndef _GM_INCLUDE_H
#define _GM_INCLUDE_H

//=============================================================================
// ������������ͷ�ļ�
//=============================================================================
// ��׼��ͷ�ļ�  
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// �жϱ������汾
#if _MSC_VER

#define bnb_strcpy(buf, size, dsc)               strcpy_s(buf, size, dsc)
#define bnb_fopen(file, name, mode)              fopen_s(&(file), name, mode)
#define bnb_fread(buf, size, _byte, count, file) fread_s(buf, size, _byte, count, file)
    
#else

#define bnb_strcpy(buf, size, dsc)               strcpy(buf, dsc)
#define bnb_fopen(file, name, mode)              file = fopen(name, mode)
#define bnb_fread(buf, size, _byte, count, file) fread(buf, _byte, count, file)

#endif


// �Զ���ͷ�ļ�
#include "gm_Utility.h"
#include "gm_Resource.h"
#include "gm_Person.h"
#include "gm_Command.h"
#include "gm_Manager.h"


//-----------------------------------------------------------------------------

typedef struct _appdata
{
    admin*      m_pHeadAdmin;                   // ����Ա��Ϣ����ͷ�ڵ�
    person*     m_pHeadPer;                     // ��Ա��Ϣ����ͷ�ڵ�

    admin*      m_pCurAdmin;                    // ��ǰ��½�Ĺ���Ա
    person*     m_pCurPer;                      // ��ǰ��½���û�

    int         m_eState;                       // ��ǰ��½״̬

}AppData;   // �������ݽṹ


//=============================================================================
// ����ִ�й���
//=============================================================================
bool    Init();             // ��ʼ��

bool    Loop();             // ����ѭ��

bool    Exit();             // �˳�


//=============================================================================
// ����ȫ�ֱ���
//=============================================================================
extern  AppMgr*     g_pAppMgr;  // ȫ��Ӧ�ó��������
extern  AppData*    g_pAppData; // ȫ��Ӧ�ó�������


#endif
