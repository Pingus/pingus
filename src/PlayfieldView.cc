//  $Id: PlayfieldView.cc,v 1.1 2001/04/13 22:17:46 grumbel Exp $
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

#include "PlayfieldView.hh"

PlayfieldView::PlayfieldView (boost::dummy_ptr<World> w,
			      int x1, int y1, int x2, int y2)
  : world (w), x_offset (-320), y_offset (0)
{
  this->x1 = x1;
  this->y1 = y1;
  this->x2 = x2;
  this->y2 = y2;
  set_clip_rect(x1, y1, x2, y2);
}

PlayfieldView::~PlayfieldView ()
{
}

void 
PlayfieldView::update (float delta)
{
}

void 
PlayfieldView::draw ()
{
  world->draw(x1, y1,
	      x2 - x1, y2 - y1,
	      x_offset, y_offset, 1.0f);
}

/* EOF */
