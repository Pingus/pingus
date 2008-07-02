
//  Pingus - A free Lemmings clone
//  Copyright (C) 2006 Ingo Ruhnke <grumbel@gmx.de>
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

#include "world_dot.hpp"
#include "../stat_manager.hpp"
#include "../file_reader.hpp"

namespace WorldMapNS {

WorldDot::WorldDot(FileReader reader) :
	Dot(reader.read_section("dot")),
	is_accessible(false),
	is_finished(false)
{
	// Get the status from the StatManger
	StatManager::instance()->get_bool(name + "-accessible", is_accessible);
	StatManager::instance()->get_bool(name + "-finished", is_finished);
}

void
WorldDot::unlock()
{
	is_accessible = true;
	StatManager::instance()->set_bool(name + "-accessible", true);
}

}	// WorldMapNS namespace

/* EOF */
