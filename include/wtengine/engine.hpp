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
#include <functional>

#if defined(__EMSCRIPTEN__)
#include <emscripten.h>
#include <emscripten/html5.h>
#endif

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

void do_game(void);
#if defined(__EMSCRIPTEN__)
void em_looper(void);
#endif

/*!
 * \class engine
 * \brief The main engine class.
 * 
 * Sets up various system objects used by the engine.
 * Contains the main game loop and members for managing the game and engine.
 */
class engine final : public config, public input, public display {
  friend void do_game(void);
  #if defined(__EMSCRIPTEN__)
  friend void em_looper(void);
  #endif

  public:
    engine(const engine&) = delete;          //  Delete copy constructor.
    void operator=(engine const&) = delete;  //  Delete assignment operator.

    /*!
     * \brief Add file path to provide to PhysFS.
     * 
     * This should be called during engine initialization before the main object is created.
     * 
     * \param flocation File location to add to PhysFS.
     */
    static void add_file_location(const std::string& flocation) {
      file_locations.push_back(flocation);
    };

    /*!
     * \brief Initialize the engine.
     * \param argc Command line arguments count.
     * \param argv Command line arguments.
     */
    static void initialize(const int& argc, char** const& argv);
    
    /*!
     * \brief De-initialize the engine.
     */
    static void de_init(void) {
      std::cout << "\nStopping WTEngine...\n";
      PHYSFS_deinit();

      std::cout << "Cleaning up engine objects... ";
      al_destroy_timer(main_timer);
      al_destroy_event_queue(main_event_queue);
      input::destroy_event_queue();
      destroy_display();
      al_inhibit_screensaver(false);
      std::cout << "OK!\n";
      std::cout << "Stopping Allegro... ";
      al_shutdown_font_addon();
      if(config::flags::audio_installed) al_uninstall_audio();
      al_shutdown_primitives_addon();
      al_uninstall_system();
      std::cout << "OK!\n";

      if constexpr (build_options.debug_mode) {
        logger::stop();
        mgr::messages::message_log_stop();
      }

      initialized = false;
      std::cout << "\nGood bye!\n\n";
    };

    //!  Define this to load all systems to be used by the game.
    inline static std::function<void(void)> load_systems = [](){};
    //!  Define what gets loaded when a game starts.
    inline static std::function<void(void)> new_game = [](){};
    //!  Define what happens at the end of a game.
    inline static std::function<void(void)> end_game = [](){};
    //!  Optional:  On engine pause handler.
    inline static std::function<void(void)> on_engine_pause = [](){};
    //!  Optional:  On engine unpause handler.
    inline static std::function<void(void)> on_engine_unpause = [](){};
    //!  Optional:  Window out of focus handler.
    inline static std::function<void(void)> out_of_focus = [](){};
    //!  Optional:  Window back in focus handler.
    inline static std::function<void(void)> back_in_focus = [](){};

  private:
    engine() = default;
    ~engine() = default;

    /*
     * Load the engine's managers.
     * Called before the main loop starts.
     */
    static void wte_load(void) {
      //  Generate Allegro's default font and load into asset mgr.
      mgr::assets::load<ALLEGRO_FONT>("wte_default_font", make_asset<ALLEGRO_FONT>());

      //  Initialize managers that require it.
      mgr::audio::initialize();
      mgr::gfx::renderer::initialize();

      //  Set default states.
      config::_flags::is_running = true;
      config::_flags::engine_started = false;
      config::flags::engine_paused = false;
    };

    /*
     * Unload the engine's managers.
     * Called after the main loop ends running.
     */
    static void wte_unload(void) {
      mgr::audio::de_init();
      mgr::gfx::renderer::de_init();
      mgr::assets::clear_al_objects();
    };

