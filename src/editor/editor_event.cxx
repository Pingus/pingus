//  $Id: editor_event.cxx,v 1.37 2002/09/28 11:52:23 torangan Exp $
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

#include <stdio.h>
#include <fstream>
#include <ClanLib/GUI/gui_manager.h>
#include <ClanLib/Core/System/error.h>
#include <ClanLib/Display/Input/input.h>
#include <ClanLib/Display/Input/keyboard.h>
#include <ClanLib/Display/Display/display.h>
#include <ClanLib/Display/Input/mouse.h>
#include "../html_browser.hxx"
#include "../debug.hxx"
#include "../globals.hxx"
#include "../game_session.hxx"
#include "../message_box.hxx"
#include "../system.hxx"
#include "../pingus_error.hxx"
#include "../loading.hxx"
#include "../my_gettext.hxx"
#include "selection.hxx"
#include "editorobj_group.hxx"
#include "string_reader.hxx"
#include "editor_event.hxx"
#include "scroll_map.hxx"
#include "panel.hxx"
#include "object_selector.hxx"
#include "editor.hxx"
#include "action_window.hxx"
#include "property_window.hxx"
#include "level_property_window.hxx"
#include "../screen_manager.hxx"

namespace EditorNS {

EditorEvent::EditorEvent()
  : is_enabled (false)
{
  for (float i = 0; i < 1.0f; i += 0.1f)
    background_colors.push_back (Color (i, i, i));

  background_colors.push_back (Color (1.0, 1.0, 0.0));
  background_colors.push_back (Color (0.0, 1.0, 1.0));
  background_colors.push_back (Color (1.0, 0.0, 1.0));

  background_colors.push_back (Color (0.0, 1.0, 0.0));
  background_colors.push_back (Color (1.0, 0.0, 0.0));
  background_colors.push_back (Color (0.0, 0.0, 1.0));
}

EditorEvent::~EditorEvent()
{
  
}

void
EditorEvent::set_editor(Editor* e)
{
  editor = e;
  editor->last_level = System::get_statdir() + "levels/";
  object_manager = editor->object_manager;
  selection = editor->selection;
}

void
EditorEvent::enable()
{
  assert (is_enabled == false);

  std::cout << "EditorEvent::enable()" << std::endl;
  is_enabled = true;
  editor->get_gui_manager ()->enable_input ();
}

void
EditorEvent::disable()
{
  assert (is_enabled == true);

  std::cout << "EditorEvent::disable()" << std::endl;
  is_enabled = false;
  editor->get_gui_manager ()->disable_input ();
}

void
EditorEvent::on_button_press(CL_InputDevice *device, const CL_Key& key)
{
  std::cout << "EditorEvent::on_button_press: " << is_enabled << std::endl;

  if (!accept_input ())
    return;

  if (device == CL_Input::keyboards[0])
    {
      switch (key.id)
	{
	case CL_KEY_R: 
	  if (CL_Keyboard::get_keycode(CL_KEY_RSHIFT)
	      || CL_Keyboard::get_keycode(CL_KEY_LSHIFT))
	    { // rotate 90 counterclockwise
	      editor_rotate_270_current_selection ();
	    }
	  else
	    { // rotate 90 clockwise
	      editor_rotate_90_current_selection ();
	    }
	  break;

	case CL_KEY_H: // horizontal flip
	  editor_horizontal_flip_current_selection ();
	  break;

	case CL_KEY_V: // vertical flip
	  editor_vertical_flip_current_selection ();
	  break;

	case CL_KEY_I: // import prefab (shouldn't be here, its handled in the object selector already)
	  editor_import_prefab ();
	  break;

	case CL_KEY_X: // export prefab 
	  editor_export_prefab_from_selection ();
	  break;

	case CL_KEY_F3:
	  editor_toggle_background_color();
	  break;
    
	  // Start the created level and come back if done.
	case CL_KEY_F4:
	  editor_start_current_level();
	  break;

	  // Quit the editor and make a copy of the level to `/tmp/'.
	case CL_KEY_ESCAPE:
	  editor_exit();
	  break;

	  // Set a checkpoint.
	case CL_KEY_F7:
	  /*
	  editor->save_tmp_level();
	  editor->checkpoint = std::string(tmpnam(0)) + ".pingus";
	  if (verbose) std::cout << "Setting checkpoint: " << editor->checkpoint << std::endl;
	  object_manager->save_level(editor->checkpoint);*/
	  break;
	
	  // Restore a previously created checkpoint.
	case CL_KEY_F8:
	  /* 
	  editor->save_tmp_level();
	  if (!editor->checkpoint.empty()) 
	    {
	      if (verbose) std::cout << "Restoring checkpoint: " << editor->checkpoint << std::endl;
	      selection->clear ();
	      object_manager->load_level(editor->checkpoint);
	    } 
	  else 
	    {
	      if (verbose) std::cout << "No checkpoint set, no restoring done. " << std::endl;
	      }*/
	  break;
      
	  // Insert a new object, present the ObjectsSelector to select
	  // one. 
	case CL_KEY_INSERT:
	  editor_insert_new_object();
	  break;
    
	  // Load a level from file.
	case CL_KEY_F5:
	  editor_load_level();
	  break;

	  // Save the level to file.
	case CL_KEY_F6:
	  editor_save_level_as();
	  break;

	  // Add all objects to the current selection.
	case CL_KEY_A:
	  editor_mark_all_objects();
	  break;
    
	  /*	case CL_KEY_F11:
	  CL_Display::set_videomode(screen_width, screen_height, 16, 
				    fullscreen_enabled, 
				    true); // allow resize
	  break;
	  */
	case CL_KEY_F12:
	  // FIXME: disable cause it gives a segfault
	  //CL_Display::set_videomode(900, 700, 16, 
	  //fullscreen_enabled, 
	  //		    true); // allow resize
	  break;
    
	  // Delete all objects of the curret selection.
	case CL_KEY_DELETE:
	  editor_delete_selected_objects();
	  break;
      
	  // Form the current selection into a group
	case CL_KEY_G:
	  if (CL_Keyboard::get_keycode(CL_KEY_RSHIFT)
	      || CL_Keyboard::get_keycode(CL_KEY_LSHIFT))
	    editor_convert_group_to_selection();
	  else
	    editor_convert_selection_to_group();
	  break;

	case CL_KEY_HOME:
	  if (selection->get_current_obj())
	    selection->get_current_obj()->make_larger ();
	  break;

	case CL_KEY_END:
	  if (selection->get_current_obj())
	    selection->get_current_obj()->make_smaller ();
	  break;

	  // Lower all object in the current selection
	case CL_KEY_PAGEDOWN:
	  selection->lower();
	
	  if (CL_Keyboard::get_keycode(CL_KEY_RSHIFT)) 
	    {
	      selection->move(Vector(0, 0, -50));
	    }
	  else if (CL_Keyboard::get_keycode(CL_KEY_RCTRL))
	    {
	      selection->move(Vector(0, 0, -1));
	    }
	  break;
    
	  // Raise all objects in the current selection.
	case CL_KEY_PAGEUP:
	  selection->raise();
	  if (CL_Keyboard::get_keycode(CL_KEY_RSHIFT)) 
	    {
	      selection->move(Vector(0, 0, 50));
	    }
	  else if (CL_Keyboard::get_keycode(CL_KEY_RCTRL)) 
	    {
	      selection->move(Vector(0, 0, 1));
	    }
	  break;

	case CL_KEY_F1:
	  editor->toggle_help_screen ();
	  break;

	case CL_KEY_F2:
	  HTMLBrowser::launch ("http://pingus.seul.org/levels.html");
	  break;

	  // Change the width and height of the current level.
	case CL_KEY_F9:
	  {
	    std::string tmp_str;
	    char str[16];
	    int  theight, twidth;

	    snprintf(str, 16, "%d", object_manager->width);
	    tmp_str = editor->read_string("Input Width: ", str);
	
	    if (sscanf(tmp_str.c_str(), "%d", &twidth) == 1) 
	      {
		object_manager->width = twidth;
	      }

	    snprintf(str, 16, "%d", object_manager->height);
	    tmp_str = editor->read_string("Input Height: ", str);

	    if (sscanf(tmp_str.c_str(), "%d", &theight) == 1) 
	      {
		object_manager->height = theight;
	      }
	  }
	  break;

	case CL_KEY_S:
	  object_manager->z_pos_sort ();
	  break;

	case CL_KEY_Z: // FIXME: stupid key binding
	  editor_zoom_in ();
	  break;

	case CL_KEY_W: // FIXME: stupid key binding
	  editor_zoom_out ();
	  break;

	case CL_KEY_ENTER:
	  editor->view->set_zoom (1.0);	  
	  break;

	case CL_KEY_D:
	  editor_duplicate_current_selection();
	  break;
	  
	default:
	  if (verbose)
	    std::cout << "EditorEvent: Unknown key pressed: id=" << key.id << " ascii=" << key.ascii << std::endl;
	}
    }
  else if (device == CL_Input::pointers[0])
    {
      /*std::cout << "Mouse: (" << CL_Mouse::get_x () << ", " << CL_Mouse::get_y () << ") "
		<< "World: " << editor->view->screen_to_world (Vector(CL_Mouse::get_x (), CL_Mouse::get_y ()))
		<< std::endl;*/
      
      switch (key.id)
	{
	case CL_MOUSE_LEFTBUTTON:
	  if (editor->panel->mouse_over((int) key.x, (int)key.y))
	    editor->panel->on_click();
	  else if (editor->scroll_map->mouse_over ((int)key.x, (int)key.y))
	    editor->scroll_map->on_button_press(device, key);
	  else
	    editor_mark_or_move_object();
	  break;
	case CL_MOUSE_MIDDLEBUTTON:
	  editor->rect_get_current_objs();
	  break;
	case CL_MOUSE_RIGHTBUTTON:
	  editor->scroll();
	  break;
	}
    }
  else
    {
      if (verbose) std::cout << "Warning: Unknown input device" << std::endl;
    }
  
  // Redraw the screen, since something may have changed
  editor->draw();
}

void
EditorEvent::on_button_release(CL_InputDevice *device, const CL_Key &key)
{
  std::cout << "EditorEvent::on_button_release: " << is_enabled << std::endl;

  if (!accept_input ())
    return;

  if (device == CL_Input::keyboards[0])
    {
    }
  else if (device == CL_Input::pointers[0])
    {
      if (key.id == CL_MOUSE_LEFTBUTTON)
	editor->panel->on_release();
    }
  else
    {
    }
  
  return;
}

void
EditorEvent::editor_convert_group_to_selection()
{
  if (selection->get_current_obj())
    {
      EditorObj* obj = selection->get_current_obj();
      EditorObjGroup* group = dynamic_cast<EditorObjGroup*>(obj);

      if (group)
	{      
	  std::vector<EditorObj*>* objs = group->get_objs();
	  selection->clear();

	  for(std::vector<EditorObj*>::iterator i = objs->begin();
	      i != objs->end();
	      ++i)
	    {
	      EditorObj* obj = (*i)->duplicate ();
	      if (obj)
		{
		  object_manager->add (obj);
		  selection->add(obj);
		}
	      else
		{
		  std::cout << "EditorEvent::editor_convert_group_to_selection(): duplicate not implemented" << std::endl;
		}
	    }

	  object_manager->erase(group);
	}
      else
	{
	  std::cout << "EditorEvent::editor_convert_group_to_selection: No group selected" << std::endl;
	}
    }
  else
    {
      std::cout << "EditorEvent::editor_convert_group_to_selection: To many/(not enough) objects selected" << std::endl;
    }
}

void
EditorEvent::editor_convert_selection_to_group()
{
  if (selection->size() > 1)
    {
      EditorObjGroup* group = new EditorObjGroup();
      std::vector<EditorObj*> to_erase;

      // We need to collect the objects out of the editor_objs list to keep the correct sorting
      for (ObjectManager::EditorObjIter j = object_manager->editor_objs.begin();
	   j != object_manager->editor_objs.end();
	   ++j)
	{
	  for (std::vector<EditorObj*>::const_iterator i = selection->get_objects().begin();
	       i != selection->get_objects().end();
	       ++i)
	    { 
	      if (*j == *i)
		{
		  group->add (*i);
		  to_erase.push_back(*i);
		}
	    }
	}

      for(std::vector<EditorObj*>::iterator i = to_erase.begin();
	  i != to_erase.end();
	  ++i)
	object_manager->erase(*i);

      object_manager->add (group);
      selection->clear();
      selection->add(group);
    }
  else
    {
      std::cout << "EditorEvent::editor_convert_selection_to_group: Not enough objects selected for a group" << std::endl;
    }
}

void
EditorEvent::editor_mark_all_objects()
{
  selection->clear();
  
  for(ObjectManager::EditorObjIter i = object_manager->editor_objs.begin(); 
      i != object_manager->editor_objs.end(); 
      ++i) 
    {
      selection->add(*i);
    }
}

void
EditorEvent::editor_toggle_background_color()
{
  std::vector<Color>::iterator i = 
    std::find(background_colors.begin (), background_colors.end (), object_manager->bg);

  if (i == background_colors.end ())
    object_manager->bg = *background_colors.begin ();
  else
    {
      ++i;

      if (i == background_colors.end ())
	object_manager->bg = *background_colors.begin ();
      else
	object_manager->bg = *i;
    }
}

void
EditorEvent::editor_delete_selected_objects()
{
  editor->save_tmp_level ();     
  object_manager->erase (selection->get_objects());
  selection->clear();
}

void
EditorEvent::editor_start_current_level()
{
  loading_screen.draw();

  try 
    {
      std::string levelfile = editor->save_tmp_level();
      ScreenManager::instance()->push_screen(new PingusGameSession (levelfile), true);
    }
  catch(const PingusError& err) 
    {
      std::cout << "Editor: Error caught from Pingus: " << err.get_message () << std::endl;
    }  
  catch (const CL_Error& err) 
    {
      std::cout << "Editor: Error caught from ClanLib: " << err.message << std::endl;
    }
}

void 
EditorEvent::editor_load_level()
{
  std::string str;
  System::Directory dir;
  std::list<std::string> strings;
  std::string temp_str;

  StringReader reader("Input filename to load the file (without .plf!)", editor->last_level);
  std::cout << "Loading level, input filename" << std::endl;

  dir = System::opendir(System::get_statdir() + "levels/", "*");

  for (System::Directory::iterator i = dir.begin(); i != dir.end(); ++i)
    {
      //std::cout << "dirs: " << System::get_statdir() + "levels/" +  i->name << std::endl;
      
      temp_str = System::get_statdir() + "levels/" + i->name;

      strings.push_back(temp_str);
    }

  reader.set_strings(&strings);

  disable();
  str = reader.read_string();
  enable();

  if (!str.empty()) 
    {
      try {
	selection->clear ();
	object_manager->load_level(str);
	editor->last_level = str;
      }
      catch(const PingusError& err) {
	std::cout << err.get_message () << std::endl;
	PingusMessageBox(err.get_message ());
      }
    }
}

void
EditorEvent::editor_save_level_as()
{
  std::cout << "Saving level, input filename" << std::endl;

  std::string str;
  System::Directory dir;
  std::list<std::string> strings;

  StringReader reader(_("Enter filename to save as (without .plf!)"), editor->last_level);

  // FIXME: The pattern '*' will not work on windows, we should use
  // FIXME: *.xml or better switch to CL_DirectoryScanner
  dir = System::opendir(System::get_statdir() + "levels/", "*");

  for (System::Directory::iterator i = dir.begin(); i != dir.end(); ++i)
    {
      //std::cout << "dirs: " << System::get_statdir() + "levels/" +  i->name << std::endl;
      str = System::get_statdir() + "levels/" + i->name;

      strings.push_back(str);
    }

  reader.set_strings(&strings);

  disable();
  str = reader.read_string();
  enable();

  if (!str.empty()) 
    {
      if (str.substr(str.size() - 4) ==  ".xml")
	object_manager->save_level_xml(str);	
      else
	object_manager->save_level_xml(str + ".xml");

      editor->last_level = str;
    }
}

void
EditorEvent::editor_duplicate_current_selection()
{
  std::vector<EditorObj*> new_objs;
  
  for (std::vector<EditorObj*>::const_iterator i = selection->get_objects().begin(); 
       i != selection->get_objects().end();
       ++i)
    {
      /*ObjectManager::EditorObjIter iter = std::find(object_manager->editor_objs.begin(), 
						    object_manager->editor_objs.end(), 
						    *i);*/
      
      EditorObj* obj = (*i)->duplicate();
      if (obj)
	{
	  // Apply object offset (not really needed, but makes it
	  // easier to see that the object got duplicated)
	  obj->set_position_offset (Vector(8, 8));

	  // FIXME: We don't take the object position into account!
	  object_manager->add (obj); // ObjectManager will take care of the deletion
	  new_objs.push_back(obj);
	}
    }

  selection->clear();
  selection->add(new_objs);
}

void
EditorEvent::editor_insert_new_object()
{
  editor->save_tmp_level();

  EditorObjLst objs;
  try 
    {
      disable();
      std::cout << "Object selector on" <<std::endl;
      objs = editor->object_selector->get_obj(int(editor->view->get_offset ().x + CL_Display::get_width ()/2),
					      int(editor->view->get_offset ().y + CL_Display::get_height ()/2));
      std::cout << "Object selector off" << std::endl;
      enable();
    }
  
  catch (CL_Error err) {
    std::cout << "Editor: Error caught from ClanLib: " << err.message << std::endl;
    enable();
  }
      
  if (!objs.empty ()) 
    {
      for (ObjectManager::EditorObjIter i = objs.begin (); i != objs.end (); ++i)
	object_manager->editor_objs.push_back(*i);
    } 
  else 
    {
      std::cout << "EditorEvent: Something went wrong while inserting" << std::endl;
    }
}

void
EditorEvent::editor_new_level()
{
  editor->last_level = System::get_statdir() + "levels/";
  object_manager->new_level();
}

void 
EditorEvent::editor_exit()
{
  std::cout << "Exit editor" << std::endl;
  editor->save_tmp_level();
  ScreenManager::instance ()->pop_screen ();
}

void
EditorEvent::editor_mark_or_move_object()
{
  if (editor->tool != Editor::SELECTOR_TOOL)
    return;

  EditorObj* obj 
    = object_manager->find_object(editor->view->screen_to_world (Vector(CL_Mouse::get_x(), 
									   CL_Mouse::get_y())));
  
  if (obj)
    {
      if (selection->object_selected(obj))
	{
	  editor->interactive_move_object();
	}
      else
	{
	  if (!CL_Keyboard::get_keycode(CL_KEY_LSHIFT))
	    selection->clear();
	  
	  selection->add(obj);
	}
    }
  else
    {
      selection->clear();
    }
}

void 
EditorEvent::editor_display_help()
{
  /*  CL_Display::clear_display();
      font->print_center(CL_Display::get_width() / 2, 20, "Help Screen");*/
}

void
EditorEvent::editor_increase_count ()
{
}

void
EditorEvent::editor_decrease_count ()
{
  
}

void 
EditorEvent::editor_zoom_in ()
{
  editor->view->set_zoom (editor->view->get_zoom () * 1.4f);
}

void 
EditorEvent::editor_zoom_out ()
{
  editor->view->set_zoom (editor->view->get_zoom () / 1.4f);
}

void 
EditorEvent::zoom_mode ()
{
  editor->zoom_mode ();
}

void
EditorEvent::editor_export_prefab_from_selection ()
{
  std::cout << "EditorEvent:editor_export_object_group_from_selection ()" << std::endl;
  
  std::vector<EditorObj*> temp;
  for (std::vector<EditorObj*>::const_iterator it  = selection->get_objects().begin();
       it != selection->get_objects().end(); ++it)
    temp.push_back(*it);
  
  EditorObjGroup group (temp);
  std::ofstream xml ("/tmp/metaobj.xml");
  group.write_xml (xml);
}

void
EditorEvent::editor_import_prefab ()
{
  editor->object_manager->add_prefab_from_file ("/tmp/metaobj.xml");
}

void 
EditorEvent::editor_horizontal_flip_current_selection()
{
  selection->horizontal_flip ();
}

void
EditorEvent::editor_vertical_flip_current_selection()
{
  selection->vertical_flip ();
}

void
EditorEvent::editor_rotate_90_current_selection()
{
  selection->rotate_90 ();
}

void
EditorEvent::editor_rotate_270_current_selection()
{
  selection->rotate_270 ();
}

void
EditorEvent::editor_show_actions_window ()
{
  editor->action_window->show ();
}

void
EditorEvent::editor_show_object_properties ()
{
  if (editor->property_window->is_visible ())
    editor->property_window->show (false);
  else
    editor->property_window->show (true);
}

void
EditorEvent::editor_show_level_properties ()
{
  if (editor->level_property_window->is_visible ())
    editor->level_property_window->show (false);
  else
    editor->level_property_window->show (true);  
}

bool
EditorEvent::accept_input ()
{
  return
    is_enabled
    && 
    (editor->get_gui_manager ()->get_focus () == editor->get_gui_manager ());
}

} // namespace EditorNS

/* EOF */
