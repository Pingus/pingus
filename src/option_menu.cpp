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
#include "config_manager.hpp"
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

#define C(x) x

extern TinyGetText::DictionaryManager dictionary_manager;

OptionMenu::OptionMenu()
{
  background = Resource::load_sprite("core/menu/optionmenu");
  ok_button  = Resource::load_sprite("core/start/ok");

  x_pos = 0;
  y_pos = 0;

  ChoiceBox* resolution_box = new ChoiceBox(Rect());
  resolution_box->add_choice("640x480");   // 4:3, VGA
  resolution_box->add_choice("768x576");   // 4:3, PAL
  resolution_box->add_choice("800x480");   // Nokia N770, N800
  resolution_box->add_choice("800x600");   // 4:3, SVGA
  resolution_box->add_choice("1024x768");  // 4:3, XGA
  resolution_box->add_choice("1152x864");  // 4:3
  resolution_box->add_choice("1280x720");  // 16:9, HD-TV, 720p
  resolution_box->add_choice("1280x960");  // 4:3
  resolution_box->add_choice("1280x1024"); // 5:4
  resolution_box->add_choice("1366x768");  // ~16:9, Wide XGA
  resolution_box->add_choice("1440x900");  // 16:10
  resolution_box->add_choice("1600x1200"); // 4:3, UXGA
  resolution_box->add_choice("1680x1050"); // 16:10, WSXGA
  resolution_box->add_choice("1920x1080"); // 16:9, HD-TV, 1080p
  resolution_box->add_choice("1920x1200"); // 16:10
  
  resolution_box->add_choice("Custom");

  ChoiceBox* language_box = new ChoiceBox(Rect());
  std::set<std::string> lst = dictionary_manager.get_languages();
  for (std::set<std::string>::iterator i = lst.begin(); i != lst.end(); ++i)
    {
      TinyGetText::LanguageDef* lang = TinyGetText::get_language_def(*i);
      if (lang)
        language_box->add_choice(lang->name);
    }

  ChoiceBox* scroll_box = new ChoiceBox(Rect());
  scroll_box->add_choice("Drag&Drop");
  scroll_box->add_choice("Rubberband");
  
  swcursor_box      = new CheckBox(Rect());
  fullscreen_box    = new CheckBox(Rect());
  autoscroll_box    = new CheckBox(Rect());
  fast_mode_box     = new CheckBox(Rect());
  mousegrab_box     = new CheckBox(Rect());
  printfps_box      = new CheckBox(Rect());

  master_volume_box = new SliderBox(Rect());
  sound_volume_box  = new SliderBox(Rect());
  music_volume_box  = new SliderBox(Rect());

  swcursor_box->on_change.connect(boost::bind(&OptionMenu::on_swcursor_change, this, _1));
  fullscreen_box->on_change.connect(boost::bind(&OptionMenu::on_fullscreen_change, this, _1));
  autoscroll_box->on_change.connect(boost::bind(&OptionMenu::on_autoscroll_change, this, _1));
  fast_mode_box->on_change.connect(boost::bind(&OptionMenu::on_fastmode_change, this, _1));
  mousegrab_box->on_change.connect(boost::bind(&OptionMenu::on_mousegrab_change, this, _1));
  printfps_box->on_change.connect(boost::bind(&OptionMenu::on_printfps_change, this, _1));

  master_volume_box->on_change.connect(boost::bind(&OptionMenu::on_master_volume_change, this, _1));
  sound_volume_box->on_change.connect(boost::bind(&OptionMenu::on_sound_volume_change, this, _1));
  music_volume_box->on_change.connect(boost::bind(&OptionMenu::on_music_volume_change, this, _1));

  resolution_box->on_change.connect(boost::bind(&OptionMenu::on_resolution_change, this, _1));

  add_item(_("Language:"),        language_box);
  add_item(_("Scroll Mode:"),     scroll_box);
  add_item(_("Resolution:"),      resolution_box);
  add_item(_("Fullscreen:"),      fullscreen_box);
  add_item(_("Autoscrolling:"),   autoscroll_box);
  add_item(_("Low Detail:"),      fast_mode_box);

  add_item(_("Master Volume:"),   master_volume_box);
  add_item(_("Sound Volume:"),    sound_volume_box);
  add_item(_("Music Volume:"),    music_volume_box);
  add_item(_("Print FPS:"),       printfps_box);
  add_item(_("Mouse Grab:"),      mousegrab_box);
  add_item(_("Software Cursor:"), swcursor_box);

  // Connect with ConfigManager
  mousegrab_box->set_state(config_manager.get_mouse_grab(), false);
  C(config_manager.on_mouse_grab_change.connect(boost::bind(&CheckBox::set_state, mousegrab_box, _1, false)));

  printfps_box->set_state(config_manager.get_print_fps(), false);
  C(config_manager.on_print_fps_change.connect(boost::bind(&CheckBox::set_state, printfps_box, _1, false)));

  fullscreen_box->set_state(config_manager.get_fullscreen(), false);
  C(config_manager.on_fullscreen_change.connect(boost::bind(&CheckBox::set_state, fullscreen_box, _1, false)));

  swcursor_box->set_state(config_manager.get_swcursor(), false);
  C(config_manager.on_swcursor_change.connect(boost::bind(&CheckBox::set_state, swcursor_box, _1, false)));

  autoscroll_box->set_state(config_manager.get_autoscroll(), false);
  C(config_manager.on_autoscroll_change.connect(boost::bind(&CheckBox::set_state, autoscroll_box, _1, false)));

  fast_mode_box->set_state(config_manager.get_fast_mode(), false);
  C(config_manager.on_fast_mode_change.connect(boost::bind(&CheckBox::set_state, fast_mode_box, _1, false)));
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
  for(Connections::iterator i = connections.begin(); i != connections.end(); ++i)
    {
      (*i).disconnect();
    }
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
  gc.fill_screen(Color(0, 0, 0));

  // gc.draw_fillrect(Rect(100, 100, 400, 400), Color(255, 0, 0));
  gc.draw(background, Vector2i(gc.get_width()/2 - background.get_width()/2, gc.get_height()/2 - background.get_height()/2));

  gc.print_center(Fonts::chalk_large, gc.get_width()/2, 90, "Option Menu");

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
OptionMenu::resize(const Size& size)
{
  
}

void
OptionMenu::on_swcursor_change(bool v)
{
  config_manager.set_swcursor(v);
}

void
OptionMenu::on_fullscreen_change(bool v)
{
  config_manager.set_fullscreen(v);
}

void
OptionMenu::on_autoscroll_change(bool v)
{
  config_manager.set_autoscroll(v);
}

void
OptionMenu::on_fastmode_change(bool v)
{
  config_manager.set_fast_mode(v);
}

void
OptionMenu::on_mousegrab_change(bool v)
{
  config_manager.set_mouse_grab(v);
}

void
OptionMenu::on_printfps_change(bool v)
{
  config_manager.set_print_fps(v);
}

void
OptionMenu::on_master_volume_change(int v)
{
  config_manager.set_master_volume(v);
}

void
OptionMenu::on_sound_volume_change(int v)
{
  config_manager.set_sound_volume(v);
}

void
OptionMenu::on_music_volume_change(int v)
{
  config_manager.set_music_volume(v);
}

void
OptionMenu::on_resolution_change(const std::string& str)
{
  if (str != "Custom")
    {
      Size size;
      if (sscanf(str.c_str(), "%dx%d", &size.width, &size.height) == 2)
        {
          config_manager.set_resolution(size); 
        }
    }
}

/* EOF */
