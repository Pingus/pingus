//  $Id: Background.cc,v 1.3 2000/12/16 23:11:22 grumbel Exp $
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

#include "../PingusError.hh"
#include "ThunderstormBackground.hh"
#include "StarfieldBackground.hh"
#include "SurfaceBackground.hh"
#include "SolidColorBackground.hh"
#include "Background.hh"

Background::Background()
{
}

Background::~Background()
{
}

shared_ptr<Background>
Background::create (shared_ptr<BackgroundData> data)
{
  if (dynamic_cast<SurfaceBackgroundData*>(data.get()) != 0)
    return SurfaceBackground::create (data);
  /*if (dynamic_cast<ThunderstormBackgroundData*>(data) != 0)
    return SurfaceBackground::create (data);*/
  else if (dynamic_cast<ThunderstormBackgroundData*>(data.get()) != 0)
    return ThunderstormBackground::create (data);
  else if (dynamic_cast<StarfieldBackgroundData*>(data.get()) != 0)
    return StarfieldBackground::create (data);
  else if (dynamic_cast<SolidColorBackgroundData*>(data.get()) != 0)
    return SolidColorBackground::create (data);
  else 
    {
      throw PingusError ("Background::create (): Unknown BackgroundData");
    }
}

/* EOF */
