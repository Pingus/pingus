//  $Id: surface_background.hxx,v 1.8 2003/10/20 13:11:09 grumbel Exp $
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

#include <ClanLib/Display/sprite.h>
#include "../res_descriptor.hxx"
#include "../game_counter.hxx"
#include "../worldobj.hxx"

namespace Pingus {

namespace WorldObjsData {
class SurfaceBackgroundData;
}

namespace WorldObjs {

class SurfaceBackground : public WorldObj
{
private:
  float para_x;
  float para_y;

  /** Position of the background, only the z_pos is really used. */
  Vector pos;

  /** The amount of pixel the background is scrolled each frame in x
      direction. */
  float scroll_x;

  /** The amount of pixel the background is scrolled each frame in x
      direction. */
  float scroll_y;

  /** fill_rect() components
      An fill_rect() can be drawn over the background, the following
      for components are passed to the fill_rect() call. */
  CL_Colorf color;

  /** Stretch the background to the full screen size in x direction */
  bool stretch_x;

  /** Stretch the background to the full screen size in x direction */
  bool stretch_y;

  /** If streched in x or y direction keep the aspect ratio */
  bool keep_aspect;


  GameCounter counter;

  CL_Surface bg_surface;

  /** The horizontal scrolling speed in pixels per tick */
  float scroll_ox;

  /** The vertical scrolling speed in pixels per tick */
  float scroll_oy;

public:
  SurfaceBackground(const FileReader& reader);

  float get_z_pos () const;

  void update ();
  void draw (SceneContext& gc);

private:
  SurfaceBackground (const SurfaceBackground&);
  SurfaceBackground& operator= (const SurfaceBackground&);
};

} // namespace WorldObjs
} // namespace Pingus

#endif

/* EOF */
