
#include <string>
#include <vector>
#include <algorithm>

#include "gm_manager.h"

bool Manager::Init()
{
    return Load();
}

bool Manager::Load()
{
    return 0;
}

bool Manager::Save()
{
    return 0;
}

bool Manager::Login(const std::string& strAccount, const std::string& password)
{
    m_eRole = Role::role_student;
    return true;
}

bool Manager::AddStudent(const Student& student)
{

    return false;
}

bool Manager::RemoveStudent(const std::string& strAccount)
{
    return false;
}

bool Manager::UpdateStudent(const Student& student, unsigned int attr)
{
    return false;
}
