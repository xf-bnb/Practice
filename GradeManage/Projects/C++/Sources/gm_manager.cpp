
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
            m_vtrStudents.emplace_back(Student(text));

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
            fout << student.Serialize() << std::endl;

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
            m_vtrAdministrators.emplace_back(Administrator(text));

        fin.close();

        return true;
    }

    return false;
}

bool Manager::_SaveAdmin() const
{
    std::ofstream fout(_AdminFile, std::ios::out | std::ios::trunc);
    if (fout.is_open())
    {
        for (auto admin : m_vtrAdministrators)
            fout << admin.Serialize() << std::endl;

        fout.close();

        return true;
    }

    return false;
}

bool Manager::Login(const std::string& strAccount, const std::string& strPassword)
{
    for (auto admin : m_vtrAdministrators)
    {
        if (0 == admin.CompareAccount(strAccount))
        {
            if (admin.GetPassword() == strPassword)
            {
                m_strAccount = strAccount;
                m_eRole = Role::role_admin;

                return true;
            }

            return false;
        }
    }

    for (auto student : m_vtrStudents)
    {
        if (0 == student.CompareAccount(strAccount))
        {
            if (student.GetPassword() == strPassword)
            {
                m_strAccount = strAccount;
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
        if (0 == student.CompareAccount(strAccount))
            return true;

    return false;
}

bool Manager::AddStudent(const Student& student)
{
    m_vtrStudents.push_back(student);

    return _SaveStudent();
}

bool Manager::RemoveStudent(const std::string& strAccount)
{
    for (auto iter = m_vtrStudents.begin(); iter != m_vtrStudents.end(); ++iter)
    {
        if (0 == iter->CompareAccount(strAccount))
        {
            m_vtrStudents.erase(iter);
            return true;
        }
    }

    return false;
}

bool Manager::UpdateStudent(const Student& student, Student::_Attribute attr)
{
    for (Student& item : m_vtrStudents)
    {
        if (0 == item.CompareAccount(student.GetAccount()))
        {
            switch (attr)
            {
            case Student::attr_account:
                item.SetAccount(student.GetAccount());
                return true;
            case Student::attr_password:
                item.SetPassword(student.GetPassword());
                return true;
            case Student::attr_name:
                item.SetName(student.GetName());
                return true;
            case Student::attr_birthday:
                item.SetBirthday(student.GetBirthday());
                return true;
            case Student::attr_score:
                item.SetScore(student.GetScore());
                return true;
            case Student::attr_sex:
                item.SetSex(student.GetSex());
                return true;
            case Student::attr_right:
                item.SetRight(student.GetRight());
                return true;
            default:
                return false;
            }
        }
    }

    return false;
}
