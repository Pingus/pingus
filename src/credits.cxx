//  $Id: credits.cxx,v 1.18 2003/04/09 23:35:28 grumbel Exp $
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
#include <ClanLib/Display/Font/font.h>
#include "gui/screen_manager.hxx"
#include "sound/sound.hxx"
#include "pingus_resource.hxx"
#include "credits.hxx"
#include "fonts.hxx"
#include "my_gettext.hxx"

Credits* Credits::instance_ = 0;

Credits::Credits()
{
  fast_scrolling = false;
  background = PingusResource::load_surface("menu/startscreenbg", "core");
  pingu = PingusResource::load_surface("misc/creditpingu", "core");

  font       = Fonts::chalk_normal;
  font_small = Fonts::chalk_large;

  // The credits vector holds the strings to display. The first
  // character of each string is a special character, which indicates
  // the size of the font or other special stuff. "-" means large
  // font, "_" is a small font and "n" means a newline.

  credits.push_back(_("-Idea"));
  credits.push_back("_Ingo Ruhnke");
  credits.push_back("n");

  credits.push_back(_("-Maintaining"));
  credits.push_back("_Ingo Ruhnke");
  credits.push_back("n");

  credits.push_back(_("-Programming"));
  credits.push_back("_David Philippi");
  credits.push_back("_Gervase Lam");
  credits.push_back("_Ingo Ruhnke");
  credits.push_back("n");

  credits.push_back(_("-Porting (Win32)"));
  credits.push_back("_Alberto Curro");
  credits.push_back("_Björn Christoph Fischer");
  credits.push_back("_Kenneth Gangstø");
  credits.push_back("_Michael KŽäser");
  credits.push_back("_Neil Mitchell");
  credits.push_back("n");

  credits.push_back(_("-Gfx"));
  credits.push_back("_Alan Tennent");
  credits.push_back("_Craig Timpany");
  credits.push_back("_Ingo Ruhnke");
  credits.push_back("_Joel Fauche");
  credits.push_back("_Mark Collinson");
  credits.push_back("_Michael Mestre");
  credits.push_back("_Stefan Stiasny");
  credits.push_back("_Tom Flavel");
  credits.push_back("n");

  credits.push_back(_("-Music"));
  credits.push_back("_H. Matthew Smith"); 
  credits.push_back("_Joseph Toscano");
  credits.push_back("_Yuri Sucupira");
  credits.push_back("n");

  credits.push_back(_("-Level Design"));
  credits.push_back("_Craig Timpany");
  credits.push_back("_Ingo Ruhnke");
  credits.push_back("n");

  credits.push_back(_("-Story"));
  credits.push_back("_Ingo Ruhnke");
  credits.push_back("n");

  credits.push_back(_("-Translation (german)"));
  credits.push_back("_David Philippi");
  credits.push_back("_Ingo Ruhnke");
  credits.push_back("n");

  credits.push_back(_("-Special"));
  credits.push_back(_("-Thanks to"));
  credits.push_back("_Bernhard Trummer");
  credits.push_back("_Cagri Coltekin");
  credits.push_back("_David Fauthoux");
  credits.push_back("_David Flores");
  credits.push_back("_David M. Turner");
  credits.push_back("_Felix Natter");
  credits.push_back("_Giray Devlet ");
  credits.push_back("_Giuseppe D'Aqui");
  credits.push_back("_Henri Manson");
  credits.push_back("_Jens Henrik Goebbert");
  credits.push_back("_Johnny Taporg");
  credits.push_back("_Jules Bean");
  credits.push_back("_Keir Fraser");
  credits.push_back("_Magnus Norddahl");
  credits.push_back("_Mattias Andersson");
  credits.push_back("_Michael Wand");
  credits.push_back("_Peter Todd");
  credits.push_back("_Richard Stallman");
  credits.push_back("_Rob Gietema");
  credits.push_back("_Robert Wittams");
  credits.push_back("_Robin Hogan");
  credits.push_back("_Shigeru Miyamoto");
  credits.push_back("_Stefan Ruhnke");
  credits.push_back("_Till Hellweg");
  credits.push_back("_Tuomas (Tigert) Kuosmanen");
  credits.push_back("_Werner Steiner");
  credits.push_back("n");
  credits.push_back("n");
  credits.push_back("n");
  credits.push_back("n");
  credits.push_back("n");

  credits.push_back(_("_And a very Special Thanks"));
  credits.push_back(_("_to all the people who"));
  credits.push_back(_("_contribute to"));
  credits.push_back(_("_Free Software!"));
  credits.push_back("n");
  credits.push_back("n");
  credits.push_back("n");
  credits.push_back("n");
  credits.push_back("n");
  credits.push_back(_("_Thank you for"));
  credits.push_back(_("_playing!"));

  end_offset = -350; // screen height + grace time 
  for (std::vector<std::string>::iterator i = credits.begin(); i != credits.end(); ++i)
    {
      switch ((*i)[0])
	{
	case '-':
	  end_offset += font->get_height() + 5;
	  break;
	case '_':
	  end_offset += font_small->get_height() + 5;
	  break;
	case 'n':
	  end_offset += 50;
	  break;
	default:
	  std::cout << "Credits: Syntax error: Unknown format: '" << (*i)[0] << "'" << std::endl;
	  break;
	}
    }
  end_offset = -end_offset;
}

Credits::~Credits ()
{
}

void 
Credits::update (float delta)
{
  if (offset < end_offset)
    {
      //ScreenManager::instance()->pop_screen ();
    }
  else
    {
      if (fast_scrolling)
        offset -= 450.0f * delta;
      else
        offset -= 35.0f * delta;
    }
}

void 
Credits::draw_background (GraphicContext& gc)
{
  int x;
  int y;
  int yof;

  x = CL_Display::get_width()/2;
  y = int(offset);

  gc.draw(background, 0, 0);
  
  gc.draw(pingu, (gc.get_width() / 2) - (pingu.get_width() / 2),
          (gc.get_height() / 2) - (pingu.get_height() / 2) - 20);

  /*CL_Display::fill_rect(CL_Display::get_width() / 2, 0,
			CL_Display::get_width(), CL_Display::get_height(),
			0.0, 0.0, 0.0, 1.0);*/

  CL_Display::push_clip_rect(CL_ClipRect(0, 75, 600, 500));
  yof = 0;
  for (std::vector<std::string>::iterator i = credits.begin(); i != credits.end(); ++i)
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
  CL_Display::pop_clip_rect();
}

void
Credits::on_startup ()
{
  offset = CL_Display::get_height() + 50;
  PingusSound::play_music("pingus-2.it");
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
Credits::init()
{
  instance_ = 0;
}

void 
Credits::deinit()
{
  delete instance_;
}

void
Credits::on_pause_press ()
{
}

void
Credits::on_fast_forward_press ()
{
  fast_scrolling = ! fast_scrolling;
}

void
Credits::on_escape_press ()
{
  ScreenManager::instance ()->pop_screen ();
}

/* EOF */
