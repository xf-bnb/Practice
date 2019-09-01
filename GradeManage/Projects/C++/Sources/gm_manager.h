#pragma once

#include <vector>

#include "gm_user.h"

class Manager final
{
private:

    std::vector<Student> m_vtrStudentList;
    std::vector<Administrator> m_vtrAdminList;

    Manager() = default;
    Manager(const Manager&) = delete;
    Manager& operator = (const Manager&) = delete;


public:

    static Manager& GetInstance() { static Manager mgr; return mgr; }

    unsigned int LoadStudent();
    unsigned int SaveStudent();

    bool AddStudent(const Student& student);
    bool RemoveStudent(const std::string& strAccount);
    bool UpdateStudent(const Student& student, unsigned int attr);

    template<typename _FuncType>
    std::vector<Student> FilterStudent(_FuncType pf) const
    {
        std::vector<Student> students;
        for (const auto& student : m_vtrStudentList)
            if (pf(student))
                students.push_back(student);

        return students;
    }



};