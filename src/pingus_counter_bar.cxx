//  $Id: pingus_counter_bar.cxx,v 1.8 2003/10/19 12:25:47 grumbel Exp $
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

namespace Pingus {

PingusCounterBar::PingusCounterBar (Server * s, Orientation o, const CL_Rect& arg_rect)
  : server (s),
    orientation (o),
    rect (arg_rect.left + 2, arg_rect.top + 2, arg_rect.right - 2, arg_rect.bottom - 2)
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
  int length = rect.bottom - rect.top;

  int complete      = server->get_world ()->get_pingus()->get_number_of_allowed();
  int current_out   = server->get_world ()->get_pingus()->get_number_of_released();
  int current_saved = server->get_world ()->get_pingus()->get_number_of_exited();

  //std::cout << "Drawing Counterbar: " << rect << std::endl;
  CL_Display::fill_rect (rect.left, rect.top, rect.right, rect.bottom,
			 0.0, 0.0, 1.0, 0.5);

  int y_pos = rect.bottom;
  int tmp_y_pos = y_pos;

  y_pos -= (current_saved * length) / complete;
  CL_Display::fill_rect (rect.left, tmp_y_pos, rect.right, y_pos,
			 0.0f, 1.0f, 0.0f, 0.8f);
  tmp_y_pos = y_pos;

  y_pos -= ((current_out - current_saved) * length) / complete;
  CL_Display::fill_rect (rect.left, tmp_y_pos, rect.right, y_pos,
			 1.0f, 0.0f, 0.0f, 0.8f);
  tmp_y_pos = y_pos;
}

} // namespace Pingus

/* EOF */
