#include "dance_action.h"

inline void spin(char(&s)[length], unsigned int n)
{
    char t[length]{ 0 };
    unsigned int z = length - n;
    for (unsigned int i = 0; i < z; ++i)
        t[i] = s[i];

    for (unsigned int i = 0; i < n; ++i)
        s[i] = s[z + i];

    for (unsigned int i = 0; i < z; ++i)
        s[n + i] = t[i];
}

inline void exchange(char(&s)[length], int a, int b)
{
    s[a] ^= s[b] ^= s[a] ^= s[b];
}

inline void partner(char(&s)[length], char a, char b)
{
    for (char& x : s)
    {
        if (x == a)
            x = b;
        else if (x == b)
            x = a;
    }
}

dance_action::dance_action(const std::string& s)
{
    switch (s[0])
    {
    case 's':
        assign(std::stoi(std::string(s, 1)));
        break;
    case 'x':
    {
        unsigned int pos = s.find('/', 2);
        assign((unsigned int)std::stoi(s.substr(1, (pos - 1))), (unsigned int)std::stoi(s.substr(pos + 1)));
        break;
    }
    case 'p':
        assign(s[1], s[3]);
        break;
    default:
        break;
    }
}

void dance_action::assign(unsigned int offset)
{
    act_type = at_spin;
    act_func = [=](char(&s)[length]) { spin(s, offset); };
}

void dance_action::assign(unsigned int a, unsigned int b)
{
    act_type = at_exchange;
    act_func = [=](char(&s)[length]) { exchange(s, a, b); };
}

void dance_action::assign(char x, char y)
{
    act_type = at_partner;
    act_func = [=](char(&s)[length]) { partner(s, x, y); };
}

unsigned int dance_action::parse_action(const std::string& s, dance_action::action_list& actions)
{
    unsigned int n = 0;
    for (unsigned int start = 0; start < s.size(); ++n)
    {
        unsigned int pos = s.find(',', start);
        if (pos == std::string::npos)
        {
            actions.emplace_back(dance_action(s.substr(start)));
            ++n;
            break;
        }

        actions.emplace_back(dance_action(s.substr(start, pos - start)));
        start = pos + 1;
    }

    return n;
}
