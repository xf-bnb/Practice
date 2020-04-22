
#include <vector>
#include <string>
#include <array>
#include <fstream>

#include "gm_resource.h"

static const char* ResFiles[static_cast<std::size_t>(Resource::LanguageType::LangMax)]{
    "resources/lang_chinese.txt",
    "resources/lang_english.txt"
};

bool Resource::Init()
{
    return (LoadString(ResFiles[static_cast<std::size_t>(_language)]));
}

bool Resource::ChangeLanguage(LanguageType lang)
{
    if (_language != lang)
    {
        if (LoadString(ResFiles[static_cast<std::size_t>(lang)]))
        {
            _language = lang;
            return true;
        }
    }

    return false;
}

bool Resource::LoadString(const char* file)
{
    std::ifstream fin(file);
    if (fin.is_open())
    {
        std::vector<string_type> strs;
        for (std::size_t id = 0; id < ResID::ResMax; ++id)
        {
            string_type str;
            if (!std::getline(fin, str))
                break;

            strs.emplace_back(str);
        }

        fin.close();

        if (ResID::ResMax == strs.size())
        {
            std::copy(strs.begin(), strs.end(), _res_text.begin());
            return true;
        }
    }

    return false;
}
