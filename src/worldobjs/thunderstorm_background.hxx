//  $Id: thunderstorm_background.hxx,v 1.7 2003/04/19 10:23:19 torangan Exp $
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

#ifndef HEADER_PINGUS_WORLDOBJS_THUNDERSTORM_BACKGROUND_HXX
#define HEADER_PINGUS_WORLDOBJS_THUNDERSTORM_BACKGROUND_HXX

#include <ClanLib/Display/Display/surface.h>
#include "../worldobj.hxx"

namespace WorldObjsData {
class ThunderstormBackgroundData;
}

namespace WorldObjs {

class ThunderstormBackground : public WorldObj
{
private:
  WorldObjsData::ThunderstormBackgroundData* const data;
  CL_Surface clouds_sur;
  //std::vector<CL_Surface> flash_sur;
  int x_pos;

public:
  ThunderstormBackground (const WorldObjsData::ThunderstormBackgroundData& data_);
 ~ThunderstormBackground ();

  float get_z_pos () const;

  void update ();
  void draw (GraphicContext& gc);

private:
  ThunderstormBackground (const ThunderstormBackground&);
  ThunderstormBackground& operator= (const ThunderstormBackground&);
};

} // namespace WorldObjs

#endif

/* EOF */
