//  $Id: credits.cxx,v 1.4 2002/08/16 17:15:31 grumbel Exp $
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
#include <ClanLib/Display/Input/keyboard.h>
#include <ClanLib/Display/Font/font.h>
#include <ClanLib/Display/Input/key.h>
#include "delta_manager.hxx"
#include "display.hxx"
#include "sound.hxx"
#include "pingus_resource.hxx"
#include "credits.hxx"
#include "game_delta.hxx"
#include "screen_manager.hxx"

/* Headers needed for i18n / gettext */
#include <clocale>
#include <config.h>
#include "my_gettext.hxx"

Credits* Credits::instance_ = 0;

Credits::Credits() 
  : is_init(false)
{
}

///
void
Credits::init ()
{
  if (!is_init)
    {
      is_init = true;
      
      fast_scrolling = false;
      surface = PingusResource::load_surface("Game/pingubw", "game");
      font = PingusResource::load_font("Fonts/pingus","fonts");
      font_small = PingusResource::load_font("Fonts/pingus_small","fonts");

      // The credits vector holds the strings to display. The first
      // character of each string is a special character, which indicates
      // the size of the font or other special stuff. "-" means large
      // font, "_" is a small font and "n" means a newline.

      credits.push_back(_("_If I forgot somebody in this"));
      credits.push_back(_("_list, just drop me a line and"));
      credits.push_back(_("_I'll fix this."));
      credits.push_back(_("_Last Updated: 2000/06/27"));
      credits.push_back(_("n"));

      credits.push_back(_("-Programming"));
      credits.push_back("_Ingo Ruhnke");
      credits.push_back("_Michael K\"aser");
      credits.push_back("_Peter Todd");
      credits.push_back("n");

      credits.push_back(_("-Gfx"));
      credits.push_back("_Stefan Stiasny");
      credits.push_back("_Alan Tennent");
      credits.push_back("_Craig Timpany");
      credits.push_back("_Joel Fauche");
      credits.push_back("_Michael Mestre");
      credits.push_back("n");

      credits.push_back(_("-Music"));
      credits.push_back("_H. Matthew Smith"); 
      credits.push_back("_Joseph Toscano");
      credits.push_back("n");

      credits.push_back(_("-Special"));
      credits.push_back(_("-Thanks to"));
      credits.push_back("_Jules Bean");
      credits.push_back("_David Philippi");
      credits.push_back("_Kenneth Gangstø");
      credits.push_back("_Johnny Taporg");
      credits.push_back("_Werner Steiner");
      credits.push_back("_Kenneth Gangstø");
      credits.push_back("_Tuomas (Tigert) Kuosmanen");
      credits.push_back("_Keir Fraser");  

      credits.push_back("n");

      credits.push_back("-### END ###");
    }
}

Credits::~Credits()
{
  
}

void 
Credits::update (const GameDelta& delta)
{
  if (fast_scrolling)
    offset -= 250.0f * delta.get_time ();
  else
    offset -= 50.0f * delta.get_time ();

  if (offset < -1200.0f)
    ScreenManager::instance()->pop_screen ();
}

void 
Credits::draw_background ()
{
  int x;
  int y;
  int yof;

  x = CL_Display::get_width() * 3 / 4;
  y = int(offset);

  CL_Display::clear_display(1.0, 1.0, 1.0);
      
  surface.put_screen((CL_Display::get_width() / 4) - (surface.get_width() / 2),
		     (CL_Display::get_height() / 2) - (surface.get_height() / 2));

  CL_Display::fill_rect(CL_Display::get_width() / 2, 0,
			CL_Display::get_width(), CL_Display::get_height(),
			0.0, 0.0, 0.0, 1.0);

  yof = 0;
  for (std::vector<std::string>::iterator i = credits.begin(); i != credits.end(); i++)
    {
      switch ((*i)[0])
	{
	case '-':
	  font->print_center(x, y + yof, i->substr(1).c_str());
	  yof += font->get_height() + 5;
	  break;
	case '_':
	  font_small->print_center(x, y + yof, i->substr(1).c_str());
	  yof += font_small->get_height() + 5;
	  break;
	case 'n':
	  yof += 50;
	  break;
	default:
	  std::cout << "Credits: Syntax error: Unknown format: '" << (*i)[0] << "'" << std::endl;
	  break;
	}
    }
}

void
Credits::on_startup ()
{
  init ();

  offset = CL_Display::get_height() + 50;

  PingusSound::play_music("music/pingus-2.it");
}

Credits*
Credits::instance ()
{
  if (instance_)
    return instance_;
  else
    return instance_ = new Credits ();
}

void
Credits::on_pause_press ()
{
}

void
Credits::on_fast_forward_press ()
{
  fast_scrolling != fast_scrolling;
}

void
Credits::on_escape_press ()
{
  ScreenManager::instance ()->pop_screen ();
}

/* EOF */
