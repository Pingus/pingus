//  $Id: starfield_background.hxx,v 1.6 2002/10/01 19:53:46 grumbel Exp $
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

#ifndef HEADER_PINGUS_WORLDOJBS_STARFIELD_BACKGROUND_HXX
#define HEADER_PINGUS_WORLDOBJS_STARFIELD_BACKGROUND_HXX

#include <vector>
#include "../worldobj.hxx"

namespace WorldObjsData {
class StarfieldBackgroundData;
}

namespace WorldObjs {

class StarfieldBackgroundStars;

class StarfieldBackground : public WorldObj
{
private:
  WorldObjsData::StarfieldBackgroundData* const data;
  std::vector<StarfieldBackgroundStars*>        stars;

public:
  StarfieldBackground (const WorldObjsData::StarfieldBackgroundData& data_);
 ~StarfieldBackground ();

  // FIXME: Make z_pos handling editable via xml
  float get_z_pos() const { return -10; }

  void update ();
  void draw (GraphicContext& gc);
  
private:
  StarfieldBackground (const StarfieldBackground&);
  StarfieldBackground& operator= (const StarfieldBackground&);
};

} // namespace WorldObjs

#endif

/* EOF */
