#pragma once

#include "gm_user.h"

class Manager final
{
public:

    enum Role { role_unknow, role_student, role_admin };

    Role m_eRole{ Role::role_unknow };
    std::string m_strAccount;

private:

    std::vector<Student> m_vtrStudents;
    std::vector<Administrator> m_vtrAdministrators;

    Manager() = default;
    Manager(const Manager&) = delete;
    Manager& operator = (const Manager&) = delete;

public:

    static Manager& GetInstance() { static Manager mgr; return mgr; }

    bool Init();

    bool Load();
    bool Save();
    bool Login(const std::string& strAccount, const std::string& password);

    Role GetRole() const { return m_eRole; }
    const std::string& GetLoginAccount() const { return m_strAccount; }

    bool AddStudent(const Student& student);
    bool RemoveStudent(const std::string& strAccount);
    bool UpdateStudent(const Student& student, unsigned int attr);
    const std::vector<Student>& GetStudents() const { return m_vtrStudents; }

    template<typename _FuncType>
    std::vector<Student> GetStudents(_FuncType pf) const
    {
        std::vector<Student> students;
        for (const auto& student : m_vtrStudents)
            if (pf(student))
                students.push_back(student);

        return students;
    }

};

#define _Mgr_ Manager::GetInstance()
