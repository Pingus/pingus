//  Pingus - A free Lemmings clone
//  Copyright (C) 1999 Ingo Ruhnke <grumbel@gmx.de>
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//  
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//  
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.

/** This file defines some global variables, most of them are used to
    keep the values of command line arguments. */

#ifndef HEADER_PINGUS_PINGUS_GLOBALS_HPP
#define HEADER_PINGUS_PINGUS_GLOBALS_HPP

#include <string>

enum FramebufferType 
{
 DELTA_FRAMEBUFFER, 
 SDL_FRAMEBUFFER, 
 NULL_FRAMEBUFFER, 
 OPENGL_FRAMEBUFFER 
};

enum 
{
  PINGUS_DEBUG_ACTIONS     = (1 << 0),
  PINGUS_DEBUG_SOUND       = (1 << 1),
  PINGUS_DEBUG_GAMETIME    = (1 << 2),
  PINGUS_DEBUG_TILES       = (1 << 3),
  PINGUS_DEBUG_LOADING     = (1 << 4),

  PINGUS_DEBUG_BLITTER     = (1 << 6),
  PINGUS_DEBUG_TRANSLATOR  = (1 << 7),
  PINGUS_DEBUG_RESOURCES   = (1 << 8),
  PINGUS_DEBUG_GUI         = (1 << 9),
  PINGUS_DEBUG_INPUT       = (1 << 10),
  PINGUS_DEBUG_WORLDMAP    = (1 << 11),
  PINGUS_DEBUG_PATHMGR     = (1 << 12),
  PINGUS_DEBUG_DIRECTORIES = (1 << 13),
  PINGUS_DEBUG_ALL         = (~0)
};

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
extern std::string demo_file;                       ///<
extern bool        auto_scrolling;                  ///< --enable-auto-scrolling
extern bool        drag_drop_scrolling; 
extern int         tile_size;                       ///< --tile-size
extern bool        fullscreen_enabled;              ///< --enable-fullscreen
extern int         default_screen_width;            ///< default screen width
extern int         default_screen_height;           ///< default screen height
extern bool        draw_collision_map;              ///<
extern bool        software_cursor;                 ///< --enable-swcursor

extern std::string controller_file;                 ///< --controller

extern std::string  global_username;                 ///< The name of the currently logged in user
extern std::string  global_email;                    ///< The email address of the currently logged in user
extern std::string  default_language;                ///< The default language, which is used when the env var LANG is not set

extern FramebufferType framebuffer_type;

} // namespace globals

#endif

/* EOF */
