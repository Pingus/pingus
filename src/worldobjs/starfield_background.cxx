//  $Id: starfield_background.cxx,v 1.1 2002/09/16 20:52:22 torangan Exp $
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

#include "../worldobjsdata/starfield_background_data.hxx"
#include "starfield_background.hxx"
#include "starfield_background_stars.hxx"

namespace WorldObjs {

StarfieldBackground::StarfieldBackground (WorldObjsData::StarfieldBackgroundData* data_)
  : data(new WorldObjsData::StarfieldBackgroundData(*data_))
{

  int i;

  for (i=0; i < data->small_stars_count; ++i)
    stars.push_back(new StarfieldBackgroundStars(StarfieldBackgroundStars::SMALL_STAR));

  for (i=0; i < data->middle_stars_count; ++i)
    stars.push_back(new StarfieldBackgroundStars(StarfieldBackgroundStars::MIDDLE_STAR));

  for (i=0; i < data->large_stars_count; ++i)
    stars.push_back(new StarfieldBackgroundStars(StarfieldBackgroundStars::LARGE_STAR));
}

StarfieldBackground::~StarfieldBackground ()
{
  delete data;
  for (unsigned int i = 0; i < stars.size(); ++i)
    delete stars[i];
}


void 
StarfieldBackground::update (float delta)
{
  for (std::vector<StarfieldBackgroundStars*>::iterator i = stars.begin();
       i != stars.end(); ++i)
    {
      (*i)->update (delta);
    }
}

void 
StarfieldBackground::draw_offset (int x_of, int y_of, float s)
{
  for (std::vector<StarfieldBackgroundStars*>::iterator i = stars.begin();
       i != stars.end(); ++i)
    {
      (*i)->draw_offset(x_of, y_of);
    }

  UNUSED_ARG(s);
}

} // namespace WorldObjs

/* EOF */
