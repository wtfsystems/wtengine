/*!
 * \brief WTEngine | File:  texture.hpp
 * \author Matthew Evans
 *
 * \version 0.1a
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2020
 *
 * \details Texture component.
 */

#ifndef WTE_CMP_TEXTURE_HPP
#define WTE_CMP_TEXTURE_HPP

#include <allegro5/allegro.h>

#include "../component.hpp"

namespace wte
{

namespace cmp
{

//! Texture component
/*!
 * WIP
 */
class texture : public component {
    public:
        /*!
         * Texture
         * \param void
         * \return void
         */
        inline ~texture() {
            al_destroy_bitmap(texture_bitmap);
        }

        ALLEGRO_BITMAP *texture_bitmap;
};

} //  namespace cmp

} //  namespace wte

#endif
