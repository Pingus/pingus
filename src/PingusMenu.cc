//  $Id: PingusMenu.cc,v 1.55 2002/06/05 17:51:08 grumbel Exp $
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
#include "Credits.hh"
// #include "FileSelector.hh"
#include "PingusResource.hh"
#include "PingusMessageBox.hh"
#include "PingusError.hh"
#include "PingusMenu.hh"
#include "Loading.hh"
#include "Display.hh"
#include "PingusSound.hh"
#include "Debug.hh"
#include "blitter.hh"

PingusMenu::PingusMenu(PingusMenuManager* m)
  : PingusSubMenu (m)
{
  is_init = false;
}

void
PingusMenu::preload ()
{
  if (!is_init)
    {
      pout << "PingusMenu::init ()" << std::endl;
      event_enabled = true;
      is_init = true;
      boost::shared_ptr<SurfaceButton> editor_button (new EditorButton (this));
 
      if (start_editor)
	{
	  
	  editor_button->on_click ();
	}

      background         = PingusResource::load_surface("misc/logo", "core");

      buttons.push_back(boost::shared_ptr<SurfaceButton>(new OptionsButton (this)));
      buttons.push_back(boost::shared_ptr<SurfaceButton>(new PlayButton (this)));
      buttons.push_back(boost::shared_ptr<SurfaceButton>(new QuitButton (this)));
      buttons.push_back(boost::shared_ptr<SurfaceButton>(new MultiplayerButton (this)));
      buttons.push_back(boost::shared_ptr<SurfaceButton>(new ThemeButton (this)));
      buttons.push_back(boost::shared_ptr<SurfaceButton>(new StoryButton (this)));
      buttons.push_back(editor_button);
    }

  // FIXME: Preload is probally the wrong place for this, there should
  // be show_hook and remove_hook
  PingusSound::play_music("../data/music/pingus-1.it");
}

PingusMenu::~PingusMenu()
{  
}

void
PingusMenu::draw()
{
  for(std::list<boost::shared_ptr<SurfaceButton> >::iterator i = buttons.begin();
      i != buttons.end(); ++i)
    (*i)->draw();
}

void
PingusMenu::on_mouse_move(CL_InputDevice *, int /*mouse_x*/, int /*mouse_y*/)
{
}

void
PingusMenu::on_button_press(CL_InputDevice *device, const CL_Key &key)
{
  if (!event_enabled) return;
  pout << "Buttonpress: " << event_enabled << std::endl;

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
	  pout << "PingusMenu: Unknown key pressed:" << key.ascii << std::endl;
	}
    }
}

void
PingusMenu::on_button_release(CL_InputDevice *device, const CL_Key &key)
{
  if (!event_enabled) return;
  pout << "Buttonrel: " << event_enabled << std::endl;
  
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
	  if (verbose) pout << "PingusMenu::Event: Unknown key pressed: " << key.id << std::endl;
	  break;
	}
    }
  else if (device == CL_Input::pointers[0])
    {
      if (verbose) pout << "PingusMenu::Event: on_button_press" << std::endl;

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
	      Display::show_cursor();
	      event_enabled = true;
	    }
	}
    }
}

void
PingusMenu::on_resize(int w, int h)
{
  pout << "Width: " << w << " Height: " << h << std::endl;
}

/* EOF */
