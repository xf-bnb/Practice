//=============================================================================
// 文件：gm_Utility.h
// 描述：定义程序需要的通用全局函数和数据参数。
//=============================================================================
#ifndef _GM_UTILITY_H
#define _GM_UTILITY_H

//-----------------------------------------------------------------------------

#define Size_FileName       64      // 文件名大小
#define Size_String         128     // 字符串大小
#define Size_Path           256     // 路径名大小
#define Size_Buffer         256     // 缓冲区大小
#define Size_Word           32      // 单词长度

//-----------------------------------------------------------------------------

extern const char g_cReturn;        // 回车符
extern const char g_cSeparator;     // 分隔符
extern const char g_cSymbolFill;    // 填充符
extern const char g_cSymbolBlank;   // 间隔符
extern const char g_cSymbolSpace;   // 空白符

//-----------------------------------------------------------------------------

typedef enum _strType
{
    strTy_NotRule   = 0x0,          // 无规则
    strTy_HasDigit  = 0x1,          // 仅数字
    strTy_HasAlpha  = 0x2,          // 仅字母
    strTy_HasAlnum  = 0x3,          // 仅数字或字母

    strTy_Password  = 0x4,          // 是否密码
    
    strTy_Max
}strType;   // 字符串类型


//=============================================================================
// 声明全局通用函数
//=============================================================================

/*
 *  函数：GetChar
 *  功能：从标准输入获取一个字符，且在输入字符的时候不带回显。
 *  参数：无
 *  返回：获取到的输入字符。
 */
char GetChar();

/*
 *	函数：StringCompareNoCase
 *  功能：比较两个字符串
 *  参数：str1 指定第一个字符串，str2 指定第二个字符串，bCase 指定是否区分大小写。
 *  返回：两个字符串不同字符的差值，完全相同返回0。
 */
int StringCompare(const char* str1, const char* str2, bool bCase);

/*
 *	函数：RuleInputString
 *  功能：限定录入字符串
 *  参数：str 指定缓冲区地址，size 指定缓冲区大小，eRule 指定录入规则(参见 strType 枚举)。
 *  返回：实际录入字符个数
 */
size_t RuleInputString(char* str, size_t size, int eRule);

/*
 *	函数：RuleInputDouble
 *  功能：限定录入数字
 *  参数：dMax 指定数字的范围，precition 指定小数位数。
 *  返回：实际录入的数字
 */
double RuleInputDouble(double dMax, unsigned int presition);

/*
 *	函数：RuleInputDate
 *  功能：限定录入日期
 *  参数：录入的日期字符串将存储在 str 中，如果该参数为空，则忽略。
 *  返回：返回录入的日期数字。
 *  备注：录入的格式为yyyy-mm-dd，存储的格式为yyyymmdd。
 */
unsigned int RuleInputDate(char* str);


#endif