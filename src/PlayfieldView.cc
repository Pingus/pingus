//  $Id: PlayfieldView.cc,v 1.5 2002/06/01 18:05:35 torangan Exp $
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

#include <ClanLib/display.h>
#include "PlayfieldView.hh"

PlayfieldView::PlayfieldView (boost::dummy_ptr<World> w,
			      const CL_Rect& rect)
  : world (w), x_offset (0), y_offset (0)
{
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
  //std::cout << "Offset: " << x_offset << " " << y_offset << std::endl;
  world->draw(//x1, y1,
	      //x2 - x1, y2 - y1,
	      0, 0, CL_Display::get_width (), CL_Display::get_height (),
	      x_offset + x1, y_offset + y1, 1.0f);
}

void 
PlayfieldView::scroll (CL_Vector delta)
{
  x_offset += (int) delta.x;
  y_offset += (int) delta.y;
}

boost::shared_ptr<Pingu>
PlayfieldView::get_pingu (const CL_Vector& pos)
{
  return world->get_pingu (CL_Vector(pos.x - x1 - x_offset,
				     pos.y - y1 - y_offset));
}

/* EOF */
