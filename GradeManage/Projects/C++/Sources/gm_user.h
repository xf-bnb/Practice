#pragma once


class User
{
public:

    using string_type = std::string;

    User() = default;

    User(const string_type& strAccount, const string_type& strPassword)
        : account(strAccount), password(strPassword)
    { }

    void SetAccount(const string_type& strAccount) { account = strAccount; }
    void SetPassword(const string_type& strPassword) { password = strPassword; }

    const string_type& GetAccount() const { return account; }
    const string_type& GetPassword() const { return password; }

    int CompareAccount(const string_type& strAccount) const;

protected:

    string_type account, password;

};

class Administrator : public User
{
public:

    using User::User;

    static string_type ToString(const Administrator& admin);
    static Administrator FromString(const string_type& text);

};

class Student : public User
{
public:

    enum _Right { right_public = 0x0001 };

    enum _Attribute {
        attr_account, attr_password, attr_name, attr_birthday, attr_score, attr_sex, attr_right, attr_max
    };

    Student() = default;

    Student(const string_type& strAccount, const string_type& strPassword, const string_type& strName, const string_type& strBirthday,
        unsigned int score, unsigned int sex, unsigned int right)
        : User(strAccount, strPassword)
        , m_strName(strName)
        , m_strBirthday(strBirthday)
        , m_uScore(score)
        , m_uSex(sex)
        , m_uRight(right)
    { }

    void SetName(const string_type& strName) { m_strName = strName; }
    void SetBirthday(const string_type& strBirthday) { m_strBirthday = strBirthday; }
    void SetScore(unsigned int score) { m_uScore = score; }
    void SetSex(unsigned int sex) { m_uSex = sex; }
    void SetRight(unsigned int value) { m_uRight = value; }
    void SetRight(unsigned int right, bool ok) { if (ok) m_uRight |= right; else m_uRight &= ~right; }

    const string_type& GetName() const { return m_strName; }
    const string_type& GetBirthday() const { return m_strBirthday; }
    unsigned int GetScore() const { return m_uScore; }
    unsigned int GetSex() const { return m_uSex; }
    unsigned int GetRight() const { return m_uRight; }
    bool IsPublic() const { return CheckRight(static_cast<unsigned int>(_Right::right_public)); }
    bool CheckRight(unsigned int right) const { return (right == (right & m_uRight)); }

    static string_type ToString(const Student& student);
    static Student FromString(const string_type& text);

private:

    string_type m_strName;
    string_type m_strBirthday;
    unsigned int m_uScore;
    unsigned int m_uSex;
    unsigned int m_uRight;

};