//  $Id: ThunderstormBackground.hh,v 1.13 2002/06/12 14:37:37 torangan Exp $
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

#ifndef THUNDERSTORMBACKGROUND_HH
#define THUNDERSTORMBACKGROUND_HH

#include <vector>
#include "../WorldObj.hh"
#include "ThunderstormBackgroundData.hh"

class ThunderstormBackground : public WorldObj,
			       public ThunderstormBackgroundData
{
private:
  //SurfaceBackground* background;
  CL_Surface clouds_sur;
  std::vector<CL_Surface> flash_sur;
  int x_pos;
public:
  ///
  ThunderstormBackground();
  ///
  virtual ~ThunderstormBackground();
  ///

  float get_z_pos() const { return pos.z; }
  
  void update(float delta);
  void draw_offset(int x_of, int y_of, float s = 1.0);
};

#endif

/* EOF */
