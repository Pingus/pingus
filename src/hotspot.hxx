//  $Id: hotspot.hxx,v 1.5 2002/09/17 21:45:56 grumbel Exp $
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

#ifndef HEADER_PINGUS_HOTSPOT_HXX
#define HEADER_PINGUS_HOTSPOT_HXX

#include <ClanLib/Display/Display/surface.h>

#include "worldobj.hxx"
#include "anim_counter.hxx"
#include "hotspot_data.hxx"
#include "sprite.hxx"

class Hotspot : public HotspotData, 
		public WorldObj
{
private:
  Sprite sprite;
  float para;

public:
  Hotspot(const HotspotData& spot);
  
  void  draw (GraphicContext& gc);
  float get_z_pos() const { return (int) pos.z; }
  
private:
  Hotspot (const Hotspot&);
  Hotspot operator= (const Hotspot&);
};

#endif

/* EOF */

