
#include <vector>
#include <string>
#include <fstream>

#include "gm_resource.h"

static const std::string ResFiles[_ResType::ResTypeMax]{
    "../Resources/lang_chinese.txt",
    "../Resources/lang_english.txt"
};

bool Resource::Init()
{
    return Load() == (_ResID::ResMax * _ResType::ResTypeMax);
}

unsigned int Resource::Load()
{
    unsigned int n = 0;

    for (unsigned int i = 0; i < ResTypeMax; ++i)
    {
        std::ifstream fin(ResFiles[i], std::ios::in);
        if (fin.is_open())
        {
            for (unsigned int id = 0; id < ResMax; ++id)
            {
                string_type str;
                if (!std::getline(fin, str))
                    break;

                m_Res[i].push_back(str);
                ++n;
            }

            fin.close();
        }
    }

    return n;
}
