//  $Id: SurfaceSelector.cc,v 1.3 2000/04/24 13:15:43 grumbel Exp $
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

#include <ClanLib/core.h>
#include "../Display.hh"
#include "../PingusResource.hh"
#include "SurfaceSelector.hh"

using namespace std;

SurfaceSelector::SurfaceSelector(vector<surface_obj>* s)
{
  y_of = 0;
  sur_list = s;
  font = CL_Font::load("Fonts/courier_small",PingusResource::get("fonts.dat"));
}

SurfaceSelector::~SurfaceSelector()
{
  
}

vector<surface_obj>::iterator
SurfaceSelector::get_current_obj()
{
  int x = 0;
  int y = -y_of;
  
  for(vector<surface_obj>::iterator i = sur_list->begin(); 
      i != sur_list->end(); 
      i++)
    {
      if (CL_Mouse::get_x() > x && CL_Mouse::get_x() <= x + 50
	  && CL_Mouse::get_y() > y && CL_Mouse::get_y() <= y + 50)
	{
	  return i;
	}
      
      x += 50;
      if (x + 50 > CL_Display::get_width())
	{
	  y += 50;
	  x = 0;
	}
    }

  return vector<surface_obj>::iterator();
}

void
SurfaceSelector::draw()
{
  // FIXME: This could heavily optimized if ClanLib would have a put_target(x,y,w,h)
  vector<surface_obj>::iterator c_obj = get_current_obj();
  int x = 0;
  int y = -y_of;

  CL_System::keep_alive();
 
  CL_Display::clear_display();

  for(vector<surface_obj>::iterator i = sur_list->begin(); i != sur_list->end(); i++)
    {
      if (i->sur->get_width() <= 50 && i->sur->get_height() <= 50)
	{
	  i->sur->put_screen(x + 25 - (i->sur->get_width() / 2), 
			     y + 25 - (i->sur->get_height() / 2));
	}
      else
	{
	  i->sur->put_screen(x, y, 50, 50);
	}

      if (i == c_obj)
	{
	  Display::draw_rect(x, y, x + 50, y + 50,
			     1.0, 1.0, 1.0, 1.0);
	}

      x += 50;
      if (x + 50 > CL_Display::get_width()) 
	{
	  y += 50;
	  x = 0;
	}
    }

  if (c_obj != vector<surface_obj>::iterator())
    {
      CL_Display::fill_rect(0, CL_Display::get_height() - c_obj->sur->get_height(),
			    c_obj->sur->get_width(), CL_Display::get_height(),
			    0.5, 0.5, 0.5, 0.8);
      c_obj->sur->put_screen(0, CL_Display::get_height() - c_obj->sur->get_height());
    }
  CL_Display::flip_display();
}

void
SurfaceSelector::scroll()
{
  const int range = 100;

  if (CL_Mouse::get_y() > CL_Display::get_height() - range)
    {
      y_of += range - (CL_Display::get_height() - CL_Mouse::get_y());
    }
  else if (CL_Mouse::get_y() < range)
    {
      y_of -= range - CL_Mouse::get_y();
    }
  
  if (y_of < 0) y_of = 0;    
}

string
SurfaceSelector::select()
{
  string str;
  
  while (!CL_Mouse::left_pressed())
    {
      draw();

      scroll();
      
      CL_System::keep_alive();
    } 

  str = get_current_obj()->name;

  cout << "str: " << str << endl;

  return str;
}

/* EOF */
