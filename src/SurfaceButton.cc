//  $Id: SurfaceButton.cc,v 1.34 2001/11/22 20:08:34 grumbel Exp $
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

#include "my_gettext.hh"

#include "PingusSound.hh"
#include "globals.hh"
#include "PingusResource.hh"
#include "Loading.hh"
#include "OptionMenu.hh"
#include "System.hh"
#include "PingusMenuManager.hh"
#include "Credits.hh"
#include "editor/Editor.hh"

#include "SurfaceButton.hh"

SurfaceButton::SurfaceButton()
{
  font = PingusResource::load_font("Fonts/pingus_small", "fonts");
  font_large = PingusResource::load_font("Fonts/pingus", "fonts");

  is_mouse_over = false;
  //std::cout << "Generating font cache: " << std::flush;
  // We print all available letters, so that they are in the cache
  //font->print_left (0,0, "!\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~");
  //font_large->print_left (0,0, "!\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~");
  //std::cout << "done" << std::endl;
}

SurfaceButton::~SurfaceButton()
{
}

void 
SurfaceButton::draw()
{
  if (mouse_over() && !CL_Mouse::left_pressed()) 
    {
      if (!is_mouse_over) 
	{
	  PingusSound::play_sound ("sound/chain.wav", 0.1f);
	}

      is_mouse_over = true;
      // font->print_center(CL_Display::get_width() / 2, 10, desc.c_str());
      font->print_center(CL_Display::get_width() / 2, 
			 CL_Display::get_height() - 20, 
			 desc.c_str());

      //      surface->put_screen(x_pos - surface->get_width()/2,
      //			  y_pos - surface->get_height()/2);
      surface_p.put_screen(x_pos - surface_p.get_width()/2,
			   y_pos - surface_p.get_height()/2);

      if (line2.empty())
	{
	  font_large->print_center(x_pos + 32, 
				   y_pos - 32 - font_large->get_height()/2,
				   line1.c_str());
	}
      else
	{
	  font_large->print_center(x_pos + 32, y_pos - 32 - font_large->get_height(), 
				   line1.c_str());
	  font_large->print_center(x_pos + 32, y_pos - 32,
				   line2.c_str());
	}
    }
  else if (mouse_over() && CL_Mouse::left_pressed()) 
    {
      float shrink = 0.9f;
      
      // font->print_center(CL_Display::get_width() / 2, 10, desc.c_str());
      font->print_center(CL_Display::get_width() / 2, 
			 CL_Display::get_height() - 20, 
			 desc.c_str());

      //      surface->put_screen(x_pos - surface->get_width()/2,
      //		  y_pos - surface->get_height()/2);

      surface_p.put_screen(x_pos - surface_p.get_width()/2 * shrink,
			   y_pos - surface_p.get_height()/2 * shrink,
			   shrink, shrink);
      if (line2.empty())
	{
	  font_large->print_center(x_pos + 32, 
				   y_pos - 32 - font_large->get_height()/2,
				   line1.c_str());
	}
      else
	{
	  font_large->print_center(x_pos + 32, y_pos - 32 - font_large->get_height(), 
				   line1.c_str());
	  font_large->print_center(x_pos + 32, y_pos - 32,
				   line2.c_str());
	}
    } 
  else 
    {
      is_mouse_over = false;
      surface_p.put_screen(x_pos - surface_p.get_width()/2,
			   y_pos - surface_p.get_height()/2);

      //      surface->put_screen(x_pos - surface->get_width()/2,
      //			  y_pos - surface->get_height()/2);
    }
}

bool
SurfaceButton::mouse_over()
{
  assert (surface);

  if (CL_Mouse::get_x() > x_pos - int(surface.get_width()) / 2
      && CL_Mouse::get_x() < x_pos + int(surface.get_width()) / 2
      && CL_Mouse::get_y() > y_pos - int(surface.get_height()) / 2
      && CL_Mouse::get_y() < y_pos + int(surface.get_height()) / 2)
    {
      return true;
    }
  else
    {
      return false;
    }
}

///////////////////////////////////////////////

