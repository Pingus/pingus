//  $Id: dot.hxx,v 1.8 2003/10/19 12:25:47 grumbel Exp $
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

#ifndef HEADER_PINGUS_DOT_HXX
#define HEADER_PINGUS_DOT_HXX

#include <string>
#include "../math/vector3f.hpp"
#include "drawable.hpp"

namespace WorldMapNS {

/** A Dot is a node between all the pathes on the worldmap, there are
    LevelDots TubeDots and other availabe. */
class Dot : public Drawable
{
protected:
  Vector3f pos;

public:
  Dot(FileReader reader);

  /** Draw stuff that should be displayed if the mouse is over the dot */
  virtual void draw_hover(DrawingContext& gc) =0;

  Vector3f get_pos() { return pos; }

  virtual void on_click() =0;

  virtual bool finished() =0;
  virtual bool accessible() =0;
  /** makes the node accessible */
  virtual void unlock() =0;
private:
  Dot (const Dot&);
  Dot& operator= (const Dot&);
};

} // namespace WorldMapNS

#endif

/* EOF */
