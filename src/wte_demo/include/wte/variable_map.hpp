/*!
 * \brief WTEngine | File:  variable_map.hpp
 * \author Matthew Evans
 *
 * \version 0.1a
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2020
 *
 * \details Variable map.
 */

#ifndef WTE_VARIABLE_MAP_HPP
#define WTE_VARIABLE_MAP_HPP

#include <string>
#include <map>
#include <sstream>
#include <any>

namespace wte
{

//!  Variable map
/*!
 * Create a map of variables
 */
template <class derived> class variable_map {
    public:
        //!  Remove copy constructor
        variable_map(const variable_map&) = delete;
        //!  Remove assignment operator
        void operator=(variable_map const&) = delete;

        /*!
         * Create a new entry in the map.
         * Call this first before accessing.
         * \param void
         * \return void
         */
        inline static const bool reg(const std::string var, const std::string val) {
            if(var.find('=') != std::string::npos)
                return false;
            if(val.find('=') != std::string::npos)
                return false;
            auto ret = _map.insert(std::make_pair(var, val));
            if(ret.second == false) return false;  //  Key exists already
            else return true;  //  Inserted new key/pair
        };

        /*!
         * Create a new entry in the map.
         * Call this first before accessing.
         * \param void
         * \return void
         */
        inline static const bool reg(const std::string expr) {
            std::string var = expr.substr(0, expr.find("="));
            std::string val = expr.substr(expr.find("=") + 1, expr.length());

            return reg(var, val);
        };

        /*!
         * Check if a variable is registered in the map.
         * \param void
         * \return void
         */
        inline static const bool is_reg(const std::string var) {
            try {
                _map.at(var);
                return true;
            } catch (std::out_of_range& e) {
                return false;  //  Didn't find var
            }
        }

        /*!
         * Set key to value
         * \param void
         * \return void
         */
        inline static const bool set(const std::string var, const std::string val) {
            try {
                _map.at(var) = val;
                return true;
            } catch (std::out_of_range& e) {
                return false;  //  Didn't find var
            }
        };

        /*!
         * Set based on string
         * \param void
         * \return void
         */
        inline static const bool set(const std::string expr) {
            std::string var = expr.substr(0, expr.find("="));
            std::string val = expr.substr(expr.find("=") + 1, expr.length());

            return set(var, val);
        };

        /*!
         * Get value
         * Returns by string
         * \param void
         * \return void
         */
        inline static const std::string get(const std::string var) {
            try {
                return _map.at(var);
            } catch (std::out_of_range& e) {
                return nullptr;  //  Didn't find
            }
        };

        /*!
         * Get value
         * Returns by type
         * \param void
         * \return void
         */
        template <typename T> inline static const T get(const std::string var) {
            try {
                T temp;
                std::stringstream(_map.at(var)) >> temp;
                return std::any_cast<const T>(temp);
            } catch (std::out_of_range& e) {
                return std::any_cast<const bool>(false);  //  Didn't find
            } catch (std::bad_any_cast& e) {
                return std::any_cast<const bool>(false);  //  Bad cast
            }
        };

    protected:
        static std::map<std::string, std::string> _map;

    private:
        inline variable_map() { _map.clear(); };
        inline ~variable_map() { _map.clear(); };
};

} //  end namespace wte

#endif
