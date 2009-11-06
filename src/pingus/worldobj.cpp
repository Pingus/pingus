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

#include "pingus/worldobj.hpp"

#include <typeinfo>
#include <iostream>
#include "engine/display/scene_context.hpp"


World* WorldObj::world;

void
WorldObj::set_world(World* arg_world)
{
  world = arg_world;
}

WorldObj::WorldObj(const FileReader& reader) :
  id()
{
  reader.read_string("id", id);
}

WorldObj::WorldObj() :
  id()
{
  // z_pos = 0;
}

WorldObj::~WorldObj()
{

}

void
WorldObj::on_startup()
{
  // do nothing
}

void
WorldObj::update()
{
  // do nothing
}

void
WorldObj::draw_offset(int /*x*/, int /*y*/, float /*s*/)
{
  // do nothing
  std::cout << "WorldObj::draw_offset(): not implemented, probally a bug: "
	    << typeid(*this).name () << std::endl;
}

void
WorldObj::draw_smallmap(SmallMap* smallmap)
{
}

void
WorldObj::draw (SceneContext& gc)
{
  // FIXME: I need some docu on the meaning of get_x_offset() and co.
  std::cout << "WorldObj:draw(SceneContext): Using compat-wrapper: "
	    << typeid(*this).name () << std::endl;
#if 0 // FIXME:
  draw_offset (static_cast<int>(gc.get_x_offset () + gc.get_width ()/2),
	       static_cast<int>(gc.get_y_offset () + gc.get_height ()/2),
	       gc.get_zoom ());
#endif
}


/* EOF */
