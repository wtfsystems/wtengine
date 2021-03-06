/*!
 * WTEngine | File:  manager.hpp
 * 
 * \author Matthew Evans
 * \version 0.4
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2021
 */

#ifndef WTE_MGR_MANAGER_HPP
#define WTE_MGR_MANAGER_HPP

#include <memory>
#include <stdexcept>

namespace wte
{

namespace mgr
{

/*!
 * \class manager
 * \brief Extend this to create a manager class.
 * 
 * These are restricted to a single instance.
 */
template <class derived> class manager {
    public:
        //!  Free up single instance,
        //!  sets the initialized flag to false.
        inline virtual ~manager() {
            initialized = false;
        };

        //!  Remove copy constructor
        manager(const manager&) = delete;
        //!  Remove assignment operator
        void operator=(manager const&) = delete;

    private:
        //  Flag used to force single instance
        static bool initialized;

    protected:
        //!  Force single instance, sets the initialized flag to true.
        //!  Throws a runtime error if more than one instance of this class is running
        inline manager() {
            if(initialized == true) throw std::runtime_error("An instance of this manager is already running!");
            initialized = true;
        };
};

}  // end namespace mgr

}  // end namespace wte

#endif
