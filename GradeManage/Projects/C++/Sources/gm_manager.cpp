
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>

#include "gm_manager.h"

static const char* _StudentFile("ss_student.db");
static const char* _AdminFile("ss_admin.db");

void Manager::Init()
{
    _LoadStudent();
    _LoadAdmin();
}

bool Manager::_LoadStudent()
{
    std::ifstream fin(_StudentFile);
    if (fin.is_open())
    {
        std::string text;
        while (std::getline(fin, text))
            m_vtrStudents.emplace_back(StringToStudent(text));

        fin.close();

        return true;
    }

    return false;
}

bool Manager::_SaveStudent() const
{
    std::ofstream fout(_StudentFile, std::ios::out | std::ios::trunc);
    if (fout.is_open())
    {
        for (auto student : m_vtrStudents)
            fout << StudentToString(student) << std::endl;

        fout.close();

        return true;
    }

    return false;
}

bool Manager::_LoadAdmin()
{
    std::ifstream fin(_AdminFile);
    if (fin.is_open())
    {
        std::string text;
        while (std::getline(fin, text))
            m_vtrAdministrators.emplace_back(StringToAdmin(text));

        fin.close();

        return true;
    }
    else
    {
        m_vtrAdministrators.emplace_back(admin_t("admin", "admin"));
        return _SaveAdmin();
    }
}

bool Manager::_SaveAdmin() const
{
    std::ofstream fout(_AdminFile, std::ios::out | std::ios::trunc);
    if (fout.is_open())
    {
        for (auto admin : m_vtrAdministrators)
            fout << AdminToString(admin) << std::endl;

        fout.close();

        return true;
    }

    return false;
}

bool Manager::Login(const std::string& strAccount, const std::string& strPassword)
{
    for (auto admin : m_vtrAdministrators)
    {
        if (MatchAccount(admin, strAccount))
        {
            if (MatchPassword(admin, strPassword))
            {
                account = strAccount;
                m_eRole = Role::role_admin;

                return true;
            }

            return false;
        }
    }

    for (auto student : m_vtrStudents)
    {
        if (MatchAccount(student, strAccount))
        {
            if (MatchPassword(student, strPassword))
            {
                account = strAccount;
                m_eRole = Role::role_student;

                return true;
            }

            return false;
        }
    }

    return false;
}

bool Manager::IsExisting(const std::string& strAccount) const
{
    for (auto student : m_vtrStudents)
        if (MatchAccount(student, strAccount))
            return true;

    return false;
}

bool Manager::AddStudent(const student_t& student)
{
    m_vtrStudents.push_back(student);

    return _SaveStudent();
}

bool Manager::RemoveStudent(const std::string& strAccount)
{
    for (auto iter = m_vtrStudents.begin(); iter != m_vtrStudents.end(); ++iter)
    {
        if (MatchAccount(*iter, strAccount))
        {
            m_vtrStudents.erase(iter);
            return true;
        }
    }

    return false;
}

bool Manager::UpdateStudent(const student_t& student, attr_e attr)
{
    for (auto& [account, password, name, birthday, score, sex, right] : m_vtrStudents)
    {
        if (MatchAccount(student, account))
        {
            switch (attr)
            {
            case attr_e::attr_password:
                password = std::get<attr_e::attr_password>(student);
                return true;
            case attr_e::attr_name:
                name = std::get<attr_e::attr_name>(student);
                return true;
            case attr_e::attr_birthday:
                birthday = std::get<attr_e::attr_birthday>(student);
                return true;
            case attr_e::attr_score:
                score = std::get<attr_e::attr_score>(student);
                return true;
            case attr_e::attr_sex:
                sex = std::get<attr_e::attr_sex>(student);
                return true;
            case attr_e::attr_right:
                right = std::get<attr_e::attr_right>(student);
                return true;
            default:
                return false;
            }
        }
    }

    return false;
}
