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

#include <string>
#include <utility>
#include <map>

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>

#include "animator.hpp"
#include "../../mgr/entity_manager.hpp"

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
        inline sprite(float sw, float sh, float dox, float doy, std::size_t sp, std::size_t l) :
        sprite_width(sw), sprite_height(sh), draw_offset_x(dox), draw_offset_y(doy),
        start_frame(0), stop_frame(0), current_frame(0), speed(sp),
        animator(l, [](entity e_id, mgr::entity_manager& world, int64_t engine_time) {
            //  Define sprite animation process
            if(world.get_component<sprite>(e_id)->current_frame > world.get_component<sprite>(e_id)->stop_frame)
                world.set_component<sprite>(e_id)->current_frame = world.get_component<sprite>(e_id)->start_frame;
        }) {
            sprite_bitmap = NULL;
        };

        /*!
         * Sprite
         * \param void
         * \return void
         */
        inline ~sprite() {
            al_destroy_bitmap(sprite_bitmap);
        };

        /*!
         * Sprite
         * \param void
         * \return void
         */
        inline void load_sprite(std::string fname) {
            sprite_bitmap = al_load_bitmap(fname.c_str());
        };

        /*!
         * Sprite
         * \param void
         * \return void
         */
        inline void add_cycle(std::string name, std::size_t start, std::size_t stop) {
            cycles.insert(std::make_pair(name, std::make_pair(start, stop)));
        };

        /*!
         * Sprite
         * \param void
         * \return void
         */
        inline void set_cycle(std::string name) {
            auto it = cycles.find(name);
            if(it != cycles.end()) {
                start_frame = it->second.first;
                stop_frame = it->second.second;
            }
        };

        ALLEGRO_BITMAP* sprite_bitmap;

        float sprite_height, sprite_width;
        float draw_offset_x, draw_offset_y;
        std::size_t current_frame, speed;
        std::size_t start_frame, stop_frame;

    private:
        std::map<std::string, std::pair<std::size_t, std::size_t>> cycles;
};

} //  namespace cmp

} //  namespace wte

#endif