PlayButton::PlayButton()
{
  // x_pos = CL_Display::get_width() * 500 / 640;
  // y_pos = CL_Display::get_height() * 420 / 480;

  x_pos = CL_Display::get_width() * 126 / 640;
  y_pos = CL_Display::get_height() * 369 / 480;

  //  desc["en"] = "..:: Starts the level you played at last ::..";
  desc = _("..:: The people who brought this game to you ::..");
  //desc["de"] = "..:: Wer hat den dieses Spiel verbrochen...? ::..";
  
  line1 = _("Credits");
  //line1["de"] = "Credits";

  surface = PingusResource::load_surface("menu/ice_off", "core");
  surface_p = PingusResource::load_surface("menu/credits_on", "core");
  //  surface   = PingusResource::load_surface("NewButtons/credits_off", "menu");

  //surface   = PingusResource::load_surface("Buttons/play", "menu");
  //surface_p = PingusResource::load_surface("Buttons/play_p", "menu");
}

PlayButton::~PlayButton()
{
}

void 
PlayButton::on_click()
{
  Credits credits;
  credits.display();
  
  // FIXME: Hack, abusing the play button for the credits
  //  loading_screen.draw();

  /// PingusGame game;
  // game.start_game();
}

///////////////////////////////////////////////

OptionsButton::OptionsButton(PingusMenu* menu)
  : menu (menu)
{
  // x_pos = CL_Display::get_width() * 150 / 640; //150;
  // y_pos = CL_Display::get_height() * 330 / 480; //330;

  x_pos = CL_Display::get_width() * 516 / 640; //150;
  y_pos = CL_Display::get_height() * 113 / 480; //330;

  desc = _("..:: Takes you to the options menu ::..");
  //  desc["de"] = "..:: Einstellungen und Mogeleien ::..";
  
  line1 = _("Options");
  //line1["de"] = "Einstellungen";

  surface   = PingusResource::load_surface("menu/ice_off", "core");
  //  surface   = PingusResource::load_surface("NewButtons/options_off", "menu");
  surface_p = PingusResource::load_surface("menu/options_on", "core");

  // surface   = PingusResource::load_surface("Buttons/options", "menu");
  // surface_p = PingusResource::load_surface("Buttons/options_p", "menu");
}

OptionsButton::~OptionsButton()
{
}

void 
OptionsButton::on_click()
{
  //option_menu.display();
  menu->get_manager ()->set_menu (&menu->get_manager ()->optionmenu);
}

///////////////////////////////////////////////

QuitButton::QuitButton(PingusMenu* m)
  : menu (m)
{
  // x_pos = CL_Display::get_width() * 500 / 640; 
  // y_pos = CL_Display::get_height() * 320 / 480; 

  x_pos = CL_Display::get_width() * 516 / 640; 
  y_pos = CL_Display::get_height() * 369 / 480; 

  desc = _("..:: Bye, bye ::..");
  //desc["de"] = "..:: Auf Wiedersehen ::..";

  line1 = _("Exit");
  //line1["de"] = "Ausgang";

  // surface   = PingusResource::load_surface("Buttons/quit", "menu");
  // surface_p = PingusResource::load_surface("Buttons/quit_p", "menu");  

  //  surface   = PingusResource::load_surface("NewButtons/exit_off", "menu");
  surface   = PingusResource::load_surface("menu/ice_off", "core");
  surface_p = PingusResource::load_surface("menu/exit_on", "core");  
}

QuitButton::~QuitButton()
{
}

void
QuitButton::on_click()
{
  menu->get_manager ()->exit ();
}

///////////////////////////////////////////////

LoadButton::LoadButton()
{
  x_pos = CL_Display::get_width() * 226 / 640;
  y_pos = CL_Display::get_height() * 482 / 480; 

  surface   = PingusResource::load_surface("menu/load", "core");
  surface_p = PingusResource::load_surface("menu/load_p", "core");
}

LoadButton::~LoadButton()
{
}

