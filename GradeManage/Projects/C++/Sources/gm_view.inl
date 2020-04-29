

#ifdef _MSC_VER
#include <conio.h>
#else
#include <termios.h>
#include <unistd.h>
#include "gm_ui.h"

class _tc_echo_lock
{
    struct termios _tc_attr;

public:
    _tc_echo_lock() {
        tcgetattr(STDIN_FILENO, &_tc_attr);

        struct termios new_attr = _tc_attr;
        new_attr.c_lflag &= ~(ECHO | ICANON);
        tcsetattr(STDIN_FILENO, TCSANOW, &new_attr);
    }

    ~_tc_echo_lock() {
        tcsetattr(STDIN_FILENO, TCSANOW, &_tc_attr);
    }
};


#endif // _MSC_VER

#ifdef _MSC_VER
const char g_cReturn = '\r';
#else
const char g_cReturn = '\n';
#endif

template <bool _Return = true> void OutputText(const char*);

template <> inline void OutputText<false>(const char* text)
{
    std::cout << text;
}

template <> inline void OutputText<true>(const char* text)
{
    OutputText<false>(text);
    std::cout << std::endl;
}

template <bool _Return = true> inline void OutputText(const std::string& text)
{
    OutputText<_Return>(text.data());
}

template <bool _Return = true> inline void OutputText(unsigned int resId)
{
    OutputText<_Return>(Resource::GetInstance().GetText(static_cast<ResID>(resId)));
}

//-----------------------------------------------------------------------------

inline char GetChar()
{
#ifdef _MSC_VER
    return _getch();
#else
    _tc_echo_lock _lock;
    return getchar();
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

inline void ShowWelcome() {
    std::cout << "********************************************************************************" << std::endl;
    std::cout << "*                                                                              *" << std::endl;
    std::cout << "*                                                                              *" << std::endl;
    std::cout << "*                     Welcome to Student Manage System !                       *" << std::endl;
    std::cout << "*                                                                              *" << std::endl;
    std::cout << "*                                                                              *" << std::endl;
    std::cout << "********************************************************************************" << std::endl;
    std::cout << std::endl << std::endl << std::endl;
}

inline void ShowEnd() {
    std::cout << "********************************************************************************" << std::endl;
    std::cout << "*                                    Bye bye !                                 *" << std::endl;
    std::cout << "********************************************************************************" << std::endl;
    std::cout << std::endl;
}


// template<bool _Case> int _CompareString(const std::string& a, const std::string& b);

// template<> inline int _CompareString<true>(const std::string& a, const std::string& b) { return a.compare(b); }
// template<> inline int _CompareString<false>(const std::string& a, const std::string& b) { return (_str_icmp(a.c_str(), b.c_str())); }

