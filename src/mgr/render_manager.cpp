/*!
 * WTEngine | File:  render_manager.hpp
 * 
 * \author Matthew Evans
 * \version 0.1a
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2020
 */

#include <string>
#include <utility>
#include <set>
#include <iterator>
#include <stdexcept>

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>

#include "wtengine/mgr/render_manager.hpp"

namespace wte
{

namespace mgr
{

template <> bool render_manager::manager<render_manager>::initialized = false;

void render_manager::initialize(void) {
    //  Create the arena bitmap.
    if(arena_w == 0 || arena_h == 0) throw std::runtime_error("Arena size not defined!");
    al_set_new_bitmap_flags(ALLEGRO_NO_PRESERVE_TEXTURE);
    arena_bmp = al_create_bitmap(arena_w, arena_h);
    al_set_new_bitmap_flags(ALLEGRO_CONVERT_BITMAP);
    arena_created = true;

    //  Load the title screen bitmap.
    if(title_screen_file.empty()) {
        title_bmp = al_create_bitmap(1, 1);
        al_set_target_bitmap(title_bmp);
        al_clear_to_color(WTE_COLOR_BLACK);
    } else {
        ALLEGRO_FILE* file;
        file = al_fopen(title_screen_file.c_str(), "rb");
        if(!file) {
            title_bmp = al_create_bitmap(1, 1);
            al_set_target_bitmap(title_bmp);
            al_clear_to_color(WTE_COLOR_BLACK);
        } else {
            title_bmp = al_load_bitmap_f(file, title_screen_file.substr(title_screen_file.find("."),
                                            title_screen_file.length()).c_str());
        }
        al_fclose(file);
    }

    //  Load the background bitmap.
    if(background_file.empty()) {
        background_bmp = al_create_bitmap(1, 1);
        al_set_target_bitmap(background_bmp);
        al_clear_to_color(WTE_COLOR_BLACK);
    } else {
        ALLEGRO_FILE* file;
        file = al_fopen(background_file.c_str(), "rb");
        if(!file) {
            background_bmp = al_create_bitmap(1, 1);
            al_set_target_bitmap(background_bmp);
            al_clear_to_color(WTE_COLOR_BLACK);
        } else {
            background_bmp = al_load_bitmap_f(file, background_file.substr(background_file.find("."),
                                                background_file.length()).c_str());
        }
        al_fclose(file);
    }

    //  Load font used in renderer.
    if(render_font_file.empty()) overlay_font = al_create_builtin_font();
    else {
        overlay_font = al_load_font(render_font_file.c_str(), render_font_size, render_font_flags);
        if(!overlay_font) overlay_font = al_create_builtin_font();
    }
    if(!overlay_font) throw std::runtime_error("Unable to set font for renderer!");

    fps_timer = al_create_timer(1);
    fps_event_queue = al_create_event_queue();
    al_register_event_source(fps_event_queue, al_get_timer_event_source(fps_timer));
    al_start_timer(fps_timer);
}

void render_manager::de_init(void) {
    al_destroy_bitmap(title_bmp);
    al_destroy_bitmap(background_bmp);
    al_destroy_bitmap(arena_bmp);

    al_destroy_font(overlay_font);

    al_destroy_event_queue(fps_event_queue);
    al_destroy_timer(fps_timer);
}

void render_manager::update_resolution(const int& w, const int& h) {
    screen_w = w;
    screen_h = h;
}

const int render_manager::get_screen_width(void) {
    return screen_w;
}

const int render_manager::get_screen_height(void) {
    return screen_h;
}

void render_manager::set_scale_factor(const float& f) {
    scale_factor = f;
}

const int render_manager::get_scale_factor() {
    return scale_factor;
}

void render_manager::reload_arena_bitmap(void) {
    al_destroy_bitmap(arena_bmp);
    al_set_new_bitmap_flags(ALLEGRO_NO_PRESERVE_TEXTURE);
    arena_bmp = al_create_bitmap(arena_w, arena_h);
    al_set_new_bitmap_flags(ALLEGRO_CONVERT_BITMAP);
}

void render_manager::set_arena_size(const int& w, const int& h) {
    if(!arena_created) {
        arena_w = w;
        arena_h = h;
    }
}

const int render_manager::get_arena_width(void) {
    return arena_w;
}

const int render_manager::get_arena_height(void) {
    return arena_h;
}

void render_manager::set_title_screen(const std::string& fname) {
    title_screen_file = fname;
}

void render_manager::set_background_screen(const std::string& fname) {
    background_file = fname;
}

void render_manager::set_font_file(const std::string& fname, const int& size, const int& flags) {
    render_font_file = fname;
    render_font_size = size;
    render_font_flags = flags;
}

void render_manager::render(const menu_manager& menus, const entity_manager& world) {
    /*
     * Calculate fps.
     */
    fps_counter++;
    //  Update fps on unique ticks only.
    const bool queue_not_empty = al_get_next_event(fps_event_queue, &fps_event);
    if(fps_event.type == ALLEGRO_EVENT_TIMER && queue_not_empty) {
        fps = fps_counter;
        fps_counter = 0;
        al_set_timer_count(fps_timer, 0);
    }

    //  Toggle no preserve texture for faster rendering.
    al_set_new_bitmap_flags(ALLEGRO_NO_PRESERVE_TEXTURE);

    //  Set drawing to the screen.
    al_set_target_backbuffer(al_get_current_display());

    /*
     * Render world if the game is running.
     */
    if(engine_flags::is_set(GAME_STARTED)) {
        /*
         * Draw the full screen background.
         */
        al_draw_scaled_bitmap(background_bmp, 0, 0,
                              al_get_bitmap_width(background_bmp),
                              al_get_bitmap_height(background_bmp),
                              0, 0, screen_w, screen_h, 0);

        //  Set drawing to the arena bitmap.
        al_set_target_bitmap(arena_bmp);
        al_clear_to_color(WTE_COLOR_BLACK);

        /*
         * Draw the backgrounds.
         */
        const const_component_container<cmp::background> background_components =
            world.get_components<cmp::background>();

        //  Sort the background layers.
        std::multiset<entity_component_pair<cmp::background>,
            comparator<entity_component_pair<cmp::background>>> background_componenet_set(
            background_components.begin(), background_components.end());

        //  Draw each background by layer.
        for(auto & it : background_componenet_set) {
            if(world.has_component<cmp::visible>(it.first) &&
               world.get_component<cmp::visible>(it.first)->check())
            {
                if(it.second->draw_tinted())
                    al_draw_tinted_bitmap(&it.second->get_bitmap(), it.second->get_tint(), 0, 0, 0);
                else
                    al_draw_bitmap(&it.second->get_bitmap(), 0, 0, 0);
            }
        }

        /*
         * Draw the sprites.
         */
        const const_component_container<cmp::sprite> sprite_components =
            world.get_components<cmp::sprite>();

        //  Sort the sprite components.
        std::multiset<entity_component_pair<cmp::sprite>,
            comparator<entity_component_pair<cmp::sprite>>> sprite_componenet_set(
            sprite_components.begin(), sprite_components.end());

        //  Draw each sprite in order.
        for(auto & it : sprite_componenet_set) {
            if(world.has_component<cmp::location>(it.first) &&
               world.has_component<cmp::visible>(it.first) &&
               world.get_component<cmp::visible>(it.first)->check())
            {
                //  Get the current sprite frame.
                render_tmp_bmp = al_create_sub_bitmap(
                    &it.second->get_bitmap(),
                    it.second->get_sprite_x(),
                    it.second->get_sprite_y(),
                    it.second->get_sprite_width(),
                    it.second->get_sprite_height()
                );

                float sprite_angle = 0.0f;
                float center_x = 0.0f, center_y = 0.0f;
                float destination_x = 0.0f, destination_y = 0.0f;

                //  Check if the sprite should be rotated.
                if(world.has_component<cmp::direction>(it.first) &&
                   world.get_component<cmp::direction>(it.first)->show_rotated()) {
                    sprite_angle = world.get_component<cmp::direction>(it.first)->get_radians();
                    center_x = (al_get_bitmap_width(render_tmp_bmp) / 2);
                    center_y = (al_get_bitmap_height(render_tmp_bmp) / 2);

                    destination_x = world.get_component<cmp::location>(it.first)->get_x() +
                        (al_get_bitmap_width(render_tmp_bmp) * it.second->get_scale_factor_x() / 2) +
                        (it.second->get_draw_offset_x() * it.second->get_scale_factor_x());
                    destination_y = world.get_component<cmp::location>(it.first)->get_y() +
                        (al_get_bitmap_height(render_tmp_bmp) * it.second->get_scale_factor_y() / 2) +
                        (it.second->get_draw_offset_y() * it.second->get_scale_factor_y());
                } else {
                        destination_x = world.get_component<cmp::location>(it.first)->get_x() +
                            it.second->get_draw_offset_x();
                        destination_y = world.get_component<cmp::location>(it.first)->get_y() +
                            it.second->get_draw_offset_y();
                }

                //  Draw the sprite.
                if(it.second->draw_tinted())
                    al_draw_tinted_scaled_rotated_bitmap(
                        render_tmp_bmp, it.second->get_tint(),
                        center_x, center_y, destination_x, destination_y,
                        it.second->get_scale_factor_x(),
                        it.second->get_scale_factor_y(),
                        sprite_angle, 0
                    );
                else
                    al_draw_scaled_rotated_bitmap(
                        render_tmp_bmp, center_x, center_y, destination_x, destination_y,
                        it.second->get_scale_factor_x(),
                        it.second->get_scale_factor_y(),
                        sprite_angle, 0
                    );
                al_destroy_bitmap(render_tmp_bmp);
            }
        }

        /*
         * Draw sprite hitboxes if enabled.
         * Use different colors for each team.
         * Note:  Re-uses sprite container for rendering.
         */
        #if WTE_DEBUG_MODE == 3 || WTE_DEBUG_MODE == 9
        for(auto & it : sprite_componenet_set) {
            //  Make sure the entity has a hitbox and is enabled.
            if(world.has_component<cmp::hitbox>(it.first) &&
               world.has_component<cmp::team>(it.first) &&
               world.has_component<cmp::location>(it.first) &&
               world.has_component<cmp::enabled>(it.first) &&
               world.get_component<cmp::enabled>(it.first)->check())
            {
                //  Select color based on team.
                ALLEGRO_COLOR team_color;
                switch(world.get_component<cmp::team>(it.first)->get_team()) {
                    case 0: team_color = WTE_COLOR_GREEN; break;
                    case 1: team_color = WTE_COLOR_RED; break;
                    case 2: team_color = WTE_COLOR_BLUE; break;
                    default: team_color = WTE_COLOR_YELLOW;
                }
                //  Draw the hitbox.
                render_tmp_bmp = al_create_bitmap(world.get_component<cmp::hitbox>(it.first)->get_width(),
                                                  world.get_component<cmp::hitbox>(it.first)->get_height());
                al_set_target_bitmap(render_tmp_bmp);
                al_clear_to_color(team_color);
                al_set_target_bitmap(arena_bmp);
                al_draw_bitmap(render_tmp_bmp,
                               world.get_component<cmp::location>(it.first)->get_x(),
                               world.get_component<cmp::location>(it.first)->get_y(), 0);
                al_destroy_bitmap(render_tmp_bmp);
            }  //  End hitbox/enabled test.
        }  //  End render component loop.
        #endif  //  End draw hitbox check.

        /*
         * Draw the overlays.
         */
        const const_component_container<cmp::overlay> overlay_components =
            world.get_components<cmp::overlay>();

        //  Sort the overlay layers.
        std::multiset<entity_component_pair<cmp::overlay>,
            comparator<entity_component_pair<cmp::overlay>>> overlay_componenet_set(
            overlay_components.begin(), overlay_components.end());

        //  Draw each overlay by layer.
        for(auto & it : overlay_componenet_set) {
            if(world.has_component<cmp::visible>(it.first) &&
               world.get_component<cmp::visible>(it.first)->check())
            {
                if(it.second->draw_tinted())
                    al_draw_tinted_bitmap(&it.second->get_bitmap(), it.second->get_tint(),
                                           it.second->get_pos_x(), it.second->get_pos_y(), 0);
                else
                    al_draw_bitmap(&it.second->get_bitmap(),
                                    it.second->get_pos_x(), it.second->get_pos_y(), 0);
            }
        }

        /*
         * Draw the arena bitmap to the screen.
         */
        al_set_target_backbuffer(al_get_current_display());
        al_draw_scaled_bitmap(arena_bmp, 0, 0, arena_w, arena_h,
                              (screen_w / 2) - (arena_w * scale_factor / 2),
                              (screen_h / 2) - (arena_h * scale_factor / 2),
                              arena_w * scale_factor, arena_h * scale_factor, 0);
    } else {
        /*
         * Game is not running - draw the title screen.
         */
        al_draw_scaled_bitmap(title_bmp, 0, 0,
                              al_get_bitmap_width(title_bmp), al_get_bitmap_height(title_bmp),
                              0, 0, screen_w, screen_h, 0);
    }

    /*
     * Render game menu if it's opened.
     */
    if(engine_flags::is_set(GAME_MENU_OPENED)) {
        render_tmp_bmp = al_clone_bitmap(&menus.render_menu());
        al_set_target_backbuffer(al_get_current_display());

        al_draw_scaled_bitmap(
            render_tmp_bmp, 0, 0,
            al_get_bitmap_width(render_tmp_bmp), al_get_bitmap_height(render_tmp_bmp),
            (screen_w / 2) - std::floor((al_get_bitmap_width(render_tmp_bmp) * scale_factor) / 2),
            (screen_h / 2) - std::floor((al_get_bitmap_height(render_tmp_bmp) * scale_factor) / 2),
            al_get_bitmap_width(render_tmp_bmp) * scale_factor,
            al_get_bitmap_height(render_tmp_bmp) * scale_factor, 0
        );
        al_destroy_bitmap(render_tmp_bmp);
    }

    /*
     * Render alerts.
     */
    if(alert::is_set()) {
        int font_size = al_get_font_line_height(overlay_font);

        render_tmp_bmp = al_create_bitmap((alert::get_alert().length() * font_size) + 20, font_size + 20);
        al_set_target_bitmap(render_tmp_bmp);
        al_clear_to_color(alert::get_bg_color());

        al_draw_text(overlay_font, alert::get_font_color(),
                     (al_get_bitmap_width(render_tmp_bmp) / 2), 10,
                     ALLEGRO_ALIGN_CENTER, alert::get_alert().c_str());

        al_set_target_backbuffer(al_get_current_display());
        al_draw_scaled_bitmap(
            render_tmp_bmp, 0, 0,
            al_get_bitmap_width(render_tmp_bmp), al_get_bitmap_height(render_tmp_bmp),
            (screen_w / 2) - std::floor((al_get_bitmap_width(render_tmp_bmp) * scale_factor) / 2),
            (screen_h / 2) - std::floor((al_get_bitmap_height(render_tmp_bmp) * scale_factor) / 2),
            al_get_bitmap_width(render_tmp_bmp) * scale_factor,
            al_get_bitmap_height(render_tmp_bmp) * scale_factor, 0
        );
        al_destroy_bitmap(render_tmp_bmp);
    }

    /*
     * Framerate and timer rendering.
     */
    //  Draw frame rate.
    if(engine_flags::is_set(DRAW_FPS)) {
        std::string fps_string = "FPS: " + std::to_string(fps);
        al_draw_text(overlay_font, WTE_COLOR_YELLOW, screen_w, 1, ALLEGRO_ALIGN_RIGHT, fps_string.c_str());
    }

    //  Draw time if debug mode is enabled.
    #if WTE_DEBUG_MODE == 1 || WTE_DEBUG_MODE == 9
    std::string timer_string = "Timer: " + std::to_string(check_time());
    al_draw_text(overlay_font, WTE_COLOR_YELLOW, screen_w, 10, ALLEGRO_ALIGN_RIGHT, timer_string.c_str());
    #endif

    /*
     * Update the screen
     */
    al_flip_display();
    //  Set default bitmap flags.
    al_set_new_bitmap_flags(ALLEGRO_CONVERT_BITMAP);
}

} //  namespace mgr

} //  namespace wte