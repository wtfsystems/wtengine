/*!
 * WTEngine | File:  background.hpp
 * 
 * \author Matthew Evans
 * \version 0.4
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2021
 */

#ifndef WTE_CMP_BACKGROUND_HPP
#define WTE_CMP_BACKGROUND_HPP

#include <string>
#include <map>
#include <functional>

#include <allegro5/allegro.h>

#include "wtengine/cmp/gfx/animator.hpp"
//#include "wtengine/cmp/gfx/bitmap_map.hpp"

namespace wte
{

namespace cmp
{

/*!
 * \class background
 * \brief Component for storing a background image and defining its animation process.
 */
class background final : public animator {
    public:
        /*!
         * \brief Static background, solid color.
         * 
         * \param w Background width.
         * \param h Background height.
         * \param c Allegro color.
         * \param l Background layer.
         */
        background(const std::string& bmp, const std::size_t& l, const ALLEGRO_COLOR& c);

        /*!
         * \brief Background with custom animation.
         * 
         * \param w Background width.
         * \param h Background height.
         * \param l Background layer.
         * \param func Animcation function.
         */
        background(const std::string& bmp, const std::size_t& l,
                   const std::function<void(const entity_id&)>& func);

        /*!
         * \brief Background destructor.
         */
        ~background();

        /*!
         * \brief Get the saved color.
         * 
         * \return Allegro color object.
         */
        ALLEGRO_COLOR get_color(void) const;

    private:
        ALLEGRO_COLOR color;
};

} //  namespace cmp

} //  namespace wte

#endif
