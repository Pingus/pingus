//  $Id: playfield_view.cxx,v 1.5 2003/04/19 10:23:17 torangan Exp $
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

#include "playfield_view.hxx"
#include "world.hxx"

PlayfieldView::PlayfieldView (World * w, const CL_Rect& rect)
  : gc (0, 0, 300, 300, 0, 0), // FIXME: broken
    world (w), x_offset (0), y_offset (0)
{
  // FIXME: Cliprects should be handled in the GraphicContext
  set_clip_rect(rect.x1, rect.y1, rect.x2, rect.y2);
}

PlayfieldView::~PlayfieldView ()
{
}

void
PlayfieldView::update (float /*delta*/)
{
}

void
PlayfieldView::draw ()
{
  world->draw(gc);
}

void
PlayfieldView::scroll (Vector delta)
{
  x_offset += (int) delta.x;
  y_offset += (int) delta.y;
}

Pingu*
PlayfieldView::get_pingu (const Vector& pos)
{
  return world->get_pingu (Vector(pos.x - x1 - x_offset,
				     pos.y - y1 - y_offset));
}

/* EOF */
