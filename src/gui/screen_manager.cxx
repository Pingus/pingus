//  $Id: screen_manager.cxx,v 1.12 2003/10/20 19:28:55 grumbel Exp $
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
#include "../globals.hxx"
#include "cursor.hxx"
#include "display.hxx"
#include "screen_manager.hxx"
#include "../path_manager.hxx"
#include "../display/drawing_context.hxx"
#include "../input/controller.hxx"

namespace Pingus {

ScreenManager* ScreenManager::instance_ = 0;

ScreenManager::ScreenManager ()
{
#if 0 
  // FIXME:
  if (render_preview)
    display_gc = new BufferGraphicContext(800, 600);
  else
    display_gc = new DisplayGraphicContext(0, 0, CL_Display::get_width (), CL_Display::get_height (),
                                           0, 0);
#endif
////  display_gc = new DrawingContext();

  cached_action = CA_NONE;
}

ScreenManager::~ScreenManager ()
{
////	delete display_gc;
}

void
ScreenManager::display ()
{
#if 0
  Input::Controller* input_controller = 0;
#if 0
  if (controller_file.empty())
    input_controller = new Input::Controller(path_manager.complete("controller/default.xml"));
  else
    input_controller = new Input::Controller(controller_file);
#endif
  Input::Controller::set_current(input_controller);

#if 0
  Cursor* cursor = 0;
  if (swcursor_enabled)
    {
      cursor = new Cursor("core/cursors/animcross");
      Display::add_flip_screen_hook(cursor);
      //CL_MouseCursor::hide();
    }
#endif 

  DeltaManager delta_manager;

  // Main loop for the menu
  while (!screens.empty())
    {
      float time_delta = delta_manager.getset ();

      if (time_delta > 1.0)
	{
          if (maintainer_mode)
            std::cout << "ScreenManager: detected large delta (" << time_delta
                      << "), ignoring and doing frameskip" << std::endl;
	  continue;
	}

      // Let ClanLib fetch events
      ///CL_System::keep_alive ();

      // Get new events from ClanLib
      input_controller->update(time_delta);

      // Fill the delta with values
      GameDelta delta(time_delta, delta_manager.get_absolute(),
                      input_controller->get_events());

      input_controller->clear();

      last_screen = get_current_screen();

      // Most likly the screen will get changed in this update call
      get_current_screen()->update (delta);

#if 0
      if (cursor)
        cursor->update(time_delta);
#endif

      // Last screen has poped, so we are going to end here
      if (screens.empty ())
	continue;

      while (cached_action != CA_NONE)
        {
          switch (cached_action)
            {
            case CA_POP:
              real_pop_screen ();
              break;
            case CA_REPLACE:
              real_replace_screen (replace_screen_arg);
              break;
            case CA_CLEAR:
              real_clear();
              break;
            default:
              break;
            }
        }

      // FIXME: is there a more gentel way to do that instead of spreading the checks all around here?
      // Last screen has poped, so we are going to end here
      if (screens.empty())
	continue;

      // skip draw if the screen changed to avoid glitches
      if (last_screen == get_current_screen())
      	{
	  if (get_current_screen()->draw(*display_gc))
            {
              ////display_gc->render(CL_Display::get_current_window()->get_gc());
              Display::flip_display ();
              ////display_gc->clear();
            }
        }
      else
	{
	  //std::cout << "ScreenManager: fading screens" << std::endl;
	  fade_over (last_screen, get_current_screen());
	}

      // Stupid hack to make this thing take less CPU
      ////CL_System::keep_alive(5);
    }

////  Display::remove_flip_screen_hook(cursor);
////  delete cursor;
  delete input_controller;
#endif 
}

ScreenPtr&
ScreenManager::get_current_screen()
{
  assert(!screens.empty());
  return screens.back ();
}

ScreenManager*
ScreenManager::instance ()
{
  if (instance_)
    return instance_;
  else
    return instance_ = new ScreenManager ();
}

void
ScreenManager::push_screen (Screen* screen, bool delete_screen)
{
  if (!screens.empty())
    {
      screens.back ()->on_shutdown ();
    }

  screens.push_back (ScreenPtr(screen, delete_screen));
  screen->on_startup ();
}

void
ScreenManager::pop_screen ()
{
  assert (cached_action == CA_NONE);
  cached_action = CA_POP;
}

void
ScreenManager::replace_screen (Screen* screen, bool delete_screen)
{
  assert (cached_action == CA_NONE);
  cached_action = CA_REPLACE;
  replace_screen_arg = ScreenPtr(screen, delete_screen);
}

void
ScreenManager::real_replace_screen (const ScreenPtr& ptr)
{
  cached_action = CA_NONE;
  screens.back ()->on_shutdown ();
  screens.back () = ptr;
  screens.back ()->on_startup ();
}

void
ScreenManager::real_pop_screen ()
{
  cached_action = CA_NONE;
  ScreenPtr back = screens.back ();
  screens.pop_back();
  back->on_shutdown();

  if (!screens.empty ())
    {
      screens.back ()->on_startup ();
    }
}

void
ScreenManager::clear()
{
  cached_action = CA_CLEAR;
}

void
ScreenManager::real_clear()
{
  cached_action = CA_NONE;
  screens.clear();
}

void
ScreenManager::fade_over (ScreenPtr& old_screen, ScreenPtr& new_screen)
{
  // FIXME: This entire function doesn't work very well.
#if 0
  if (0)
    {
      DeltaManager delta_manager;
      float passed_time = 0;

      //Input::EventLst events;

      int screen_width = CL_Display::get_width ();
      int screen_height = CL_Display::get_height ();
      float progress = 0.0f;
      while (progress <= 1.0f)
        {
          float time_delta = delta_manager.getset ();
          passed_time += time_delta;

          int border_x = int((screen_width/2) * (1.0f - progress));
          int border_y = int((screen_height/2) * (1.0f - progress));

          old_screen->draw(*display_gc);
          CL_Display::get_current_window()->get_gc()
            ->push_cliprect(CL_Rect(0 + border_x,
                                    0 + border_y,
                                    screen_width - border_x,
                                    screen_height - border_y));
          new_screen->draw(*display_gc);

          //GameDelta delta (time_delta, CL_System::get_time(), events);
          // FIXME: Animation looks nifty but doesn't work all that good
          //new_screen->update (delta);
          //old_screen->update (delta);

          CL_Display::get_current_window()->get_gc()->pop_cliprect ();

          Display::flip_display ();
          CL_System::keep_alive (5);

          progress = passed_time/1.0f;
        }
    }
#endif
}

void
ScreenManager::init()
{
  instance_ = 0;
}

void
ScreenManager::deinit()
{
  delete instance_;
	instance_ = 0;
}

} // namespace Pingus

/* EOF */
