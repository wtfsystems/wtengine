/*
 * wtengine
 * --------
 * By Matthew Evans
 * See LICENSE.md for copyright information.
 */

#if !defined(WTE_ENGINE_HPP)
#define WTE_ENGINE_HPP

#include <ctime>
#include <iostream>
#include <string>
#include <vector>
#include <map>

#include <allegro5/allegro.h>
#include <allegro5/allegro_opengl.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_physfs.h>
#include <physfs.h>

#include "wtengine/config.hpp"
#include "wtengine/display.hpp"
#include "wtengine/input.hpp"

#include "wtengine/_debug/exceptions.hpp"
#include "wtengine/_debug/logger.hpp"
#include "wtengine/_globals/_defines.hpp"
#include "wtengine/_globals/commands.hpp"
#include "wtengine/_globals/engine_time.hpp"
#include "wtengine/_globals/wte_asset.hpp"
#include "wtengine/mgr/_managers.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_allegro5.h"

namespace wte {

/*!
 * \class engine
 * \brief The main engine class.
 * 
 * Sets up various system objects used by the engine.
 * Contains the main game loop and members for managing the game and engine.
 */
class engine : public config, public input, public display {
    public:
        /*
         * Unloads the game engine. 
         * Frees up instance, set initialized flag to false.
         */
        ~engine();

        engine(const engine&) = delete;          //  Delete copy constructor.
        void operator=(engine const&) = delete;  //  Delete assignment operator.

        /*!
         * \brief Add file path to provide to PhysFS.
         * 
         * This should be called during engine initialization before the main object is created.
         * 
         * \param flocation File location to add to PhysFS.
         */
        static void add_file_location(const std::string& flocation);

        /*!
         * \brief The main engine loop.
         */
        void do_game(void);

    protected:
        /*!
         * \brief Create a new instance of the game engine.
         * 
         * Force single instance, set initialized flag to true.
         * Throws a runtime error if another instance is called.
         * 
         * \param argc Command line arguments.
         * \param argv Command line arguments count.
         */
        engine(const int& argc, char** const& argv);

        /* These function members are overridden in the derived class */
        //!  Define this to load all systems to be used by the game.
        virtual void load_systems(void) = 0;
        //!  Define what gets loaded when a game starts.
        virtual void new_game(void) = 0;
        //!  Define what happens at the end of a game.
        virtual void end_game(void) = 0;
        //!  Optional:  On engine pause handler.
        virtual void on_engine_pause(void) {};
        //!  Optional:  On engine unpause handler.
        virtual void on_engine_unpause(void) {};
        //!  Optional:  Window out of focus handler.
        virtual void out_of_focus(void) {};
        //!  Optional:  Window back in focus handler.
        virtual void back_in_focus(void) {};
        /* *** End overridden function members *** */

    private:
        /*
         * Load the engine's managers.
         * Called before the main loop starts.
         */
        void wte_load(void);

        /*
         * Unload the engine's managers.
         * Called after the main loop ends running.
         */
        void wte_unload(void);

        /*
         * Main engine loop (single pass)
         */
        void main_loop(void);

        /*
         * Call to start a new game.
         * Loads a game data file and user defined systems and starting entities.
         * Gets passed game data file to load.
         */
        void process_new_game(const std::string& game_data);

        /*
         * Call to end the game.
         * Clears out the entities and systems and runs user defined end process.
         * If passed true, skips the custom game cleanup.
         */
        void process_end_game(const bool& force);

        //  Internal commands for the engine.
        static commands cmds;

        //  Allegro objects used by the engine.
        static ALLEGRO_TIMER* main_timer;
        static ALLEGRO_EVENT_QUEUE* main_event_queue;

        //  Vector of file paths to provide to PhysFS.
        static std::vector<std::string> file_locations;
        //  Restrict to one instance of the engine running.
        static bool initialized;
};

}  //  end namespace wte

#endif
