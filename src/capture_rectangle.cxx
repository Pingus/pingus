//  $Id: capture_rectangle.cxx,v 1.11 2002/10/04 13:46:56 grumbel Exp $ 
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

#include "vector.hxx"
#include <ClanLib/Display/Font/font.h>
#include "pingu.hxx"
#include "pingus_resource.hxx"
#include "capture_rectangle.hxx"
#include "pingu_action.hxx"
#include "button_panel.hxx"
#include "globals.hxx"
#include "string_converter.hxx"

CaptureRectangle::CaptureRectangle (ButtonPanel* arg_button_panel)
  : pingu(0),
    owner_id(0),
    good(PingusResource::load_surface("Cursors/capgood", "game")),
    bad(PingusResource::load_surface("Cursors/capbad",  "game")),
    arrow_left(PingusResource::load_surface("Cursors/arrow_left",  "game")),
    arrow_right(PingusResource::load_surface("Cursors/arrow_right", "game")),
    button_panel(arg_button_panel),
    font(PingusResource::load_font("Fonts/courier_small", "fonts"))
{
  good.set_align_center();
  bad.set_align_center();
  arrow_left.set_align_center();
  arrow_right.set_align_center();
}

CaptureRectangle::~CaptureRectangle ()
{
}

void
CaptureRectangle::draw_offset (int x_offset, int y_offset, float s)
{ 
  if (pingu && pingu->catchable())
    {
      Sprite* sur;
      
      // FIXME: A check for surface good/bad should  be placed here
      if (pingu->change_allowed(button_panel->get_action_name()))
	sur = &good;
      else
	sur = &bad;
      
      if (s == 1.0) 
	{ // FIXME: this should use GC

	  // Draw the caputure rectangle
	  sur->put_screen(pingu->get_center_pos() + Vector(x_offset,y_offset));
	  
	  font->print_center(static_cast<int>(pingu->get_center_pos().x) + x_offset,
			     static_cast<int>(pingu->get_center_pos().y) + y_offset - 32,
			     action_str.c_str());
	  /*font->print_center(pingu->get_center_pos().x + x_offset,
			     pingu->get_center_pos().y + y_offset - 16 + 62,
			     to_string(pingu->get_owner()).c_str());*/
	  

	  // Paint the direction arrow
	  if (pingu->direction.is_left()) 
	    {
	      arrow_left.put_screen(pingu->get_center_pos() + Vector(x_offset, y_offset + 28));
	    }
	  else
	    {
	      arrow_right.put_screen(pingu->get_center_pos() + Vector(x_offset, y_offset + 28));
	    }
	} 
      else 
	{
	  sur->put_screen(pingu->get_center_pos() + Vector(x_offset, y_offset));
	}
    }
}

void
CaptureRectangle::set_pingu (Pingu* p)
{
  pingu = p;

  if (pingu)
    {
      action_str = pingu->get_action()->get_name();
  
      if (pingu->get_wall_action() || pingu->get_fall_action())
        {
          action_str += "[";

          if (pingu->get_wall_action())
            action_str += pingu->get_wall_action()->get_persistent_char();

          if (pingu->get_fall_action())
            action_str += pingu->get_fall_action()->get_persistent_char();

          action_str += "]";
        }

      if (maintainer_mode)
        {
          action_str += " Id: ";
          action_str += to_string(pingu->get_id());
        }
    }
}

/* EOF */
