/*  $Id$
**   __      __ __             ___        __   __ __   __
**  /  \    /  \__| ____    __| _/_______/  |_|__|  | |  |   ____
**  \   \/\/   /  |/    \  / __ |/  ___/\   __\  |  | |  | _/ __ \
**   \        /|  |   |  \/ /_/ |\___ \  |  | |  |  |_|  |_\  ___/
**    \__/\  / |__|___|  /\____ /____  > |__| |__|____/____/\___  >
**         \/          \/      \/    \/                         \/
**  Copyright (C) 2007 Ingo Ruhnke <grumbel@gmx.de>
**
**  This program is free software; you can redistribute it and/or
**  modify it under the terms of the GNU General Public License
**  as published by the Free Software Foundation; either version 2
**  of the License, or (at your option) any later version.
**
**  This program is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**  GNU General Public License for more details.
** 
**  You should have received a copy of the GNU General Public License
**  along with this program; if not, write to the Free Software
**  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
**  02111-1307, USA.
*/

#include "config_manager.hpp"

void
ConfigManager::set_master_volume(int v)
{
}

int
ConfigManager::get_master_volume()
{
  return 0;
}

void
ConfigManager::set_sound_volume(int v)
{
}

int
ConfigManager::get_sound_volume()
{
  return 0;
}

void
ConfigManager::set_music_volume(int v)
{
}

int
ConfigManager::get_music_volume()
{
  return 0;
}

void
ConfigManager::set_resolution(const Size& size)
{
}

Size
ConfigManager::get_resolution()
{
  return Size();
}

void
ConfigManager::set_fullscreen(bool)
{
}

bool
ConfigManager::get_fullscreen()
{
  return false;
}

void
ConfigManager::set_allow_resize(bool)
{
}

bool
ConfigManager::get_allow_resize()
{
  return false;
}

void
ConfigManager::set_fast_mode(bool)
{
}

bool
ConfigManager::get_fast_mode()
{
  return false;
}

void
ConfigManager::set_mouse_grab(bool)
{
}

bool
ConfigManager::get_mouse_grab()
{
  return false;
}

void
ConfigManager::set_print_fps(bool)
{
}

bool
ConfigManager::get_print_fps()
{
  return false;
}

void
ConfigManager::set_language(const std::string&)
{
}

std::string
ConfigManager::get_language()
{
  return "";
}

void
ConfigManager::set_swcursor(bool)
{
}

bool
ConfigManager::get_swcursor()
{
  return false;
}

void
ConfigManager::set_autoscroll(bool)
{
}

bool
ConfigManager::get_autoscroll()
{
  return false;
}

/* EOF */
