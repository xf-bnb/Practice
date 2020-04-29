
#include <array>
#include <vector>
#include <string>
#include <algorithm>
#include <iostream>

#include "gm_resource.h"
#include "gm_manager.h"
#include "gm_view.h"
#include "gm_view.inl"


bool View::Show() const
{
    ShowWelcome();

    MainPage();

    ShowEnd();

    return true;
}

void View::MainPage() const
{
    MenuLoop(UI::Menu::menu_main);

}

template<typename FuncType>
string_t RuleInputString(size_t n, bool mask, FuncType func)
{
    size_t i = 0;
    char str[0x40]{ 0 };

    for (char c = GetChar(); g_cReturn != c; c = GetChar())
    {
        if ('\b' == c)
        {
            if (0 < i)
            {
                str[--i] = 0;
                std::cout << "\b \b";
            }
        }
        else
        {
            if (i < n && func(c))
            {
                str[i++] = c;
                std::cout << (mask ? '*' : c);
            }
        }
    }

    std::cout << std::endl;

    return { str, i };
}

inline string_t InputAccount()
{

    return RuleInputString(MaxLengthAccount, false, std::isprint);
}

inline string_t InputPassword()
{
    return string_t();
}

void View::LoginPage() const
{
    auto strAccount = InputAccount();
    auto strPassword = InputPassword();

    if (Manager::GetInstance().Login(strAccount, strPassword))
    {
        OutputText<true>(str_SuccessLogin);

        switch (Manager::GetInstance().GetRole())
        {
        case Manager::Role::role_student:
            MenuLoop(UI::Menu::menu_student);
            break;
        case Manager::Role::role_admin:
            MenuLoop(UI::Menu::menu_admin);
            break;
        default:
            break;
        }
    }
    else
    {
        OutputText<true>(str_ErrorLogin);
    }
}

void View::RegisterPage() const
{
    _ui._ShowItem(str_SuccessRegister);

    MenuLoop(UI::Menu::menu_student);
}

void View::SwitchLanguage() const
{
    switch (static_cast<UI::MenuLang>(_AcceptCommand(_ui._ShowMenu(UI::Menu::menu_language))))
    {
    case UI::MenuLang::item_chinese:
        Resource::GetInstance().ChangeLanguage(Resource::LanguageType::Chinese);
        break;
    case UI::MenuLang::item_english:
        Resource::GetInstance().ChangeLanguage(Resource::LanguageType::English);
        break;
    default:
        break;
    }
}

void View::SortPage() const
{
    MenuLoop(UI::Menu::menu_sort);
}

void View::FilterPage() const
{
    MenuLoop(UI::Menu::menu_filter);
}

void View::CancelPage() const
{
    OutputText<true>(str_SuccessLogout);
}

void View::ModifyPassword() const
{
    OutputText<true>(str_SuccessModifyPwd);
}

void View::ModifyRight() const
{
    OutputText<true>(str_SuccessModifyPub);
}

void View::ModifyScore() const
{
    OutputText<true>("修改分数成功");
}

void View::DeletePage() const
{
    OutputText<true>(str_SuccessDelete);
}

void View::ViewPage() const
{
    MenuLoop(UI::Menu::menu_view);
}

void View::FilterByAccount() const
{
    std::string strInput;

    ViewStudents(Manager::GetInstance().GetStudents([&strInput](const student_t& student) {
        return (IsVisiable(student) && (MatchAccount(student, strInput))); }));
}

void View::FilterByName() const
{
    std::string strInput;

    ViewStudents(Manager::GetInstance().GetStudents([&strInput](const student_t& student) {
        return (IsVisiable(student) && std::get<attr_e::attr_name>(student) == strInput); }));
}

void View::FilterByScore() const
{
    unsigned int uInput;

    ViewStudents(Manager::GetInstance().GetStudents([&uInput](const student_t& student) {
        return (IsVisiable(student) && std::get<attr_e::attr_score>(student) == uInput); }));
}

void View::FilterByBirthday() const
{
    std::string strInput;

    ViewStudents(Manager::GetInstance().GetStudents([&strInput](const student_t& student) {
        return (IsVisiable(student) && std::get<attr_e::attr_birthday>(student) == strInput); }));
}

void View::FilterBySex() const
{
    unsigned int uInput;

    ViewStudents(Manager::GetInstance().GetStudents([&uInput](const student_t& student) {
        return (IsVisiable(student) && std::get<attr_e::attr_sex>(student) == uInput); }));
}

void View::FilterByRight() const
{
    bool bInput;

    ViewStudents(Manager::GetInstance().GetStudents([&bInput](const student_t& student) { return bInput == IsVisiable(student); }));
}

void View::ViewStudents(const std::vector<student_t>& students) const
{
    OutputText<true>("There is student view !");
}

unsigned int View::_AcceptCommand(unsigned int nMax) const
{
    std::cout << std::endl;
    _ui._ShowLabel(str_InputCmd);

    char key = '0';

    for (std::cout << key << '\b'; ; std::cout << key << '\b')
    {
        char x = GetChar();
        if (g_cReturn == x)
        {
            if (' ' == key) continue; else break;
        }
        else
        {
            if ('\b' == x || '\x7f' == x)
                key = ' ';
            else if (isdigit(x) && unsigned int(x - '0') < nMax)
                key = x;
            else
                continue;
        }
    }

    ClearScreen();

    return (key - '0');
}

//-----------------------------------------------------------------------------

void View::UI::_ShowMenuItem(unsigned int index, unsigned int id) const
{
    std::cout << index << ": ";
    _ShowItem(id);
}

void View::UI::_ShowItem(unsigned int id) const
{
    OutputText<true>(id);
}

void View::UI::_ShowLabel(unsigned int id) const
{
    OutputText<false>(id);
}
