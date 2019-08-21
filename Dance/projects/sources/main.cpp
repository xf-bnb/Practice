#include <string>
#include <iostream>
#include <vector>
#include <functional>
#include <fstream>
#include <streambuf>
#include <time.h>

constexpr int length = 16;

void show(const char(&s)[length])
{
    for (char x : s) std::cout << x;

    std::cout << std::endl;
}

void spin(char(&s)[length], int n)
{
    char t[length]{ 0 };
    int z = length - n;
    for (int i = 0; i < z; ++i)
        t[i] = s[i];

    for (int i = 0; i < n; ++i)
        s[i] = s[z + i];

    for (int i = 0; i < z; ++i)
        s[n + i] = t[i];
}

void exchange(char(&s)[length], int a, int b)
{
    s[a] ^= s[b] ^= s[a] ^= s[b];
}

void partner(char(&s)[length], char a, char b)
{
    for (char& x : s)
    {
        if (x == a)
            x = b;
        else if (x == b)
            x = a;
    }
}

class action_type
{
public:
    enum type {
        at_unknow, at_spin, at_exchange, at_partner
    };

    action_type(const std::string& a)
    {
        switch (a[0])
        {
        case 's':
        {
            int n = std::stoi(std::string(a, 1));
            f = [=](char(&s)[length]) { spin(s, n); };
            t = at_spin;
            break;
        }
        case 'x':
        {
            size_t pos = a.find('/', 1);
            int m = std::stoi(a.substr(1, pos));
            int n = std::stoi(a.substr(pos + 1));
            f = [=](char(&s)[length]) { exchange(s, m, n); };
            t = at_exchange;
            break;
        }
        case 'p':
        {
            f = [=](char(&s)[length]) { partner(s, a[1], a[3]); };
            t = at_partner;
            break;
        }
        default:
            f = nullptr;
            t = at_unknow;
            break;
        }
    }

    void operator()(char(&s)[length]) const
    {
        if (f != nullptr)
            f(s);
    }

    unsigned int getType() const { return t; }

private:
    type t;
    std::function<void(char(&)[length])> f;

};

std::vector<action_type> parse_step(const std::string& s)
{
    std::vector<action_type> actions;
    std::vector<action_type> partners;

    for (size_t start = 0;;)
    {
        size_t pos = s.find(',', start);
        if (pos == std::string::npos)
        {
            action_type at(s.substr(start));
            if (action_type::type::at_partner == at.getType())
                actions.push_back(at);
            else
                partners.push_back(at);
            break;
        }

        action_type at(s.substr(start, pos - start));
        if (action_type::type::at_partner == at.getType())
            actions.push_back(at);
        else
            partners.push_back(at);

        start = pos + 1;
    }

    std::cout << partners.size() << std::endl;

    for (auto& at : partners)
        actions.push_back(at);

    return actions;
}

int main()
{
    std::ifstream fin("../resources/step.txt", std::ios::in);
    std::istreambuf_iterator<char> beg(fin), end;
    std::string step_text(beg, end);
    fin.close();

    std::vector<action_type> actions = parse_step(step_text);

    std::cout << actions.size() << std::endl;

    char s[length]{ 0 };
    for (int i = 0; i < length; ++i)
        s[i] = 'a' + i;

    time_t t1 = clock();
    for (int i = 0; i < 1; ++i)
    {
        unsigned int n = actions.size() - 7766;

        const std::string p("nbdfcpoejgamhilk");
        const std::string x("lkinbmhjpgofaced");
           
        // abcdefghijklmnop
        // nbdfcpoejgamhilk
        for (int i = 0; i < length; ++i) s[i] = x[i];

        // lkinbmhjpgofaced
        // cknmidebghlajpfo
        for (unsigned int i = n; i < actions.size(); ++i) actions[i](s);

        // nbdfcpoejgamhilk
        // abcdefghijklmnop

        // lkinbmhjpgofaced
        // majibhegkolpndcf
        // for (unsigned int i = 0; i < n; ++i) actions[i](s);
    }

    time_t t2 = clock();

    std::cout << "time: " << t2 - t1 << std::endl;

    show(s);

    // step: cknmidebghlajpfo
    // a: hibnladopckjgefm
    // b: hibnladopckjgefm

    return 0;
}


