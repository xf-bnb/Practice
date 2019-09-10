#pragma once

class User
{
public:

    using string_type = std::string;

    User(const string_type& strAccount, const string_type& strPassword)
        : m_strAccount(strAccount)
        , m_strPassword(strPassword)
    { }

    User(const User&) = default;

    void SetAccount(const string_type& strAccount) { m_strAccount = strAccount; }
    void SetPassword(const string_type& strPassword) { m_strPassword = strPassword; }

    const string_type& GetAccount() const { return m_strAccount; }
    const string_type& GetPassword() const { return m_strPassword; }

protected:

    string_type m_strAccount;
    string_type m_strPassword;

};

class Administrator : public User { using User::User; };

class Student : public User
{
    enum _Right { right_public = 0x0001 };

public:

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
    bool IsPublic() const { return CheckRight(static_cast<unsigned int>(_Right::right_public)); }
    bool CheckRight(unsigned int right) const { return (right == (right & m_uRight)); }

private:

    string_type m_strName;
    string_type m_strBirthday;
    unsigned int m_uScore;
    unsigned int m_uSex;
    unsigned int m_uRight;

};