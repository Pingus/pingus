
//  Pingus - A free Lemmings clone
//  Copyright (C) 1999 Ingo Ruhnke <grumbel@gmx.de>
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//  
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//  
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include <stdio.h>
#include <iostream>
#include "../globals.hpp"
#include "../pingu_holder.hpp"
#include "../display/drawing_context.hpp"
#include "../display/scene_context.hpp"
#include "../world.hpp"
#include "../server.hpp"
#include "../pingu.hpp"
#include "../display/display.hpp"
#include "../game_session.hpp"
#include "playfield.hpp"

Playfield::Playfield(Server* server_, GameSession* session_, const Rect& rect_)
  : RectComponent(rect_),
    server(server_),
    session(session_),
    current_pingu(0),
    // We keep the SceneContext has member variable so that we don't
    // have to reallocate it every frame, which is quite a costly operation
    scene_context(new SceneContext()),
    state(rect),
    capture_rectangle(session)
{
  mouse_scrolling    = false;

  state.set_limit(Rect(Vector2i(0, 0), Size(server->get_world()->get_width(), server->get_world()->get_height())));

  // FIXME: Temporary workaround till start-pos is integrated a bit more properly
  state.set_pos(server->get_world()->get_start_pos(0));
}

Playfield::~Playfield()
{
  delete scene_context;
}

void
Playfield::draw(DrawingContext& gc)
{
  scene_context->clear();
  scene_context->set_cliprect(rect);

  //scene_context->light().fill_screen(Color(50, 50, 50));
 
  state.push(*scene_context);

  capture_rectangle.set_pingu(current_pingu);
  capture_rectangle.draw(*scene_context);

  server->get_world()->draw(*scene_context);
 
  // Draw the scrolling band
  if (mouse_scrolling && !drag_drop_scrolling)
    {
      gc.draw_line(mouse_pos.x, mouse_pos.y,
                   scroll_center.x, scroll_center.y-15,
                   Color(0, 255, 0));

      gc.draw_line(mouse_pos.x, mouse_pos.y,
                   scroll_center.x, scroll_center.y,
                   Color(255, 0, 0));

      gc.draw_line(mouse_pos.x, mouse_pos.y,
                   scroll_center.x, scroll_center.y+15,
                   Color(0, 0, 255));

      gc.draw_line(mouse_pos.x, mouse_pos.y,
                   scroll_center.x + 15, scroll_center.y,
                   Color(0, 255, 255));

      gc.draw_line(mouse_pos.x, mouse_pos.y,
                   scroll_center.x - 15, scroll_center.y,
                   Color(255, 255, 0));
    }

  state.pop(*scene_context);
  gc.draw(new SceneContextDrawingRequest(scene_context, Vector3f(0,0,-10000)));
}

Pingu*
Playfield::current_pingu_find(const Vector2f& pos)
{
  double min_dist = 500.0;
  double dist;
  Pingu* c_pingu = 0;

  for (PinguIter pingu = server->get_world()->get_pingus()->begin();
       pingu != server->get_world()->get_pingus()->end();
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
      capture_rectangle.set_pingu(current_pingu);
    }
  else
    {
      if (drag_drop_scrolling)
        {
          state.set_pos(old_state_pos + (scroll_center - mouse_pos));
        }
      else
        { 
          state.set_pos(Vector2f(state.get_pos().x - float(scroll_center.x - mouse_pos.x) * 0.2f,
                                 state.get_pos().y - float(scroll_center.y - mouse_pos.y) * 0.2f));
        }
    }

  if (auto_scrolling && (fullscreen_enabled || SDL_WM_GrabInput(SDL_GRAB_QUERY) == SDL_GRAB_ON))
    {
      // FIXME: May need to modify this function if it's not gradient enough.
      scroll_speed = static_cast<int>(default_screen_width * delta);
      //std::cout << "scroll_speed: " << scroll_speed << std::endl;
    
      if (mouse_pos.x < 10)
	{
	  state.set_pos(state.get_pos() - Vector2i(scroll_speed, 0));
	}
      else if (mouse_pos.x > Display::get_width() - 10)
	{
	  state.set_pos(state.get_pos() + Vector2i(scroll_speed, 0));
	}

      if (mouse_pos.y < 10)
	{
	  state.set_pos(state.get_pos() - Vector2i(0, scroll_speed));
	}
      else if (mouse_pos.y > Display::get_height() - 10)
	{
	  state.set_pos(state.get_pos() + Vector2i(0, scroll_speed));	 
	}
    }
}

void
Playfield::on_primary_button_press(int x, int y)
{
  if (session)
    {
      current_pingu = current_pingu_find(state.screen2world( Vector2i(x,y) ));

      if (current_pingu) 
        {
          server->send_pingu_action_event(current_pingu, session->get_action_name());
        }
    }
}

void
Playfield::on_secondary_button_press(int x, int y)
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

  if (maintainer_mode)
    {
      Uint8 *keystate = SDL_GetKeyState(NULL);
      if (keystate[SDLK_r])
        {
          CollisionMask mask("other/bash_radius_gfx");
          Vector2f p = state.screen2world(mouse_pos);
          server->get_world()->remove(mask, 
                                      int(p.x - mask.get_width()/2), 
                                      int(p.y - mask.get_height()/2));
        }
      else if (keystate[SDLK_g])
        {
          CollisionMask mask("other/bash_radius_gfx");
          Vector2f p = state.screen2world(mouse_pos);
          server->get_world()->put(mask, 
                                   int(p.x - mask.get_width()/2), 
                                   int(p.y - mask.get_height()/2),
                                   Groundtype::GP_GROUND);
        }
      else if (keystate[SDLK_b])
        {
          CollisionMask mask("other/bash_radius_gfx");
          Vector2f p = state.screen2world(mouse_pos);
          server->get_world()->put(mask, 
                                   int(p.x - mask.get_width()/2), 
                                   int(p.y - mask.get_height()/2),
                                   Groundtype::GP_BRIDGE);
        }
    }
}

Vector2i
Playfield::get_pos() const
{
  return Vector2i(static_cast<int>(state.get_pos().x), 
                  static_cast<int>(state.get_pos().y));
}

void
Playfield::set_viewpoint(int x, int y)
{
  state.set_pos(Vector2f((float)x, (float)y));
}

void
Playfield::scroll (int x, int y)
{
  state.set_pos(state.get_pos() + Vector2f((float)x, (float)y));
}


/* EOF */
