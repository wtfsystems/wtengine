/*
 * wtengine
 * --------
 * By Matthew Evans
 * See LICENSE.md for copyright information.
 */

#if !defined(WTE_MGR_SPAWNER_HPP)
#define WTE_MGR_SPAWNER_HPP

#include <string>
#include <utility>
#include <map>
#include <functional>

#include "wtengine/mgr/manager.hpp"

#include "wtengine/_globals/message.hpp"
#include "wtengine/mgr/world.hpp"

namespace wte {
    class engine;
}

namespace wte::mgr {

/*!
 * \class spawner
 * \brief Create or delete entities while the engine is running.
 */
class spawner final : private manager<spawner> {
    friend class wte::engine;

    public:
        /*!
         * \brief Add a spawn to the spawner map.
         * 
         * Stores a function in the map for entity creation on demand.
         * 
         * \param name Reference name for the spwaner item.
         * \param num_args Number of arguments the spawn accepts.
         * \param func Function for creating the entity.
         * \return True if inserted into the spawn map, false if not.
         */
        static bool add(
            const std::string& name,
            const std::size_t& num_args,
            const std::function<void(const entity_id&, const msg_args&)>& func
        );

        /*!
         * \brief Delete a spawn.
         * \param name Name of spawn to delete.
         * \return True if removed, else false.
         */
        static bool remove(const std::string& name);

        /*!
         * \brief Spawn entity.
         * \param name Name of entity to spawn.
         * \param args Arguments to entity creation.
         * \return True if spawned, else false.
         */
        static bool spawn(const std::string& name, const msg_args& args);

    private:
        spawner() = default;
        ~spawner() = default;

        //  Takes spawner messages and processes.
        static void process_messages(const message_container& messages);

        static std::map<
            const std::string,
            std::pair<
                const std::size_t,
                const std::function<void(const entity_id&, const msg_args&)>
        >> spawns;
};

}  //  end namespace wte::mgr

#endif
