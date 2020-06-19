/*!
 * \brief WTEngine | File:  wte.hpp
 * \author Matthew Evans
 *
 * \version 0.1a
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2020
 *
 * \details Main WTE Engine include.
 * Non-engine source files include this.
 */

#ifndef WTE_MAIN_INCLUDE_HPP
#define WTE_MAIN_INCLUDE_HPP

#include "wte_global_defines.hpp"

#include "_globals/engine_cfg_map.hpp"
#include "_globals/game_cfg_map.hpp"

#include "mnu/menu.hpp"
#include "mnu/menu_items.hpp"
#include "mgr/managers.hpp"
#include "cmp/components.hpp"
#include "sys/systems.hpp"

#include "wte_main.hpp"

#ifdef _WTE_USE_HELPER_MACROS
#include "wte_helper_macros.hpp"
#endif

#endif
