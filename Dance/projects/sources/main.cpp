#include <iostream>
#include <fstream>
#include <streambuf>
#include <time.h>

#include "dance_action.h"

constexpr unsigned int dance_number = 1000000000;

inline void show(const char(&s)[length])
{
    for (char x : s) std::cout << x;

    std::cout << std::endl;
}

inline bool is_origin(const char(&s)[length], const char* origin)
{
    for (int i = 0; i < length; ++i)
        if (s[i] != origin[i])
            return false;

    return true;
}

void case1(const std::string& text, char (&s)[length])
{
    dance_action::action_list actions;
    unsigned int n = dance_action::parse_action(text, actions);
    std::cout << "parse action size: " << n << std::endl;

    for (auto& action : actions) action(s);
}

void case2(const std::string& text, char (&s)[length])
{
    dance_map dm(text);

    clock_t t1 = clock();

    for (int i = 0; i < dance_number; ++i) dm(s);

    clock_t t2 = clock();

    std::cout << "time: " << t2 - t1 << std::endl;
}

void case3(const std::string& text, char (&s)[length])
{
    dance_action::action_list actions;
    dance_action::parse_action(text, actions);

    clock_t t1 = clock();

    int n = 1;
    for (auto& action : actions)
        action(s);

    for (; !is_origin(s, "abcdefghijklmnop"); ++n)
        for (auto& action : actions)
            action(s);

    for (int i = 0; i < (dance_number % n); ++i)
        for (auto& action : actions)
            action(s);

    clock_t t2 = clock();

    std::cout << "circle number: " << n << std::endl;
    std::cout << "time: " << t2 - t1 << std::endl;
}

int main()
{
    char s[length]{ 0 };
    for (int i = 0; i < length; ++i) s[i] = 'a' + i;
    show(s);

    std::ifstream fin("../resources/actions.txt", std::ios::in);
    std::istreambuf_iterator<char> beg(fin), end;
    std::string action_text(beg, end);
    fin.close();

    void (*func[])(const std::string&, char(&)[length]) = { case1, case2, case3 };
          
    func[2](action_text, s);

    show(s);

    return 0;
}
