
#include <sstream>

#include "gm_user.h"

struct _map_type
{
    char _char_map[0x10]{ 0 };
    unsigned char _index_map[0x80]{ 0 };

    constexpr _map_type(const char(&_map)[0x10])
    {
        for (unsigned int i = 0; i < 0x10; ++i) { _char_map[i] = _map[i]; _index_map[_map[i]] = i; }
    }

    char operator[](unsigned int i) const { return _char_map[i]; }
    unsigned char operator()(unsigned char x) const { return _index_map[x]; }
};

static constexpr _map_type _char_map({ 'C', '7', 'S', 'R', 'O', 'Y', 'T', '2', '/', '8', 'K', 'V', '.', 'A', 'H', 'X' });

inline std::string _string_encode(const std::string& str)
{
    std::string text;
    for (auto x : str)
    {
        x ^= 0xff;
        text.append(1, _char_map[x & 0x0f]);
        text.append(1, _char_map[(x & 0xf0) >> 4]);
    }

    return text;
}

inline std::string _string_decode(const std::string& str)
{
    std::string text;
    for (unsigned int i = 1; i < str.size(); i += 2)
        text.append(1, (_char_map(str[i - 1]) | (_char_map(str[i]) << 4)) ^ 0xff);

    return text;
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
    return (_str_icmp(account.c_str(), strAccount.c_str()));
}

User::string_type Administrator::ToString(const Administrator& admin)
{
    return (_string_encode(admin.GetAccount() + ',' + (_string_encode(admin.GetAccount()))));
}

Administrator Administrator::FromString(const User::string_type& text)
{
    string_type str = _string_decode(text);

    size_t pos = str.find(',', 0);
    if (string_type::npos == pos)
        throw std::runtime_error("parse admin data error !");

    return { str.substr(0, pos), _string_decode(str.substr(pos + 1)) };
}

User::string_type Student::ToString(const Student& student)
{
    std::ostringstream oss;
    oss << student.GetAccount()
        << ',' << (_string_encode(student.GetPassword()))
        << ',' << student.GetName()
        << ',' << student.GetBirthday()
        << ',' << student.GetScore()
        << ',' << student.GetSex()
        << ',' << student.GetRight();

    return (_string_encode(oss.str()));
}

Student Student::FromString(const User::string_type& text)
{
    string_type str = _string_decode(text);
    
    string_type strs[_Attribute::attr_max];

    size_t start = 0;
    for (unsigned int i = 0; i < (_Attribute::attr_max - 1); ++i)
    {
        size_t pos = str.find(',', start);
        if (string_type::npos == pos)
            throw std::runtime_error("parse student data error !");

        strs[i] = str.substr(start, pos - start);
        start = pos + 1;
    }

    strs[_Attribute::attr_max - 1] = str.substr(start);

    return { strs[_Attribute::attr_account],
             _string_decode(strs[_Attribute::attr_password]),
             strs[_Attribute::attr_name],
             strs[_Attribute::attr_birthday],
             std::stoul(strs[_Attribute::attr_score]),
             std::stoul(strs[_Attribute::attr_sex]),
             std::stoul(strs[_Attribute::attr_right]) };
}
