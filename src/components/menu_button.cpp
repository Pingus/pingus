//  $Id$
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

#include "../globals.hpp"
#include "../debug.hpp"
#include "../sound/sound.hpp"
#include "../resource.hpp"
#include "../pingus_menu_manager.hpp"
#include "../gettext.h"

#include "menu_button.hpp"


MenuButton::MenuButton(PingusMenu* menu_,
                       const Vector2i& pos_,
                       const std::string& text_, const std::string& desc_)
  : menu(menu_)
{
  surface_p = Resource::load_sprite("core/menu/menuitem");

  text = text_;
  desc  = desc_;

  x_pos = pos_.x;
  y_pos = pos_.y;

  font       = Fonts::pingus_small;
  font_large = Fonts::chalk_large;

  mouse_over = false;
  pressed    = false;
}

MenuButton::MenuButton ()
{
  font       = Fonts::pingus_small;
  font_large = Fonts::pingus_large;

  mouse_over = false;
  pressed = false;

  //std::cout << "Generating font cache: " << std::flush;
  // We print all available letters, so that they are in the cache
  //font->print_left (0,0, "!\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~");
  //font_large->print_left (0,0, "!\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~");
  //std::cout << "done" << std::endl;
}

MenuButton::~MenuButton ()
{
}

void
MenuButton::on_click ()
{
  //std::cout << "MenuButton: Click" << std::endl;
  menu->on_click(this);
}

void
MenuButton::draw (DrawingContext& gc)
{
  if (mouse_over) // pressed
    {
      gc.draw(surface_p,Vector2i(x_pos, y_pos));
      gc.print_center(font_large, x_pos, y_pos - 28, text);
      // poor mans bold formating
      gc.print_center(font_large, x_pos-1, y_pos - 28 - 1, text);
      gc.print_center(font_large, x_pos+1, y_pos - 28 + 1, text);
      gc.print_center(font_large, x_pos+1, y_pos - 28 - 1, text);
      gc.print_center(font_large, x_pos-1, y_pos - 28 + 1, text);
    }
  else
    {
      gc.draw(surface_p, Vector2i(x_pos, y_pos));
      gc.print_center(font_large, x_pos, y_pos - 28, text);
    }
  UNUSED_ARG(gc);
}

void
MenuButton::update (float delta)
{
  UNUSED_ARG(delta);
}

void
MenuButton::on_pointer_enter ()
{
  mouse_over = true;
  Sound::PingusSound::play_sound ("tick");
  //std::cout << "X: " << this << "enter" << std::endl;
  menu->set_hint(desc);
}

void
MenuButton::on_pointer_leave ()
{
  //std::cout << "X: " << this << "leave" << std::endl;
  mouse_over = false;
  menu->set_hint("");
}

void
MenuButton::on_pointer_press ()
{
  pressed = true;
}

void
MenuButton::on_pointer_release ()
{
  pressed = false;
}

bool
MenuButton::is_at(int x, int y)
{
  return (x > x_pos - int(surface_p.get_width()) / 2
	  && x < x_pos + int(surface_p.get_width()) / 2
	  && y > y_pos - int(surface_p.get_height()) / 2
	  && y < y_pos + int(surface_p.get_height()) / 2);
}


/* EOF */
