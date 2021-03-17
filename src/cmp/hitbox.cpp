/*!
 * WTEngine | File:  hitbox.hpp
 * 
 * \author Matthew Evans
 * \version 0.1a
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2020
 */

#include "wtengine/cmp/hitbox.hpp"

namespace wte
{

namespace cmp
{

hitbox::hitbox(float w, float h) : width(w), height(h), solid(true) {}

hitbox::hitbox(float w, float h, bool s) : width(w), height(h), solid(s) {}

const float hitbox::get_width(void) const {
    return width;
}

const float hitbox::get_height(void) const {
    return height;
}

void hitbox::set_width(const float& w) {
    width = w;
}

void hitbox::set_height(const float& h) {
    height = h;
}

const bool hitbox::is_solid(void) const {
    return solid;
}

void hitbox::make_solid(void) {
    solid = true;
}

void hitbox::make_fluid(void) {
    solid = false;
}

} //  namespace cmp

} //  namespace wte