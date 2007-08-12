//  $Id: surface_drawable.cxx,v 1.8 2003/12/14 00:30:04 grumbel Exp $
//
//  Pingus - A free Lemmings clone
//  Copyright (C) 2002 Ingo Ruhnke <grumbel@gmx.de>
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

#include <iostream>

#include "../resource.hpp"
#include "../display/drawing_context.hpp"
#include "manager.hpp"
#include "worldmap.hpp"
#include "pingus.hpp"
#include "surface_drawable.hpp"

namespace WorldMapNS {

SurfaceDrawable::SurfaceDrawable(FileReader reader)
  : Drawable(reader)
{
  auto_uncover = false;
  ResDescriptor desc;

  reader.read_desc  ("surface", desc);
  reader.read_vector("position", pos);
  reader.read_bool  ("auto-uncover", auto_uncover);

  surface = Resource::load_sprite(desc);
  //std::cout << "XXX Desc: " << desc.res_name << std::endl;
}

void
SurfaceDrawable::update(float delta)
{
  UNUSED_ARG(delta);
}

void
SurfaceDrawable::draw(DrawingContext& gc)
{
  if (surface)
    {
      if (auto_uncover)
        {
          Vector3f pingus_pos = WorldMapManager::instance()->get_worldmap()->get_pingus()->get_pos();
          // Pingu is not over the surface
          if (!(pingus_pos.x > pos.x && pingus_pos.x < pos.x + surface.get_width()
                &&
                pingus_pos.y > pos.y && pingus_pos.y < pos.y + surface.get_height()))
            {
              gc.draw(surface, pos);
            }
          else if (pingus_pos.z > pos.z + 1000)
            { // FIXME: Hack for the 0.6.0 release/tutorial world remove later
              gc.draw(surface, pos);
            }
        }
      else
        {
          gc.draw(surface, pos);
        }
    }
}

} // namespace WorldMapNS

/* EOF */
