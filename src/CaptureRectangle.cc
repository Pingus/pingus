//  $Id: CaptureRectangle.cc,v 1.4 2000/06/25 20:22:18 grumbel Exp $ 
//
//  Pingus - A free Lemmings clone
//  Copyright (C) 1999 Ingo Ruhnke <grumbel@gmx.de>
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
#include <cstdio>
#include <cassert>

#include "globals.hh"
#include "Pingu.hh"
#include "Pingu.hh"
#include "PingusResource.hh"
#include "CaptureRectangle.hh"

Pingu* CaptureRectangle::pingu; 
PinguAction* CaptureRectangle::button_action;

CaptureRectangle::CaptureRectangle()
{
  good = PingusResource::load_surface("Cursors/capgood", "game");
  bad  = PingusResource::load_surface("Cursors/capbad",  "game"); 
  font = PingusResource::load_font("Fonts/courier_small", "fonts");
  button_action = 0;

  arrow_left  = PingusResource::load_surface("Cursors/arrow_left",  "game"); 
  arrow_right = PingusResource::load_surface("Cursors/arrow_right", "game"); 
} 

CaptureRectangle::~CaptureRectangle()
{
  if (button_action) {
    delete button_action;
  }
}

void
CaptureRectangle::draw_offset(int x_offset, int y_offset, float s)
{ 
  if (!pingu) 
    {
      return;
    } 
  else 
    {
      CL_Surface* sur;
      
      if (button_action 
	  && (button_action->get_environment() & pingu->get_environment()))
	{
	  sur = good;
	} 
      else 
	{
	  sur = good;
	}
    
      if (s == 1.0) 
	{
	  sur->put_screen(pingu->get_x() + pingu->x_offset() + x_offset - 4,
			  pingu->get_y() + pingu->y_offset() + y_offset - 4);

	  if (pingu->get_action()) 
	    {
	      font->print_center(pingu->get_x() + pingu->x_offset() + x_offset + 16,
				 pingu->get_y() + pingu->y_offset() + y_offset - 16,
				 pingu->get_action()->name().c_str());
	    }

	  if (pingu->direction.is_left()) 
	    {
	      arrow_left->put_screen(pingu->get_x() + x_offset - 10,
				     pingu->get_y() + y_offset + 4);
	    }
	  else
	    {
	      arrow_right->put_screen(pingu->get_x() + x_offset - 10,
				      pingu->get_y() + y_offset + 4);
	    }
	} 
      else 
	{
	  sur->put_screen(int(pingu->get_x() + pingu->y_offset() + x_offset * s) - 4,
			  int(pingu->get_y() + pingu->y_offset() + y_offset * s) - 4,
			  s, s);
	}
    }
}

// Sets the current buttons action, it is used to change the color of
// the cap rect, if the action can't be applied.
void 
CaptureRectangle::set_action(PinguAction* action)
{
  if (button_action) 
    delete button_action;
  
  button_action = action;
}

void
CaptureRectangle::set_pingu(Pingu* p)
{
  pingu = p; 
}

/* EOF */

