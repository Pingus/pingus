//  $Id: pingus_counter_bar.cxx,v 1.6 2003/04/19 10:23:17 torangan Exp $
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

#include <ClanLib/Display/Display/display.h>
#include "pingus_counter_bar.hxx"
#include "pingu_holder.hxx"
#include "server.hxx"
#include "world.hxx"

PingusCounterBar::PingusCounterBar (Server * s, Orientation o, const CL_Rect& arg_rect)
  : server (s),
    orientation (o),
    rect (arg_rect.x1 + 2, arg_rect.y1 + 2, arg_rect.x2 - 2, arg_rect.y2 - 2)
{
}

PingusCounterBar::~PingusCounterBar ()
{
}

void
PingusCounterBar::update(float /*delta*/)
{
}

void
PingusCounterBar::draw ()
{
  int length = rect.y2 - rect.y1;

  int complete      = server->get_world ()->get_pingus()->get_number_of_allowed();
  int current_out   = server->get_world ()->get_pingus()->get_number_of_released();
  int current_saved = server->get_world ()->get_pingus()->get_number_of_exited();

  //std::cout << "Drawing Counterbar: " << rect << std::endl;
  CL_Display::fill_rect (rect.x1, rect.y1, rect.x2, rect.y2,
			 0.0, 0.0, 1.0, 0.5);

  int y_pos = rect.y2;
  int tmp_y_pos = y_pos;

  y_pos -= (current_saved * length) / complete;
  CL_Display::fill_rect (rect.x1, tmp_y_pos, rect.x2, y_pos,
			 0.0f, 1.0f, 0.0f, 0.8f);
  tmp_y_pos = y_pos;

  y_pos -= ((current_out - current_saved) * length) / complete;
  CL_Display::fill_rect (rect.x1, tmp_y_pos, rect.x2, y_pos,
			 1.0f, 0.0f, 0.0f, 0.8f);
  tmp_y_pos = y_pos;
}

/* EOF */
