//  $Id: editor.cxx,v 1.14 2002/07/01 16:10:29 torangan Exp $
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

#include <cstdio>
#include <string>

#include <ClanLib/gui.h>

#include <ClanLib/Core/System/system.h>
#include <ClanLib/Core/Math/rect.h>
#include <ClanLib/Display/Display/display.h>
#include <ClanLib/Display/Input/input.h>
#include <ClanLib/Display/Display/mousecursor_provider.h>
#include <ClanLib/Display/Input/mouse.h>
#include <ClanLib/Display/Font/font.h>
#include <ClanLib/Display/Input/inputbuffer.h>
#include <ClanLib/Display/Input/keyboard.h>
#include "../algo.hxx"
#include "../globals.hxx"
#include "../pingus_error.hxx"
#include "../message_box.hxx"
#include "../display.hxx"
#include "../pingus_resource.hxx"
#include "../system.hxx"
#include "editor.hxx"
#include "editor_event.hxx"
#include "panel.hxx"
#include "scroll_map.hxx"
#include "status_line.hxx"
#include "object_selector.hxx"
#include "object_manager.hxx"
#include "selection.hxx"
#include "action_window.hxx"
#include "property_window.hxx"
#include "level_property_window.hxx"

using namespace Pingus::Editor;

Editor* Editor::instance_ = 0;

Editor*
Editor::instance ()
{ 
  if ( ! instance_)
    instance_ = new Editor ();
  
  return instance_; 
}

Editor::Editor () : event_handler_ref_counter(0),
                    move_x(0), move_y(0), show_help_screen(true), tool(SELECTOR_TOOL)
{
  EditorObj::set_editor(this);
  event      = new EditorEvent;

  panel      = new Panel;
  scroll_map = new ScrollMap;
  object_manager  = new ObjectManager;
  selection       = new EditorSelection(object_manager);
  status_line     = new StatusLine;
  object_selector = new ObjectSelector;

  // FIXME: Should use PingusResource, Memleak
  CL_ResourceManager* gui_resources = new CL_ResourceManager("../data/data/gui.scr", false);
  
  style_manager = new CL_StyleManager_Default (gui_resources);
  gui   = new CL_GUIManager (style_manager);

  action_window = new ActionWindow (gui, object_manager->get_actions ());
  property_window = new Pingus::Editor::PropertyWindow (gui);
  level_property_window = new Pingus::Editor::LevelPropertyWindow (gui, object_manager);

  view = new EditorView (0, 0, CL_Display::get_width (), CL_Display::get_height (), 0, 0);

  event->set_editor(this);
  
  font = PingusResource::load_font("Fonts/courier_small", "fonts");
  panel->init();
  status_line->set_current_objs(&selection->get_current_objs());
  panel->set_editor(this);
  scroll_map->editor_event = event;
}

Editor::~Editor ()
{
  delete object_manager;
  delete status_line;
  delete object_selector;

  delete scroll_map;
  delete event;
  delete panel;
}

void
Editor::register_event_handler()
{
  if (event_handler_ref_counter == 0)
    {
      event_handler_ref_counter++;
      CL_System::keep_alive();
      if (verbose) std::cout << "Editor: Registering event handler..." << event << std::flush; 

      //CL_Input::chain_button_press.push_back(event);
      //CL_Input::chain_button_release.push_back(event);

      on_button_press_slot
	= CL_Input::sig_button_press ().connect(event, &EditorEvent::on_button_press);
      on_button_release_slot
	= CL_Input::sig_button_release ().connect(event, &EditorEvent::on_button_release);

      if (verbose) std::cout << "done: " << event_handler_ref_counter << std::endl;
    }
  else
    {
      if (verbose) std::cout << "Editor: event_handler already installed" << std::endl;
    }
}

void
Editor::unregister_event_handler()
{
  event_handler_ref_counter--;
  if (verbose) std::cout << "Editor: unregistering event handler" << event << "... " << std::flush; 

  //CL_Input::chain_button_release.remove(event);
  //CL_Input::chain_button_press.remove(event);

  CL_Input::sig_button_press ().disconnect (on_button_press_slot);
  CL_Input::sig_button_release ().disconnect (on_button_release_slot);

  CL_System::keep_alive();
  if (verbose) std::cout << "done: " << event_handler_ref_counter << std::endl;
}

