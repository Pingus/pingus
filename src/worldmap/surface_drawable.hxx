//  $Id: surface_drawable.hxx,v 1.1 2002/10/12 23:37:23 grumbel Exp $
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
public:
  SurfaceDrawable(xmlDocPtr doc, xmlNodePtr cur);

  void update();
  void draw(GraphicContext&);

private:
  SurfaceDrawable (const SurfaceDrawable&);
  SurfaceDrawable& operator= (const SurfaceDrawable&);
};

} // namespace WorldMapNS

#endif

/* EOF */
