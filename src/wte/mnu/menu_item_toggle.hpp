/*!
 * \brief WTEngine | File:  menu_item_toggle.hpp
 * \author Matthew Evans
 *
 * \version 0.1a
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2020
 *
 * \details Toggle menu item.
 */

#ifndef WTE_MNU_MENU_ITEM_TOGGLE_HPP
#define WTE_MNU_MENU_ITEM_TOGGLE_HPP

#include <string>
#include <vector>
#include <functional>

#include "menu_item.hpp"

namespace wte
{

namespace mnu
{

//!
/*!
 *
 */
class menu_item_toggle final : public menu_item {
    public:
        /*!
         * Create a menu toggle item.  Uses default setting type.
         * \param label Item display label.
         * \param con Command to run when toggled on.
         * \param aon Arguments to use when toggled on.
         * \param coff Command to run when toggled off.
         * \param aoff Arguments to use when toggled off.
         * \param func Fuction to determine defaut setting.
         * \return void
         */
        inline menu_item_toggle(const std::string label,
                                const std::string con,
                                const std::string aon,
                                const std::string coff,
                                const std::string aoff,
                                bool func(void)) :
        menu_item(label), cmd_on(con), arg_on(aon), cmd_off(coff), arg_off(aoff), defaulter(func) {
            toggled = default_val = defaulter();
        };

        /*!
         * Create a menu toggle item.
         * \param label Item display label.
         * \param con Command to run when toggled on.
         * \param aon Arguments to use when toggled on.
         * \param coff Command to run when toggled off.
         * \param aoff Arguments to use when toggled off.
         * \param st Setting type.
         * \param func Fuction to determine defaut setting.
         * \return void
         */
        inline menu_item_toggle(const std::string label,
                                const std::string con,
                                const std::string aon,
                                const std::string coff,
                                const std::string aoff,
                                const std::size_t st,
                                bool func(void)) :
        menu_item(label, st), cmd_on(con), arg_on(aon), cmd_off(coff), arg_off(aoff), defaulter(func) {
            toggled = default_val = defaulter();
        };

        /*!
         * Menu toggle item destructor.
         * \param void
         * \return void
         */
        inline ~menu_item_toggle() {};

        /*!
         * \brief On left process.
         * Toggles on.
         * \param alt_trigger Alt trigger flag (unused).
         * \return void
         */
        inline void on_left(bool alt_trigger) override { toggled = true; };

        /*!
         * \brief On right process.
         * Toggles off.
         * \param alt_trigger Alt trigger flag (unused).
         * \return void
         */
        inline void on_right(bool alt_trigger) override { toggled = false; };

        /*!
         * Get display text
         * \param void
         * \return Vector of strings to render.
         */
        inline const std::vector<std::string> get_text(void) override {
            if(toggled) return { get_label(), "< On >", "Off" };
            return { get_label(), "On", "< Off >" };
        };

        /*!
         * \brief Get the active command.
         * Return the command depending if the setting is toggled on or off.
         * \param void
         * \return Active command string.
         */
        inline const std::string get_active_cmd(void) {
            if(toggled) return cmd_on;
            return cmd_off;
        };

        /*!
         * \brief Get the active arguments.
         * Return arguments depending if the setting is toggled on or off.
         * \param void
         * \return Active arguments string.
         */
        inline const std::string get_active_args(void) {
            if(toggled) return arg_on;
            return arg_off;
        };

        /*!
         * \brief Reset to default value.
         * If the menu is canceled, return to proper setting.
         * \param void
         * \return void
         */
        inline void reset_to_default(void) override { toggled = default_val; };

        /*!
         * \brief Set default value.
         * Determine and set the default value for the menu toggle item.
         * \param void
         * \return void
         */
        inline void set_default(void) override { toggled = default_val = defaulter(); };

        /*!
         * \brief Setting changed process.
         * Checks if the current selection is the same as the default.
         * \param void
         * \return True if the setting changed, false if it did not.
         */
        inline const bool setting_changed(void) const override {
            if(toggled == default_val) return false;
            return true;
        };

    private:
        bool toggled, default_val;
        std::string cmd_on;
        std::string cmd_off;
        std::string arg_on;
        std::string arg_off;

        std::function<bool(void)> defaulter;
};

}  // end namespace mnu

}  // end namespace wte

#endif
