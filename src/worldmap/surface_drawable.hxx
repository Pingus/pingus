//  $Id: surface_drawable.hxx,v 1.4 2003/04/10 11:51:32 grumbel Exp $
// 
//  Pingus - A free Lemmings clone
//  Copyright (C) 2002 Ingo Ruhnke <grumbel@gmx.de>
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

#ifndef HEADER_SURFACE_DRAWABLE_HXX
#define HEADER_SURFACE_DRAWABLE_HXX

#include <ClanLib/Display/Display/surface.h>
#include "../vector.hxx"
#include "drawable.hxx"

namespace WorldMapNS {

/** */
class SurfaceDrawable : public Drawable
{
private:
  CL_Surface surface;
  Vector     pos;
  /** If set to true the surface will disappear if the pingu gets
      covered by it, so that the Pingu can travel into caves and other
      things, while the roof will disapear */
  bool       auto_uncover;
public:
  SurfaceDrawable(xmlDocPtr doc, xmlNodePtr cur);

  void update(float delta);
  void draw(GraphicContext&);

  float get_z_pos() const { return pos.z; }
private:
  SurfaceDrawable (const SurfaceDrawable&);
  SurfaceDrawable& operator= (const SurfaceDrawable&);
};

} // namespace WorldMapNS

#endif

/* EOF */