void
Editor::edit ()
{
  quit = false;

  Display::set_cursor(CL_MouseCursorProvider::load("cursors/cursor", PingusResource::get("core")));
  
  Display::show_cursor();

  register_event_handler();

  std::list<boost::shared_ptr<EditorObj> > tmp_selection;

  while (!quit) 
    {
      // FIXME: This busy loop should be replaced by redraw events
      CL_System::sleep (20);
      CL_System::keep_alive();
      move_objects();
      draw();
      if (get_gui_manager ()->get_focus () == get_gui_manager ())
	CL_Display::draw_rect (25, 0, CL_Display::get_width (), CL_Display::get_height (),
			       1.0f, 1.0f, 1.0f);
      gui->show ();
      //std::cout << "GUI has Focus: " << (gui->get_focus () !=  gui) << std::endl;
      Display::flip_display(true);

      // FIXME: This should be moved to the object manager
      if (tmp_selection != object_manager->current_objs)
	{
	  std::cout << "Selection changed" << std::endl;
	  tmp_selection = object_manager->current_objs;

	  // FIXME: dirty hack
	  if (object_manager->current_objs.size() == 1)
	    {
	      boost::shared_ptr<EditorObj>  obj = *object_manager->current_objs.begin ();
	      property_window->update_frame (obj);
	      //CL_Component* comp = obj->get_gui_dialog (editor->property_window);
	    }
	  else
	    {
	      property_window->update_frame (boost::shared_ptr<EditorObj>());
	      std::cout << "EditorEvent::editor_show_object_properties (): error: multiple objects selected" << std::endl;
	    }
	}
    }

  unregister_event_handler();

  Display::hide_cursor();
}


void
Editor::draw ()
{
  CL_Display::clear_display();

  object_manager->draw(view);
  panel->draw();

  /*  {
      int x1_pos = CL_Display::get_width() - 200;
      int y1_pos = CL_Display::get_height() - 150;
      int x2_pos = CL_Display::get_width() - 1;
      int y2_pos = CL_Display::get_height() - 1;
      int width  = (CL_Display::get_width() - 25) * 200 / object_manager.width;
      int height = CL_Display::get_height() * 150 / object_manager.height;;

      Display::draw_rect(x1_pos, y1_pos , x2_pos, y2_pos,
      1.0, 1.0, 1.0, 1.0);

      Display::draw_rect(x1_pos - (object_manager.x_offset * 200 / object_manager.width),
      y1_pos - (object_manager.y_offset * 150 / object_manager.height),
      x1_pos - (object_manager.x_offset * 200 / object_manager.width) + width,  
      y1_pos - (object_manager.y_offset * 150 / object_manager.height) + height,
      1.0, 1.0, 1.0, 1.0);
      }*/

  status_line->draw(view);
  scroll_map->draw();
    
  if (show_help_screen)
    {
      help_screen.draw ();
    }
}


void
Editor::scroll()
{
  int mouse_x = CL_Mouse::get_x();
  int mouse_y = CL_Mouse::get_y();

  if (verbose) std::cout << "Editor::scroll()..." << std::flush;

  CL_System::keep_alive();

  while (CL_Mouse::right_pressed())
    {
      CL_System::keep_alive();

      if (mouse_x != CL_Mouse::get_x() || mouse_y != CL_Mouse::get_y())
	{
	  view->move (CL_Vector ((mouse_x - CL_Mouse::get_x()) / 5,
				 (mouse_y - CL_Mouse::get_y()) / 5));
      //	  object_manager->x_offset += (mouse_x - CL_Mouse::get_x()) / 5;
      //  object_manager->y_offset += (mouse_y - CL_Mouse::get_y()) / 5;
	  
	  /*cout << "ObjectManager: "
	       << "X: " << object_manager.x_offset 
	       << " Y: " << object_manager.y_offset << std::endl;
	  */
	  draw();
	  Display::flip_display(true);
	}
    }
  
  if (verbose) std::cout << "finished" << std::endl;
}

