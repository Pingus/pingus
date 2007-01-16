//  $Id: pingus_map_manager.cxx,v 1.6 2003/10/21 11:01:52 grumbel Exp $
//
//  Pingus - A free Lemmings clone
//  Copyright (C) 2000 Ingo Ruhnke <grumbel@gmx.de>
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

#include <ClanLib/Display/surface.h>
#include <ClanLib/Display/pixel_buffer.h>
#include "pingus_map_manager.hxx"


std::string PingusMapManager::directory;

CL_Surface
PingusMapManager::get_surface (const std::string& level_filename)
{
  UNUSED_ARG(level_filename);
  return CL_Surface ();
}

void
PingusMapManager::set_surface (const std::string& level_filename, const CL_Surface& surf)
{
  PixelBuffer provider = surf.get_pixeldata();

  provider.lock ();

  //provider->get_data ();

  provider.unlock ();

  UNUSED_ARG(level_filename);
}


/* EOF */
