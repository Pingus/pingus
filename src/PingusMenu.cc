//  $Id: PingusMenu.cc,v 1.44 2001/05/18 19:17:08 grumbel Exp $
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

#include <iostream>

#include "globals.hh"
#include "AlphaButton.hh"
#include "PingusGame.hh"
#include "Credits.hh"
// #include "FileSelector.hh"
#include "PingusResource.hh"
#include "PingusMessageBox.hh"
#include "PingusError.hh"
#include "PingusMenu.hh"
#include "Loading.hh"
#include "Display.hh"
#include "PingusSound.hh"
#include "blitter.hh"

PingusMenu::PingusMenu()
{
  is_init = false;
  // We need this button a bit earlier than the other ones, so we
  // allocate it here.
}

void
PingusMenu::init ()
{
  boost::shared_ptr<SurfaceButton> editor_button (new EditorButton ());
 
  if (start_editor)
    editor_button->on_click ();

  background         = PingusResource::load_surface("misc/logo_t", "core");

  layer_manager.add_layer (PingusResource::load_surface ("menu/layer1", "core"),  0, 0, 12, 0);
  layer_manager.add_layer (PingusResource::load_surface ("menu/layer2", "core"),  0, 150, 25, 0);
  layer_manager.add_layer (PingusResource::load_surface ("menu/layer3", "core"), 0, 200, 50, 0);
  layer_manager.add_layer (PingusResource::load_surface ("menu/layer4", "core"), 0, 377, 100, 0);
  layer_manager.add_layer (PingusResource::load_surface ("menu/layer5", "core"), 0, 500, 200, 0);

  background = Blitter::scale_surface (background, CL_Display::get_width (), CL_Display::get_height ());
  //  background = PingusResource::load_surface("Textures/stones", "textures");
 
  event_enabled = false;
  
  //  CL_Input::chain_button_press.push_back(event);
  //  CL_Input::chain_button_release.push_back(event);
  //  CL_Input::chain_mouse_move.push_back(event);

  on_button_press_slot   = CL_Input::sig_button_press.connect (CL_CreateSlot(this, &PingusMenu::on_button_press));
  on_button_release_slot = CL_Input::sig_button_release.connect (CL_CreateSlot(this, &PingusMenu::on_button_release));
  //on_mouse_move_slot     = CL_Input::sig_mouse_move.connect (CL_CreateSlot(event, &PingusMenu::on_mouse_move));

  //on_resize_slot = CL_Display::sig_resized().connect(CL_CreateSlot(this, &PingusMenu::on_resize));

  buttons.push_back(boost::shared_ptr<SurfaceButton>(new OptionsButton ()));
  buttons.push_back(boost::shared_ptr<SurfaceButton>(new PlayButton ()));
  buttons.push_back(boost::shared_ptr<SurfaceButton>(new QuitButton (this)));
  buttons.push_back(boost::shared_ptr<SurfaceButton>(new MultiplayerButton ()));
  buttons.push_back(boost::shared_ptr<SurfaceButton>(new ThemeButton ()));
  buttons.push_back(editor_button);
}

PingusMenu::~PingusMenu()
{
  CL_Input::sig_button_press.disconnect (on_button_press_slot);
  CL_Input::sig_button_release.disconnect (on_button_release_slot);
  //CL_Input::sig_mouse_move.disconnect (on_mouse_move_slot);
}

void
PingusMenu::draw()
{
  /*
  // Filling the background with a texture
  for(int y = 0; y < CL_Display::get_height(); y += background->get_height())
    for(int x = 0; x < CL_Display::get_width(); x += background->get_width())
      background->put_screen(x, y);
  */
  //background->put_screen(0, 0, CL_Display::get_width(), CL_Display::get_height());
  layer_manager.draw ();
  //background->put_screen(0, 0);
  // Putting the logo
  //bg->put_screen(CL_Display::get_width()/2 - bg->get_width()/2, 3);

  for(std::list<boost::shared_ptr<SurfaceButton> >::iterator i = buttons.begin();
      i != buttons.end(); 
      i++)
    {
      //  std::cout << "PingusMenu: painting buttons: " <<  std::endl;
      // Mouse_over drawing is handled in SurfaceButton.cc
      (*i)->draw();
    }
  //std::cout << "PingusMenu: painting buttons: done" <<  std::endl;  
  Display::flip_display();
}

void
PingusMenu::select(void)
{
  if (!is_init) init();

  do_quit = false;

  draw();

  event_enabled = true;

  //Display::set_cursor(CL_MouseCursorProvider::load("Cursors/cursor",
  //						   PingusResource::get("game")));
  Display::show_cursor();

  PingusSound::play_mod("../data/music/pingus-1.it");
  
  unsigned int last_time = CL_System::get_time ();
  while(!do_quit) 
    {
      float delta = (CL_System::get_time () -  last_time) / 1000.0;

      layer_manager.update (delta);
      last_time = CL_System::get_time ();

      draw ();
      CL_System::keep_alive();
    }

  event_enabled = false;

  Display::hide_cursor();
}

void
PingusMenu::on_mouse_move(CL_InputDevice *, int mouse_x, int mouse_y)
{
  // if (!enabled) return;
  // draw();
}

void
PingusMenu::on_button_press(CL_InputDevice *device, const CL_Key &key)
{
  if (!event_enabled) return;

  draw();
  
  if (device == CL_Input::keyboards[0])
    {
      switch(key.id)
	{
	case CL_KEY_C:
	  {
	    event_enabled = false;
	    Credits credits;
	    credits.display();
	    draw();
	    event_enabled = true;
	  }
	  break;
	default:
	  std::cout << "PingusMenu: Unknown key pressed:" << key.ascii << std::endl;
	}
    }
}

void
PingusMenu::on_button_release(CL_InputDevice *device, const CL_Key &key)
{
  if (!event_enabled) return;
  
  draw();

  if (device == CL_Input::keyboards[0])
    {
      switch (key.id)
	{
	case CL_KEY_L:
	  break;
	case CL_KEY_F:
	  break;
	default:
	  if (verbose) std::cout << "PingusMenu::Event: Unknown key pressed: " << key.id << std::endl;
	  break;
	}
    }
  else if (device == CL_Input::pointers[0])
    {
      if (verbose) std::cout << "PingusMenu::Event: on_button_press" << std::endl;

      for(std::list<boost::shared_ptr<SurfaceButton> >::iterator i = buttons.begin(); 
	  i != buttons.end(); 
	  i++)
	{
	  // Mouse_over drawing is handled in SurfaceButton.cc
	  if ((*i)->mouse_over())
	    {
	      event_enabled = false;
	      Display::hide_cursor();
	      (*i)->on_click();
	      //Display::set_cursor(CL_MouseCursorProvider::load("Cursors/cursor", 
	      //PingusResource::get("game")));
	      Display::show_cursor();
	      event_enabled = true;
	    }
	}
    }
}

void
PingusMenu::on_resize(int w, int h)
{
  std::cout << "Width: " << w << " Height: " << h << std::endl;
}

/* EOF */
