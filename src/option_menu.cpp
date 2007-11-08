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

#include <set>
#include <boost/bind.hpp>
#include "globals.hpp"
#include "gettext.h"
#include "resource.hpp"
#include "screen/screen_manager.hpp"
#include "fonts.hpp"
#include "display/drawing_context.hpp"
#include "components/label.hpp"
#include "components/check_box.hpp"
#include "components/slider_box.hpp"
#include "components/choice_box.hpp"
#include "gui/gui_manager.hpp"
#include "tinygettext/dictionary_manager.hpp"
#include "option_menu.hpp"

extern TinyGetText::DictionaryManager dictionary_manager;

OptionMenu::OptionMenu()
{
  background = Resource::load_sprite("core/menu/optionmenu");
  ok_button  = Resource::load_sprite("core/start/ok");

  x_pos = 0;
  y_pos = 0;

  ChoiceBox* resolution_box = new ChoiceBox(Rect());
  resolution_box->add_choice("640x480");
  resolution_box->add_choice("800x480");
  resolution_box->add_choice("800x600");
  resolution_box->add_choice("1024x768");
  resolution_box->add_choice("1152x864");
  resolution_box->add_choice("1280x960");
  resolution_box->add_choice("1280x1024");
  resolution_box->add_choice("1600x1200");
  resolution_box->add_choice("1920x1080");

  ChoiceBox* language_box = new ChoiceBox(Rect());
  std::set<std::string> lst = dictionary_manager.get_languages();
  for (std::set<std::string>::iterator i = lst.begin(); i != lst.end(); ++i)
    language_box->add_choice(TinyGetText::get_language_def(*i)->name);

  ChoiceBox* scroll_box = new ChoiceBox(Rect());
  scroll_box->add_choice("Auto-Scroll");
  scroll_box->add_choice("Drag&Drop");
  scroll_box->add_choice("Rubberband");
  
  swcursor_box   = new CheckBox(Rect());
  fullscreen_box = new CheckBox(Rect());

  swcursor_box->on_change.connect(boost::bind(&OptionMenu::on_swcursor_change, this, _1));
  fullscreen_box->on_change.connect(boost::bind(&OptionMenu::on_fullscreen_change, this, _1));

  add_item(_("Resolution:"),      resolution_box);
  add_item(_("Fullscreen:"),      fullscreen_box);
  add_item(_("Allow Resize:"),    new CheckBox(Rect()));
  add_item(_("Fast Mode:"),       new CheckBox(Rect()));
  add_item(_("Frame Skip:"),      new CheckBox(Rect()));
  add_item(_("Software Cursor:"), swcursor_box);

  add_item(_("Language:"),        language_box);
  add_item(_("Master Volume:"),   new SliderBox(Rect()));
  add_item(_("Sound Volume:"),    new SliderBox(Rect()));
  add_item(_("Music Volume:"),    new SliderBox(Rect()));
  add_item(_("Scroll Mode:"),     scroll_box);
  add_item(_("Mouse Grab:"),      new CheckBox(Rect()));
}

void
OptionMenu::add_item(const std::string& label, GUI::RectComponent* control)
{
  gui_manager->add(new Label(label, Rect(Vector2i(120 + x_pos * 312, 177 + y_pos*32), 
                                         Size(140, 32))),
                             true);
  gui_manager->add(control, true);

  if (dynamic_cast<ChoiceBox*>(control))
    {
      control->set_rect(Rect(120 + x_pos * 312 + 140, 177 + y_pos*32,
                             120 + x_pos * 312 + 256, 177 + y_pos*32 + 32));                             
    }
  else if (dynamic_cast<SliderBox*>(control))
    {
      control->set_rect(Rect(120 + x_pos * 312 + 140, 177 + y_pos*32,
                             120 + x_pos * 312 + 256, 177 + y_pos*32 + 32));
    }
  else if (dynamic_cast<CheckBox*>(control))
    {
      control->set_rect(Rect(Vector2i(120 + x_pos * 312 + 156 + 32+28+8, 177 + y_pos*32), 
                             Size(32, 32)));
    }
  else
    {
      assert(!"Unhandled control type");
    }

  y_pos += 1;  
  if (y_pos > 5)
    {
      y_pos = 0; 
      x_pos += 1;
    }
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
  gc.draw(background, Vector2i(gc.get_width()/2 - background.get_width()/2, gc.get_height()/2 - background.get_height()/2));

  gc.print_center(Fonts::chalk_large, gc.get_width()/2, 90, "Option Menu");

  if (0)  
    {
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
          //gc.print_left(Fonts::chalk_normal,  120, y += 32, i->left);
          y += 32;
          if (i->right != "[X]")
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
          //gc.print_left(Fonts::chalk_normal,  gc.get_width()/2 + 32, y += 32, i->left);
          y += 32;
          if (i->right != "[X]")
            gc.print_right(Fonts::chalk_normal, gc.get_width()/2 + 280, y, i->right);
        }
    }

  gc.print_center(Fonts::chalk_normal, gc.get_width()/2 + 225 + 30, gc.get_height()/2 + 125 - 20, _("Close"));
  gc.draw(ok_button, Vector2i(gc.get_width()/2 + 225, gc.get_height()/2 + 125));

  gc.print_center(Fonts::chalk_normal, gc.get_width()/2, gc.get_height()/2 + 160, "Reset to Defaults [ ]");
  gc.print_left(Fonts::chalk_normal, gc.get_width()/2 - 280, gc.get_height()/2 + 160, "Save [ ]");
}

void
OptionMenu::on_escape_press()
{
  std::cout << "OptionMenu: poping screen" << std::endl;
  ScreenManager::instance()->pop_screen();
}

void
OptionMenu::resize(const Size&)
{
}

void
OptionMenu::on_swcursor_change(bool v)
{
  std::cout << "v: " << v << std::endl;
  swcursor_enabled = v;
}

void
OptionMenu::on_fullscreen_change(bool v)
{
  fullscreen_enabled = true;
}

/* EOF */
