//  $Id: SurfaceButton.cc,v 1.3 2000/02/11 16:58:26 grumbel Exp $
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
#include "PingusGame.hh"
#include "OptionMenu.hh"

#include "SurfaceButton.hh"

SurfaceButton::SurfaceButton()
{
}

SurfaceButton::~SurfaceButton()
{
}

void 
SurfaceButton::draw()
{
  if (mouse_over() && !CL_Mouse::left_pressed()) 
    {
      surface_p->put_screen(x_pos - surface_p->get_width()/2,
			    y_pos - surface_p->get_height()/2);
    }
  else if (mouse_over() && CL_Mouse::left_pressed()) 
    {
      float shrink = 0.9;
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
  x_pos = 500;
  y_pos = 420;
  surface   = CL_Surface::load("Buttons/play", PingusResource::get("menu.dat"));
  surface_p = CL_Surface::load("Buttons/play_p", PingusResource::get("menu.dat"));
}

PlayButton::~PlayButton()
{
}

void 
PlayButton::on_click()
{
  PingusGame game;
  game.start();
}

///////////////////////////////////////////////

OptionsButton::OptionsButton()
{
  x_pos = 150;
  y_pos = 330;
  surface   = CL_Surface::load("Buttons/options", PingusResource::get("menu.dat"));
  surface_p = CL_Surface::load("Buttons/options_p", PingusResource::get("menu.dat"));
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
  x_pos = 500;
  y_pos = 320;
  surface   = CL_Surface::load("Buttons/quit", PingusResource::get("menu.dat"));
  surface_p = CL_Surface::load("Buttons/quit_p", PingusResource::get("menu.dat"));  
}

QuitButton::~QuitButton()
{
}

void QuitButton::on_click()
{
  
}

///////////////////////////////////////////////

LoadButton::LoadButton()
{
  x_pos = 150;
  y_pos = 420;
  surface   = CL_Surface::load("Buttons/load", PingusResource::get("menu.dat"));
  surface_p = CL_Surface::load("Buttons/load_p", PingusResource::get("menu.dat"));
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
  x_pos = 335;
  y_pos = 370;
  surface   = CL_Surface::load("Buttons/editor", PingusResource::get("menu.dat"));
  surface_p = CL_Surface::load("Buttons/editor_p", PingusResource::get("menu.dat"));
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
  x_pos = 150;
  y_pos = 430;
  surface   = CL_Surface::load("Buttons/theme", PingusResource::get("menu.dat"));
  surface_p = CL_Surface::load("Buttons/theme_p", PingusResource::get("menu.dat"));     
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
