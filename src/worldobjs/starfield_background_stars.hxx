//  $Id: starfield_background_stars.hxx,v 1.5 2003/04/19 10:23:19 torangan Exp $
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

#ifndef HEADER_PINGUS_WORLDOBJS_STARFIELD_BACKGROUND_STARS_HXX
#define HEADER_PINGUS_WORLDOBJS_STARFIELD_BACKGROUND_STARS_HXX

#include <ClanLib/Display/Display/surface.h>
#include "../worldobj.hxx"

namespace WorldObjs {

class StarfieldBackgroundStars : public WorldObj
{
private:
  static CL_Surface small_star;
  static CL_Surface middle_star;
  static CL_Surface large_star;
  static bool       is_init;

  CL_Surface sur;

  float x_pos;
  float y_pos;

  float x_add;
  float y_add;

public:
  enum Type { SMALL_STAR, MIDDLE_STAR, LARGE_STAR };

  StarfieldBackgroundStars () {}
  StarfieldBackgroundStars (Type type);

  // Never used
  float get_z_pos () const { return -100; }

  void init ();
  void update ();
  void draw (GraphicContext& gc);

private:
  StarfieldBackgroundStars (const StarfieldBackgroundStars&);
  StarfieldBackgroundStars& operator= (const StarfieldBackgroundStars&);
};

} // namespace WorldObjs

#endif

/* EOF */