std::string
Editor::read_string (const std::string & prefix, const std::string & default_str)
{
  event->disable();
 
  std::string str;

  if (default_str.empty()) 
    {
      if (last_level.empty()) 
	{
	  str = System::get_statdir() + "levels/";
	} 
      else 
	{
	  str = last_level;
	}
    }
  else 
    {
      str = default_str;
    }
      
  CL_Display::clear_display();
  font->print_left(20, 20, prefix.c_str());
  font->print_left(20, 40, str.c_str());
  font->print_left(20, 400, "For information about the editor, have a look at the info pages.");
  font->print_left(20, 420, "$ info pingus");
  Display::flip_display();
  CL_Display::sync_buffers();

  CL_Key key;
  CL_InputBuffer* keys = new CL_InputBuffer;
  bool  finished = false;

  while (!finished) 
    { 
      CL_System::keep_alive();
      
      if (keys->peek_key().state != CL_Key::NoKey) 
	{
	  key = keys->get_key();
	  
	  if (key.state == CL_Key::Pressed)
	    {
	      switch (key.id)
		{
		case CL_KEY_ENTER:
		  finished = true;
		  std::cout << "--- Enter pressed" << std::endl;
		  break;
	   
		case CL_KEY_ESCAPE:
		  str = "";
		  finished = true;
		  break;
	    
		case CL_KEY_DELETE:
		case CL_KEY_BACKSPACE:
		  if (!str.empty())
		    str = str.substr(0, str.size() - 1); 
		  break;
	      
		case CL_KEY_TAB:
		  std::cout << "Tab completion not implemented" << std::endl;
		  break;
	    
		default:
		  if (key.ascii > 0)
		    str += key.ascii;
		  break;
		}
	    }
	  CL_Display::clear_display();
	  font->print_left(20, 20, prefix.c_str());
	  font->print_left(20, 40, str.c_str());
	  Display::flip_display();
	}
    }
  delete keys;
  event->enable();

  return str;
}

std::string
Editor::save_tmp_level ()
{
  std::string filename = std::string(tmpnam(0)) + ".pingus.xml";
  
  std::cout << "Saving level to: " << filename << std::endl;
  
  object_manager->save_level_xml(filename.c_str());

  return filename;
}

void 
Editor::zoom_mode ()
{
  CL_Rect rect;
  bool mouse_down = false;

  tool = ZOOM_TOOL;

  CL_Surface mouse_cursor = PingusResource::load_surface("editor/region-zoom", "core");
  
  while (true)
    {
      CL_System::keep_alive();      
      
      if (CL_Mouse::left_pressed () && !mouse_down)
	{
	  mouse_down = true;
	  rect.x1 = CL_Mouse::get_x ();
	  rect.y1 = CL_Mouse::get_y ();
	}
      else if (!CL_Mouse::left_pressed () && mouse_down)
	{
	  break;
	}

      draw ();

      if (mouse_down)
	{
	  rect.x2 = CL_Mouse::get_x ();
	  rect.y2 = CL_Mouse::get_y ();
	  
	  CL_Display::draw_rect (rect.x1, rect.y1, rect.x2, rect.y2, 
				 1.0, 1.0, 0.0, 1.0);
	}

      mouse_cursor.put_screen (CL_Mouse::get_x (), CL_Mouse::get_y ());

      Display::flip_display(true);
    }
  
  view->zoom_to (rect);
  tool = SELECTOR_TOOL;
}

void
Editor::rect_get_current_objs()
{
  CL_Vector start_pos (CL_Mouse::get_x(),
		       CL_Mouse::get_y ());
  
  //std::cout << "Editor: Selecting rectangle... " << std::flush;

  CL_Vector end_pos;

  while (CL_Mouse::middle_pressed())
    {
      CL_System::keep_alive();

      end_pos = CL_Vector(CL_Mouse::get_x(),
			  CL_Mouse::get_y ());
        
      // Draw the screen
      CL_Display::clear_display();
      object_manager->draw(view);
      Display::draw_rect((int) start_pos.x, (int)start_pos.y, (int)end_pos.x, (int)end_pos.y,
			 0.0f, 1.0f, 0.0f, 1.0f);
      panel->draw();
      status_line->draw(view);
      Display::flip_display(true);
    }
  
  start_pos = view->screen_to_world (start_pos);
  end_pos = view->screen_to_world (end_pos);

  object_manager->rect_get_current_objs(start_pos.x, start_pos.y, end_pos.x, end_pos.y);

  //std::cout << "finished" << std::endl;
}

