//  $Id: CaptureRectangle.cc,v 1.8 2001/04/12 20:52:40 grumbel Exp $ 
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
#include <boost/smart_ptr.hpp>

#include "globals.hh"
#include "Pingu.hh"
#include "Pingu.hh"
#include "StringConverter.hh"
#include "PinguAction.hh"
#include "PingusResource.hh"
#include "CaptureRectangle.hh"

using namespace boost;

shared_ptr<Pingu> CaptureRectangle::pingu; 
shared_ptr<PinguAction> CaptureRectangle::button_action;

CaptureRectangle::CaptureRectangle()
  : owner_id (0)
{
  good = PingusResource::load_surface("Cursors/capgood", "game");
  bad  = PingusResource::load_surface("Cursors/capbad",  "game"); 
  font = PingusResource::load_font("Fonts/courier_small", "fonts");

  arrow_left  = PingusResource::load_surface("Cursors/arrow_left",  "game"); 
  arrow_right = PingusResource::load_surface("Cursors/arrow_right", "game"); 
} 

CaptureRectangle::~CaptureRectangle()
{
}

void
CaptureRectangle::draw_offset(int x_offset, int y_offset, float s)
{ 
  if (!pingu.get()) 
    {
      return;
    } 
  else 
    {
      CL_Surface sur;
      
      if (button_action.get() 
	  && (button_action->get_environment() & pingu->get_environment()))
	{
	  sur = bad;
	} 
      else 
	{
	  sur = good;
	}
    
      if (s == 1.0) 
	{
	  // Draw the caputure rectangle
	  sur.put_screen(pingu->get_x() + pingu->x_offset() + x_offset - 4,
			 pingu->get_y() + pingu->y_offset() + y_offset - 4);

	  // If pingu has an action, print its name
	  if (pingu->get_action().get()) 
	    {
	      font->print_center(pingu->get_x() + pingu->x_offset() + x_offset + 16,
				 pingu->get_y() + pingu->y_offset() + y_offset - 16,
				 pingu->get_action()->name().c_str());
	    }

	  font->print_center(pingu->get_x() + pingu->x_offset() + x_offset + 16,
			     pingu->get_y() + pingu->y_offset() + y_offset - 16 + 62,
			     StringConverter::to_string(pingu->get_owner()).c_str());
	  

	  // Paint the direction arrow
	  if (pingu->direction.is_left()) 
	    {
	      arrow_left.put_screen(pingu->get_x() + x_offset - 10,
				     pingu->get_y() + y_offset + 4);
	    }
	  else
	    {
	      arrow_right.put_screen(pingu->get_x() + x_offset - 10,
				     pingu->get_y() + y_offset + 4);
	    }
	} 
      else 
	{
	  sur.put_screen(int(pingu->get_x() + pingu->y_offset() + x_offset * s) - 4,
			  int(pingu->get_y() + pingu->y_offset() + y_offset * s) - 4,
			  s, s);
	}
    }
}

// Sets the current buttons action, it is used to change the color of
// the cap rect, if the action can't be applied.
void 
CaptureRectangle::set_action(shared_ptr<PinguAction> action)
{
  button_action = action;
}

void
CaptureRectangle::set_pingu(shared_ptr<Pingu> p)
{
  pingu = p; 
}

/* EOF */

