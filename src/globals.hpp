//  $Id$
//
//  Pingus - A free Lemmings clone
//  Copyright (C) 1999 Ingo Ruhnke <grumbel@gmx.de>
//
//  This program is free software; you can redistribute it and/or
//  modify it under the terms of the GNU General Public License
//  as published by the Free Software Foundation; either version 2
//  of the License, or (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

/** This file defines some global variables, most of them are used to
    keep the values of command line arguments. */

#ifndef HEADER_PINGUS_GLOBALS_HXX
#define HEADER_PINGUS_GLOBALS_HXX

#include "pingus.hpp"
#include <string>


extern int         game_speed;                      ///< -t, --set-speed
extern bool        print_fps;                       ///< --print-fps
extern int         verbose;                         ///< -v, --verbose

extern bool        music_enabled;                   ///< -m, --enable-music
extern bool        sound_enabled;                   ///< -s, --enable-sound

extern bool        enable_demo_recording;
extern bool        play_demo;                       ///< --play-demo

extern bool        debug_actions;                   ///< --debug-actions
extern bool        fast_mode;                       ///< --fast
extern bool        maintainer_mode;                 ///< --maintainer-mode
extern std::string demo_file;                       ///<
extern bool        auto_scrolling;                  ///< --enable-auto-scrolling
extern bool        drag_drop_scrolling; 
extern int         tile_size;                       ///< --tile-size
extern bool        fullscreen_enabled;              ///< --enable-fullscreen
extern bool        resize_enabled;                  ///< --resize-fullscreen
extern int         screen_width;                    ///< default screen width
extern int         screen_height;                   ///< default screen height
extern bool        draw_collision_map;              ///<
extern bool        swcursor_enabled;                ///< --enable-swcursor

extern std::string controller_file;                 ///< --controller

extern bool        use_opengl;                      ///< --use-opengl
extern bool        action_help;                     ///< --action-help
extern bool        show_input_debug_screen;         ///<
extern bool        render_preview;                  ///<

extern std::string global_username;                 ///< The name of the currently logged in user
extern std::string global_email;                    ///< The email address of the currently logged in user
extern std::string default_language;                ///< The default language, which is used when the env var LANG is not set
extern unsigned int pingus_debug_flags;              ///< Set some bits in this thing to get debug infos

extern int         min_frame_skip;
extern int         max_frame_skip;

enum { PINGUS_DEBUG_ACTIONS     = (1 << 0),
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


#endif

/* EOF */
