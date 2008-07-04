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

#ifndef WIN32
#  include <unistd.h>
#  include <sys/types.h>
#  include <sys/stat.h>
#endif

#include <assert.h>

#include "system.hpp"
#include "path_manager.hpp"
#include "globals.hpp"
#include "font_description.hpp"
#include "resource.hpp"
#include "res_descriptor.hpp"
#include "blitter.hpp"
#include "sprite_description.hpp"
#include "debug.hpp"

ResourceManager Resource::resmgr;
#if 0
std::map<ResDescriptor, CL_Surface>       Resource::surface_map;
#endif

void
Resource::init()
{
  resmgr.add_resources(path_manager.complete("data/core.res"));
  resmgr.add_resources(path_manager.complete("data/entrances.res"));
  resmgr.add_resources(path_manager.complete("data/exits.res"));
  ////  resmgr.add_resources(path_manager.complete("data/fonts.res"));
  resmgr.add_resources(path_manager.complete("data/game.res"));
  resmgr.add_resources(path_manager.complete("data/special.res"));
  resmgr.add_resources(path_manager.complete("data/groundpieces-bridge.res"));
  resmgr.add_resources(path_manager.complete("data/groundpieces-ground.res"));
  resmgr.add_resources(path_manager.complete("data/groundpieces-remove.res"));
  resmgr.add_resources(path_manager.complete("data/groundpieces-solid.res"));
  resmgr.add_resources(path_manager.complete("data/groundpieces-transparent.res"));
  resmgr.add_resources(path_manager.complete("data/hotspots.res"));
  resmgr.add_resources(path_manager.complete("data/liquids.res"));
  resmgr.add_resources(path_manager.complete("data/pingus-player0.res"));
  resmgr.add_resources(path_manager.complete("data/pingus-player1.res"));
  resmgr.add_resources(path_manager.complete("data/pingus-player2.res"));
  resmgr.add_resources(path_manager.complete("data/pingus-player3.res"));
  resmgr.add_resources(path_manager.complete("data/pingus-common.res"));
  resmgr.add_resources(path_manager.complete("data/particles.res"));
  resmgr.add_resources(path_manager.complete("data/story.res"));
  resmgr.add_resources(path_manager.complete("data/textures.res"));
  resmgr.add_resources(path_manager.complete("data/traps.res"));
  resmgr.add_resources(path_manager.complete("data/worldmaps.res"));
  resmgr.add_resources(path_manager.complete("data/worldobjs.res"));
  resmgr.add_resources(path_manager.complete("data/alias.res"));
}

// Returns all resources in the given section
#if 0
std::vector<std::string>
Resource::get_resources(const std::string& type, const std::string& section)
{
  if (section == "")
    return resmgr.get_resources_of_type(type);
  else
    return resmgr.get_resources_of_type(type, section);
}

// Returns a list of sections.  Returns all sections if left blank.
std::vector<std::string>
Resource::get_sections(const std::string& section)
{
  if (section == std::string())
    return resmgr.get_all_sections();
  else
    return resmgr.get_sections(section);
}
#endif

void
Resource::deinit()
{
  cleanup();
#if 0
  surface_map.clear();
#endif
}

SpriteDescription*
Resource::load_sprite_desc(const std::string& res_name)
{
  return resmgr.get_sprite_description(res_name);
}

CollisionMask
Resource::load_collision_mask(const std::string& name)
{
  return CollisionMask(name);
}

CollisionMask
Resource::load_collision_mask(const ResDescriptor& res_desc)
{
  return CollisionMask(res_desc);
}

Surface
Resource::load_surface(const ResDescriptor& desc_)
{
  SpriteDescription* desc = resmgr.get_sprite_description(desc_.res_name);
  if (desc)
    {
      if (desc_.modifier == ResourceModifierNS::ROT0)
        return Surface(desc->filename);
      else
        return Surface(desc->filename).mod(desc_.modifier);
    }
  else
    {
      return Surface(Pathname("images/core/misc/404.png", Pathname::DATA_PATH));
    }
}

Surface
Resource::load_surface(const std::string& res_name)
{
  return load_surface(ResDescriptor(res_name));
}

Font
Resource::load_font(const std::string& res_name)
{
  FontDescription desc(Pathname("images/" + res_name + ".font", Pathname::DATA_PATH));
  //("data/images/fonts/chalk_large-iso-8859-1.font");
  return Font(desc);
}

void
Resource::cleanup ()
{
#if 0
  CL_Resource res;
  std::vector<std::string> resources = resmgr.get_all_resources();
  for (std::vector<std::string>::iterator i = resources.begin(); i != resources.end(); i++)
    {
      res = resmgr.get_resource(*i);
      while (res.get_reference_count() > 0)
        res.unload();
    }
#endif
}

Sprite
Resource::load_thumb_sprite(const std::string& name)
{
  Sprite sprite = Sprite(name);

  Size thumb_size;
  if (sprite.get_width() <= 48)
    thumb_size.width = sprite.get_width();
  else
    thumb_size.width = 48;

  if (sprite.get_height() <= 48)
    thumb_size.height = sprite.get_height();
  else
    thumb_size.height = 48;

  sprite.scale(thumb_size.width, thumb_size.height);

  sprite.set_hotspot(origin_top_left, (48 - sprite.get_width())/2, (48 - sprite.get_height())/2);

  return sprite;
}

/* EOF */
