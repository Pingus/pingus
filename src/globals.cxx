//  $Id: globals.cxx,v 1.7 2003/03/21 22:08:06 grumbel Exp $
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

#include "globals.hxx"

int         game_speed                      = 25;
bool        print_fps                       = false;
int         verbose                         = 0;
bool        music_enabled                   = true;
bool        sound_enabled                   = true;
bool        gimmicks_enabled                = false;
bool        intro_disabled                  = false;
bool        record_demo                     = false;
bool        play_demo                       = false;
bool        cursor_enabled                  = false;
bool        fs_preload                      = false;  
bool        fast_mode                       = false;
bool        previews_enabled                = true;
bool        quick_play                      = false;
bool        maintainer_mode                 = false;
std::string demo_file;
bool        start_editor                    = false;
bool        auto_scrolling                  = true;
int         tile_size                       = 32;
bool        fullscreen_enabled              = false;
int         screen_width                    = 800;
int         screen_height                   = 600;
bool        preload_data                    = false; // FIXME: Should be default if it would work
bool        draw_collision_map              = false;
bool        swcursor_enabled                = false;
std::string config_file;
bool        background_manipulation_enabled = true;
bool        max_cpu_usage                   = true;
bool        use_opengl                      = false;
bool        action_help                     = true;
bool        show_input_debug_screen         = false;

int         min_frame_skip                  = 1;
int         max_frame_skip                  = 5;

std::string global_username;
std::string global_email; 
std::string default_language                = "en";

int         pingus_debug_flags              = 0;

/* EOF */
