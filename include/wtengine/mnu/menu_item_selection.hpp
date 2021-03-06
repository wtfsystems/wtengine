/*!
 * WTEngine | File:  menu_item_selection.hpp
 * 
 * \author Matthew Evans
 * \version 0.4
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2021
 */

#ifndef WTE_MNU_MENU_ITEM_SELECTION_HPP
#define WTE_MNU_MENU_ITEM_SELECTION_HPP

#include <string>
#include <vector>
#include <algorithm>
#include <stdexcept>

#include "wtengine/mnu/menu_item.hpp"

namespace wte
{

namespace mnu
{

/*!
 * \class menu_item_selection
 * \brief Menu item with multiple options.
 */
class menu_item_selection final : public menu_item {
    public:
        /*!
         * \brief Menu item selection constructor.
         * 
         * \param label Item display label.
         * \param vr Variable to adjust.
         * \param dvl Vector of display items.
         * \param vl Vector of allowed settings.
         * \param st Type of setting.
         */
        menu_item_selection(const std::string label,
                                   const std::string vr,
                                   const std::vector<std::string> dvl,
                                   const std::vector<std::string> vl,
                                   const std::size_t st);

        /*!
         * \brief Menu item selection destructor.
         */
        ~menu_item_selection();

        /*!
         * \brief Define left click process.
         */
        void on_left(void) override;

        /*!
         * \brief Define right click process.
         */
        void on_right(void) override;

        /*!
         * \brief Return display text for the menu item when rendering.
         * 
         * \return Vector of display text strings.
         */
        const std::vector<std::string> get_text(void) const override;

        /*!
         * \brief Reset to the default variable.
         */
        void reset_to_default(void) override;

        /*!
         * \brief Set the default variable.
         */
        void set_default(void) override;

        /*!
         * \brief Setting changed process.
         * 
         * Checks if the current selection is the same as the default.
         * 
         * \return True if the setting changed, false if it did not.
         */
        const bool setting_changed(void) const override;

        /*!
         * \brief Get the setting to be applied by the system.
         * 
         * \return A string in format var=val
         */
        const std::string get_setting(void);

    private:
        std::string var;
        std::vector<std::string> display_vals;
        std::vector<std::string> vals;
        std::vector<std::string>::const_iterator current_val;
        std::vector<std::string>::const_iterator default_val;
};

}  // end namespace mnu

}  // end namespace wte

#endif
