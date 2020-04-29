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
        auto item = static_cast<UI::MenuOrder>(_AcceptCommand(_ui._ShowMenu(UI::Menu::menu_order)));
        if (UI::MenuOrder::item_back < item && item < UI::MenuOrder::item_order_max)
        {
            auto students = Manager::GetInstance().GetStudents([](const auto& student) { return IsVisiable(student); });
            if (item == UI::MenuOrder::item_ascending)
                std::sort(students.begin(), students.end(), pfunc);
            else
                std::sort(students.begin(), students.end(), [&pfunc](const auto& a, const auto& b) { return pfunc(b, a); });

            ViewStudents(students);
        }
    }

    template<unsigned int n>
    void _MenuLoop(UI::Menu menuId, const handler_type(&functions)[n]) const
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

    void MenuLoop(UI::Menu menuId) const
    {
        switch (menuId)
        {
        case UI::Menu::menu_main:
            _MenuLoop(menuId, { &View::LoginPage, &View::RegisterPage, &View::SwitchLanguage });
            break;
        case UI::Menu::menu_student:
            _MenuLoop(menuId, { &View::ViewPage, &View::ModifyPassword, &View::ModifyRight, &View::CancelPage });
            break;
        case UI::Menu::menu_admin:
            _MenuLoop(menuId, { &View::ViewPage, &View::ModifyPassword, &View::DeletePage });
            break;
        case UI::Menu::menu_view:
            _MenuLoop(menuId, { &View::SortPage, &View::FilterPage });
            break;
        case UI::Menu::menu_sort:
            _MenuLoop(menuId, { &View::SortByAccount, &View::SortByName, &View::SortByScore, &View::SortByBirthday });
            break;
        case UI::Menu::menu_filter:
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

    void SortByAccount() const  { _SortView(LessThan<attr_e::attr_account>); }
    void SortByName() const     { _SortView(LessThan<attr_e::attr_name>); }
    void SortByScore() const    { _SortView(LessThan<attr_e::attr_score>); }
    void SortByBirthday() const { _SortView(LessThan<attr_e::attr_birthday>); }

    void ViewStudents(const std::vector<student_t>& students) const;

    static bool IsVisiable(const student_t& student) {
        return IsPublic(std::get<attr_e::attr_right>(student));
    }

};
