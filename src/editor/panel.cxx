//  $Id: panel.cxx,v 1.9 2003/10/18 23:17:27 grumbel Exp $
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
#include <ClanLib/Display/mouse.h>
#include <ClanLib/Display/display.h>
#include <ClanLib/Display/font.h>
#include "../globals.hxx"
#include "../pingus_resource.hxx"
#include "../fonts.hxx"
#include "panel_icons.hxx"

namespace Pingus {
namespace EditorNS {

Editor* PanelIcon::editor;

PanelIcon::PanelIcon()
{
  button = PingusResource::load_surface("editor/button","core");
  button_pressed = PingusResource::load_surface("editor/button_pressed", "core");
  font = Fonts::courier_small;
}

PanelIcon::~PanelIcon()
{
}

void
PanelIcon::start()
{
}

void
PanelIcon::draw(int /*x*/, int y)
{
  if (CL_Mouse::get_x() < 25 && CL_Mouse::get_y() > y && CL_Mouse::get_y() < y + 25)
    {
      if (CL_Mouse::get_keycode(CL_MOUSE_LEFT))
	{
	  button_pressed.draw(0, y);
	}
      else
	{
	  button.draw(0, y);
	}

      if (mouse_over_time == 0)
	mouse_over_time = CL_System::get_time ();

      if (CL_System::get_time () - mouse_over_time > 5) {
	int width = font.bounding_rect(0, 0, tooltip.c_str()).get_width();
	CL_Display::fill_rect(CL_Rect(28, y + 6 - 2, 32 + width, y + font.get_height () + 6 + 2),
                              CL_Color(100, 100, 0));
	font.draw(30, y + 6, tooltip.c_str ());
      }
    }
  else
    {
      mouse_over_time = 0;
    }
  sur.draw(0, y);
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

  CL_Display::fill_rect(CL_Rect(0, 0, 25, CL_Display::get_height()),
                        CL_Color(190, 190, 190, 255));

  logo.draw(0, CL_Display::get_height() - logo.get_height());

  for (std::vector<PanelIcon*>::iterator i = buttons.begin(); i != buttons.end(); ++i)
    {
      (*i)->draw(0, y);
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
      buttons.push_back(new PanelIconObjectProperties);
      buttons.push_back(new PanelIconDelete);

      buttons.push_back(new PanelIconRegionZoom ());
      buttons.push_back(new PanelIconZoomIn ());
      buttons.push_back(new PanelIconZoomOut ());

      buttons.push_back(new PanelIconLevelProperties);
      buttons.push_back(new PanelIconSetupActions());

      buttons.push_back(new PanelIconRun);

      buttons.push_back(new PanelIconHelp);
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

      for (std::vector<PanelIcon*>::iterator i = buttons.begin(); i != buttons.end(); ++i)
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

      for (std::vector<PanelIcon*>::iterator i = buttons.begin(); i != buttons.end(); ++i)
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

} // namespace EditorNS
} // namespace Pingus

/* EOF */
