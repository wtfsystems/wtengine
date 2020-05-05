/*!
 * \brief WTEngine | File:  sprite.hpp
 * \author Matthew Evans
 *
 * \version 0.1a
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2020
 *
 * \details Sprite component.
 */

#ifndef WTE_CMP_SPRITE_HPP
#define WTE_CMP_SPRITE_HPP

#include <allegro5/allegro.h>

#include "animator.hpp"

namespace wte
{

namespace cmp
{

//! Sprite component
/*!
 * WIP
 */
class sprite final : public animator {
    public:
        /*!
         * Sprite
         * \param void
         * \return void
         */
        inline ~sprite() {
            al_destroy_bitmap(sprite_bitmap);
        }

        ALLEGRO_BITMAP *sprite_bitmap;
        float sprite_height, sprite_width, draw_offset;
        unsigned int current_frame, speed;
};

} //  namespace cmp

} //  namespace wte

#endif
