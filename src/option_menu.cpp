//  $Id$
//
//  Pingus - A free Lemmings clone
//  Copyright (C) 2007 Ingo Ruhnke <grumbel@gmx.de>
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

#include "gettext.h"
#include "resource.hpp"
#include "screen/screen_manager.hpp"
#include "fonts.hpp"
#include "display/drawing_context.hpp"
#include "option_menu.hpp"

OptionMenu::OptionMenu()
{
  background = Resource::load_sprite("core/menu/optionmenu");
  ok_button  = Resource::load_sprite("core/start/ok");
}

OptionMenu::~OptionMenu()
{
}

void
OptionMenu::update(const GameDelta& delta)
{
  GUIScreen::update(delta);
  SDL_Delay(50);
}
  
struct OptionEntry {
  OptionEntry(const std::string& left_,
        const std::string& right_)
    : left(left_), right(right_)
  {}
  
  std::string left;
  std::string right;
};

void
OptionMenu::draw_background(DrawingContext& gc)
{
  // gc.draw_fillrect(Rect(100, 100, 400, 400), Color(255, 0, 0));
  gc.draw(background, gc.get_width()/2 - background.get_width()/2, gc.get_height()/2 - background.get_height()/2);

  gc.print_center(Fonts::chalk_large, gc.get_width()/2, 90, "Option Menu");
  
  std::vector<OptionEntry> strs;
  strs.push_back(OptionEntry("Resolution:",    "<800x600>"));
  strs.push_back(OptionEntry("Fullscreen:",    "[X]"));
  strs.push_back(OptionEntry("Allow Resize:",  "[X]"));
  strs.push_back(OptionEntry("Fast Mode:",     "[X]"));
  strs.push_back(OptionEntry("Frame Skip:",      "<5>"));
  strs.push_back(OptionEntry("Software Cursor:", "[X]"));

  int y = 145;
  for(std::vector<OptionEntry>::iterator i = strs.begin(); i != strs.end(); ++i)
    {
      gc.print_left(Fonts::chalk_normal,  120, y += 32, i->left);
      gc.print_right(Fonts::chalk_normal, gc.get_width()/2 - 32, y, i->right);
    }

  std::vector<OptionEntry> strs2;
  strs2.push_back(OptionEntry("Language:",        "<German>"));
  strs2.push_back(OptionEntry("Master Volume:", "[||||||||||||||||||||||||||||||]"));
  strs2.push_back(OptionEntry("Sound Volume:",  "[||||||||||||||||||||||||||||||]"));
  strs2.push_back(OptionEntry("Music Volume:",  "[||||||||||||||||||||||||||||||]"));
  strs2.push_back(OptionEntry("Scroll Mode:",     "<drag&drop>"));
  strs2.push_back(OptionEntry("Mouse Grab:",     "[X]"));
  //strs2.push_back(OptionEntry("Auto Online Updates:", "[X]"));

  y = 145;
  for(std::vector<OptionEntry>::iterator i = strs2.begin(); i != strs2.end(); ++i)
    {
      gc.print_left(Fonts::chalk_normal,  gc.get_width()/2 + 32, y += 32, i->left);
      gc.print_right(Fonts::chalk_normal, gc.get_width()/2 + 280, y, i->right);
    }


  gc.print_center(Fonts::chalk_normal, gc.get_width()/2 + 225 + 30, gc.get_height()/2 + 125 - 20, _("Close"));
  gc.draw(ok_button, gc.get_width()/2 + 225, gc.get_height()/2 + 125);

  gc.print_center(Fonts::chalk_normal, gc.get_width()/2, gc.get_height()/2 + 160, "Reset to Defaults [ ]");
  gc.print_left(Fonts::chalk_normal, gc.get_width()/2 - 280, gc.get_height()/2 + 160, "Save [ ]");
}

void
OptionMenu::on_escape_press()
{
  std::cout << "OptionMenu: poping screen" << std::endl;
  ScreenManager::instance()->pop_screen();
}

/* EOF */
