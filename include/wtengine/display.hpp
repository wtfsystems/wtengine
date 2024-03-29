/*
 * wtengine
 * --------
 * By Matthew Evans
 * See LICENSE.md for copyright information.
 */

#if !defined(WTE_DISPLAY_HPP)
#define WTE_DISPLAY_HPP

#include <allegro5/allegro.h>
#include <allegro5/allegro_opengl.h>

#include <cmath>
#include <string>
#include <map>
#include <tuple>
#include <utility>
#include <type_traits>

#include "wtengine/_debug/exceptions.hpp"
#include "wtengine/_globals/_defines.hpp"
#include "wtengine/mgr/renderer.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_allegro5.h"

namespace wte {

/*!
 * \class display
 * \brief Handles control of the display.
 */
class display {
    friend class engine;

    public:
        virtual ~display() = default;             //  Default virtual destructor.
        display(const display&) = delete;         //  Delete copy constructor.
        void operator=(display const&) = delete;  //  Delete assignment operator.

        /*!
         * \brief Set the window title.
         * \param title Window title.
         */
        static void set_window_title(const std::string& title);

        /*!
         * \brief Set the scale factor.
         * \param f Scale factor value.
         */
        static void set_scale_factor(float f);

        /*!
         * \brief Change the display mode.  Requires engine restart.
         * \param m New display mode.
         */
        static void set_display_mode(std::size_t m);

        /*!
         * \brief Set the vsync setting.
         * \param v New vsync setting.
         */
        static void set_vsync(std::size_t v);

        /*!
         * \brief Resize the display.
         * \param w Screen width.
         * \param h Screen height.
         */
        static void resize_display(int w, int h);

    protected:
        display();

    private:
        static void create_display(void);   //  Configure the display.
        static void destroy_display(void);  //  Destroy the display.
        static std::string window_title;    //  Title for application window.
        static bool initialized;            //  Restrict to one instance.

        static ALLEGRO_DISPLAY* _display;   //  Allegro object for the display.
};

}  //  end namespace wte

#endif
