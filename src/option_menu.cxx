//  $Id: option_menu.cxx,v 1.12 2003/02/18 18:41:59 grumbel Exp $
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

#include <iostream>
#include <ClanLib/Display/Display/display.h>
#include <ClanLib/Display/Font/font.h>
#include <ClanLib/Display/Input/mouse.h>
#include <ClanLib/Display/Input/input.h>
#include <ClanLib/Display/Input/keyboard.h>
#include <ClanLib/Display/Display/mousecursor_provider.h>
#include "globals.hxx"
#include "pingus_resource.hxx"
#include "string_converter.hxx"
#include "pingus_menu_manager.hxx"
#include "display.hxx"
#include "cheat.hxx"
#include "my_gettext.hxx"

// Define the global option menu
OptionMenu   option_menu (0);

// ----- OptionEntry -----

OptionEntry::OptionEntry (const std::string& s, bool* v, int x, int y)
                        : value_bool(v),
			  value_int(0),
			  value_str(0),
			  font(PingusResource::load_font("Fonts/smallfont_h", "fonts")),
			  x_pos(x),
			  y_pos(y),
			  str(s)
{
}

OptionEntry::OptionEntry (const std::string& s, std::string* v, int x, int y)
                        : value_bool(0),
			  value_int(0),
			  value_str(v),
			  font(PingusResource::load_font("Fonts/smallfont_h", "fonts")),
			  x_pos(x),
			  y_pos(y),
			  str(s)
{
}

OptionEntry::OptionEntry (const std::string& s, int* v, int x, int y) 
                        : value_bool(0),
                          value_int(v),
			  value_str(0),
			  font(PingusResource::load_font("Fonts/smallfont_h", "fonts")),
			  x_pos(x),
			  y_pos(y),
			  str(s)
{
}

OptionEntry::OptionEntry (const OptionEntry& old) : value_bool(0),
                                                    value_int(0),
						    value_str(0),
						    font(new CL_Font(*(old.font))),
                                                    x_pos(old.x_pos),
					            y_pos(old.y_pos),
					            str(old.str)
{
  if (old.value_bool)
    value_bool = new bool(*(old.value_bool));

  if (old.value_int)
    value_int = new int(*(old.value_int));

  if (old.value_str)
    value_str = new std::string(*(old.value_str));
}

OptionEntry&
OptionEntry::operator= (const OptionEntry& old)
{
  if (this == &old)
    return *this;
    
  font       = new CL_Font(*(old.font));
  x_pos      = old.x_pos;
  y_pos      = old.y_pos;
  str        = old.str;
  
  if (old.value_bool)
    value_bool = new bool(*(old.value_bool));
  else
    value_bool = 0;

  if (old.value_int)
    value_int = new int(*(old.value_int));
  else
    value_int = 0;

  if (old.value_str)
    value_str = new std::string(*(old.value_str));
  else
    value_str = 0;
  
  return *this;
}

OptionEntry::~OptionEntry ()
{
  delete value_bool;
  delete value_int;
  delete value_str;
  delete font;
}

void
OptionEntry::draw(GraphicContext& gc)
{
  if (!mouse_over()) {
    CL_Display::fill_rect(x_pos - 7, y_pos - 1,
			  x_pos + CL_Display::get_width() - 35, y_pos + 11,
			  0.0, 0.0, 0.0, 1.0);
  } else {
    CL_Display::fill_rect(x_pos - 7, y_pos - 1,
			  x_pos + CL_Display::get_width() - 35, y_pos + 11,
			  1.0f, 0.0f, 0.0f, 0.4f);
  }
  font->print_left(x_pos, y_pos, str.c_str());

  if (value_bool) {
    if (*value_bool) {
      font->print_right(x_pos + CL_Display::get_width() - 35, y_pos, _("on"));
    } else {
      font->print_right(x_pos + CL_Display::get_width() - 35, y_pos, _("off"));
    }
  } else if (value_str) {
    font->print_right(x_pos + CL_Display::get_width() - 35, y_pos, value_str->c_str());    
  } else if (value_int) {

    font->print_right(x_pos + CL_Display::get_width() - 35, y_pos, to_string (*value_int).c_str ());
  }
  UNUSED_ARG(gc);
}

