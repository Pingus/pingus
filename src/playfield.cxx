//  $Id: playfield.cxx,v 1.40 2003/10/22 11:11:22 grumbel Exp $
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

#include <stdio.h>
#include <iostream>
#include <ClanLib/Display/display.h>
#include "globals.hxx"
#include "playfield.hxx"
#include "pingu_holder.hxx"
#include "display/drawing_context.hxx"
#include "display/scene_context.hxx"
#include "world.hxx"
#include "server.hxx"
#include "true_server.hxx"
#include "pingu.hxx"
#include "gui/display.hxx"
#include "button_panel.hxx"

namespace Pingus {

Playfield::Playfield (Client* client_, const CL_Rect& rect_)
  : rect(rect_),
    client(client_),
    buttons(client->get_button_panel()),
    current_pingu(0),
    scene_context(new SceneContext()),
    state(rect.get_width(), rect.get_height()),
    cap(client->get_button_panel())
{
  world              = client->get_server()->get_world();
  mouse_scrolling    = false;
  needs_clear_screen = false;

  state.set_limit(CL_Rect(CL_Point(0, 0), CL_Size(world->get_width(), world->get_height())));

  if (0)
    {
      // Special handling for levels smaller than the screen
      int x1, x2, y1, y2;

      x1 = (CL_Display::get_width() - world->get_width()) / 2;
      x2 = x1 + world->get_width() - 1;

      y1 = (CL_Display::get_height() - world->get_height()) / 2;
      y2 = y1 + world->get_height() - 1;

      if (x1 < 0)
        x1 = 0;
      if (x2 >= CL_Display::get_width())
        x2 = CL_Display::get_width() - 1;
      if (y1 < 0)
        y1 = 0;
      if (y2 >= CL_Display::get_height())
        y2 = CL_Display::get_height() - 1;

      if (x1 > 0 || x2 < (CL_Display::get_width() - 1)
          || y1 > 0 || y2 < (CL_Display::get_height() - 1))
        {
          std::cout << "Playfield:: Activating clear screen" << std::endl;
          needs_clear_screen = true;
          generate_clipping_rects(x1, y1, x2, y2);
        }
      else
        {
          needs_clear_screen = false;
        }
    }
}

Playfield::~Playfield()
{
}

void
Playfield::draw (DrawingContext& gc)
{
  scene_context->clear();
  scene_context->light().fill_screen(CL_Color(50, 50, 50));
 
  state.push(*scene_context);

  cap.set_pingu(current_pingu);
  cap.draw(*scene_context);
  
  world->draw(*scene_context);
 
  if (needs_clear_screen)
    {
      for(std::vector<CL_Rect>::iterator i = clipping_rectangles.begin();
	  i != clipping_rectangles.end();
	  i++)
	{
	  CL_Display::fill_rect(CL_Rect(i->left, i->top,
                                        i->right + 1, i->bottom + 1),
				CL_Color(0, 0, 0));
	}
    }

  // Draw the scrolling band
  if (mouse_scrolling && !drag_drop_scrolling)
    {
      gc.draw_line(mouse_pos.x, mouse_pos.y,
                   scroll_center.x, scroll_center.y-15,
                   CL_Color(0, 255, 0));

      gc.draw_line(mouse_pos.x, mouse_pos.y,
                   scroll_center.x, scroll_center.y,
                   CL_Color(255, 0, 0));

      gc.draw_line(mouse_pos.x, mouse_pos.y,
                   scroll_center.x, scroll_center.y+15,
                   CL_Color(0, 0, 255));

      gc.draw_line(mouse_pos.x, mouse_pos.y,
                   scroll_center.x + 15, scroll_center.y,
                   CL_Color(0, 255, 255));

      gc.draw_line(mouse_pos.x, mouse_pos.y,
                   scroll_center.x - 15, scroll_center.y,
                   CL_Color(255, 255, 0));
    }

  state.pop(*scene_context);
  gc.draw(new SceneContextDrawingRequest(scene_context, -10000));
}

Pingu*
Playfield::current_pingu_find (const CL_Pointf& pos)
{
  double min_dist = 500.0;
  double dist;
  Pingu* c_pingu = 0;

  for (PinguIter pingu = world->get_pingus()->begin();
       pingu != world->get_pingus()->end();
       ++pingu)
    {
      if ((*pingu)->is_over(static_cast<int>(pos.x), static_cast<int>(pos.y)))
	{
	  dist = (*pingu)->dist(static_cast<int>(pos.x), static_cast<int>(pos.y));

	  if (dist < min_dist)
	    {
	      min_dist = dist;
	      c_pingu = *pingu;
	    }
	}
    }
  return c_pingu;
}

void
Playfield::update(float delta)
{
  // FIXME: This should be delta dependant
  if (!mouse_scrolling)
    {
      current_pingu = current_pingu_find(state.screen2world(mouse_pos));
      cap.set_pingu(current_pingu);
    }
  else
    {
      if (drag_drop_scrolling)
        {
          state.set_pos(old_state_pos + (scroll_center - mouse_pos));
        }
      else
        { 
          state.set_pos(CL_Pointf(state.get_pos().x - (scroll_center.x - mouse_pos.x) * 0.2,
                                  state.get_pos().y - (scroll_center.y - mouse_pos.y) * 0.2));
        }
    }

  if (auto_scrolling)
    {
      // FIXME: This should be delta dependant
      scroll_speed = 15;

      if (mouse_pos.x < 2)
	{
	  state.set_pos(state.get_pos() - CL_Point(scroll_speed, 0));
	}
      else if (mouse_pos.x > CL_Display::get_width() - 3)
	{
	  state.set_pos(state.get_pos() + CL_Point(scroll_speed, 0));
	}

      if (mouse_pos.y < 2)
	{
	  state.set_pos(state.get_pos() - CL_Point(0, scroll_speed));
	}
      else if (mouse_pos.y > CL_Display::get_height() - 3)
	{
	  state.set_pos(state.get_pos() + CL_Point(0, scroll_speed));	 
	}
    }
}

void
Playfield::on_primary_button_press(int x, int y)
{
  UNUSED_ARG(x);
  UNUSED_ARG(y);

  if (current_pingu)
    {
      server->send_pingu_action_event(current_pingu, buttons->get_action_name());
    }
}

void
Playfield::on_secondary_button_press (int x, int y)
{
  mouse_scrolling = true;
  scroll_center.x = x;
  scroll_center.y = y;

  old_state_pos = state.get_pos();
}

void
Playfield::on_secondary_button_release (int x, int y)
{
  UNUSED_ARG(x);
  UNUSED_ARG(y);

  mouse_scrolling = false;
}

void
Playfield::on_pointer_move (int x, int y)
{
  // FIXME: useless stuff, but currently the controller doesn't have a state
  mouse_pos.x = x;
  mouse_pos.y = y;
}

void
Playfield::set_server(Server* s)
{
  server = s;
}

CL_Point
Playfield::get_pos() const
{
  return CL_Point(state.get_pos());
}

void
Playfield::set_viewpoint(int x, int y)
{
  state.set_pos(CL_Point(x, y));
}

void
Playfield::generate_clipping_rects(int x1, int y1, int x2, int y2)
{
  clipping_rectangles.push_back(CL_Rect(0, 0, CL_Display::get_width() - 1, y1));
  clipping_rectangles.push_back(CL_Rect(0, y1, x1, y2+1));
  clipping_rectangles.push_back(CL_Rect(x2+1, y1, CL_Display::get_width() - 1, y2+1));
  clipping_rectangles.push_back(CL_Rect(0, y2+1, CL_Display::get_width() - 1, CL_Display::get_height() - 1));
}

void
Playfield::scroll (int x, int y)
{
  state.set_pos(state.get_pos() + CL_Point(x, y));
}

} // namespace Pingus

/* EOF */

