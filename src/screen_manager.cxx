//  $Id: screen_manager.cxx,v 1.23 2002/10/20 21:38:54 grumbel Exp $
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
#include <ClanLib/Display/Display/display.h>
#include "globals.hxx"
#include "input/controller.hxx"
#include "delta_manager.hxx"
#include "display.hxx"
#include "screen_manager.hxx"
#include "path_manager.hxx"
#include "fade_out.hxx"

ScreenManager* ScreenManager::instance_ = 0;

ScreenManager::ScreenManager ()
  : display_gc (0, 0, CL_Display::get_width (), CL_Display::get_height (), 0, 0)
{
  cached_action = none;
}

ScreenManager::~ScreenManager ()
{
}

void
ScreenManager::display ()
{
  Input::Controller input_controller(path_manager.complete ("controller/default.xml"));
  DeltaManager delta_manager;

  // Main loop for the menu
  while (!screens.empty())
    {
      float time_delta = delta_manager.getset ();
      
      if (time_delta > 1.0)
	{
	  std::cout << "PingusMenuManager: detected large delta (" << time_delta
		    << "), ignoring and doing frameskip" << std::endl;
	  continue;
	}

      // Let ClanLib fetch events
      CL_System::keep_alive ();

      // Get new events from ClanLib
      input_controller.update (time_delta);

      // Fill the delta with values
      GameDelta delta (time_delta, input_controller.get_events ());

      last_screen = get_current_screen();

      // Most likly the screen will get changed in this update call
      get_current_screen()->update (delta);    

      // Last screen has poped, so we are going to end here
      if (screens.empty ())
	continue;

      if (cached_action == pop)
	{
	  real_pop_screen ();
	  cached_action = none;
	}
      else if (cached_action == replace)
	{
	  real_replace_screen (replace_screen_arg);
	  cached_action = none;
	}
      
      // skip draw if the screen changed to avoid glitches
      if (last_screen == get_current_screen())
      	{
	  if (get_current_screen()->draw (display_gc))
	    Display::flip_display ();

          //CL_Display::put_display(CL_Rect(0, 0, 100, 100));
	}
      else
	{
	  std::cout << "ScreenManager: fading screens" << std::endl;
	  //fade_over (last_screen, get_current_screen());
	}

      // Stupid hack to make this thing take less CPU
      CL_System::sleep (0);

      /** Delete all screens that are no longer needed */
      delete_screens.clear();
    } 
}

ScreenPtr
ScreenManager::get_current_screen()
{
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
  std::cout << "XXXXXXXX ScreenManager::push_screen" << std::endl;

  if (!screens.empty())
    {
      std::cout << "ScreenManager::push_screen" << std::endl;
      screens.back ()->on_shutdown ();
    }

  screen->on_startup ();
  screens.push_back (ScreenPtr(screen, delete_screen));
}

void
ScreenManager::pop_screen ()
{
  assert (cached_action == none);
  cached_action = pop;
}

void
ScreenManager::replace_screen (Screen* screen, bool delete_screen)
{
  assert (cached_action == none);
  cached_action = replace;
  replace_screen_arg = ScreenPtr(screen, delete_screen);
}

void
ScreenManager::real_replace_screen (const ScreenPtr& ptr)
{
  std::cout << "XXXXXXXX ScreenManager::replace_screen" << std::endl;

  screens.back ()->on_shutdown ();

  delete_screens.push_back(screens.back ());
  
  screens.back () = ptr;
  screens.back ()->on_startup ();
}

void
ScreenManager::real_pop_screen ()
{
  std::cout << "XXXXXXXX ScreenManager::pop_screen" << std::endl;

  screens.back ()->on_shutdown ();

  delete_screens.push_back(screens.back ());
  
  std::cout << "ScreenManager::real_pop_screen ()" << std::endl;
  screens.pop_back ();

  if (!screens.empty ())
    {
      screens.back ()->on_startup ();
    }
}

void
ScreenManager::fade_over (const ScreenPtr& old_screen, const ScreenPtr& new_screen)
{
  FadeOut::fade_to_black();
  UNUSED_ARG(old_screen);
  UNUSED_ARG(new_screen);

#if 0
  DeltaManager delta_manager;
  float passed_time = 0;


  std::list<Input::Event*> events;
  while (passed_time < 2.0f)
    {
      float time_delta = delta_manager.getset ();
      passed_time += time_delta;

      int border_x = int((CL_Display::get_width ()/2) * passed_time/2.0f);
      int border_y = int((CL_Display::get_height ()/2) * passed_time/2.0f);

      //std::cout << "FadeOver: " << border_x << " " << border_y << std::endl;

      new_screen->draw (display_gc);

      CL_Display::push_clip_rect(CL_ClipRect (0 + border_x, 
					      0 + border_y,
					      CL_Display::get_width () - border_x,
					      CL_Display::get_height () - border_y));
      old_screen->draw (display_gc);

      GameDelta delta (time_delta, events);
      new_screen->update (delta);
      old_screen->update (delta);

      CL_Display::pop_clip_rect ();

      Display::flip_display ();
      CL_System::keep_alive ();
    }
#endif 
}

/* EOF */