    /*
     * Main engine loop (single pass)
     */
    static void main_loop(void) {
      input::check_events();  //  Check for input.

      //  Game not running, make sure the timer isn't.
      if(!config::flags::engine_started) al_stop_timer(main_timer);
      else {
        //  Pause / resume timer check.  Also process the on_pause events.
        if(config::flags::engine_paused && al_get_timer_started(main_timer)) {
          al_stop_timer(main_timer);
          on_engine_pause();
        }
        if(!config::flags::engine_paused && !al_get_timer_started(main_timer)) {
          on_engine_unpause();
          al_resume_timer(main_timer);
        }
      }

      ALLEGRO_EVENT event;
      if(al_get_next_event(main_event_queue, &event)) {
        switch(event.type) {
        //  Call our game logic update on timer events.
        //  Timer is only running when the game is running.
        case ALLEGRO_EVENT_TIMER:
          //  Set the engine_time object to the current time.
          engine_time::set(al_get_timer_count(main_timer));
          //  Run all systems.
          mgr::systems::run();
          //  Process messages.
          mgr::messages::dispatch();
          //  Get any spawner messages and pass to handler.
          mgr::spawner::process_messages(mgr::messages::get("spawner"));
          break;
        //  Check if display looses focus.
        case ALLEGRO_EVENT_DISPLAY_SWITCH_OUT:
          out_of_focus();
          break;
        //  Check if display returns to focus.
        case ALLEGRO_EVENT_DISPLAY_SWITCH_IN:
          back_in_focus();
          break;
        //  Force quit if the game window is closed.
        case ALLEGRO_EVENT_DISPLAY_CLOSE:
          if(config::flags::engine_started) process_end_game(true);
          config::_flags::is_running = false;
          break;
        //  Window has been resized.
        case ALLEGRO_EVENT_DISPLAY_RESIZE:
          if constexpr (build_options.use_imgui)
            ImGui_ImplAllegro5_InvalidateDeviceObjects();
          al_acknowledge_resize(_display);
          if constexpr (build_options.use_imgui)
            ImGui_ImplAllegro5_CreateDeviceObjects();
          break;
        }
      }

      //  Render the screen.
      mgr::gfx::renderer::render();
      //  Get any system messages and pass to handler.
      cmds.process_messages(mgr::messages::get("system"));
      //  Send audio messages to the audio queue.
      mgr::audio::process_messages(mgr::messages::get("audio"));
      //  Delete unprocessed messages.
      mgr::messages::prune();
    };

    /*
     * Call to start a new game.
     * Loads a game data file and user defined systems and starting entities.
     * Gets passed game data file to load.
     */
    static void process_new_game(const std::string& game_script) {
      std::cout << "Starting new game... ";
      std::srand(std::time(nullptr));  //  Seed random, using time.

      //  Load a new message data file.
      if(!game_script.empty()) mgr::messages::load_file(game_script);

      //  Load systems and prevent further systems from being loaded.
      load_systems();
      mgr::systems::finalized = true;
      if(mgr::systems::empty()) throw engine_error("No systems have been loaded!");

      //  Stop audio manager from playing sounds.
      mgr::audio::music::a::stop();
      mgr::audio::music::b::stop();
      mgr::audio::ambiance::stop();
      mgr::audio::voice::stop();
      mgr::audio::sample::clear_instances();
      
      //  Clear world and load starting entities.
      mgr::world::clear();
      
      //  Call custom start game process
      new_game();

      //  Restart the timer at zero.
      al_stop_timer(main_timer);
      al_set_timer_count(main_timer, 0);
      engine_time::set(al_get_timer_count(main_timer));
      config::_flags::engine_started = true;
      config::flags::engine_paused = false;
      al_start_timer(main_timer);
      std::cout << "READY!\n";
    };

    /*
     * Call to end the game.
     * Clears out the entities and systems and runs user defined end process.
     * If passed true, skips the custom game cleanup.
     */
    static void process_end_game(const bool& force) {
      std::cout << "Ending game... ";
      al_stop_timer(main_timer);
      config::_flags::engine_started = false;
      al_set_timer_count(main_timer, 0);
      engine_time::set(al_get_timer_count(main_timer));

      //  Stop audio manager from playing sounds.
      mgr::audio::music::a::stop();
      mgr::audio::music::b::stop();
      mgr::audio::ambiance::stop();
      mgr::audio::voice::stop();
      mgr::audio::sample::clear_instances();

      //  Call custom end game process.
      if(!force) end_game();

      //  Clear managers.
      mgr::world::clear();
      mgr::systems::clear();
      mgr::messages::clear();
      std::cout << "DONE!\n";
    };

    //  Internal commands for the engine.
    static commands cmds;

    //  Allegro objects used by the engine.
    inline static ALLEGRO_TIMER* main_timer = NULL;
    inline static ALLEGRO_EVENT_QUEUE* main_event_queue = NULL;

    //  Vector of file paths to provide to PhysFS.
    static std::vector<std::string> file_locations;
    //  Restrict to one instance of the engine running.
    inline static bool initialized = false;
};

#if defined(__EMSCRIPTEN__)
inline void em_looper(void) {
  engine::main_loop();
  if(!config::flags::is_running) emscripten_cancel_main_loop();
}
#endif

/*!
 * \brief The main engine loop.
 */
inline void do_game(void) {
  //  Load engine.
  engine::wte_load();

  //  MAIN ENGINE LOOP
  #if defined(__EMSCRIPTEN__)
    emscripten_set_main_loop(em_looper, 0, true);
  #else
    while(config::flags::is_running) engine::main_loop();
  #endif

  // Unload engine.
  engine::wte_unload();
}

}  //  end namespace wte

#endif
