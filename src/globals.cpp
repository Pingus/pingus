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

#include "globals.hpp"

int         game_speed                      = 20;
float       desired_fps                     = 40;
bool        print_fps                       = false;
bool        music_enabled                   = true;
bool        sound_enabled                   = true;
int         fast_forward_time_scale         = 4;
bool        maintainer_mode                 = false;
std::string demo_file;
bool        auto_scrolling                  = true;
bool        drag_drop_scrolling             = false;
int         tile_size                       = 32;

int         default_screen_width            = 800;
int         default_screen_height           = 600;
bool        draw_collision_map              = false;
bool        swcursor_enabled                = false;

std::string controller_file;

std::string global_username;
std::string global_email;
std::string default_language                = "en";

unsigned int pingus_debug_flags              = 0;

bool        fullscreen_enabled              = false;
bool        delta_drawing                   = false;

FramebufferType framebuffer_type = DELTA_FRAMEBUFFER;

/* EOF */
