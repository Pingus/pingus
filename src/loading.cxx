//  $Id: loading.cxx,v 1.10 2003/10/21 21:37:06 grumbel Exp $
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

#include <ClanLib/Display/display.h>
#include <ClanLib/Display/font.h>
#include "resource.hxx"
#include "gui/display.hxx"
#include "fonts.hxx"
#include "loading.hxx"

namespace Pingus {

Loading loading_screen;

Loading::Loading()
{
  is_init = false;
}

Loading::~Loading()
{
}

void
Loading::init()
{
  sur  = Resource::load_surface("Game/loading", "game");
  font = Fonts::courier_small;
  is_init = true;
}

void
Loading::draw()
{
  if (!is_init)
    init();

  CL_Display::clear();

  sur.draw((CL_Display::get_width() - sur.get_width())/2,
		 (CL_Display::get_height() - sur.get_height())/2);

  Display::flip_display();
}

void
Loading::draw_progress(const std::string& str, float progress)
{
  if (!is_init)
    init();

  CL_Display::clear();

  sur.draw((CL_Display::get_width() - sur.get_width())/2,
		 (CL_Display::get_height() - sur.get_height())/2);

  CL_Font myfont = font;
  myfont.set_alignment(origin_top_center);
  myfont.draw(CL_Display::get_width() / 2,
              CL_Display::get_height() - 130,
              str);

  CL_Display::fill_rect(CL_Rect(50, CL_Display::get_height() - 100,
                                CL_Display::get_width() - 50, CL_Display::get_height() - 50),
			Display::to_color(0.5, 0.5, 0.5, 1.0));

  CL_Display::fill_rect(CL_Rect(50, CL_Display::get_height() - 100,
                                50 + (int)((CL_Display::get_width()-100) * progress), 
                                CL_Display::get_height() - 50),
			Display::to_color(1.0, 0.0, 0.0, 1.0));

  Display::flip_display();
}

} // namespace Pingus

/* EOF */
