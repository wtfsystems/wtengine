/*!
 * WTEngine | File:  dispatcher.hpp
 * 
 * \author Matthew Evans
 * \version 0.4
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2021
 */

#ifndef WTE_CMP_DISPATCHER_HPP
#define WTE_CMP_DISPATCHER_HPP

#include <functional>

#include "wtengine/cmp/component.hpp"
#include "wtengine/mgr/messages.hpp"
#include "wtengine/mgr/entities.hpp"
#include "wtengine/_globals/message.hpp"
#include "wtengine/_globals/alert.hpp"
#include "wtengine/_globals/wte_exception.hpp"

namespace wte
{

namespace cmp
{

/*!
 * \class dispatcher
 * \brief Tag components to be processed by the Logic system.
 * 
 * Define message processing in handle_msg lambda.
 */
class dispatcher final : public component {
    public:
        /*!
         * \brief Dispatcher constructor.
         * 
         * \param func Function to define message processing.
         */
        dispatcher(
            const std::function<void(const entity_id&, const message&)>& func
        );

        /*!
         * \brief Dispatched destructor.
         */
        inline ~dispatcher() {};

        /*!
         * \brief Run dispatcher function wrapper.
         * 
         * \param e_id ID of the entity calling the wrapper.
         * \param world Reference to the entity manager.
         * \param messages Reference to the message manager.
         * \param current_time Current engine time.
         * \param msg Message to process.
         */
        void proc_msg(const entity_id& e_id, const message& msg);

    private:
        std::function<void(const entity_id&, const message&)> handle_msg;
};

} //  namespace cmp

} //  namespace wte

#endif
