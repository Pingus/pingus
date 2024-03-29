// Pingus - A free Lemmings clone
// Copyright (C) 1999 Ingo Ruhnke <grumbel@gmail.com>
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

#include "pingus/resource.hpp"

#include <logmich/log.hpp>

#include "engine/display/font_description.hpp"
#include "engine/display/sprite_description.hpp"
#include "engine/display/sprite_description.hpp"
#include "pingus/path_manager.hpp"
#include "util/pathname.hpp"

namespace pingus {

ResourceManager Resource::resmgr;

void
Resource::init()
{
  resmgr.add_resources_from_directory(Pathname("images", Pathname::DATA_PATH));
}

void
Resource::deinit()
{
}

SpriteDescription*
Resource::load_sprite_desc(std::string const& res_name)
{
  return resmgr.get_sprite_description(res_name);
}

Surface
Resource::load_surface(ResDescriptor const& desc_)
{
  SpriteDescription* desc = resmgr.get_sprite_description(desc_.res_name);
  if (desc)
  {
    if (desc->array != geom::isize(1, 1) ||
        desc->frame_pos != geom::ipoint(0, 0) ||
        desc->frame_size != geom::isize(-1, -1))
    {
      Surface surface(desc->filename);

      surface = surface.subsection(geom::irect(desc->frame_pos, desc->frame_size));

      if (desc_.modifier == ResourceModifier::ROT0)
      {
        return surface;
      }
      else
      {
        return surface.mod(desc_.modifier);
      }
    }
    else
    {
      if (desc_.modifier == ResourceModifier::ROT0)
      {
        return Surface(desc->filename);
      }
      else
      {
        return Surface(desc->filename).mod(desc_.modifier);
      }
    }
  }
  else
  {
    log_error("failed to load surface: {}", desc_.res_name);
    return Surface(Pathname("images/core/misc/404.png", Pathname::DATA_PATH));
  }
}

Surface
Resource::load_surface(std::string const& res_name)
{
  return load_surface(ResDescriptor(res_name));
}

Font
Resource::load_font(std::string const& res_name)
{
  FontDescription desc(Pathname("images/" + res_name + ".font", Pathname::DATA_PATH));
  //("data/images/fonts/chalk_large-iso-8859-1.font");
  return Font(desc);
}

Sprite
Resource::load_thumb_sprite(std::string const& name)
{
  Pathname thumb_path("thumbnails/" + name + ".png", Pathname::DATA_PATH);
  if (thumb_path.exist())
  {
    log_info("Loading thumb from: {}", thumb_path.str());
    return Sprite(thumb_path);
  }
  else
  {
    Surface surface = load_surface(name);
    if (!surface)
    {
      return Sprite();
    }
    else
    {
      geom::isize thumb_size(std::min(surface.get_width(), 48),
                      std::min(surface.get_height(), 48));

      Sprite sprite(surface.scale(thumb_size.width(), thumb_size.height()));
      sprite.set_hotspot(geom::origin::TOP_LEFT, (48 - sprite.get_width())/2, (48 - sprite.get_height())/2);
      return sprite;
    }
  }
}

} // namespace pingus

/* EOF */
