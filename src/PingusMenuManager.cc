//  $Id: PingusMenuManager.cc,v 1.1 2001/06/14 11:07:19 grumbel Exp $
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

#include "globals.hh"
#include "DeltaManager.hh"
#include "Intro.hh"
#include "PingusResource.hh"
#include "PingusMenu.hh"
#include "ExitMenu.hh"
#include "FadeOut.hh"
#include "PingusMenuManager.hh"

PingusMenuManager::PingusMenuManager ()
  : event_register_counter (0), intro (this), mainmenu (this), optionmenu (this), background (this),
    exitmenu (this)
{
  push_menu (&background);

  //current_menu = 0;
  if (intro_disabled)
    push_menu (&mainmenu);
  else
    push_menu (&intro); 
}

PingusMenuManager::~PingusMenuManager ()
{
}

/// Register all event-handling stuff
void 
PingusMenuManager::register_events ()
{
  ++event_register_counter;
  on_button_press_slot = CL_Input::sig_button_press.connect (CL_CreateSlot(this, &PingusMenuManager::on_button_press));
  on_button_release_slot = CL_Input::sig_button_release.connect (CL_CreateSlot(this, &PingusMenuManager::on_button_release));
  on_mouse_move_slot = CL_Input::sig_mouse_move.connect (CL_CreateSlot(this, &PingusMenuManager::on_mouse_move));
}

/// Unregister all event-handling stuff
void 
PingusMenuManager::unregister_events ()
{
  --event_register_counter;
  CL_Input::sig_button_press.disconnect (on_button_press_slot);
  CL_Input::sig_button_release.disconnect (on_button_release_slot);
  CL_Input::sig_mouse_move.disconnect (on_mouse_move_slot);
}

void 
PingusMenuManager::on_button_press (CL_InputDevice* device,const CL_Key& key)
{
  std::cout << "PingusMenuManager::on_button_press (" 
	    << device << ", " << key.id 
	    << ")" << std::endl;
  if (device == CL_Input::keyboards[0] && key.id == CL_KEY_ESCAPE && menu_stack.size () > 2)
    pop_menu ();
  else
    current_menu ()->on_button_press (device, key);
}

void
PingusMenuManager::on_button_release (CL_InputDevice* device,const CL_Key& key)
{
  current_menu ()->on_button_release (device, key);
}

void 
PingusMenuManager::on_mouse_move (CL_InputDevice* device, int x, int y)
{
  std::cout << "PingusMenuManager::on_mouse_move ("
	    << device << ", " << x << ", " << y << ")" << std::endl;
  current_menu ()->on_mouse_move (device, x, y);
}

void 
PingusMenuManager::display ()
{
  std::cout << "Pingusmenumanager: display ()" << std::endl;
  DeltaManager delta_manager;
  float delta;
  
  current_menu ()->preload ();
  register_events ();
  loop = true;

  while (loop)
    {
      //std::cout << "Displaying menu..." << std::endl;
      delta = delta_manager.getset ();

      // We ignore delta's larger then one second, to avoid jumps in
      // the scrolling (for example when starting a level and then
      // after some minutes going back to the menu would cause delta's
      // from >100, not nice)
      if (delta > 1.0)
	delta = 0;

      for (MenuStackIter i = menu_stack.begin (); i != menu_stack.end (); ++i)
	(*i)->draw ();

      for (MenuStackIter i = menu_stack.begin (); i != menu_stack.end (); ++i)
	(*i)->update (delta);
      
      CL_Display::flip_display ();

      
      CL_System::keep_alive ();
      CL_System::sleep (20);
    }
  unregister_events ();
}

void 
PingusMenuManager::set_menu (boost::dummy_ptr<PingusSubMenu> menu)
{
  /*if (current_menu.get ())
    fadeout ();*/
  pop_menu ();
  push_menu (menu);
  //current_menu = menu;
  //current_menu->preload ();
}

void 
PingusMenuManager::push_menu (boost::dummy_ptr<PingusSubMenu> menu)
{
  menu->preload ();
  menu_stack.push_back (menu);
}

void 
PingusMenuManager::pop_menu ()
{
  if (!menu_stack.empty ())
    menu_stack.pop_back();
}

boost::dummy_ptr<PingusSubMenu> 
PingusMenuManager::current_menu ()
{
  if (!menu_stack.empty ())
    {
      MenuStackRIter i = menu_stack.rbegin ();
      if (!i->get ())
	std::cout << "PingusMenuManager: Error: current_menu is " << i->get () << std::endl;
      return *i;
    }
  else
    {
      std::cout << "PingusMenuManager: Error: MenuStack empty!" << std::endl;
      return 0;
    }
}

void 
PingusMenuManager::fadeout ()
{
  DeltaManager delta_manager;
  EnlargingRectFadeOut fadeout;
  while (!fadeout.finished ())
    {
      float delta = delta_manager.getset ();
      fadeout.update (delta);
      current_menu ()->update (delta);
      current_menu ()->draw ();
      fadeout.draw ();
      CL_Display::flip_display ();

      CL_System::keep_alive ();
    }
}

void
PingusMenuManager::exit ()
{
  push_menu (&exitmenu);

  /* FIXME: !QUICKHACK! Rewrite me...
  ExitMenu exitmenu;
  exitmenu.preload ();
  DeltaManager delta;

  CL_Display::fill_rect (0,0, CL_Display::get_width (), CL_Display::get_height (),
			 0.0, 0.0, 0.0, 0.5);
  CL_Display::flip_display ();
  CL_Display::sync_buffers ();
  while (CL_Mouse::left_pressed ())
    CL_System::keep_alive ();

  while (!CL_Mouse::left_pressed ()) {
    exitmenu.update (delta.getset ());
    exitmenu.draw ();
    CL_Display::flip_display ();
    CL_System::keep_alive ();
    CL_System::sleep (20);
  }
  if (CL_Mouse::get_x () > CL_Display::get_width ()/2)
    loop = true;
  else
    loop = false;
  while (CL_Mouse::left_pressed ())
    CL_System::keep_alive ();

  if (loop == false)
  fadeout ();*/
}

/* EOF */
