//  Pingus - A free Lemmings clone
//  Copyright (C) 2000 Ingo Ruhnke <grumbel@gmx.de>
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

#include "pingus/path_manager.hpp"

#include <sstream>

#include "pingus/debug.hpp"
#include "pingus/globals.hpp"
#include "util/system.hpp"

PathManager g_path_manager;

PathManager::PathManager() :
  base_path()
{
}

PathManager::~PathManager()
{
}

void
PathManager::set_path(const std::string& path)
{
  base_path = path;
}

std::string
PathManager::complete(const std::string& relative_path)
{
  std::string comp_path = base_path + "/" + relative_path;
  pout(PINGUS_DEBUG_PATHMGR) << "PathManager: " << relative_path << " -> " << comp_path << std::endl;

  return comp_path;
}

/* EOF */
