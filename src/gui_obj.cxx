//  $Id: gui_obj.cxx,v 1.1 2002/06/12 19:09:37 grumbel Exp $
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

#include <ClanLib/Display/Display/display.h>
#include "gui_obj.hxx"

GuiObj::GuiObj()
{
  // These variables are used for default clipping
  x1 = 0;
  y1 = 0;
  x2 = CL_Display::get_width();
  y2 = CL_Display::get_height();
}

GuiObj::~GuiObj()
{
}

void 
GuiObj::set_clip_rect(int ax1, int ay1, int ax2, int ay2)
{
  x1 = ax1;
  y1 = ay1;
  x2 = ax2;
  y2 = ay2;
}

void
GuiObj::draw_clipped()
{
  CL_Display::push_clip_rect();
  CL_Display::set_clip_rect(CL_ClipRect(x1, y1, x2, y2));
  draw();
  CL_Display::pop_clip_rect();
}

void
GuiObj::update(float /*delta*/)
{
}

void
GuiObj::updateX()
{
}

/* EOF */




