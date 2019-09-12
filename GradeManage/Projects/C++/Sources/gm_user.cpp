
#include <sstream>

#include "gm_user.h"

template<unsigned int n>
struct _map_type
{
    static const unsigned char& _xor_char;

    char _char_map[n]{ 0 };
    unsigned char _index_map[0x80]{ 0 };

    constexpr _map_type(const char(&_map)[n])
    {
        for (unsigned int i = 0; i < n; ++i) { _char_map[i] = _map[i]; _index_map[_map[i]] = i; }
    }

    char operator[](unsigned int i) const { return _char_map[i]; }
    unsigned char operator()(unsigned char x) const { return _index_map[x]; }
};

template<unsigned int n> const unsigned char& _map_type<n>::_xor_char('Z');

static constexpr _map_type<0x10> _char_map({ 'C', '7', 'S', 'R', 'O', 'Y', 'T', '2', 'I', '8', 'K', '-', '+', 'A', 'H', 'X' });

inline std::string _string_encode(const std::string& str)
{
    std::string text;
    for (auto x : str)
    {
        x ^= _map_type<0x10>::_xor_char;
        text.append(1, _char_map[x & 0x0f]);
        text.append(1, _char_map[(x & 0xf0) >> 4]);
    }

    return text;
}

inline std::string _string_decode(const std::string& str)
{
    std::string text;
    for (unsigned int i = 1; i < str.size(); i += 2)
        text.append(1, (_char_map(str[i - 1]) | (_char_map(str[i]) << 4)) ^ _map_type<0x10>::_xor_char);

    return text;
}

User::string_type Administrator::Serialize() const
{
    return (_string_encode(m_strAccount + ',' + (_string_encode(m_strPassword))));
}

bool Administrator::Deserialize(const string_type& text)
{
    string_type str = _string_decode(text);

    size_t pos = str.find(',', 0);
    if (string_type::npos == pos)
        return false;

    SetAccount(str.substr(0, pos));
    SetPassword(_string_decode(str.substr(pos+1)));

    return true;
}

User::string_type Student::Serialize() const
{
    std::ostringstream oss;
    oss << m_strAccount
        << ',' << (_string_encode(m_strPassword))
        << ',' << m_strName
        << ',' << m_strBirthday
        << ',' << m_uScore
        << ',' << m_uSex
        << ',' << m_uRight;

    return (_string_encode(oss.str()));
}

bool Student::Deserialize(const string_type& text)
{
    string_type str = _string_decode(text);
    
    string_type strs[_Attribute::attr_max];

    size_t start = 0;
    for (unsigned int i = 0; i < (_Attribute::attr_max - 1); ++i)
    {
        size_t pos = str.find(',', start);
        if (string_type::npos == pos)
            return false;

        strs[i] = str.substr(start, pos - start);
        start = pos + 1;
    }

    strs[6] = str.substr(start);

    SetAccount(strs[_Attribute::attr_account]);
    SetPassword(_string_decode(strs[_Attribute::attr_password]));
    SetName(strs[_Attribute::attr_name]);
    SetBirthday(strs[_Attribute::attr_birthday]);
    SetScore(std::stoi(strs[_Attribute::attr_score]));
    SetSex(std::stoi(strs[_Attribute::attr_sex]));
    SetRight(std::stoi(strs[_Attribute::attr_right]));

    return true;
}

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

int User::CompareAccount(const string_type& strAccount) const
{
    return (_str_icmp(m_strAccount.c_str(), strAccount.c_str()));
}
