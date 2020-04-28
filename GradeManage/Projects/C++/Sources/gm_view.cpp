
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
std::string RuleInputString(size_t size, bool mask, FuncType func)
{
    size_t i = 0;
    char c = 0;

    for (c = GetChar(); '\r' != c; c = GetChar())
    {
        if ('\b' == c)
        {
            if (0 < i)
            {
                str[--i] = 0;
                printf("\b%c\b", g_cSymbolSpace);
            }
        }
        else
        {
            if (i < size && func(c))
            {
                str[i++] = c;
                std::cout << (mask ? g_cSymbolFill : c);
            }
        }
    }

    printf("\n");

    return i;
}

void View::LoginPage() const
{
    std::string strAccount, strPassword;

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

    ViewStudents(Manager::GetInstance().GetStudents([&strInput](const Student& student) {
        return (IsVisiable(student) && (0 == student.CompareAccount(strInput))); }));
}

void View::FilterByName() const
{
    std::string strInput;

    ViewStudents(Manager::GetInstance().GetStudents([&strInput](const Student& student) {
        return (IsVisiable(student) && student.GetName() == strInput); }));
}

void View::FilterByScore() const
{
    unsigned int uInput;

    ViewStudents(Manager::GetInstance().GetStudents([&uInput](const Student& student) {
        return (IsVisiable(student) && uInput == student.GetScore()); }));
}

void View::FilterByBirthday() const
{
    std::string strInput;

    ViewStudents(Manager::GetInstance().GetStudents([&strInput](const Student& student) {
        return (IsVisiable(student) && student.GetBirthday() == strInput); }));
}

void View::FilterBySex() const
{
    unsigned int uInput;

    ViewStudents(Manager::GetInstance().GetStudents([&uInput](const Student& student) {
        return (IsVisiable(student) && uInput == student.GetSex()); }));
}

void View::FilterByRight() const
{
    bool bInput;

    ViewStudents(Manager::GetInstance().GetStudents([&bInput](const Student& student) { return bInput == IsVisiable(student); }));
}

void View::SortByAccount() const
{
    _SortView([](const auto& a, const auto& b) { return (a.CompareAccount(b.GetAccount()) < 0); });
}

void View::SortByName() const
{
    _SortView([](const auto& a, const auto& b) { return (a.GetName() < b.GetName()); });
}

void View::SortByScore() const
{
    _SortView([](const auto& a, const auto& b) { return (a.GetScore() < b.GetScore()); });
}

void View::SortByBirthday() const
{
    _SortView([](const auto& a, const auto& b) { return (a.GetBirthday() < b.GetBirthday()); });
}

void View::ViewStudents(const std::vector<Student>& students) const
{
    OutputText<true>("There is student view !");
}

unsigned int View::_AcceptCommand(unsigned int nMax) const
{
    std::cout << std::endl;
    _ui._ShowLabel(str_InputCmd);

    char key = '0';

    for (std::cout << "0\b"; ;)
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

        std::cout << key << '\b';
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
