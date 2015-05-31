// Pingus - A free Lemmings clone
// Copyright (C) 1999 Ingo Ruhnke <grumbel@gmail.com>
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

/** This file defines some global variables, most of them are used to
    keep the values of command line arguments. */

#ifndef HEADER_PINGUS_PINGUS_GLOBALS_HPP
#define HEADER_PINGUS_PINGUS_GLOBALS_HPP

#include <string>

namespace globals {

extern int         game_speed;                      ///< -t, --set-speed
extern float       desired_fps;                     ///< -k, --set-fps
extern bool        print_fps;                       ///< --print-fps

extern bool        music_enabled;                   ///< -m, --enable-music
extern bool        sound_enabled;                   ///< -s, --enable-sound

extern bool        static_graphics;                 ///< Disables scrolling in menu and other places to make delta renderer faster
extern bool        debug_actions;                   ///< --debug-actions
extern int         fast_forward_time_scale;         ///< how much faster the game runs in FF mode
extern bool        developer_mode;                  ///< --developer-mode
extern bool        auto_scrolling;                  ///< --enable-auto-scrolling
extern bool        drag_drop_scrolling;
extern int         tile_size;                       ///< --tile-size
extern int         default_screen_width;            ///< default screen width
extern int         default_screen_height;           ///< default screen height
extern bool        draw_collision_map;              ///<
extern bool        software_cursor;                 ///< --enable-swcursor

extern std::string  global_username;                 ///< The name of the currently logged in user
extern std::string  global_email;                    ///< The email address of the currently logged in user
extern std::string  default_language;                ///< The default language, which is used when the env var LANG is not set

} // namespace globals

#endif

/* EOF */
