//  $Id: SurfaceButton.cc,v 1.12 2000/08/28 00:34:39 grumbel Exp $
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

#include "globals.hh"
#include "PingusResource.hh"
#include "Loading.hh"
#include "PingusGame.hh"
#include "OptionMenu.hh"
#include "System.hh"

#include "SurfaceButton.hh"

SurfaceButton::SurfaceButton()
{
  font = PingusResource::load_font("Fonts/pingus_small", "fonts");
  font_large = PingusResource::load_font("Fonts/pingus", "fonts");
}

SurfaceButton::~SurfaceButton()
{
}

void 
SurfaceButton::draw()
{
  if (mouse_over() && !CL_Mouse::left_pressed()) 
    {
      // font->print_center(CL_Display::get_width() / 2, 10, desc.c_str());
      font->print_center(CL_Display::get_width() / 2, 
			 CL_Display::get_height() - 20, 
			 System::translate(desc).c_str());

      surface->put_screen(x_pos - surface->get_width()/2,
			  y_pos - surface->get_height()/2);
      surface_p->put_screen(x_pos - surface_p->get_width()/2,
			    y_pos - surface_p->get_height()/2);

  if (System::translate(line2).empty())
    {
      font_large->print_center(x_pos + 32, 
			       y_pos - 32 - font_large->get_height()/2,
			       System::translate(line1).c_str());
    }
  else
    {
      font_large->print_center(x_pos + 32, y_pos - 32 - font_large->get_height(), 
			       System::translate(line1).c_str());
      font_large->print_center(x_pos + 32, y_pos - 32,
			       System::translate(line2).c_str());
    }
    }
  else if (mouse_over() && CL_Mouse::left_pressed()) 
    {
      float shrink = 0.9;
      
      // font->print_center(CL_Display::get_width() / 2, 10, desc.c_str());
      font->print_center(CL_Display::get_width() / 2, 
			 CL_Display::get_height() - 20, 
			 System::translate(desc).c_str());

      surface->put_screen(x_pos - surface->get_width()/2,
			  y_pos - surface->get_height()/2);

      surface_p->put_screen(x_pos - surface_p->get_width()/2 * shrink,
			    y_pos - surface_p->get_height()/2 * shrink,
			    shrink, shrink);
  if (System::translate(line2).empty())
    {
      font_large->print_center(x_pos + 32, 
			       y_pos - 32 - font_large->get_height()/2,
			       System::translate(line1).c_str());
    }
  else
    {
      font_large->print_center(x_pos + 32, y_pos - 32 - font_large->get_height(), 
			       System::translate(line1).c_str());
      font_large->print_center(x_pos + 32, y_pos - 32,
			       System::translate(line2).c_str());
    }
    } 
  else 
    {
      surface_p->put_screen(x_pos - surface_p->get_width()/2,
			    y_pos - surface_p->get_height()/2);

      surface->put_screen(x_pos - surface->get_width()/2,
			  y_pos - surface->get_height()/2);
    }
}

bool
SurfaceButton::mouse_over()
{
  if (CL_Mouse::get_x() > x_pos - int(surface->get_width()) / 2
      && CL_Mouse::get_x() < x_pos + int(surface->get_width()) / 2
      && CL_Mouse::get_y() > y_pos - int(surface->get_height()) / 2
      && CL_Mouse::get_y() < y_pos + int(surface->get_height()) / 2)
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

  surface   = PingusResource::load_surface("NewButtons/ice_off", "menu");
  //  surface   = PingusResource::load_surface("NewButtons/credits_off", "menu");
  surface_p = PingusResource::load_surface("NewButtons/credits_on", "menu");

  //surface   = PingusResource::load_surface("Buttons/play", "menu");
  //surface_p = PingusResource::load_surface("Buttons/play_p", "menu");
}

PlayButton::~PlayButton()
{
}

void 
PlayButton::on_click()
{
  loading_screen.draw();

  PingusGame game;
  game.start_game();
}

///////////////////////////////////////////////

OptionsButton::OptionsButton()
{
  // x_pos = CL_Display::get_width() * 150 / 640; //150;
  // y_pos = CL_Display::get_height() * 330 / 480; //330;

  x_pos = CL_Display::get_width() * 516 / 640; //150;
  y_pos = CL_Display::get_height() * 113 / 480; //330;

  desc["en"] = "..:: Brings you to the option menu ::..";
  desc["de"] = "..:: Einstellungen und Mogeleien ::..";
  
  line1["en"] = "Options";
  line1["de"] = "Einstellungen";

  surface   = PingusResource::load_surface("NewButtons/ice_off", "menu");
  //  surface   = PingusResource::load_surface("NewButtons/options_off", "menu");
  surface_p = PingusResource::load_surface("NewButtons/options_on", "menu");

  // surface   = PingusResource::load_surface("Buttons/options", "menu");
  // surface_p = PingusResource::load_surface("Buttons/options_p", "menu");
}

