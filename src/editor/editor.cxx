//  $Id: editor.cxx,v 1.51 2003/04/19 10:23:18 torangan Exp $
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

#include <ClanLib/Core/System/system.h>
#include <ClanLib/Display/Display/display.h>
#include <ClanLib/Display/Input/input.h>
#include <ClanLib/Display/Display/mousecursor_provider.h>
#include <ClanLib/Display/Input/mouse.h>
#include <ClanLib/Display/Font/font.h>
#include <ClanLib/Display/Input/inputbuffer.h>
#include <ClanLib/Display/Input/keyboard.h>
#include <ClanLib/GUI/gui_manager.h>
#include <ClanLib/GUI/stylemanager_default.h>

#include "../globals.hxx"
#include "../gui/display.hxx"
#include "../pingus_resource.hxx"
#include "../system.hxx"
#include "../sound/sound.hxx"
#include "editor_event.hxx"
#include "panel.hxx"
#include "scroll_map.hxx"
#include "status_line.hxx"
#include "object_selector.hxx"
#include "selection.hxx"
#include "action_window.hxx"
#include "property_window.hxx"
#include "level_property_window.hxx"
#include "editor.hxx"
#include "../path_manager.hxx"
#include "../stat_manager.hxx"
#include "../string_converter.hxx"
#include "editor_view.hxx"

#include <cstdio>

