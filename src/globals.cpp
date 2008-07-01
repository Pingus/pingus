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

#include "globals.hpp"


int         game_speed                      = 20;
float         desired_fps                     = 40;
bool        print_fps                       = false;
int         verbose                         = 0;
bool        music_enabled                   = true;
bool        sound_enabled                   = true;
bool        enable_demo_recording           = false;
bool        play_demo                       = false;
bool        fast_mode                       = false;
bool        maintainer_mode                 = false;
std::string demo_file;
bool        auto_scrolling                  = true;
bool        drag_drop_scrolling             = false;
int         tile_size                       = 32;

int         default_screen_width            = 800;
int         default_screen_height           = 600;
int         screen_width                    = default_screen_width;
int         screen_height                   = default_screen_height;
bool        draw_collision_map              = false;
bool        swcursor_enabled                = false;
bool        action_help                     = true;
bool        show_input_debug_screen         = false;
bool        render_preview                  = false;

std::string controller_file;

std::string global_username;
std::string global_email;
std::string default_language                = "en";

unsigned int pingus_debug_flags              = 0;

bool        fullscreen_enabled              = false;

bool        resize_enabled                  = false;

#if defined(WIN32) || defined(__APPLE__)
// The clanSDL target is a little buggy on Windows and OSX - Use OpenGL by default
bool        use_opengl                      = true;
#else
// Use the clanSDL target by default for systems other than Windows and OSX (aka Linux).
bool        use_opengl                      = false;
#endif

/* EOF */
