//  $Id: ThunderstormBackground.cc,v 1.4 2000/12/16 23:11:22 grumbel Exp $
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

#include "../PingusResource.hh"
#include "ThunderstormBackground.hh"

ThunderstormBackground::ThunderstormBackground()
{
  clouds_sur = PingusResource::load_surface("thunderstorm", "thunderstorm");
  // flash_sur.push_back(PingusResource::load_surface("flash1", "thunderstorm"));
  // flash_sur.push_back(PingusResource::load_surface("flash2", "thunderstorm"));
  x_pos = 0;
}

ThunderstormBackground::~ThunderstormBackground()
{
  
}

boost::shared_ptr<ThunderstormBackground>
ThunderstormBackground::create (boost::shared_ptr<BackgroundData>)
{
  return boost::shared_ptr<ThunderstormBackground>(new ThunderstormBackground());
}

void
ThunderstormBackground::let_move(void)
{
  x_pos += 1;
  if (x_pos >= (int) clouds_sur.get_width())
    x_pos = 0;
}

void
ThunderstormBackground::draw_offset(int x_of, int y_of, float s = 1.0)
{
  clouds_sur.put_screen(x_pos + x_of, y_of);
}

/* EOF */
