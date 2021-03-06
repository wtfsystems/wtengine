/*!
 * WTEngine | File:  entities.hpp
 * 
 * \author Matthew Evans
 * \version 0.4
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2021
 */

#ifndef WTE_MGR_ENTITY_HPP
#define WTE_MGR_ENTITY_HPP

#define WTE_ENTITY_ERROR (0)
#define WTE_ENTITY_START (1)
#define WTE_ENTITY_MAX (std::numeric_limits<entity_id>::max())

#include <string>
#include <cstring>
#include <vector>
#include <map>
#include <unordered_map>
#include <utility>
#include <iterator>
#include <algorithm>
#include <limits>
#include <memory>

#include "wtengine/_globals/wte_exception.hpp"
#include "wtengine/mgr/manager.hpp"
#include "wtengine/cmp/component.hpp"

namespace wte
{

/*
 * Define containers for entity/component/world storage.
 */
/*!
 * \typedef std::size_t entity_id
 * Container to store an entity id.
 */
typedef std::size_t entity_id;

/*!
 * \typedef std::pair<entity_id, std::string> entity
 * Container to store an entity reference.
 */
typedef std::pair<entity_id, std::string> entity;

/*!
 * \typedef std::vector<entity> world_container
 * Container for storing a group of entity references.
 */
typedef std::vector<entity> world_container;

/*!
 * \typedef std::vector<cmp::component_sptr> entity_container
 * Container to store a group of components related to an entity.
 */
typedef std::vector<cmp::component_sptr> entity_container;

/*!
 * \typedef std::vector<cmp::component_csptr> const_entity_container
 * Constant container to store a group of components related to an entity.
 */
typedef std::vector<cmp::component_csptr> const_entity_container;

/*!
 * Container for storing components of similar type.
 * \tparam Component type
 */
template <typename T>
using component_container = std::map<const entity_id, std::shared_ptr<T>>;

/*!
 * Constant container for storing components of similar type.
 * \tparam Component type
 */
template <typename T>
using const_component_container = std::map<const entity_id, std::shared_ptr<const T>>;

/*!
 * \typedef std::unordered_multimap<entity_id, cmp::component_sptr> world_map
 * Container to store the entire game world.
 */
typedef std::unordered_multimap<entity_id, cmp::component_sptr> world_map;

namespace mgr
{

/*!
 * \class entities
 * \brief Store a collection of entities and their corresponding components in memory.
 */
class entities final : private manager<entity> {
    public:
        /*!
         * \brief Clear the entity manager.
         * 
         * Set the entity counter to zero and clear the entities and componenets.
         */
        inline static void clear(void) {
            entity_counter = WTE_ENTITY_START;
            entity_vec.clear();
            world.clear();
        };

        /*!
         * \brief Create a new entity by name, using the next available ID.
         * 
         * Throw error if there is no room for entities.
         * 
         * \return The newly created entity ID.  WTE_ENTITY_ERROR on fail.
         */
        inline static const entity_id new_entity(void) {
            entity_id next_id;

            if(entity_counter == WTE_ENTITY_MAX) {  //  Counter hit max.
                bool test = false;
                //  Look for the first available ID.
                for(next_id = WTE_ENTITY_START; !test; next_id++) {
                    if(next_id == WTE_ENTITY_MAX) return WTE_ENTITY_ERROR;  //  No available ID, error.
                    //  See if the new ID does not exist.
                    test = (std::find_if(entity_vec.begin(), entity_vec.end(),
                                         [&next_id](const entity& e){ return e.first == next_id; })
                            == entity_vec.end());
                }
            } else {  //  Counter not max, use the counter for entity ID.
                next_id = entity_counter;
                entity_counter++;
            }

            //  Set a new name.  Make sure name doesn't exist.
            std::string entity_name = "Entity" + std::to_string(next_id);
            bool test = false;
            for(entity_id temp_id = WTE_ENTITY_START; !test; temp_id++) {
                if(temp_id == WTE_ENTITY_MAX) return WTE_ENTITY_ERROR;  //  Couldn't name entity, error.
                //  See if the new name does not exist.
                test = (std::find_if(entity_vec.begin(), entity_vec.end(),
                                     [&entity_name](const entity& e){ return e.second == entity_name; })
                        == entity_vec.end());
                //  If it does, append the temp number and try that.
                if(!test) entity_name = "Entity" + std::to_string(next_id) + std::to_string(temp_id);
            }

            //  Tests complete, insert new entity.
            entity_vec.push_back(std::make_pair(next_id, entity_name));
            return next_id;  //  Return new entity ID.
        };

