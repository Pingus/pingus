//  $Id: SurfaceButton.cc,v 1.9 2000/06/26 15:32:26 grumbel Exp $
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

#include "SurfaceButton.hh"

SurfaceButton::SurfaceButton()
{
  font = PingusResource::load_font("Fonts/pingus_small", "fonts");
}

SurfaceButton::~SurfaceButton()
{
}

void 
SurfaceButton::draw()
{
  if (mouse_over() && !CL_Mouse::left_pressed()) 
    {
      font->print_center(CL_Display::get_width() / 2, 10, desc.c_str());

      surface_p->put_screen(x_pos - surface_p->get_width()/2,
			    y_pos - surface_p->get_height()/2);
    }
  else if (mouse_over() && CL_Mouse::left_pressed()) 
    {
      float shrink = 0.9;
      
      font->print_center(CL_Display::get_width() / 2, 10, desc.c_str());

      surface_p->put_screen(x_pos - surface_p->get_width()/2 * shrink,
			    y_pos - surface_p->get_height()/2 * shrink,
			    shrink, shrink);
    } 
  else 
    {
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
  x_pos = CL_Display::get_width() * 500 / 640;
  y_pos = CL_Display::get_height() * 420 / 480;

  desc = "..:: Starts the level you played at last ::..";

  surface   = PingusResource::load_surface("Buttons/play", "menu");
  surface_p = PingusResource::load_surface("Buttons/play_p", "menu");
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
  x_pos = CL_Display::get_width() * 150 / 640; //150;
  y_pos = CL_Display::get_height() * 330 / 480; //330;

  desc = "..:: Brings you to the option menu ::..";

  surface   = PingusResource::load_surface("Buttons/options", "menu");
  surface_p = PingusResource::load_surface("Buttons/options_p", "menu");
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
  x_pos = CL_Display::get_width() * 500 / 640; //500;
  y_pos = CL_Display::get_height() * 320 / 480; //320;

  desc = "..:: Bye, bye ::..";

  surface   = PingusResource::load_surface("Buttons/quit", "menu");
  surface_p = PingusResource::load_surface("Buttons/quit_p", "menu");  
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
  x_pos = CL_Display::get_width() * 150 / 640; //150;
  y_pos = CL_Display::get_height() * 420 / 480; //420;

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
  x_pos = CL_Display::get_width() * 335 / 640; //335;
  y_pos = CL_Display::get_height() * 370 / 480; //370;

  desc = "..:: Launch the level editor ::..";

  surface   = PingusResource::load_surface("Buttons/editor", "menu");
  surface_p = PingusResource::load_surface("Buttons/editor_p", "menu");
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
  x_pos = CL_Display::get_width() * 150 / 640;  //150;
  y_pos = CL_Display::get_height() * 430 / 480; //430;

  desc = "..:: Let you select a world to start ::..";

  surface   = PingusResource::load_surface("Buttons/worlds", "menu");
  surface_p = PingusResource::load_surface("Buttons/worlds_p", "menu");     
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