void LoadButton::on_click()
{
  /*  std::string levelfile;
  levelfile = file.select(pXXXus_datadir, "*.plf");
  if (!levelfile.empty()) {
    PingusGame game;
    game.start(levelfile);
    }*/
}

EditorButton::EditorButton(  PingusMenu* menu)
  : menu (menu)
{
  // x_pos = CL_Display::get_width() * 335 / 640;
  // y_pos = CL_Display::get_height() * 370 / 480;

  x_pos = CL_Display::get_width() * 126 / 640;
  y_pos = CL_Display::get_height() * 113 / 480;

  desc = _("..:: Launch the level editor ::..");
  //desc["de"] = "..:: Den Level Editor starten ::..";

  line1 = _("Create a");
  line2 = _("Level");

  //line1["de"] = "Level";
  //line2["de"] = "Editor";

  // surface   = PingusResource::load_surface("Buttons/editor", "menu");
  // surface_p = PingusResource::load_surface("Buttons/editor_p", "menu");

  //  surface   = PingusResource::load_surface("NewButtons/ice_off", "menu");
  surface   = PingusResource::load_surface("menu/ice_off", "core");
  surface_p = PingusResource::load_surface("menu/create_on", "core");
}

EditorButton::~EditorButton()
{

}

void 
EditorButton::load_level(const std::string& str)
{
  Editor::instance ()->load_level (str);
}

void
EditorButton::on_click()
{
  menu->get_manager ()->disable_events ();
  Editor::instance()->edit();
  menu->get_manager ()->enable_events ();
}

StoryButton::StoryButton (PingusMenu* menu)
  : menu (menu)
{
  // x_pos = CL_Display::get_width() * 150 / 640; 
  // y_pos = CL_Display::get_height() * 430 / 480;

  x_pos = CL_Display::get_width() * 321 / 640; 
  y_pos = CL_Display::get_height() * 241 / 480;

  desc = _("..:: Lets you select a world to start ::..");
  //desc["de"] = "..:: Such dir eine Welt zum beginnen aus ::..";

  line1 = _("Start");
  // surface   = PingusResource::load_surface("Buttons/worlds", "menu");
  // surface_p = PingusResource::load_surface("Buttons/worlds_p", "menu");     

  surface   = PingusResource::load_surface("menu/ice_off", "core");
  //  surface   = PingusResource::load_surface("NewButtons/play_off", "menu");
  surface_p = PingusResource::load_surface("menu/play_on", "core");     
}

StoryButton::~StoryButton() {}

void 
StoryButton::on_click()
{
  PingusSound::play_sound ("sound/letsgo.wav");
  
  menu->get_manager ()->set_menu (&menu->get_manager ()->story);
  //worldmap_manager.display();
}

ThemeButton::ThemeButton ()
{
  x_pos = CL_Display::get_width() * 321 / 640; 
  y_pos = CL_Display::get_height() * 100 / 480;

  desc = _("..:: Start a contrib level ::..");

  line1 = _("Contrib");
  surface   = PingusResource::load_surface("menu/ice_off", "core");
  surface_p = PingusResource::load_surface("menu/play_on", "core");
}

void
ThemeButton::on_click()
{
  PingusSound::play_sound ("sound/letsgo.wav");
  
  //menu->get_manager ()->set_menu (&menu->get_manager ()->story);

  ThemeSelector theme_selector;
  theme_selector.display();
   //worldmap_manager.display();
}

MultiplayerButton::MultiplayerButton ()
{
  x_pos = CL_Display::get_width() * 320 / 640;
  y_pos = CL_Display::get_height() * 369 / 480;

  desc = _("..:: Multiplayer Modes... experimental stuff ::..");
    
  line1 = _("Multi");
  
  surface = PingusResource::load_surface("menu/ice_off", "core");
  surface_p = PingusResource::load_surface("menu/multi_on", "core");
}
 
MultiplayerButton::~MultiplayerButton ()
{
}
 
void
MultiplayerButton::on_click()
{
  //std::cout << "MultiplayerButton: Launching MultiplayerGame" << std::endl;
  multiplayer_config.display ();
}

/* EOF */
