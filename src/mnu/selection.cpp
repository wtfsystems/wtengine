/*!
 * WTEngine | File:  selection.cpp
 * 
 * \author Matthew Evans
 * \version 0.7
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2021
 */

#include "wtengine/mnu/selection.hpp"

namespace wte::mnu {

/*
 *
 */
selection::selection(
    const std::string& label,
    const std::vector<std::string>& dvls,
    const std::vector<std::string>& vls,
    const std::function<const std::string(void)>& dfunc,
    const std::function<void(const std::string&)>& afunc
) : item(label), display_vals(dvls), vals(vls), defaulter(dfunc), applier(afunc) {
    assert(dvls.size() == vls.size());
    set_default();
    reset_to_default();
}

/*
 *
 */
void selection::on_left(void) {
    if(current_val != vals.begin()) current_val--;
}

/*
 *
 */
void selection::on_right(void) {
    if(current_val != vals.end()) current_val++;
    if(current_val == vals.end()) current_val--;
}

/*
 *
 */
const std::vector<std::string> selection::get_text(void) const {
    return {
        get_label(), "< " + display_vals.at(std::distance(vals.cbegin(), current_val)) + " >"
    };
}

/*
 *
 */
void selection::reset_to_default(void) { current_val = default_val; }

/*
 *
 */
void selection::set_default(void) {
    auto it = std::find_if(vals.begin(), vals.end(),
                           [this](const auto& e){ return e == defaulter(); });
    if(it != vals.end()) default_val = it;
    else default_val = vals.begin();
}

/*
 *
 */
void selection::apply_setting(void) {
    applier(vals.at(std::distance(vals.cbegin(), current_val)));
}

/*
 *
 */
const bool selection::setting_changed(void) const {
    if(current_val == default_val) return false;
    return true;
}

}  //  end namespace wte::mnu