        /*!
         * \brief Delete entity by ID.
         * 
         * \param e_id The entity ID to delete.
         * \return Return true on success, false if entity does not exist.
         */
        inline static const bool delete_entity(const entity_id& e_id) {
            auto e_it = std::find_if(entity_vec.begin(), entity_vec.end(),
                                     [&e_id](const entity& e){ return e.first == e_id; });
            if(e_it != entity_vec.end()) {
                world.erase(e_id);     //  Remove all associated componenets.
                entity_vec.erase(e_it);  //  Delete the entity.
                return true;
            }
            return false;
        };

        /*!
         * \brief Check if an entity exists.
         * 
         * Check the entity vector by ID and return result.
         * 
         * \param e_id The entity ID to check.
         * \return Return true if found, return false if not found.
         */
        inline static const bool entity_exists(const entity_id& e_id) {
            return (std::find_if(entity_vec.begin(), entity_vec.end(),
                                 [&e_id](const entity& e){ return e.first == e_id; })
                    != entity_vec.end());
        };

        /*!
         * \brief Get entity name.
         * 
         * \param e_id Entity ID to get name for.
         * \return Entity name string.
         * \exception wte_exception Entity does not exist.
         */
        inline static const std::string get_name(const entity_id& e_id) {
            auto e_it = std::find_if(entity_vec.begin(), entity_vec.end(),
                                     [&e_id](const entity& e){ return e.first == e_id; });
            if(e_it != entity_vec.end()) return e_it->second;
            //  Not found, throw error.
            std::string err_str = "Entity " + std::to_string(e_id) + " does not exist";
            char err_c[err_str.size() + 1];
            strcpy(err_c, err_str.c_str());
            throw wte_exception(err_c);
        };

        /*!
         * \brief Set the entity name.
         * 
         * \param e_id Entity ID to set.
         * \param name Entity name to set.
         * \return True if set, false on error.
         */
        inline static const bool set_name(const entity_id& e_id, const std::string& name) {
            auto n_it = std::find_if(entity_vec.begin(), entity_vec.end(),
                                     [&name](const entity& e){ return e.second == name; });
            if(n_it != entity_vec.end())
                return false;  //  Entity with the new name exists, error.
            auto e_it = std::find_if(entity_vec.begin(), entity_vec.end(),
                                     [&e_id](const entity& e){ return e.first == e_id; });
            if(e_it != entity_vec.end()) {
                e_it->second = name;
                return true;  //  New name set.
            }
            return false;  //  Didn't find entity_id, error.
        };

        /*!
         * \brief Get entity ID by name.
         * 
         * \param name Name to search.
         * \return Entity ID, WTE_ENTITY_ERROR if not found.
         */
        inline static const entity_id get_id(const std::string& name) {
            auto n_it = std::find_if(entity_vec.begin(), entity_vec.end(),
                                     [&name](const entity& e){ return e.second == name; });
            if(n_it != entity_vec.end())
                return n_it->first;
            return WTE_ENTITY_ERROR;
        };

        /*!
         * \brief Get the entity reference vector.
         * 
         * \return Returns a vector of all entity IDs and names.
         */
        inline static const world_container get_entities(void) {
            return entity_vec;
        };

        /*!
         * \brief Set all components related to an entity.
         * 
         * \param e_id Entity ID to set components for.
         * \return Returns a container of components based by entity ID.
         * \exception wte_exception Entity does not exist.
         */
        inline static const entity_container set_entity(const entity_id& e_id) {
            if(!entity_exists(e_id)) {
                std::string err_str = "Entity " + std::to_string(e_id) + " does not exist";
                char err_c[err_str.size() + 1];
                strcpy(err_c, err_str.c_str());
                throw wte_exception(err_c);
            }

            entity_container temp_container;
            const auto results = world.equal_range(e_id);

            for(auto it = results.first; it != results.second; it++) {
                temp_container.emplace_back(cmp::component_sptr((*it).second));
            }
            return temp_container;
        };

        /*!
         * \brief Get all components related to an entity.
         * 
         * \param e_id Entity ID to get components for.
         * \return Returns a constant container of components based by entity ID.
         * \exception wte_exception Entity does not exist.
         */
        inline static const const_entity_container get_entity(const entity_id& e_id) {
            if(!entity_exists(e_id)) {
                std::string err_str = "Entity " + std::to_string(e_id) + " does not exist";
                char err_c[err_str.size() + 1];
                strcpy(err_c, err_str.c_str());
                throw wte_exception(err_c);
            }

            const_entity_container temp_container;
            const auto results = world.equal_range(e_id);

            for(auto it = results.first; it != results.second; it++) {
                temp_container.emplace_back(cmp::component_csptr((*it).second));
            }
            return temp_container;
        };

