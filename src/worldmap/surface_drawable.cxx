//  $Id: surface_drawable.cxx,v 1.1 2002/10/12 23:37:23 grumbel Exp $
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

#include <iostream>

#include "../xml_helper.hxx"
#include "../pingus_resource.hxx"
#include "../graphic_context.hxx"
#include "surface_drawable.hxx"

namespace WorldMapNS {

SurfaceDrawable::SurfaceDrawable(xmlDocPtr doc, xmlNodePtr cur)
  : Drawable(doc, cur)
{
  cur = cur->children;
  while(cur)
    {
      if (XMLhelper::equal_str(cur->name, "surface"))
        {
          ResDescriptor desc = XMLhelper::parse_surface(doc, cur);
          surface = PingusResource::load_surface(desc);
        }
      else if (XMLhelper::equal_str(cur->name, "position"))
        {
          pos = XMLhelper::parse_vector(doc, cur);
        }
      else
        {
          std::cout << "Uknown: " << cur->name << std::endl;
        }

      cur = cur->next;
      cur = XMLhelper::skip_blank(cur);
    }
}

void
SurfaceDrawable::update()
{
  
}

void
SurfaceDrawable::draw(GraphicContext& gc)
{
  if (surface)
    {
      gc.draw(surface, pos);
    }
}

} // namespace WorldMapNS

/* EOF */