void
OptionEntry::toggle()
{
  if (value_bool) {
    *(value_bool) = !*(value_bool);
  } else if (value_str) {
    std::cout << _("Not implemented") << std::endl;
  } else if (value_int) {
    ++(*value_int);
  }
}

void
OptionEntry::rtoggle()
{
  if (value_bool) {
    *(value_bool) = !*(value_bool);
  } else if (value_str) {
    std::cout << _("Not implemented") << std::endl;
  } else if (value_int) {
    --(*value_int);    
  }
}

bool
OptionEntry::mouse_over()
{
  if (CL_Mouse::get_x() > x_pos && CL_Mouse::get_x() < x_pos + CL_Display::get_width() - 30
      && CL_Mouse::get_y() > y_pos && CL_Mouse::get_y() < y_pos + 15) {
    return true;
  } else {
    return false;
  }
}

OptionMenu::Event::Event ()
{
}

void
OptionMenu::Event::on_button_press(CL_InputDevice * device, const CL_Key & key)
{
  UNUSED_ARG(device);
  UNUSED_ARG(key);
}

void
OptionMenu::Event::on_button_release(CL_InputDevice *device, const CL_Key &key)
{
  if (device == CL_Input::keyboards[0])
    {
      switch(key.id)
	{
	case CL_KEY_ESCAPE:
	  option_menu->quit = true;
	  break;
	default:
	  if (verbose) std::cout << _("OptionMenu::Event: Unknown key released: id=") << key.id << std::endl;
	  break;
	} 
    }
  else if (device == CL_Input::pointers[0])
    {
      switch(key.id)
	{
	case CL_MOUSE_LEFTBUTTON:
	  if (verbose) std::cout << _("Button 0 pressed") << std::endl;
	  break;
	case CL_MOUSE_MIDDLEBUTTON:
	  option_menu->quit = true;
	  if (verbose) std::cout << _("Button 1 pressed") << std::endl;
	  break;
	case CL_MOUSE_RIGHTBUTTON:
	  if (verbose) std::cout << _("Button 2 pressed") << std::endl;
	  break;
	default:
	  if (verbose) std::cout << _("OptionMenu::Event: Unknown mouse button released: id=") << key.id << std::endl;
	  break;  
	}
    }
}

// ----- OptionMenu -----

OptionMenu::OptionMenu(PingusMenuManager* m)
  : PingusSubMenu (m)
{
  quit = false;
  is_init = false;

  event = new Event;
  event->option_menu = this;
}

OptionMenu::~OptionMenu()
{
  delete event;
}

void
OptionMenu::init() 
{
  if (is_init)
    return;
  //if (!preload_data)
    //loading_screen.draw();

  font       = PingusResource::load_font("Fonts/smallfont_h",  "fonts");
  title_font = PingusResource::load_font("Fonts/pingus", "fonts");
  background =  PingusResource::load_surface("Textures/rocktile", "textures");
  back       = PingusResource::load_surface("buttons/back2", "core");

  entry_x = 20;
  entry_y = 60;
  is_init = true;

  // Bool options
  add_entry(_("Gimmicks"),  &gimmicks_enabled);
  add_entry(_("Unlimited Actions"),  &Cheat::unlimited_actions);
  add_entry(_("Print fps"), &print_fps);
  //  add_entry("Debug actions", &debug_actions);  
  add_entry(_("Cursor enabled"), &cursor_enabled);  
  add_entry(_("Fast Mode"), &fast_mode);
  //add_entry(_("Pingus Datadir"), &pingus_datadir);
  add_entry(_("Game Speed"), &game_speed);
  add_entry(_("Verbose"), &verbose);
  add_entry(_("Music"), &music_enabled);
  add_entry(_("Sound"), &sound_enabled);
  add_entry(_("Level Previews"), &previews_enabled);
  add_entry(_("Draw ColMap"), &draw_collision_map);
  //add_entry(_("Debug GameTime"), &debug_game_time);
  //add_entry(_("Debug Tiles"), &debug_tiles);
  add_entry(_("Auto Scrolling"), &auto_scrolling);
  add_entry(_("Action Help Strings"), &action_help);
}

