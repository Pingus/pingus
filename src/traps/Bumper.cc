//  $Id: Bumper.cc,v 1.1 2000/05/12 13:29:29 grumbel Exp $
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

#include "../ActionHolder.hh"
#include "../PingusResource.hh"
#include "../PingusSound.hh"
#include "../algo.hh"

#include "Bumper.hh"

Bumper::Bumper(trap_data data)
{
  x_pos = data.x_pos;
  y_pos = data.y_pos;
  z_pos = data.z_pos;
  surface = CL_Surface::load("Traps/bumper", PingusResource::get("traps.dat"));
  count = 0;
}

Bumper::~Bumper()
{
}

void
Bumper::let_move()
{
  if (upwards) 
    {
      ++count;
      if (count >= surface->get_num_frames())
	{
	  count = 0;
	  upwards = false;
	}
    }
}

void
Bumper::draw_colmap(ColMap* colmap)
{
  std::cout << "Drawing colmap entry" << std::endl;

  CL_SurfaceProvider* prov = CL_SurfaceProvider::load("Traps/bumper_cmap", PingusResource::get("traps.dat"));
  colmap->put(prov, x_pos, y_pos, surface_data::SOLID);
}

void 
Bumper::draw_offset(int x, int y, float s)
{
  assert(surface);
  surface->put_screen(x_pos + x, y_pos + y, count);
}

bool 
Bumper::catch_pingu(Pingu* pingu)
{
  if (!pingu->is_alive())
    return false;

  if (pingu->get_y() > y_pos + 60 && pingu->get_y() < y_pos + 100)
    {
      if (pingu->get_x() > x_pos + 28 && pingu->get_x() < x_pos + 32)
	{
	  if (!upwards)
	    upwards = true;
	}

      if (upwards && pingu->get_x() > x_pos + 0 && pingu->get_x() < x_pos + 60)
	{
	  pingu->apply_force(CL_Vector((pingu->get_x() - 30)/6, -5));
	}
    }
  return false;
}

/* EOF */
