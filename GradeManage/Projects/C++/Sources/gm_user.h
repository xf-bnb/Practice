#pragma once

#include <string>

class User
{
protected:

    std::string m_strAccount;
    std::string m_strPassword;

public:

    User(const std::string& strAccount, const std::string& strPassword)
        : m_strAccount(strAccount)
        , m_strPassword(strPassword)
    { }

    User(const User&) = default;

    void SetAccount(const std::string& strAccount) { m_strAccount = strAccount; }
    void SetPassword(const std::string& strPassword) { m_strPassword = strPassword; }

    const std::string& GetAccount() const { return m_strAccount; }
    const std::string& GetPassword() const { return m_strPassword; }
};

class Administrator : public User { using User::User; };

class Student : public User
{

public:

    Student(const std::string& strAccount, const std::string& strPassword, const std::string& strUsername, const std::string& strBirthday,
        unsigned int score, unsigned int sex, bool bPublic)
        : User(strAccount, strPassword)
        , m_strUsername(strUsername)
        , m_strBirthday(strBirthday)
        , m_uScore(score)
        , m_uSex(sex)
        , m_bPublic(bPublic)
    { }

    void SetUsername(const std::string& strUsername) { m_strUsername = strUsername; }
    void SetBirthday(const std::string& strBirthday) { m_strBirthday = strBirthday; }
    void SetScore(unsigned int score) { m_uScore = score; }
    void SetSex(unsigned int sex) { m_uSex = sex; }
    void SetPublic(bool value) { m_bPublic = value; }

    const std::string& GetUsername() const { return m_strUsername; }
    const std::string& GetBirthday() const { return m_strBirthday; }
    unsigned int GetScore() const { return m_uScore; }
    unsigned int GetSex() const { return m_uSex; }
    bool GetPublic() const { return m_bPublic; }

    void Show() const;

    // bool operator == (const User& other) const { return false; }

private:

    std::string m_strAccount;
    std::string m_strUsername;
    std::string m_strPassword;
    std::string m_strBirthday;
    unsigned int m_uScore;
    unsigned int m_uSex;
    bool m_bPublic;
    
};