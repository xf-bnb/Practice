#pragma once

constexpr unsigned int MinLengthAccount(3);     // 帐户名最小长度
constexpr unsigned int MaxLengthAccount(16);    // 账户名最大长度
constexpr unsigned int MinLengthPassword(6);    // 密码最小长度
constexpr unsigned int MaxLengthPassword(16);   // 密码最大长度
constexpr unsigned int MinLengthName(2);        // 姓名最大长度
constexpr unsigned int MaxLengthName(16);       // 姓名最大长度
constexpr unsigned int LengthDate(8);           // 日期长度
constexpr unsigned int MaxScore(150);           // 分数最大值

enum attr_e {
    attr_account,
    attr_password,
    attr_name,
    attr_birthday,
    attr_score,
    attr_sex,
    attr_right,
    attr_max
};

enum right_e { right_public = 0x0001 };

using string_t = std::string;
using admin_t = std::tuple<string_t, string_t>;
using student_t = std::tuple<string_t, string_t, string_t, string_t, unsigned int, unsigned int, unsigned int>;

inline auto SetRight(unsigned int right, unsigned int value, bool ok) { return (ok ? (right | value) : (right & ~value)); }
inline bool CheckRight(unsigned int right, right_e e) { return (e == (e & right)); }
inline bool IsPublic(unsigned int right) { return CheckRight(right, right_e::right_public); }

string_t AdminToString(const admin_t& admin);
admin_t StringToAdmin(const string_t& text);

string_t StudentToString(const student_t& student);
student_t StringToStudent(const string_t& text);

int CompareString(const string_t& a, const string_t& b);

template<typename UserType>
inline bool MatchAccount(const UserType& user, const string_t& account){
    return (0 == CompareString(std::get<attr_account>(user), account));
}

template<typename UserType>
inline bool MatchPassword(const UserType& user, const string_t& password) {
    return (std::get<attr_password>(user) == password);
}

template<attr_e attr>
bool LessThan(const student_t& a, const student_t& b) {
    return (std::get<attr>(a) < std::get<attr>(b));
}

template<> inline bool LessThan<attr_e::attr_account>(const student_t& a, const student_t& b) {
    return (CompareString(std::get<attr_e::attr_account>(a), std::get<attr_e::attr_account>(b)) < 0);
}
