//  $Id: PLFPreview.cc,v 1.2 2000/02/09 21:43:40 grumbel Exp $
//
//  Pingus - A free Lemmings clone
//  Copyright (C) 1999 Ingo Ruhnke <grumbel@gmx.de>
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

#include "PingusSpotMap.hh"
#include "PinguBmpMap.hh"
#include "PingusResource.hh"

#include "PLFPreview.hh"

PLFPreview::PLFPreview(PLF* p)
{
  plf = p;
  surface = 0;
}

CL_Surface*
PLFPreview::get_surface()
{
  return surface;
}

void
PLFPreview::load(PLF* p)
{
  if (surface)
    return;

  switch (plf->map_type()) {
  case SPOT: 
    {
      PingusSpotMap p_map;
      p_map.load(plf);
      surface = p_map.get_surface();
    }
    break;
  case BMP: 
    {
      PinguBmpMap map(plf->get_fg());
      surface = map.get_surface();
    }
    break;
  default:
    cout << "PLFPreview::load not implemented" << endl;
    surface = CL_Surface::load("Game/logo", PingusResource::get("global.dat"));
    break;
  }
}

/* EOF */
