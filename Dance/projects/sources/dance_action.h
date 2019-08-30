#pragma once

#include <string>
#include <vector>
#include <functional>

constexpr int length = 16;

class dance_action
{
public:

    using func_type = std::function<void(char(&)[length])>;
    using action_list = std::vector<dance_action>;

    enum action_type {
        at_unknow, at_spin, at_exchange, at_partner
    };

    explicit dance_action(const std::string& a);
    explicit dance_action(unsigned int offset) { assign(offset); }
    dance_action(unsigned int a, unsigned int b) { assign(a, b); }
    dance_action(char x, char y) { assign(x, y); }

    void operator()(char(&s)[length]) const
    {
        if (act_func != nullptr)
            act_func(s);
    }

    unsigned int getType() const { return act_type; }

    static unsigned int parse_action(const std::string& s, action_list& actions);

private:

    void assign(unsigned int offset);
    void assign(unsigned int a, unsigned int b);
    void assign(char x, char y);

    action_type act_type{at_unknow};
    func_type   act_func{nullptr};

};

class dance_map
{
public:

    dance_map()
    {
        for (unsigned int i = 0; i < length; index_map[i++] = i);
        for (unsigned int i = 0; i < length; alphabet_map[i++] = 'a' + i) ;
    }

    explicit dance_map(const std::string& s) : dance_map()
    {
        dance_action::action_list actions;
        dance_action::parse_action(s, actions);

        for (auto& action : actions)
        {
            switch (action.getType())
            {
            case dance_action::action_type::at_spin:
            case dance_action::action_type::at_exchange:
                action(index_map);
                break;
            case dance_action::action_type::at_partner:
                action(alphabet_map);
                break;
            default:
                break;
            }
        }
    }

    void operator()(char(&s)[length]) const
    {
        char x[length]{ 0 };
        for (unsigned int i = 0; i < length; ++i)
            x[i] = s[index_map[i]];

        for (unsigned int i = 0; i < length; ++i)
            s[i] = alphabet_map[x[i] - 'a'];
    }

private:

    char index_map[length];
    char alphabet_map[length];

};