bool
Editor::mouse_moved()
{
  int mouse_x = CL_Mouse::get_x();
  int mouse_y = CL_Mouse::get_y();

  while(mouse_x == CL_Mouse::get_x() && mouse_y == CL_Mouse::get_y())
    {
      CL_System::keep_alive();
    }
  return true;
}

void
Editor::move_objects()
{
  int move_speed = 1;
  move_x = 0;
  move_y = 0;

  if (CL_Keyboard::get_keycode(CL_KEY_RSHIFT)
      || CL_Keyboard::get_keycode(CL_KEY_RSHIFT))
    {
      move_speed = 10;
    }

  if (CL_Keyboard::get_keycode(CL_KEY_LEFT))
    move_x = -move_speed;
  else if (CL_Keyboard::get_keycode(CL_KEY_RIGHT))
    move_x = move_speed;

  if (CL_Keyboard::get_keycode(CL_KEY_UP))
    move_y = -move_speed;
  else if (CL_Keyboard::get_keycode(CL_KEY_DOWN))
    move_y = move_speed;
        
  object_manager->move_current_objs(move_x, move_y);
}

void
Editor::interactive_move_object()
{
  CL_System::keep_alive();

  object_manager->drag_current_objs ();
  CL_Vector old_pos (view->screen_to_world(CL_Vector(CL_Mouse::get_x(), CL_Mouse::get_y())));
  while (CL_Mouse::left_pressed()) 
    {
      CL_Vector new_pos (view->screen_to_world(CL_Vector(CL_Mouse::get_x(), CL_Mouse::get_y())));
      object_manager->move_current_objs(new_pos.x - old_pos.x,
					new_pos.y - old_pos.y);
      old_pos = new_pos;
      
      draw();
      Display::flip_display (true);
      CL_System::keep_alive();
    }
  object_manager->drop_current_objs ();
}

void
Editor::edit_current_objs()
{
  std::cout << "Editor::edit_current_objs(): Not implemented" << std::endl;
  /*  boost::shared_ptr<EditorObj> obj = object_manager->get_current_obj();

  if (obj.get())
    obj->gui_edit_obj();
  else
  std::cout << "No single object selected" << std::endl;*/
}

void
Editor::toggle_help_screen ()
{
  show_help_screen = !show_help_screen;
}

void 
Editor::load_level (const std::string& str)
{
  std::cout << "Editor::load_level(" << str << std::endl;

  // last_level = str; FIXME: we don't save the last level, since the
  // filename could be relative, insert a relative->absolute converter
  // and we might save it.

  object_manager->load_level(str);
  last_level = str;
}

/*
void
Editor::interactive_load()
{
  std::string levelfile;

  levelfile = read_string("Input level to load (without the .plf ending!):");
  if (!levelfile.empty()) 
    {
      try 
	{
	  if (exist(levelfile + ".plf")) 
	    {
	      last_level = levelfile;
	      
	      edit();
	    }
	  else
	    {
	      std::cout << "Creating new level, removing the current one..." << std::endl;
	      current_objs.erase(current_objs.begin(), current_objs.end());
	      editor_objs.erase(editor_objs.begin(), editor_objs.end());
	      
	      width = 1280;
	      height = 640;
	      start_x_pos = 640;
	      start_y_pos = 320;
	      number_of_pingus = 150;
	      number_to_save = 50;
	      level_time = -1;
	      levelname = "none";
	      description = "no levelname given";
	      edit();
	    }
	}
      
      catch (PingusError err) 
	{
	  PingusMessageBox(" PingusError: " + err.message);
	  interactive_load();
	}
      draw();
    }
}
*/

CL_GUIManager*
Editor::get_gui_manager ()
{
  return gui;
}

/* EOF */
