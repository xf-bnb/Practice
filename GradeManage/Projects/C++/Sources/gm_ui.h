
class UI final
{
public:

    enum class Menu : unsigned char {
        menu_main,
        menu_student,
        menu_admin,
        menu_view,
        menu_sort,
        menu_filter,
        menu_order,
        menu_language,
        menu_max
    };

    enum class MenuMain : unsigned char {
        item_quit,
        item_login,
        item_resgister,
        item_switch,
        item_main_max
    };

    enum class MenuStudent : unsigned char {
        item_back,
        item_view,
        item_password,
        item_right,
        item_cancel,
        item_student_max
    };

    enum class MenuAdmin : unsigned char {
        item_back,
        item_view,
        item_password,
        item_delete,
        item_admin_max
    };

    enum class MenuView : unsigned char {
        item_back,
        item_sort,
        item_filter,
        item_view_max
    };

    enum class MenuSort : unsigned char {
        item_back,
        item_by_account,
        item_by_name,
        item_by_score,
        item_by_birthday,
        item_sort_max
    };

    enum class MenuFilter : unsigned char {
        item_back,
        item_by_account,
        item_by_name,
        item_by_score,
        item_by_birthday,
        item_by_sex,
        item_by_right,
        item_filter_max
    };

    enum class MenuOrder : unsigned char {
        item_back,
        item_ascending,
        item_dscending,
        item_order_max
    };

    enum class MenuLang : unsigned char {
        item_back,
        item_chinese,
        item_english,
        item_lang_max
    };

public:

    void _ShowMenuItem(unsigned int index, unsigned int id) const;
    void _ShowItem(unsigned int id) const;
    void _ShowLabel(unsigned int id) const;

    template<unsigned int n>
    unsigned int _ShowMenu(const std::pair<unsigned int, unsigned int>(&items)[n]) const
    {
        for (auto item : items)
            _ShowMenuItem(item.first, item.second);

        return n;
    }

    unsigned int _ShowMenu(Menu menuId) const
    {
        switch (menuId)
        {
        case Menu::menu_main:
            return _ShowMenu({ {1, str_Login}, {2, str_Register}, {3, str_Switch}, {0, str_Exit} });
        case Menu::menu_student:
            return _ShowMenu({ {1, str_View}, {2, str_ModifyPassword}, {3, str_ModifyRight}, {4, str_Logout}, {0, str_Back} });
        case Menu::menu_admin:
            return _ShowMenu({ {1, str_View}, {2, str_ModifyPassword}, {3, str_Delete}, {0, str_Back} });
        case Menu::menu_view:
            return _ShowMenu({ {1, str_Sort}, {2, str_Filter}, {0, str_Back} });
        case Menu::menu_sort:
            return _ShowMenu({ {1, str_ByAccount}, {2, str_ByName}, {3, str_ByScore}, {4, str_ByBirthday}, {0, str_Back} });
        case Menu::menu_filter:
            switch (Manager::GetInstance().GetRole())
            {
            case Manager::Role::role_student:
                return _ShowMenu({ {1, str_ByAccount}, {2, str_ByName}, {3, str_ByScore}, {4, str_ByBirthday}, {5, str_BySex}, {0, str_Back} });
            case Manager::Role::role_admin:
                return _ShowMenu({ {1, str_ByAccount}, {2, str_ByName}, {3, str_ByScore}, {4, str_ByBirthday}, {5, str_BySex}, {6, str_ByRight}, {0, str_Back} });
            default:
                return 0;
            }
        case Menu::menu_order:
            return _ShowMenu({ {1, str_Ascending}, {2, str_Dscending}, {0, str_Back} });
        case Menu::menu_language:
            return _ShowMenu({ {1, str_Chinese}, {2, str_English}, {0, str_Back} });
        default:
            return 0;
        }
    }

};
