//=============================================================================
// �ļ���gm_Utility.h
// ���������������Ҫ��ͨ��ȫ�ֺ��������ݲ�����
//=============================================================================
#ifndef _GM_UTILITY_H
#define _GM_UTILITY_H

//-----------------------------------------------------------------------------

#define Size_FileName       64      // �ļ�����С
#define Size_String         128     // �ַ�����С
#define Size_Path           256     // ·������С
#define Size_Buffer         256     // ��������С
#define Size_Word           32      // ���ʳ���

//-----------------------------------------------------------------------------

extern const char g_cSeparator;     // �ָ���
extern const char g_cSymbolFill;    // ����
extern const char g_cSymbolBlank;   // �����
extern const char g_cSymbolSpace;   // �հ׷�

//-----------------------------------------------------------------------------
// Ĭ�ϱ�׼C��֧�� bool ���ͣ�����������ܹ�֧�ִ����ͣ���ע������һ�С�
typedef enum { false, true } bool;  // ���� bool ����

//-----------------------------------------------------------------------------

typedef enum _strType
{
    strTy_NotRule   = 0x0,          // �޹���
    strTy_HasDigit  = 0x1,          // ������
    strTy_HasAlpha  = 0x2,          // ����ĸ
    strTy_HasAlnum  = 0x3,          // �����ֻ���ĸ

    strTy_Password  = 0x4,          // �Ƿ�����
    
    strTy_Max
}strType;   // �ַ�������


//=============================================================================
// ����ȫ��ͨ�ú���
//=============================================================================

/*
 *	������StringCompareNoCase
 *  ���ܣ��Ƚ������ַ���
 *  ������str1 ָ����һ���ַ�����str2 ָ���ڶ����ַ�����bCase ָ���Ƿ����ִ�Сд��
 *  ���أ������ַ�����ͬ�ַ��Ĳ�ֵ����ȫ��ͬ����0��
 */
int StringCompare(const char* str1, const char* str2, bool bCase);

/*
 *	������RuleInputString
 *  ���ܣ��޶�¼���ַ���
 *  ������str ָ����������ַ��size ָ����������С��eRule ָ��¼�����(�μ� strType ö��)��
 *  ���أ�ʵ��¼���ַ�����
 */
size_t RuleInputString(char* str, size_t size, int eRule);

/*
 *	������RuleInputDouble
 *  ���ܣ��޶�¼������
 *  ������dMax ָ�����ֵķ�Χ��precition ָ��С��λ����
 *  ���أ�ʵ��¼�������
 */
double RuleInputDouble(double dMax, unsigned int presition);

/*
 *	������RuleInputDate
 *  ���ܣ��޶�¼������
 *  ������¼��������ַ������洢�� str �У�����ò���Ϊ�գ�����ԡ�
 *  ���أ�����¼����������֡�
 *  ��ע��¼��ĸ�ʽΪyyyy-mm-dd���洢�ĸ�ʽΪyyyymmdd��
 */
unsigned int RuleInputDate(char* str);


#endif