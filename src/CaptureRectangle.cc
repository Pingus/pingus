//  $Id: CaptureRectangle.cc,v 1.21 2002/06/09 11:18:50 grumbel Exp $ 
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
#include "StringConverter.hh"
#include "PingusResource.hh"
#include "CaptureRectangle.hh"

using namespace boost;

CaptureRectangle::CaptureRectangle()
  : pingu (0),
    button_action (0),
    owner_id (0),
    good (PingusResource::load_surface("Cursors/capgood", "game")),
    bad (PingusResource::load_surface("Cursors/capbad",  "game")),
    arrow_left (PingusResource::load_surface("Cursors/arrow_left",  "game")),
    arrow_right (PingusResource::load_surface("Cursors/arrow_right", "game")),
    font (PingusResource::load_font("Fonts/courier_small", "fonts"))
{
  good.set_align_center ();
  bad.set_align_center ();
  arrow_left.set_align_center ();
  arrow_right.set_align_center ();
} 

CaptureRectangle::~CaptureRectangle()
{
}

void
CaptureRectangle::draw_offset(int x_offset, int y_offset, float s)
{ 
  if (pingu && pingu->catchable ())
    {
      Sprite * sur;
      
      if (button_action && (button_action->get_environment() & pingu->get_environment()))
	sur = &bad;
      else 
	sur = &good;
      
      if (s == 1.0) 
	{
	  std::string action_str = pingu->get_action()->get_name();

	  std::vector<PinguAction*>* persitant = pingu->get_persistent_actions ();
	  if (persitant->size() > 0)
	    {
	      action_str += " [";
	      for (std::vector<PinguAction*>::iterator i = persitant->begin ();
		   i != persitant->end (); ++i)
		{
		  action_str += (*i)->get_persistent_char ();
		}
	      action_str += "]";
	    }

	  // Draw the caputure rectangle
	  sur->put_screen(pingu->get_center_pos() + CL_Vector(x_offset,y_offset));
	  
	  // If pingu has an action, print its name
	  if (pingu->get_action())
	    {
	      font->print_center(int(pingu->get_center_pos().x) + x_offset,
				 int(pingu->get_center_pos().y) + y_offset - 32,
				 action_str.c_str());
	    }
	  /*font->print_center(pingu->get_center_pos().x + x_offset,
			     pingu->get_center_pos().y + y_offset - 16 + 62,
			     to_string(pingu->get_owner()).c_str());*/
	  

	  // Paint the direction arrow
	  if (pingu->direction.is_left()) 
	    {
	      arrow_left.put_screen(pingu->get_center_pos() + CL_Vector (x_offset, y_offset + 28));
	    }
	  else
	    {
	      arrow_right.put_screen(pingu->get_center_pos() + CL_Vector (x_offset, y_offset + 28));
	    }
	} 
      else 
	{
	  sur->put_screen(pingu->get_center_pos() + CL_Vector (x_offset, y_offset));
	}
    }
}

// Sets the current buttons action, it is used to change the color of
// the cap rect, if the action can't be applied.
void 
CaptureRectangle::set_action(PinguAction* action)
{
  button_action = action;
}

void
CaptureRectangle::set_pingu(Pingu* p)
{
  pingu = p; 
}

/* EOF */

