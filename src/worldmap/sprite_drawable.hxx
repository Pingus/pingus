//  $Id: sprite_drawable.hxx,v 1.2 2003/04/10 11:51:32 grumbel Exp $
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

#ifndef HEADER_SPRITE_DRAWABLE_HXX
#define HEADER_SPRITE_DRAWABLE_HXX

#include "../sprite.hxx"
#include "../vector.hxx"
#include "drawable.hxx"

namespace WorldMapNS {

/** */
class SpriteDrawable : public Drawable
{
private:
  Sprite sprite;
  Vector pos;

public:
  SpriteDrawable(xmlDocPtr doc, xmlNodePtr cur);
  ~SpriteDrawable();

  void draw(GraphicContext& gc);

  void update(float delta) 
  {
    sprite.update(delta);
  }

private:
  SpriteDrawable (const SpriteDrawable&);
  SpriteDrawable operator= (const SpriteDrawable&);
};

} // namespace WorldMapNS

#endif

/* EOF */
