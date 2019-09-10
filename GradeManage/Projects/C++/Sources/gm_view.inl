

#ifdef _MSC_VER
#include <conio.h>
#else
#include <termios.h>
#include <unistd.h>
#include "gm_ui.h"
#endif // _MSC_VER

#ifdef _MSC_VER
const char g_cReturn = '\r';
#else
const char g_cReturn = '\n';
#endif

template <bool _Return = true> void OutputText(const std::string&);

template <> inline void OutputText<false>(const std::string& text)
{
    std::cout << text;
}

template <> inline void OutputText<true>(const std::string& text)
{
    OutputText<false>(text);
    std::cout << std::endl;
}

template <bool _Return = true> inline void OutputText(const char* text)
{
    OutputText<_Return>(std::string(text));
}

template <bool _Return = true> inline void OutputText(unsigned int langType, unsigned int resId)
{
    OutputText<_Return>(Resource::GetInstance().GetRes(langType, resId));
}

//-----------------------------------------------------------------------------

inline char GetChar()
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

inline void ClearScreen()
{
#ifdef _MSC_VER
    system("cls");
#else
    system("clear");
#endif
}

inline std::string InputAccount()
{
    return { };
}

inline void ShowWelcome() { OutputText("Hello, Welcome !"); }
inline void ShowEnd() { OutputText("Bye bye"); }


inline int _str_icmp(const char* _str1, const char* _str2)
{
    int _v1(0), _v2(0);

    do {
        if (0x40 < ((_v1 = (unsigned char)(*(_str1++)))) && (_v1 < 0x5b))
            _v1 |= 0x20;

        if (0x40 < ((_v2 = (unsigned char)(*(_str2++)))) && (_v2 < 0x5b))
            _v2 |= 0x20;
    } while (_v1 && (_v1 == _v2));

    return (_v1 - _v2);
}

template<bool _Case> int _CompareString(const std::string& a, const std::string& b);

template<> inline int _CompareString<true>(const std::string& a, const std::string& b) { return a.compare(b); }
template<> inline int _CompareString<false>(const std::string& a, const std::string& b) { return (_str_icmp(a.c_str(), b.c_str())); }

inline bool IsVisiable(const Student& student) { return (_Mgr_.GetRole() == Manager::Role::role_admin || student.IsPublic()); }