namespace EditorNS {

Editor* Editor::instance_ = 0;

Editor*
Editor::instance ()
{
  if ( ! instance_)
    instance_ = new Editor ();

  return instance_;
}

void
Editor::init()
{
  instance_ = 0;
}

void
Editor::deinit()
{
  delete instance_;
  instance_ = 0;
}

Editor::Editor () : event_handler_ref_counter(0),
                    move_x(0), move_y(0), show_help_screen(true), tool(SELECTOR_TOOL)
{
  StatManager::instance()->get_bool("show-editor-help-screen", show_help_screen);

  EditorObj::set_editor(this);
  event      = new EditorEvent;

  panel      = new Panel;
  scroll_map = new ScrollMap;
  object_manager  = new ObjectManager;
  selection       = new Selection(object_manager);
  status_line     = new StatusLine;
  object_selector = new ObjectSelector;

  // FIXME: Should use PingusResource, Memleak
  CL_ResourceManager* gui_resources = new CL_ResourceManager(path_manager.complete("data/gui.scr"), false);

  style_manager = new CL_StyleManager_Default (gui_resources);
  gui   = new CL_GUIManager (style_manager);

  action_window = new ActionWindow (gui, object_manager->get_actions ());
  property_window = new PropertyWindow (this);
  level_property_window = new LevelPropertyWindow (gui, object_manager);

  view = new EditorView (0, 0, CL_Display::get_width (), CL_Display::get_height (), 0, 0);

  event->set_editor(this);

  font = PingusResource::load_font("Fonts/courier_small", "fonts");
  panel->init();
  status_line->set_current_objs(&selection->get_objects());
  panel->set_editor(this);
  scroll_map->editor_event = event;

  //std::cout << "Editor: registering event handler" << event << "... " << std::flush;
  on_button_press_slot = CL_Input::sig_button_press ().connect(event, &EditorEvent::on_button_press);
  on_button_release_slot = CL_Input::sig_button_release ().connect(event, &EditorEvent::on_button_release);
}

Editor::~Editor ()
{
  StatManager::instance()->set_bool("show-editor-help-screen", show_help_screen);

  CL_Input::sig_button_press ().disconnect (on_button_press_slot);
  CL_Input::sig_button_release ().disconnect (on_button_release_slot);

  delete object_manager;
  delete status_line;
  delete object_selector;

  delete scroll_map;
  delete event;
  delete panel;
}

void
Editor::on_startup ()
{
  PingusSound::stop_music();
  event->enable ();
}

void
Editor::on_shutdown ()
{
  event->disable ();
}

void
Editor::update (const GameDelta& delta)
{
  // Check if the current selection is different then the last one and
  // update the property window if necesarry
  // FIXME: This should be moved to the object manager
  if (tmp_selection != selection->get_objects())
    {
      tmp_selection = selection->get_objects();

      // FIXME: dirty hack, should only be called if the selection changed
      if (selection->size() == 1)
	{
	  EditorObj* obj = selection->get_current_obj();
	  property_window->update_frame(obj);
	  //CL_Component* comp = obj->get_gui_dialog (editor->property_window);
	}
      else
	{
	  property_window->update_frame(0);
	  //std::cout << "EditorEvent::editor_show_object_properties (): error: multiple objects selected" << std::endl;
	}
    }

  object_manager->update(delta.get_time());

  move_objects();
}

bool
Editor::draw (GraphicContext& gc)
{
  draw();
  UNUSED_ARG(gc);
  return true;
}

void
Editor::draw ()
{
  CL_Display::clear_display();

  object_manager->draw(view);
  selection->draw (view);
  panel->draw();

  status_line->draw(view);
  scroll_map->draw();

  if (show_help_screen)
    {
      help_screen.draw ();
    }

  gui->show ();

  if (get_gui_manager()->get_focus () == get_gui_manager ())
    CL_Display::draw_rect (25, 0, CL_Display::get_width (), CL_Display::get_height (),
			   1.0f, 1.0f, 1.0f);

}


void
Editor::scroll ()
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
	  view->move (Vector ((mouse_x - CL_Mouse::get_x()) / 5,
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
  /*
  char buffer[32];
  struct tm current_time;
  time_t t = time ();

  localtime_r (&t, &current_time);
  strftime (buffer, 32, "%s", &current_time);
*/
  int backup_id = 0;
  StatManager::instance()->get_int("next-backup-id", backup_id);

  if (backup_id >= 50)
    backup_id = 0;

  StatManager::instance()->set_int("next-backup-id", backup_id + 1);

  std::string filename = System::get_backupdir () + "pingus-backup-" + to_string(backup_id) + ".xml";
  std::cout << "Editor: saving backup level to: " << filename << std::endl;
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
  Vector start_pos (CL_Mouse::get_x(),
		       CL_Mouse::get_y ());
  Vector end_pos;

  while (CL_Mouse::middle_pressed())
    {
      CL_System::keep_alive();

      end_pos = Vector(CL_Mouse::get_x(),
			  CL_Mouse::get_y ());

      // Draw the screen
      CL_Display::clear_display();
      object_manager->draw(view);
      selection->draw (view);

      Display::draw_rect((int) start_pos.x, (int)start_pos.y, (int)end_pos.x, (int)end_pos.y,
			 0.0f, 1.0f, 0.0f, 1.0f);
      panel->draw();
      status_line->draw(view);
      Display::flip_display(true);
    }

  start_pos = view->screen_to_world (start_pos);
  end_pos = view->screen_to_world (end_pos);

  selection->select_rect(start_pos.x, start_pos.y,
			 end_pos.x, end_pos.y);
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
  if (get_gui_manager()->get_focus () == get_gui_manager ())
    {
      // FIXME: this is really a dirty dirty hack
      // FIXME: To fix this the whole editor should be build up out of CL_Components
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

      selection->move(move_x, move_y);
    }
}

void
Editor::interactive_move_object()
{
  // FIXME: this is really a dirty dirty hack
  // FIXME: To fix this the whole editor should be build up out of CL_Components

  CL_System::keep_alive();

  selection->drag ();
  Vector old_pos (view->screen_to_world(Vector(CL_Mouse::get_x(), CL_Mouse::get_y())));
  while (CL_Mouse::left_pressed())
    {
      Vector new_pos (view->screen_to_world(Vector(CL_Mouse::get_x(), CL_Mouse::get_y())));

      new_pos.x = int(new_pos.x);
      new_pos.y = int(new_pos.y);

      selection->move(new_pos.x - old_pos.x, new_pos.y - old_pos.y);
      old_pos = new_pos;

      draw();
      Display::flip_display (true);
      CL_System::keep_alive();
    }
  selection->drop ();
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

  selection->clear ();
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

} // namespace EditorNS

/* EOF */
