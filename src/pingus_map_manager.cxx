//  $Id: pingus_map_manager.cxx,v 1.1 2002/06/12 19:06:12 grumbel Exp $
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

#include <ClanLib/Display/Display/surface.h>
#include <ClanLib/Display/Display/surfaceprovider.h>
#include "pingus_map_manager.hxx"

std::string PingusMapManager::directory;

CL_Surface 
PingusMapManager::get_surface (std::string /*level_filename*/)
{
  return CL_Surface ();
}

void 
PingusMapManager::set_surface (std::string /*level_filename*/, const CL_Surface& surf)
{
  CL_SurfaceProvider* provider = surf.get_provider ();

  assert (provider);

  provider->lock ();

  //provider->get_data ();
  
  provider->unlock ();
}

/* EOF */
