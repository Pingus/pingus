//  $Id: Panel.cc,v 1.2 2000/02/09 21:43:43 grumbel Exp $
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

#include "../PingusResource.hh"
#include "PanelIcons.hh"
#include "Panel.hh"

EditorEvent* PanelIcon::event;

PanelIcon::PanelIcon()
{
  sur = 0;
  button = CL_Surface::load("Icons/button", PingusResource::get("editor.dat"));
  button_pressed = CL_Surface::load("Icons/button_pressed", PingusResource::get("editor.dat"));
}

PanelIcon::~PanelIcon()
{
}

void
PanelIcon::start()
{
}

void 
PanelIcon::put_screen(int x, int y)
{
  if (CL_Mouse::get_x() < 25 && CL_Mouse::get_y() > y && CL_Mouse::get_y() < y + 25)
    {
      if (CL_Mouse::left_pressed())
	{
	  button_pressed->put_screen(0, y);
	}
      else
	{
	  button->put_screen(0, y);
	}
    }
  sur->put_screen(0, y);
}

void
PanelIcon:: on_click()
{
  cout << "PanelIcon: No event bound to this button?!" << endl;
}

Panel::Panel()
{
  is_init = false;
  logo = CL_Surface::load("Icons/logo", PingusResource::get("editor.dat"));
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
 
  logo->put_screen(0, CL_Display::get_height() - logo->get_height());

  for (vector<PanelIcon*>::iterator i = buttons.begin(); i != buttons.end(); i++)
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
      // cout << "Click" << endl;

      for (vector<PanelIcon*>::iterator i = buttons.begin(); i != buttons.end(); i++)
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
      // cout << "Click" << endl;
      
      for (vector<PanelIcon*>::iterator i = buttons.begin(); i != buttons.end(); i++)
	{
	  if (CL_Mouse::get_y() > y && CL_Mouse::get_y() < y + 25)
	    {
	      cout << "Recive: " << i << endl;
	      if (pressed_button == i)
		{
		  cout << "Doing button..." << endl;
		  (*i)->on_click();
		  cout << "done" << endl;
		}
	    }
	  y += 25;
	}
    }
  pressed_button = 0;
  cout << "On release finished" << endl;
}

bool
Panel::mouse_over(int x, int y)
{
  return (x <= 25);
}

void
Panel::set_event(EditorEvent* e)
{
  event = e;
  PanelIcon::event = event;
}

/* EOF */
