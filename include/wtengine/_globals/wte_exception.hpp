/*!
 * WTEngine | File:  wte_exception.hpp
 * 
 * \author Matthew Evans
 * \version 0.4
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2021
 */

#ifndef WTE_EXCEPTION_HPP
#define WTE_EXCEPTION_HPP

#include <exception>

namespace wte
{

/*!
 * \class wte_exception
 * \brief Throws an internal engine exception.
 * 
 * Exceptions thrown this way will be displayed as an alert in the engine.
 */
class wte_exception : public std::exception {
    public:
        /*!
         * \brief wte_exception constructor.
         * \param desc Exception description.
         */
        inline wte_exception(const char* desc) : exception_desc(desc) {};
        inline ~wte_exception() {};

        /*!
         * \brief Returns the description of the thrown exception.
         *
         * \return Thrown exception.
         */
        virtual const char* what() const throw() {
            return exception_desc;
        }

    private:
        const char* exception_desc;
};

} //  end namespace wte

#endif
