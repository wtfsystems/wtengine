/*!
 * \brief WTEngine | File:  game_cfg_map.hpp
 * \author Matthew Evans
 *
 * \version 0.1a
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2020
 *
 * \details Engine config.
 */

#ifndef WTE_GAME_CFG_MAP_HPP
#define WTE_GAME_CFG_MAP_HPP

#include <string>
#include <fstream>
#include <sstream>
#include <any>
#include <stdexcept>

#include "variable_map.hpp"

namespace wte
{

//!  Game config map
/*!
 * Statics to store game config variables.
 * Inherits map functionality from the variable_map class.
 * This class provides load/save functions.
 * Also provides math functions to manipulate the values.
 */
class game_cfg_map final : public variable_map<game_cfg_map> {
    public:
        /*!
         * Set the file the game config variables will be written to.
         * \param fname Filename to set saving to.
         * \return void
         */
        inline static void set_game_cfg_file(const std::string fname) {
            data_file_name = fname;
        }

        /*!
         * Load game config variables from file.
         * \param void
         * \return False on fail, true on success
         */
        inline static bool load(void) {
            std::ifstream data_file(data_file_name);
            if(!data_file.good()) return false;

            std::string it;
            //  Read each line, try to register or set
            while(std::getline(data_file, it)) {
                if(!reg(it)) set(it);
            }

            data_file.close();
            return true;
        };

        /*!
         * Clear the current game config save.
         * \param void
         * \return void
         */
        inline static void clear_save(void) {
            std::ofstream data_file(data_file_name, std::ofstream::trunc);
            data_file.close();
        };

        /*!
         * Save a game config variable to file.
         * \param var Variable name.
         * \return False on fail, true on success.
         */
        inline static bool save(const std::string var) {
            std::ofstream data_file(data_file_name, std::ofstream::app);
            if(!data_file.good()) return false;

            try {
                data_file << var << "=" << _map.at(var) << std::endl;
            } catch (std::out_of_range& e) {
                data_file.close();
                return false;  //  Didn't find var
            }

            data_file.close();
            return true;
        };

        /*!
         * Add a value to a variable in the game config map.
         * \param var Variable name.
         * \param val Value to add by.
         * \return False on fail, true on success.
         */
        template <typename T> inline static bool add(const std::string var, T val) {
            try {
                T temp;
                std::stringstream(_map.at(var)) >> temp;
                _map.at(var) = std::to_string(std::any_cast<T>(temp += val));
                return true;
            } catch (std::out_of_range& e) {
                return false;  //  Didn't find
            } catch (std::bad_any_cast& e) {
                return false;  //  Bad cast
            }
        };

        /*!
         * Subtracta value from a variable in the game config map.
         * \param var Variable name.
         * \param val Value to subtract by.
         * \return False on fail, true on success.
         */
        template <typename T> inline static bool subtract(const std::string var, T val) {
            try {
                T temp;
                std::stringstream(_map.at(var)) >> temp;
                _map.at(var) = std::to_string(std::any_cast<T>(temp -= val));
                return true;
            } catch (std::out_of_range& e) {
                return false;  //  Didn't find
            } catch (std::bad_any_cast& e) {
                return false;  //  Bad cast
            }
        };

        /*!
         * Multiplya value by a variable in the game config map.
         * \param var Variable name.
         * \param val Value to multiply by.
         * \return False on fail, true on success.
         */
        template <typename T> inline static bool multiply(const std::string var, T val) {
            try {
                T temp;
                std::stringstream(_map.at(var)) >> temp;
                _map.at(var) = std::to_string(std::any_cast<T>(temp *= val));
                return true;
            } catch (std::out_of_range& e) {
                return false;  //  Didn't find
            } catch (std::bad_any_cast& e) {
                return false;  //  Bad cast
            }
        };

        /*!
         * Dividea value by a variable in the game config map.
         * \param var Variable name.
         * \param val Value to divide by.
         * \return False on fail, true on success.
         */
        template <typename T> inline static bool divide(const std::string var, T val) {
            try {
                T temp;
                std::stringstream(_map.at(var)) >> temp;
                _map.at(var) = std::to_string(std::any_cast<T>(temp /= val));
                return true;
            } catch (std::out_of_range& e) {
                return false;  //  Didn't find
            } catch (std::bad_any_cast& e) {
                return false;  //  Bad cast
            }
        };

    private:
        inline static std::string data_file_name = "game.cfg";
};

typedef game_cfg_map::variable_map<game_cfg_map> game_cfg;

template <> inline std::map<std::string, std::string> game_cfg::_map = {};

} //  end namespace wte

#endif