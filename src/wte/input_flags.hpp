/*
  WTEngine
  By:  Matthew Evans
  File:  input_flags.hpp

  See LICENSE.txt for copyright information

  Input flags
*/

#ifndef WTE_INPUT_FLAGS_HPP
#define WTE_INPUT_FLAGS_HPP

namespace wte
{

enum INPUT_FLAG_ENUM {
   INPUT_UP, INPUT_DOWN, INPUT_LEFT, INPUT_RIGHT,
   INPUT_ACTION_1, INPUT_ACTION_2, INPUT_ACTION_3,
   INPUT_MENU_SELECT,
   MAX_INPUT_FLAGS
};

//!
/*!
*/
class input_flags final {
    public:
        inline input_flags() {};
        inline ~input_flags() {};

        input_flags(const input_flags&) = delete;
        void operator=(input_flags const&) = delete;

        inline static void set(const int f) { flags[f] = true; };
        inline static void unset(const int f) { flags[f] = false; };
        inline static const bool is_set(const int f) { return flags[f]; };

        inline static void unset_all(void) {
            for(int i = 0; i < MAX_INPUT_FLAGS; i++) flags[i] = false;
        }

    private:
        static bool flags[MAX_INPUT_FLAGS];
};

inline bool input_flags::flags[MAX_INPUT_FLAGS] = {};

} //  end namespace wte

#endif