OptionsButton::~OptionsButton()
{
}

void 
OptionsButton::on_click()
{
  option_menu.display();
}

///////////////////////////////////////////////

QuitButton::QuitButton()
{
  // x_pos = CL_Display::get_width() * 500 / 640; 
  // y_pos = CL_Display::get_height() * 320 / 480; 

  x_pos = CL_Display::get_width() * 516 / 640; 
  y_pos = CL_Display::get_height() * 369 / 480; 

  desc["en"] = "..:: Bye, bye ::..";
  desc["de"] = "..:: Auf Wiedersehen ::..";

  line1["en"] = "Exit";
  line1["de"] = "Ausgang";

  // surface   = PingusResource::load_surface("Buttons/quit", "menu");
  // surface_p = PingusResource::load_surface("Buttons/quit_p", "menu");  

  //  surface   = PingusResource::load_surface("NewButtons/exit_off", "menu");
  surface   = PingusResource::load_surface("NewButtons/ice_off", "menu");
  surface_p = PingusResource::load_surface("NewButtons/exit_on", "menu");  
}

QuitButton::~QuitButton()
{
}

void
QuitButton::set_pingus_menu(PingusMenu* m)
{
  menu = m;
}

void
QuitButton::on_click()
{
  menu->do_quit = true;
}

///////////////////////////////////////////////

LoadButton::LoadButton()
{
  x_pos = CL_Display::get_width() * 226 / 640;
  y_pos = CL_Display::get_height() * 482 / 480; 

  surface   = PingusResource::load_surface("Buttons/load", "menu");
  surface_p = PingusResource::load_surface("Buttons/load_p", "menu");
}

LoadButton::~LoadButton()
{
}

void LoadButton::on_click()
{
  /*  std::string levelfile;
  levelfile = file.select(pingus_datadir, "*.plf");
  if (!levelfile.empty()) {
    PingusGame game;
    game.start(levelfile);
    }*/
}

EditorButton::EditorButton()
{
  // x_pos = CL_Display::get_width() * 335 / 640;
  // y_pos = CL_Display::get_height() * 370 / 480;

  x_pos = CL_Display::get_width() * 126 / 640;
  y_pos = CL_Display::get_height() * 113 / 480;

  desc["en"] = "..:: Launch the level editor ::..";
  desc["de"] = "..:: Den Level Editor starten ::..";

  line1["en"] = "Create a";
  line2["en"] = "Level";

  line1["de"] = "Level";
  line2["de"] = "Editor";

  // surface   = PingusResource::load_surface("Buttons/editor", "menu");
  // surface_p = PingusResource::load_surface("Buttons/editor_p", "menu");

  //  surface   = PingusResource::load_surface("NewButtons/ice_off", "menu");
  surface   = PingusResource::load_surface("NewButtons/ice_off", "menu");
  surface_p = PingusResource::load_surface("NewButtons/create_on", "menu");
}

EditorButton::~EditorButton()
{

}

void
EditorButton::on_click()
{
  editor.edit();

  while (CL_Keyboard::get_keycode(CL_KEY_ESCAPE))
    CL_System::keep_alive();
}

ThemeButton::ThemeButton()
{
  // x_pos = CL_Display::get_width() * 150 / 640; 
  // y_pos = CL_Display::get_height() * 430 / 480;

  x_pos = CL_Display::get_width() * 321 / 640; 
  y_pos = CL_Display::get_height() * 241 / 480;

  desc["en"] = "..:: Let you select a world to start ::..";
  desc["de"] = "..:: Such dir eine Welt zum beginnen aus ::..";

  line1["en"] = "Start";
  line1["de"] = "Start";

  // surface   = PingusResource::load_surface("Buttons/worlds", "menu");
  // surface_p = PingusResource::load_surface("Buttons/worlds_p", "menu");     

  surface   = PingusResource::load_surface("NewButtons/ice_off", "menu");
  //  surface   = PingusResource::load_surface("NewButtons/play_off", "menu");
  surface_p = PingusResource::load_surface("NewButtons/play_on", "menu");     
}

ThemeButton::~ThemeButton()
{
}

void
ThemeButton::on_click()
{
  theme_selector.select();
}

/* EOF */