        /*!
         * \brief Add a new component to an entity.
         * 
         * Entities can only have a single compoenent of each type.
         * 
         * \param e_id Entity ID to add a component to.
         * \param comp Componenet to add.
         * \return Return false if the entity does not exist.
         * \return Return false if the entity already has a component of the same type.
         * \return Return true on success.
         */
        inline static const bool add_component(const entity_id& e_id, const cmp::component_sptr& comp) {
            if(!entity_exists(e_id)) return false;

            //  Check derived types of existing components, make sure one does not already exist.
            const const_entity_container check_entity = get_entity(e_id);
            for(auto & it : check_entity) {
                if(typeid(*it).name() == typeid(*comp).name()) return false;
            }

            world.insert(std::make_pair(e_id, comp));
            return true;
        };

        /*!
         * \brief Delete a component by type for an entity.
         * 
         * \tparam T Component type.
         * \param e_id Entity ID to delete component from.
         * \return Return true if a component was deleted.
         * \return Return false if no components were deleted.
         */
        template <typename T> inline static const bool delete_component(const entity_id& e_id) {
            auto results = world.equal_range(e_id);

            for(auto it = results.first; it != results.second; it++) {
                if(std::dynamic_pointer_cast<T>(it->second)) {
                    it = world.erase(it);
                    return true;
                }
            }
            return false;
        };

        /*!
         * \brief Check if an entity has a component by type.
         * 
         * \tparam T Component type.
         * \param e_id The entity ID to check.
         * \return Return true if the entity has the component.
         * \return Return false if it does not.
         */
        template <typename T> inline static const bool has_component(const entity_id& e_id) {
            const auto results = world.equal_range(e_id);

            for(auto it = results.first; it != results.second; it++) {
                if(std::dynamic_pointer_cast<T>(it->second)) return true;
            }
            return false;
        };

        /*!
         * \brief Set the value of a component by type for an entity.
         * 
         * \tparam T Component type.
         * \param e_id The entity ID to search.
         * \return Return the component.
         * \exception wte_exception Component not found.
         */
        template <typename T> inline static const std::shared_ptr<T> set_component(const entity_id& e_id) {
            const auto results = world.equal_range(e_id);

            for(auto it = results.first; it != results.second; it++) {
                if(std::dynamic_pointer_cast<T>(it->second))
                    return std::static_pointer_cast<T>(it->second);
            }
            std::string err_str = "Entity: " + std::to_string(e_id) + " - Component not found";
            char err_c[err_str.size() + 1];
            strcpy(err_c, err_str.c_str());
            throw wte_exception(err_c);
        };

        /*!
         * \brief Read the value of a component by type for an entity.
         * 
         * \tparam T Component type.
         * \param e_id The entity ID to search.
         * \return Return the component.
         * \exception wte_exception Component not found.
         */
        template <typename T> inline static const std::shared_ptr<const T> get_component(const entity_id& e_id) {
            const auto results = world.equal_range(e_id);

            for(auto it = results.first; it != results.second; it++) {
                if(std::dynamic_pointer_cast<T>(it->second))
                    return std::static_pointer_cast<const T>(it->second);
            }
            std::string err_str = "Entity: " + std::to_string(e_id) + " - Component not found";
            char err_c[err_str.size() + 1];
            strcpy(err_c, err_str.c_str());
            throw wte_exception(err_c);
        };

        /*!
         * \brief Set all components for a particulair type.
         * 
         * This will return a container of modifiable components casted to their type.
         * 
         * \tparam T Component type.
         * \return Returns a container of components of all the same type.
         */
        template <typename T> inline static const component_container<T> set_components(void) {
            component_container<T> temp_components;

            for(auto & it : world) {
                if(std::dynamic_pointer_cast<T>(it.second))
                    temp_components.insert(std::make_pair(it.first, std::static_pointer_cast<T>(it.second)));
            }
            return temp_components;
        };

        /*!
         * \brief Get all components for a particulair type.
         * 
         * This will return a container of non-modifiable components casted to their type.
         * 
         * \tparam T Component type.
         * \return Returns a constant container of components of all the same type.
         */
        template <typename T> inline static const const_component_container<T> get_components(void) {
            const_component_container<T> temp_components;

            for(auto & it : world) {
                if(std::dynamic_pointer_cast<T>(it.second))
                    temp_components.insert(std::make_pair(it.first, std::static_pointer_cast<T>(it.second)));
            }
            return temp_components;
        };

    private:
        /*!
         * \brief Entity manager constructor.
         *
         * Clears the entity & world containers.
         */
        inline entities() {
            entity_vec.clear();
            world.clear();
        };

        /*!
         * \brief Entity manager destructor.
         *
         * Clears the entity manager.
         */
        inline ~entities() {
            clear();
        };

        inline static entity_id entity_counter = WTE_ENTITY_START;
        inline static world_container entity_vec = {};
        inline static world_map world = {};
};

template <> inline bool manager<entity>::initialized = false;

} //  namespace mgr

} //  namespace wte

#endif
