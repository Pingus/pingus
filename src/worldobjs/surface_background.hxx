//  $Id: surface_background.hxx,v 1.2 2002/09/27 11:26:49 torangan Exp $
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

#ifndef HEADER_PINGUS_WORLDOBJS_SURFACE_BACKGROUND_HXX
#define HEADER_PINGUS_WORLDOBJS_SURFACE_BACKGROUND_HXX

#include "../worldobj.hxx"
#include "../game_counter.hxx"

namespace WorldObjsData {
class SurfaceBackgroundData;
}

namespace WorldObjs {

class SurfaceBackground : public WorldObj
{
private:
  GameCounter counter;

  CL_Surface bg_surface;
  
  /** The horizontal scrolling speed in pixels per tick */
  float scroll_ox;

  /** The vertical scrolling speed in pixels per tick */
  float scroll_oy;

  WorldObjsData::SurfaceBackgroundData* const data;
  
public:
  SurfaceBackground (WorldObjsData::SurfaceBackgroundData* data_);
 ~SurfaceBackground ();

  float get_z_pos () const;

  void update (float delta);
  void draw (GraphicContext& gc);
  
private:
  SurfaceBackground (const SurfaceBackground&);
  SurfaceBackground& operator= (const SurfaceBackground&);
};

} // namespace WorldObjs

#endif

/* EOF */