void
OptionMenu::add_entry (const std::string& e, bool* v)
{
  entry.push_back(OptionEntry(e, v, entry_x, entry_y));
  entry_y += 20;
}

void 
OptionMenu::add_entry (const std::string& e, int* v)
{
  entry.push_back(OptionEntry(e, v, entry_x, entry_y));
  entry_y += 20;
}

void 
OptionMenu::add_entry (const std::string& e, std::string* v)
{
  entry.push_back(OptionEntry(e, v, entry_x, entry_y));
  entry_y += 20;
}

void
OptionMenu::draw_background(GraphicContext& gc)
{
  UNUSED_ARG(gc);

  for(int y=0; y < CL_Display::get_height(); y += background.get_height()) {
    for(int x=0; x < CL_Display::get_width(); x += background.get_width()) {
      background.put_screen(x, y);
    }
  }
}

bool
OptionMenu::draw(GraphicContext& gc)
{  
  //draw_background();
  
  title_font->print_center(CL_Display::get_width() / 2, 10, _("Pingus Option Menu"));

  for(EntryIter item = entry.begin(); item != entry.end(); ++item) {
    item->draw(gc);
  }

  back.put_screen(CL_Display::get_width () - back.get_width (), 0);
  if (CL_Mouse::get_x() < (int)back.get_width()
      && CL_Mouse::get_y() < (int)back.get_height())
    CL_Display::fill_rect(0, 0, back.get_width(), back.get_height(),
			  1.0f, 1.0f, 1.0f, 0.3f);

  return true;
}

void
OptionMenu::check_click()
{
  for(EntryIter item = entry.begin(); item != entry.end(); ++item) {
    if (item->mouse_over()) {
      item->toggle();
      break;
    }
  }
}

void
OptionMenu::update (float /*delta*/)
{
  EntryIter item = EntryIter();
  item = current_item();
      
  if (CL_Mouse::left_pressed()) 
    {
      if (CL_Mouse::get_x() < (int)back.get_width() &&
	  CL_Mouse::get_y() < (int)back.get_height())
	{
	  quit = true;
	  if (manager) manager->set_menu (&manager->mainmenu);
	}

      if (item != EntryIter()) 
	item->toggle();
      //draw();
      while(CL_Mouse::left_pressed())
	CL_System::keep_alive();
    }

  if (CL_Mouse::right_pressed()) 
    {
      if (item != EntryIter()) 
	item->rtoggle();
      //draw();
      while(CL_Mouse::right_pressed())
	CL_System::keep_alive();
    }
}

void
OptionMenu::display()
{
  assert (!"kaputtt");
  if (!is_init)
    init();

  //int temp_index = -2;
  //int index = -2;
  EntryIter item = EntryIter();
  EntryIter temp_item = EntryIter();
  
  while(CL_Mouse::left_pressed())
    CL_System::keep_alive();

  quit = false;

  //CL_Input::chain_button_press.push_back(event);
  //CL_Input::chain_button_release.push_back(event);
  
  CL_Input::sig_button_press ().connect (event, &OptionMenu::Event::on_button_press);
  CL_Input::sig_button_release ().connect (event, &OptionMenu::Event::on_button_release);

  Display::set_cursor(CL_MouseCursorProvider::load("Cursors/cursor",
						   PingusResource::get("game")));
  Display::show_cursor();

  //draw();

  while(!quit)
    {
      {
	//draw();
	temp_item = item;
      }
      update (0.1f);
    }

  Display::hide_cursor();

  //CL_Input::chain_button_release.remove(event);
  //CL_Input::chain_button_press.remove(event);
}

std::vector<OptionEntry>::iterator
OptionMenu::current_item()
{
  for(EntryIter item = entry.begin(); item != entry.end(); ++item) {
    if (item->mouse_over()) {
      return item;
    }
  }
  return std::vector<OptionEntry>::iterator();
}

/* EOF */
