
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
    MenuLoop(UI::_Menu::menu_main);

}

void View::LoginPage() const
{
    std::string strAccount, strPassword;

    if (_Mgr_.Login(strAccount, strPassword))
    {
        OutputText<true>("登陆成功");

        switch (_Mgr_.GetRole())
        {
        case Manager::Role::role_student:
            MenuLoop(UI::_Menu::menu_student);
            break;
        case Manager::Role::role_admin:
            MenuLoop(UI::_Menu::menu_admin);
            break;
        default:
            break;
        }
    }
    else
    {

    }
}

void View::RegisterPage() const
{
    _ui._ShowItem(str_SuccessRegister);

    MenuLoop(UI::_Menu::menu_student);
}

void View::SwitchLanguage() const
{
    switch (static_cast<UI::_MenuLang>(_AcceptCommand(_ui._ShowMenu(UI::_Menu::menu_language))))
    {
    case UI::_MenuLang::item_Chinese:
        _ui._eLanguage = UI::_Language::Chinese;
        break;
    case UI::_MenuLang::item_English:
        _ui._eLanguage = UI::_Language::English;
        break;
    default:
        break;
    }
}

void View::SortPage() const
{
    MenuLoop(UI::_Menu::menu_sort);
}

void View::FilterPage() const
{
    MenuLoop(UI::_Menu::menu_filter);
}

void View::CancelPage() const
{
    OutputText<true>("注销成功");
}

void View::ModifyPassword() const
{
    OutputText<true>("修改密码成功");
}

void View::ModifyRight() const
{
    OutputText<true>("修改权限成功");
}

void View::ModifyScore() const
{
    OutputText<true>("修改分数成功");
}

void View::DeletePage() const
{
    OutputText<true>("删除用户成功");
}

void View::ViewPage() const
{
    MenuLoop(UI::_Menu::menu_view);
}

void View::FilterByAccount() const
{
    std::string strInput;

    ViewStudents(_Mgr_.GetStudents([&strInput](const Student& student) {
        return (IsVisiable(student) && (_CompareString<false>(strInput, student.GetAccount()) == 0)); }));
}

void View::FilterByName() const
{
    std::string strInput;

    ViewStudents(_Mgr_.GetStudents([&strInput](const Student& student) {
        return (IsVisiable(student) && (_CompareString<true>(strInput, student.GetName()) == 0)); }));
}

void View::FilterByScore() const
{
    unsigned int uInput;

    ViewStudents(_Mgr_.GetStudents([&uInput](const Student& student) {
        return (IsVisiable(student) && uInput == student.GetScore()); }));
}

void View::FilterByBirthday() const
{
    std::string strInput;

    ViewStudents(_Mgr_.GetStudents([&strInput](const Student& student) {
        return (IsVisiable(student) && (_CompareString<true>(strInput, student.GetBirthday()) == 0)); }));
}

void View::FilterBySex() const
{
    unsigned int uInput;

    ViewStudents(_Mgr_.GetStudents([&uInput](const Student& student) {
        return (IsVisiable(student) && uInput == student.GetSex()); }));
}

void View::FilterByRight() const
{
    bool bInput;

    ViewStudents(_Mgr_.GetStudents([&bInput](const Student& student) { return bInput == IsVisiable(student); }));
}

void View::SortByAccount() const
{
    _SortView([](const auto& a, const auto& b) { return (_CompareString<false>(a.GetAccount(), b.GetAccount()) < 0); });
}

void View::SortByName() const
{
    _SortView([](const auto& a, const auto& b) { return (_CompareString<true>(a.GetName(), b.GetName()) < 0); });
}

void View::SortByScore() const
{
    _SortView([](const auto& a, const auto& b) { return a.GetScore() < b.GetScore(); });
}

void View::SortByBirthday() const
{
    _SortView([](const auto& a, const auto& b) { return (_CompareString<true>(a.GetBirthday(), b.GetBirthday()) < 0); });
}

void View::ViewStudents(const std::vector<Student>& students) const
{
    OutputText<true>("There is student view !");
}

unsigned int View::_AcceptCommand(unsigned int nMax) const
{
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
    OutputText<true>(static_cast<unsigned int>(_eLanguage), id);
}

void View::UI::_ShowLabel(unsigned int id) const
{
    OutputText<false>(static_cast<unsigned int>(_eLanguage), id);
}
