//  $Id: drawable.hxx,v 1.9 2003/10/19 12:25:47 grumbel Exp $
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

#ifndef HEADER_PINGUS_DRAWABLE_HXX
#define HEADER_PINGUS_DRAWABLE_HXX

#include "../pingus_error.hxx"
#include "../xml_helper.hxx"

namespace Pingus {

class DrawingContext;

namespace WorldMapNS {

/** An drawable object on the worldmap, such has a background or a
    sprite, interface only */
class Drawable
{
protected:
  /** The symbolic id of the drawable */
  std::string name;

  /** True if object should be visible, false otherwise */
  bool visible;

public:
  Drawable()
    :visible(true)
  {
  }

  Drawable(std::string arg_name)
    : name(arg_name),
      visible(true)
  {
  }

  Drawable(xmlDocPtr doc, xmlNodePtr cur)
    : visible(true)
  {
    if (!XMLhelper::get_prop (cur, "name", name))
      {
        PingusError::raise("Drawable: Couldn't get name of object");
      }
    UNUSED_ARG(doc);
  }

  virtual ~Drawable() {}

  bool is_visible() { return visible; }
  std::string get_name() { return name; }

  virtual void draw(DrawingContext& gc) =0;
  virtual void update(float delta) =0;

  virtual float get_z_pos() const =0;

private:
  Drawable (const Drawable&);
  Drawable& operator= (const Drawable&);
};

} // namespace WorldMapNS
} // namespace Pingus

#endif

/* EOF */
