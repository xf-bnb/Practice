
#include "gm_Include.h"

//-----------------------------------------------------------------------------

#ifdef _MSC_VER
const char g_cReturn        = '\r';
#else
const char g_cReturn        = '\n';
#endif
const char g_cSeparator     = '-';
const char g_cSymbolFill    = '*';
const char g_cSymbolBlank   = '|';
const char g_cSymbolSpace   = ' ';

static bool IsValidDate(int y, int m, int d);

//-----------------------------------------------------------------------------

char GetChar()
{
#ifdef _MSC_VER

    return _getch();

#else

    struct termios old_attr, new_attr;
    tcgetattr(STDIN_FILENO, &old_attr);
    new_attr = old_attr;
    new_attr.c_lflag &= ~(ECHO | ICANON);

    tcsetattr(STDIN_FILENO, TCSANOW, &new_attr);
    char x = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &old_attr);

    return x;

#endif // _MSC_VER
}

//-----------------------------------------------------------------------------

int StringCompare( const char* str1, const char* str2, bool bCase )
{
    int ret   = 0;

    if ( bCase )
    {
        while ( !(ret = *(unsigned char*)str1 - *(unsigned char*)str2) && *str2 )
            ++str1, ++str2;
    } 
    else
    {
        while ( !(ret = tolower(*str1) - tolower(*str2)) && *str2 )
            ++str1, ++str2;
    }

    return  ret;
}

//-----------------------------------------------------------------------------

size_t RuleInputString( char* str, size_t size, int eRule )
{
    size_t i = 0;
    char c = 0;

    for (c = GetChar(); '\r' != c; c = GetChar())
    {
        if ( '\b' == c )
        {
            if (i > 0)
            {
                str[--i] = 0;
                printf("\b%c\b", g_cSymbolSpace);
            }
        }
        else
        {
            if ( i < size )
            {
                int alnum = eRule & strTy_HasAlnum;

                if (strTy_NotRule == alnum
                    || (strTy_HasDigit == alnum && isdigit(c))
                    || (strTy_HasAlpha == alnum && isalpha(c))
                    || (strTy_HasAlnum == alnum && isalnum(c)))
                {
                    str[i++] = c;

                    if ( strTy_Password == (eRule & strTy_Password) )
                    {
                        printf("%c", g_cSymbolFill);
                    }
                    else
                    {
                        printf("%c", c);
                    }                    
                }                
            }
        }        
    }

    printf("\n");

    return i;
}

//-----------------------------------------------------------------------------

double RuleInputDouble( double dMax, unsigned int presition )
{
    int index = 0, pos = -1;
    char buf[Size_Buffer] = { 0 };

    char c = 0;

    for (c = GetChar(); '\r' != c ; c = GetChar())
    {
        if ( '\b' == c && index > 0 )
        {
            if ( '.' == buf[--index] ) pos = -1;

            buf[index] = 0;
            printf("\b \b");
        }
        else if ( '.' == c && presition && pos < 0 && index < Size_Buffer )
        {
            pos = index;
            buf[index++] = c;
            printf("%c", c);
        } 
        else if ( isdigit(c) && index < Size_Buffer )
        {
            buf[index] = c;

            if ( atof(buf) <= dMax && ((pos < 0) ? true : ((unsigned int)(index - pos) <= presition)) )
            {
                printf("%c", c);
                ++index;
            }
            else
            {
                buf[index] = 0;
            }
        }
        else
        {

        }
    }

    printf("\n");

    return atof(buf);
}

//-----------------------------------------------------------------------------

unsigned int RuleInputDate( char* str )
{
    const int count = 10;
    int index = 0;
    char buf[Size_Buffer] = { 0 };

    char c = 0;

    for (c = GetChar(); ('\r' != c || count > index); c = GetChar())
    {
        if ( '\b' == c && index > 0 )
        {
            buf[--index] = 0;
            printf("\b \b");
        }

        if ( index < count )
        {
            if ('-' == c && (4 == index || 7 == index))
            {
                buf[index++] = c;
                printf("%c", c);
            }

            if ( isdigit(c) )
            {
                int y = 0, m = 0, d = 0;

                buf[index] = c;
                y = atoi(buf);
                m = atoi(buf+5);
                d = atoi(buf+8);

                if ( (index < 4)
                    || ((5 == index) && ('0' == c || '1' == c))
                    || ((6 == index) && (m > 0 && m <= 12))
                    || ((8 == index) && ('0' == c || '1' == c || '2' == c || ('3' == c && 2 != m)))
                    || ((9 == index) && IsValidDate(y, m, d)))
                {
                    printf("%c", c);
                    ++index;

                    if ( 4 == index || 7 == index )
                    {
                        c = '-';
                        buf[index++] = c;
                        printf("%c", c);
                    }
                } 
                else
                {
                    buf[index] = 0;
                }
            }
        }
    }

    for (index = 4; index < count; ++index)
    {
        if ( index < 6)
        {
            buf[index] = buf[index + 1];
        } 
        else
        {
            buf[index] = buf[index + 2];
        }
    }

    if ( str ) bnb_strcpy(str, count, buf); // for (index = 0; str[index] = buf[index]; ++index);

    printf("\n");

    return ((unsigned int)atoi(buf));
}

bool IsValidDate(int y, int m, int d)
{
    return (0 < d) &&
        (                                
            ((2 == m) && (((0 == y%4 && 0 != y%100) || 0 == y%400) ? (d <= 29) : ( d <= 28)))
            || ((4 == m || 6 == m || 9 == m || 11 == m ) && (d <= 30))
            || ((1 == m || 3 == m || 5 == m || 7 == m || 8 == m || 10 == m || 12 == m) && (d <= 31))
        );
}
