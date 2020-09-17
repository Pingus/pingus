// Pingus - A free Lemmings clone
// Copyright (C) 2002 Ingo Ruhnke <grumbel@gmail.com>
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "pingus/worldmap/sprite_drawable.hpp"

#include "pingus/res_descriptor.hpp"
#include "pingus/worldmap/pingus.hpp"
#include "pingus/worldmap/worldmap.hpp"

namespace WorldmapNS {

SpriteDrawable::SpriteDrawable(const ReaderMapping& reader) :
  Drawable(reader),
  surface(),
  pos(),
  m_z_index(0.0f),
  auto_uncover()
{
  auto_uncover = false;
  ResDescriptor desc;

  reader.read_desc  ("surface", desc);
  reader.read_vector("position", pos, m_z_index);
  reader.read_bool  ("auto-uncover", auto_uncover);

  surface = Sprite(desc);
  //log_info("XXX Desc: " << desc.res_name);
}

void
SpriteDrawable::update(float delta)
{
}

void
SpriteDrawable::draw(DrawingContext& gc)
{
  if (surface)
  {
    if (auto_uncover)
    {
      Vector3f pingus_pos = Worldmap::current()->get_pingus()->get_pos();
      float pingus_z_index = Worldmap::current()->get_pingus()->z_index();

      // Pingu is not over the surface
      if (!(pingus_pos.x() > pos.x() && pingus_pos.x() < pos.x() + static_cast<float>(surface.get_width()) &&
            pingus_pos.y() > pos.y() && pingus_pos.y() < pos.y() + static_cast<float>(surface.get_height())))
      {
        gc.draw(surface, pos, m_z_index);
      }
      else if (pingus_z_index > m_z_index + 1000)
      { // FIXME: Hack for the 0.6.0 release/tutorial world remove later
        gc.draw(surface, pos, m_z_index);
      }
    }
    else
    {
      gc.draw(surface, pos, m_z_index);
    }
  }
}

} // namespace WorldmapNS

/* EOF */
