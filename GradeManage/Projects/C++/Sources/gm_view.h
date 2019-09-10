#pragma once

class View
{
#include "gm_ui.h"

    mutable UI _ui;

public:

    bool Show() const;

private:

    using handler_type = void (View::*)()const;

    unsigned int _AcceptCommand(unsigned int nMax) const;

    template<typename _LessThan>
    void _SortView(_LessThan pfunc) const
    {
        auto item = static_cast<UI::_MenuOrder>(_AcceptCommand(_ui._ShowMenu(UI::_Menu::menu_order)));
        if (UI::_MenuOrder::item_back < item && item < UI::_MenuOrder::item_order_max)
        {
            auto students = _Mgr_.GetStudents([](const Student& student) { return IsVisiable(student); });
            if (item == UI::_MenuOrder::item_Ascending)
                std::sort(students.begin(), students.end(), pfunc);
            else
                std::sort(students.begin(), students.end(), [&pfunc](const auto& a, const auto& b) { return pfunc(b, a); });

            ViewStudents(students);
        }
    }

    template<unsigned int n>
    void _MenuLoop(UI::_Menu menuId, const handler_type (&functions)[n]) const
    {
        for (;;)
        {
            unsigned int item_number = _ui._ShowMenu(menuId);
            unsigned int k = _AcceptCommand(item_number);
            if (0 == k) return;

            if (k < item_number)
                (this->*functions[k - 1])();
            else
                ;
        }
    }

    void MenuLoop(UI::_Menu menuId) const
    {
        switch (menuId)
        {
        case UI::_Menu::menu_main:
            _MenuLoop(menuId, { &View::LoginPage, &View::RegisterPage, &View::SwitchLanguage });
            break;
        case UI::_Menu::menu_student:
            _MenuLoop(menuId, { &View::ViewPage, &View::ModifyPassword, &View::ModifyRight, &View::CancelPage });
            break;
        case UI::_Menu::menu_admin:
            _MenuLoop(menuId, { &View::ViewPage, &View::ModifyPassword, &View::DeletePage });
            break;
        case UI::_Menu::menu_view:
            _MenuLoop(menuId, { &View::SortPage, &View::FilterPage });
            break;
        case UI::_Menu::menu_sort:
            _MenuLoop(menuId, { &View::SortByAccount, &View::SortByName, &View::SortByScore, &View::SortByBirthday });
            break;
        case UI::_Menu::menu_filter:
            _MenuLoop(menuId, { &View::FilterByAccount, &View::FilterByName, &View::FilterByScore, &View::FilterByBirthday, &View::FilterBySex, &View::FilterByRight });
            break;
        default:
            break;
        }
    }

    void MainPage() const;
    void SwitchLanguage() const;
    void LoginPage() const;
    void RegisterPage() const;
    void SortPage() const;
    void ViewPage() const;
    void ModifyPassword() const;
    void ModifyRight() const;
    void ModifyScore() const;
    void DeletePage() const;
    void FilterPage() const;
    void CancelPage() const;

    void FilterByAccount() const;
    void FilterByName() const;
    void FilterByScore() const;
    void FilterByBirthday() const;
    void FilterByRight() const;
    void FilterBySex() const;
    void SortByAccount() const;
    void SortByName() const;
    void SortByScore() const;
    void SortByBirthday() const;

    void ViewStudents(const std::vector<Student>& students) const;

};
