// $Id: RectEditorObj.cc,v 1.3 2002/06/08 23:11:08 torangan Exp $
//
// Pingus - A free Lemmings clone
// Copyright (C) 1999 Ingo Ruhnke <grumbel@gmx.de>
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

#include "../my_gettext.hh"
#include "../boost/smart_ptr.hpp"
#include "EditorView.hh"
#include "RectEditorObj.hh"

RectEditorObj::RectEditorObj()
{
}

RectEditorObj::~RectEditorObj()
{
}

void
RectEditorObj::draw_mark (EditorView * view) 
{
  CL_Vector ul (get_upper_left_corner ());

  view->draw_rect(int(ul.x), 
		  int(ul.y),
		  int(ul.x + get_width()),
		  int(ul.y + get_height()),
		  1.0, 0.0, 1.0, 1.0);
}

bool
RectEditorObj::is_over(const CL_Vector& pos)
{
  CL_Vector ul (get_upper_left_corner ());

  return  (ul.x < pos.x
	   && ul.x + get_width () > pos.x
	   && ul.y < pos.y
	   && ul.y + get_height () > pos.y);
}

bool
RectEditorObj::is_in_rect(const CL_Rect& rect)
{
  CL_Vector pos (get_upper_left_corner ());
  
  // FIXME: Simple, stupid and wrong,... but works good =;-)
  return (pos.x >= rect.x1 && pos.x < rect.x2
	  && pos.y >= rect.y1 && pos.y < rect.y2);
}

/* EOF */
