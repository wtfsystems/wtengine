/*!
 * \brief WTEngine | File:  menu_item_setting.hpp
 * \author Matthew Evans
 *
 * \version 0.1a
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2020
 *
 * \details Setting menu item.
 */

#ifndef WTE_MENU_MENU_ITEM_SETTING_HPP
#define WTE_MENU_MENU_ITEM_SETTING_HPP

#include <string>
#include <vector>

#include "menu_item.hpp"

namespace wte
{

namespace mnu
{

//!
/*!
*/
class menu_item_setting final : public menu_item {
    public:
        inline menu_item_setting() {};
        inline ~menu_item_setting() {};

        std::vector<std::string> get_text(void);
        void do_item(void);
};

/*!
*/
inline std::vector<std::string> menu_item_setting::get_text(void) {
    return {};
}

/*!
*/
inline void menu_item_setting::do_item(void) {
    //
}

}  // end namespace mnu

}  // end namespace wte

#endif
