/*
  WTEngine
  By:  Matthew Evans
  File:  wte_config.hpp

  See LICENSE.txt for copyright information

  Engine config
*/

#ifndef WTE_CONFIG_HPP
#define WTE_CONFIG_HPP

namespace wte
{

//!  Engine config
/*!
  Statics to store engine config variables
*/
class wte_config {
    public:
        inline wte_config() {};
        inline ~wte_config() {};

        //!  Remove copy constructor
        wte_config(const wte_config&) = delete;
        //!  Remove assignment operator
        void operator=(wte_config const&) = delete;

        //!  Set the timer rate
        static const float TICKS_PER_SECOND;

        //!  Set screen width
        static unsigned int screen_width;
        //!  Set screen height
        static unsigned int screen_height;

    private:
        //
};

inline unsigned int wte_config::screen_width;
inline unsigned int wte_config::screen_height;

} //  end namespace wte

#endif
