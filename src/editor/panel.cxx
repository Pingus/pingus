//  $Id: panel.cxx,v 1.2 2002/06/29 09:44:56 grumbel Exp $
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

#include <iostream>
#include <ClanLib/Core/System/system.h>
#include <ClanLib/Display/Input/mouse.h>
#include <ClanLib/Display/Display/display.h>
#include <ClanLib/Display/Font/font.h>
#include "../globals.hxx"
#include "../pingus_resource.hxx"
#include "panel_icons.hxx"

Editor* PanelIcon::editor;

PanelIcon::PanelIcon()
{
  button = PingusResource::load_surface("editor/button","core");
  button_pressed = PingusResource::load_surface("editor/button_pressed", "core");
  font = PingusResource::load_font("Fonts/courier_small", "fonts");
}

PanelIcon::~PanelIcon()
{
}

void
PanelIcon::start()
{
}

void 
PanelIcon::put_screen(int /*x*/, int y)
{
  if (CL_Mouse::get_x() < 25 && CL_Mouse::get_y() > y && CL_Mouse::get_y() < y + 25)
    {
      if (CL_Mouse::left_pressed())
	{
	  button_pressed.put_screen(0, y);
	}
      else
	{
	  button.put_screen(0, y);
	}
      if (mouse_over_time == 0)
	mouse_over_time = CL_System::get_time ();
      
      if (CL_System::get_time () - mouse_over_time > 5) {
	int width = font->get_text_width (tooltip.c_str ());
	CL_Display::fill_rect (28, y + 6 - 2, 32 + width, y + font->get_height () + 6 + 2,
			       0.4f, 0.4f, 0.0f);
	font->print_left (30, y + 6, tooltip.c_str ());
      }
    }
  else
    {
      mouse_over_time = 0;
    }
  sur.put_screen(0, y);
}

void
PanelIcon:: on_click()
{
  std::cout << "PanelIcon: No event bound to this button?!" << std::endl;
}

Panel::Panel()
{
  is_init = false;
  logo = PingusResource::load_surface ("editor/logo", "core");
}

Panel::~Panel()
{
  
}

void
Panel::draw()
{
  int y = 0;

  CL_Display::fill_rect(0, 0, 25, CL_Display::get_height(),
			0.75, 0.75, 0.75, 1.0);
 
  logo.put_screen(0, CL_Display::get_height() - logo.get_height());

  for (std::vector<PanelIcon*>::iterator i = buttons.begin(); i != buttons.end(); i++)
    {
      (*i)->put_screen(0, y);
      y += 25;
    }
}

void 
Panel::init()
{
  if (!is_init)
    {
      buttons.push_back(new PanelIconNew);
      buttons.push_back(new PanelIconLoad);
      buttons.push_back(new PanelIconSave);

      buttons.push_back(new PanelIconInsertObject);
      
      buttons.push_back(new PanelIconCopy);
      buttons.push_back(new PanelIconEdit);
      buttons.push_back(new PanelIconDelete);

      buttons.push_back(new PanelIconRegionZoom ());
      buttons.push_back(new PanelIconZoomIn ());
      buttons.push_back(new PanelIconZoomOut ());

      buttons.push_back(new PanelIconSetupActions());

      buttons.push_back(new PanelIconRun);

      buttons.push_back(new PanelIconExit);

      is_init = true;
    }
}

void
Panel::on_click()
{
  if (CL_Mouse::get_x() <= 25)
    {
      int y = 0;
      // std::cout << "Click" << std::endl;

      for (std::vector<PanelIcon*>::iterator i = buttons.begin(); i != buttons.end(); i++)
	{
	  if (CL_Mouse::get_y() > y && CL_Mouse::get_y() < y + 25)
	    {
	      pressed_button = i;
	      break;
	    }
	  y += 25;
	}
    }
}

void
Panel::on_release()
{
  if (CL_Mouse::get_x() <= 25)
    {
      int y = 0;
      
      for (std::vector<PanelIcon*>::iterator i = buttons.begin(); i != buttons.end(); i++)
	{
	  if (CL_Mouse::get_y() > y && CL_Mouse::get_y() < y + 25)
	    {
	      if (pressed_button == i)
		{
		  (*i)->on_click();
		}
	    }
	  y += 25;
	}
    }
  pressed_button = std::vector<PanelIcon*>::iterator();
  if (verbose) std::cout << "On release finished" << std::endl;
}

bool
Panel::mouse_over(int x, int /*y*/)
{
  return (x <= 25);
}

void
Panel::set_editor(Editor* e)
{
  editor = e;
  PanelIcon::editor = e;
}

/* EOF */
