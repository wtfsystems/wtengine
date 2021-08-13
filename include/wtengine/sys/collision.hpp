/*!
 * WTEngine | File:  collision.hpp
 * 
 * \author Matthew Evans
 * \version 0.5
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2021
 */

#ifndef WTE_SYS_COLISION_HPP
#define WTE_SYS_COLISION_HPP

#include "wtengine/sys/system.hpp"

namespace wte
{

namespace sys
{

/*!
 * \class colision
 * \brief Selects components by team and tests for colisions.
 */
class colision final : public system {
    public:
        colision();
        ~colision() = default;

        /*!
         * \brief Selects components by team, then tests each team to see if there is a colision.
         */
        void run(void) override;
};

} //  namespace sys

} //  namespace wte

#endif
