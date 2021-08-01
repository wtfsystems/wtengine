/*!
 * WTEngine | File:  menu.cpp
 * 
 * \author Matthew Evans
 * \version 0.5
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2021
 */

#include "wtengine/mnu/menu.hpp"

namespace wte
{

namespace mnu
{

/*
 *
 */
menu::menu(
    const std::string i,
    const std::string t
) : id(i), title(t) { items.clear(); }

/*
 *
 */
menu::~menu() { items.clear(); }

/*
 *
 */
const std::string menu::get_id(void) const { return id; }

/*
 *
 */
const std::string menu::get_title(void) const { return title; }

/*
 *
 */
const std::size_t menu::num_items(void) const { return items.size(); }

/*
 *
 */
menu_item_citerator menu::items_cbegin(void) const { return items.cbegin(); }

/*
 *
 */
menu_item_citerator menu::items_cend(void) const { return items.cend(); }

/*
 *
 */
void menu::set_title(const std::string& t) { title = t; }

/*
 *
 */
void menu::render(void) {
    //
}

}  // end namespace mnu

}  // end namespace wte