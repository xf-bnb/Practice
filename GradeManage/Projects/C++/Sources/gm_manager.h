#pragma once

#include "gm_user.h"

class Manager final
{
public:

    enum Role { role_unknow, role_student, role_admin };

    Role m_eRole{ Role::role_unknow };
    std::string account;

private:

    std::vector<student_t> m_vtrStudents;
    std::vector<admin_t> m_vtrAdministrators;

    Manager() = default;
    Manager(const Manager&) = delete;
    Manager& operator = (const Manager&) = delete;

    bool _LoadStudent();
    bool _SaveStudent() const;

    bool _LoadAdmin();
    bool _SaveAdmin() const;

public:

    static Manager& GetInstance() { static Manager mgr; return mgr; }

    void Init();

    bool Login(const std::string& strAccount, const std::string& strPassword);

    Role GetRole() const { return m_eRole; }
    const std::string& GetLoginAccount() const { return account; }

    bool IsExisting(const std::string& strAccount) const;
    bool AddStudent(const student_t& student);
    bool RemoveStudent(const std::string& strAccount);
    bool UpdateStudent(const student_t& student, attr_e attr);
    const std::vector<student_t>& GetStudents() const { return m_vtrStudents; }

    template<typename _FuncType>
    std::vector<student_t> GetStudents(_FuncType pf) const
    {
        if (Role::role_admin == m_eRole)
            return GetStudents();

        std::vector<student_t> students;
        for (const auto& student : m_vtrStudents)
            if (pf(student))
                students.push_back(student);

        return students;
    }

};
