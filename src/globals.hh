//  $Id: globals.hh,v 1.22 2000/09/30 21:34:42 grumbel Exp $
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

#ifndef GLOBALS_HH
#define GLOBALS_HH

#include <string>

/** This file defines some global variables, most of them are used to
    keep the values of command line arguments. */
extern int    game_speed;        /// -t, --set-speed
extern bool   print_fps;         /// --print-fps
extern int    verbose;           /// -v, --verbose
extern bool   music_enabled;     /// -m, --enable-music
extern bool   sound_enabled;     /// -s, --enable-sound
extern std::string pingus_datadir;    /// -p, --datadir
extern bool   gimmicks_enabled;  /// -i, --enable-gimmicks
extern bool   intro_disabled;    /// -n, --disable-intro
extern bool   record_demo;       /// --record-demo
extern bool   play_demo;         /// --play-demo
extern bool   cursor_enabled;    /// --enable-cursor
extern bool   debug_actions;     /// --debug-actions
extern bool   fs_preload;        /// --fs-preload
extern bool   fast_mode;         /// --fast
extern bool   previews_enabled;  /// --disable-previews
extern bool   unlimited_actions; /// --enable-uactions
extern bool   quick_play;        /// -q, --quick_play
extern bool   maintainer_mode;   /// --maintainer-mode
extern bool   horizontal_button_panel; ///
extern bool   start_editor;   ///
extern std::string demo_file; ///
extern bool   auto_scrolling;    /// --enable-auto-scrolling
extern int    tile_size;         /// --tile-size  
extern bool   fullscreen_enabled;        /// --enable-fullscreen
extern int    screen_width; ///
extern int    screen_height; ///
extern bool   preload_data; ///
extern bool   draw_collision_map; ///
extern bool   debug_tiles; ///
extern bool   swcursor_enabled; /// --enable-swcursor
extern std::string config_file;  /// --config-file
extern bool   background_manipulation_enabled; /// --enable-bg-manipulation
extern bool   debug_game_time; /// --debug-game-time

extern bool   use_datafile;   /// --use-datafile

/// The name of the currently logged in user
extern std::string global_username;

/// The email address of the currently logged in user
extern std::string global_email; 

/** The default language, which is used when the env var LANG is not
    set */
extern std::string default_language;

/** Set some bits in this thing to get debug infos */
extern int debug_flags;

#define DEBUG_ACTIONS (1 << 0)
#define DEBUG_SOUND   (1 << 1)
#define DEBUG_MUSIC   (1 << 2)

#endif

/* EOF */
