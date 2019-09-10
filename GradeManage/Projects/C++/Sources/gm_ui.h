
class UI final
{
public:

    enum class _Language : unsigned char { Chinese, English };

    enum class _Menu : unsigned char {
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

    enum class _MenuMain : unsigned char {
        item_quit,
        item_login,
        item_resgister,
        item_switch,
        item_main_max
    };

    enum class _MenuStudent : unsigned char {
        item_back,
        item_view,
        item_password,
        item_right,
        item_cancel,
        item_student_max
    };

    enum class _MenuAdmin : unsigned char {
        item_back,
        item_view,
        item_password,
        item_delete,
        item_admin_max
    };

    enum class _MenuView : unsigned char {
        item_back,
        item_sort,
        item_filter,
        item_view_max
    };

    enum class _MenuSort : unsigned char {
        item_back,
        item_by_account,
        item_by_name,
        item_by_score,
        item_by_birthday,
        item_sort_max
    };

    enum class _MenuFilter : unsigned char {
        item_back,
        item_by_account,
        item_by_name,
        item_by_score,
        item_by_birthday,
        item_by_sex,
        item_by_right,
        item_filter_max
    };

    enum class _MenuOrder : unsigned char {
        item_back,
        item_Ascending,
        item_Dscending,
        item_order_max
    };

    enum class _MenuLang : unsigned char {
        item_back,
        item_Chinese,
        item_English,
        item_lang_max
    };

    _Language _eLanguage{ _Language::Chinese };
    
public:

    _Language _GetLanguage() const { return _eLanguage; }
    void _SetLanguage(_Language lang) { _eLanguage = lang; }

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

    unsigned int _ShowMenu(_Menu menuId) const
    {
        switch (menuId)
        {
        case _Menu::menu_main:
            return _ShowMenu({ {1, str_Login}, {2, str_Register}, {3, str_Switch}, {0, str_Exit} });
        case _Menu::menu_student:
            return _ShowMenu({ {1, str_View}, {2, str_ModifyPassword}, {3, str_ModifyRight}, {4, str_Logout}, {0, str_Back} });
        case _Menu::menu_admin:
            return _ShowMenu({ {1, str_View}, {2, str_ModifyPassword}, {3, str_Delete}, {0, str_Back} });
        case _Menu::menu_view:
            return _ShowMenu({ {1, str_Sort}, {2, str_Filter}, {0, str_Back} });
        case _Menu::menu_sort:
            return _ShowMenu({ {1, str_ByAccount}, {2, str_ByName}, {3, str_ByScore}, {4, str_ByBirthday}, {0, str_Back} });
        case _Menu::menu_filter:
            switch (_Mgr_.GetRole())
            {
            case Manager::Role::role_student:
                return _ShowMenu({ {1, str_ByAccount}, {2, str_ByName}, {3, str_ByScore}, {4, str_ByBirthday}, {5, str_BySex}, {0, str_Back} });
            case Manager::Role::role_admin:
                return _ShowMenu({ {1, str_ByAccount}, {2, str_ByName}, {3, str_ByScore}, {4, str_ByBirthday}, {5, str_BySex}, {6, str_ByRight}, {0, str_Back} });
            default:
                return 0;
            }
        case _Menu::menu_order:
            return _ShowMenu({ {1, str_Ascending}, {2, str_Dscending}, {0, str_Back} });
        case _Menu::menu_language:
            return _ShowMenu({ {1, str_Chinese}, {2, str_English}, {0, str_Back} });
        default:
            return 0;
        }
    }

};
