//  $Id: surface_selector.cxx,v 1.11 2003/10/20 13:33:43 grumbel Exp $
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
#include <ClanLib/Display/display.h>
#include <ClanLib/Display/mouse.h>
#include "../gui/display.hxx"
#include "../fonts.hxx"
#include "../resource.hxx"
#include "surface_selector.hxx"

namespace Pingus {
namespace EditorNS {

SurfaceSelector::SurfaceSelector (std::vector<surface_obj>* s)
  : font(Fonts::courier_small),
    sur_list(s),
    y_of(0),
    width(CL_Display::get_width() - (CL_Display::get_width() % 50)),
    height((sur_list->size() / (CL_Display::get_width() / 50)) * 50),
    c_obj(std::vector<surface_obj>::iterator())
{
}

SurfaceSelector::~SurfaceSelector ()
{

}

std::vector<surface_obj>::iterator
SurfaceSelector::get_current_obj ()
{
  int x = 0;
  int y = -y_of;

  for(std::vector<surface_obj>::iterator i = sur_list->begin();
      i != sur_list->end();
      ++i)
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

  return std::vector<surface_obj>::iterator();
}

void
SurfaceSelector::draw ()
{
  // FIXME: This could heavily optimized if ClanLib would have a put_target(x,y,w,h)
  std::vector<surface_obj>::iterator tmp_obj = get_current_obj();

  if (c_obj != tmp_obj)
    {
      if (tmp_obj != std::vector<surface_obj>::iterator())
	tmp_obj->display_time = CL_System::get_time();

      if (c_obj != std::vector<surface_obj>::iterator())
	c_obj->display_time = 0;
    }

  c_obj = tmp_obj;

  int x = 0;
  int y = -y_of;

  CL_System::keep_alive();

  CL_Display::clear();

  // Draw all surfaces
  for(std::vector<surface_obj>::iterator i = sur_list->begin(); i != sur_list->end(); ++i)
    {
      if (i->thumbnail.get_width() <= 50 && i->thumbnail.get_height() <= 50)
	{
	  i->thumbnail.draw(x + 25 - (i->thumbnail.get_width()  / 2),
			          y + 25 - (i->thumbnail.get_height() / 2));
	}
      else
	{
	  i->thumbnail.draw(x, y);
	}

      if (i == c_obj)
	{
	  Display::draw_rect(x, y, x + 50, y + 50, 1.0, 1.0, 1.0, 1.0);
	}

      x += 50;
      if (x + 50 > CL_Display::get_width())
	{
	  y += 50;
	  x = 0;
	}
    }

  //Display::draw_rect(CL_Display::get_width() - 16, y/10,
  //CL_Display::get_width(), y+1/10,
  //1.0, 1.0, 1.0, 1.0);

#ifdef CLANLIB_0_6
  // Draw the current object in the bottom/left corner when the
  // surface is selected for more then 1sec
  if (c_obj != std::vector<surface_obj>::iterator()
      && (c_obj->display_time + 350 < CL_System::get_time() || c_obj->large_sur))
    {
      if (!c_obj->large_sur)
	{
	  std::cout << "Loading: " << c_obj->name << " " << c_obj->datafile << std::endl;
	  c_obj->large_sur = Resource::load_surface (c_obj->name, c_obj->datafile);
	}

      CL_Display::fill_rect(0, CL_Display::get_height() - c_obj->large_sur.get_height(),
			    c_obj->large_sur.get_width(), CL_Display::get_height(),
			    0.5f, 0.5f, 0.5f, 0.8f);
      c_obj->large_sur.draw(0, CL_Display::get_height() - c_obj->large_sur.get_height());
    }
#endif

  Display::flip_display();
}

void
SurfaceSelector::scroll ()
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

  if (y_of > height) y_of = height;
}

std::string
SurfaceSelector::select ()
{
  std::string str;
  std::vector<surface_obj>::iterator iter;

  while (!CL_Mouse::get_keycode(CL_MOUSE_LEFT))
    {
      draw();

      scroll();

      CL_System::keep_alive();
    }

  iter = get_current_obj();

  if (iter != std::vector<surface_obj>::iterator())
    str = iter->name;

  std::cout << "str: " << str << std::endl;

  return str;
}

surface_obj::surface_obj ()
{
}

surface_obj::surface_obj (const surface_obj& old) : thumbnail(old.thumbnail),
                                                    large_sur(old.large_sur),
					    	    name(old.name),
						    datafile(old.datafile),
						    display_time(old.display_time)
{
}

surface_obj& surface_obj::operator= (const surface_obj& old)
{
  if (this != &old)
    {
      thumbnail    = old.thumbnail;
      large_sur    = old.large_sur;
      name         = old.name;
      datafile     = old.datafile;
      display_time = old.display_time;
    }

  return *this;
}

} // namespace EditorNS
} // namespace Pingus

/* EOF */
