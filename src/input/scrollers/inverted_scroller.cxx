//  $Id: inverted_scroller.cxx,v 1.1 2002/08/24 11:37:31 torangan Exp $
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

#include "inverted_scroller.hxx"

namespace Input {

  namespace Scrollers {

    InvertedScroller::InvertedScroller (Scroller* scroller_, bool invert_x_, bool invert_y_) 
                                      : scroller(scroller_),
				        invert_x(invert_x_), 
				        invert_y(invert_y_)
    {
    }
  
    InvertedScroller::~InvertedScroller ()
    {
      delete scroller;
    }
  
    const float&
    InvertedScroller::get_x_delta () const
    {
      return x_pos;
    }
  
    const float&
    InvertedScroller::get_y_delta () const
    {
      return y_pos;
    }
  
    void
    InvertedScroller::get_delta (float& x, float& y) const
    {
      x = x_pos;
      y = y_pos;
    }
  
    void
    InvertedScroller::update (float delta)
    {
      scroller->update(delta);
    
      (invert_x) ? x_pos = -(scroller->get_x_delta()) : x_pos = scroller->get_x_delta();
      (invert_y) ? y_pos = -(scroller->get_y_delta()) : x_pos = scroller->get_y_delta();
    }

  }
}

/* EOF */
