//  $Id: EditorEvent.cc,v 1.14 2000/06/08 20:05:35 grumbel Exp $
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

#include "../globals.hh"
#include "../PingusGame.hh"
#include "../PingusMessageBox.hh"
#include "../globals.hh"
#include "../System.hh"
#include "../PingusError.hh"
#include "../System.hh"
#include "../Loading.hh"
#include "StringReader.hh"
#include "EditorEvent.hh"

EditorEvent::EditorEvent()
{
  enable();
  //font = CL_Font::load("Fonts/courier_small", PingusResource::get("fonts.dat"));
}

EditorEvent::~EditorEvent()
{
  
}

void
EditorEvent::set_editor(Editor* e)
{
  editor = e;
  editor->last_level = System::get_statdir() + "levels/dist/";
  object_manager = &(editor->object_manager);
}

void
EditorEvent::enable()
{
  is_enabled = true;
}

void
EditorEvent::disable()
{
  is_enabled = false;
}

bool
EditorEvent::on_button_press(CL_InputDevice *device, const CL_Key &key)
{
  if (!is_enabled)
    return true;

  if (device == CL_Input::keyboards[0])
    {
      switch (key.id)
	{
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
	  editor->save_tmp_level();
	  editor->checkpoint = std::string(tmpnam(0)) + ".pingus";
	  if (verbose) std::cout << "Setting checkpoint: " << editor->checkpoint << std::endl;
	  object_manager->save_level(editor->checkpoint);
	  break;
	
	  // Restore a previously created checkpoint.
	case CL_KEY_F8:
	  editor->save_tmp_level();
	  if (!editor->checkpoint.empty()) 
	    {
	      if (verbose) std::cout << "Restoring checkpoint: " << editor->checkpoint << std::endl;
	      object_manager->load_level(editor->checkpoint);
	    } 
	  else 
	    {
	      if (verbose) std::cout << "No checkpoint set, no restoring done. " << std::endl;
	    }
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
    
	  // Select another background.
	case CL_KEY_F10:
	  object_manager->background.desc.res_name = editor->read_string("Input Background: ",
									 object_manager->background.desc.res_name);
	  break;

	case CL_KEY_F11:
	  CL_Display::set_videomode(screen_width, screen_height, 16, 
				    fullscreen_enabled, 
				    true); // allow resize
	  break;

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
      
	  // Lower all object in the current selection
	case CL_KEY_PAGEDOWN:
	  object_manager->lower_current_objs();
	
	  if (CL_Keyboard::get_keycode(CL_KEY_RSHIFT)) 
	    {
	      for (ObjectManager::CurrentObjIter i = object_manager->current_objs.begin(); 
		   i != object_manager->current_objs.end(); 
		   ++i)
		{
		  (*i)->z_pos = -100;
		}
	    }
	  break;
    
	  // Raise all objects in the current selection.
	case CL_KEY_PAGEUP:
	  object_manager->raise_current_objs();    
	  if (CL_Keyboard::get_keycode(CL_KEY_RSHIFT)) 
	    {
	      for (ObjectManager::CurrentObjIter i = object_manager->current_objs.begin();
		   i != object_manager->current_objs.end(); 
		   ++i) 
		{
		  (*i)->z_pos = 100;
		}
	    }
	  break;

	  // Change the width and height of the current level.
	case CL_KEY_F9:
	  {
	    std::string tmp_str;
	    char str[1024];
	    int  theight, twidth;

	    sprintf(str, "%d", object_manager->width);
	    tmp_str = editor->read_string("Input Width: ", str);
	
	    if (sscanf(tmp_str.c_str(), "%d", &twidth) == 1) 
	      {
		if ((twidth % tile_size) != 0)
		  {
		    twidth += (tile_size - (twidth % tile_size));
		    std::cout << "Warrning: Editor: Width not a multiple of " 
			 << tile_size << ", fixing width to " << twidth
			 << std::endl; 
		  }
		object_manager->width = twidth;
	      }

	    sprintf(str, "%d", object_manager->height);
	    tmp_str = editor->read_string("Input Height: ", str);

	    if (sscanf(tmp_str.c_str(), "%d", &theight) == 1) 
	      {
		if ((theight % tile_size) != 0)
		  {
		    theight += (tile_size - (theight % tile_size));
		    std::cout << "Editor: Height not a multiple of " 
			 << tile_size << ", fixing height to " << theight 
			 << std::endl; 
		  }
	   
		object_manager->height = theight;
	      }
	  }
	  break;

	case CL_KEY_KP_PLUS:
	case CL_KEY_D:
	  editor_duplicate_current_selection();
	  break;
	  
	default:
	  if (verbose)
	    std::cout << "EdiorEvent: Unknown key pressed: id=" << key.id << " ascii=" << key.ascii << std::endl;
	}
    }
  else if (device == CL_Input::pointers[0])
    {
      switch (key.id)
	{
	case 0:
	  if (editor->panel->mouse_over(key.x, key.y))
	    {
	      editor->panel->on_click();
	    }
	  else
	    {
	      editor_mark_or_move_object();
	    }
	  break;
	case 1:
	  editor->rect_get_current_objs();
	  break;
	case 2:
	  editor->scroll();
	  break;
	}
    }
  else
    {
      if (verbose) std::cout << "Warrning: Unkown input device" << std::endl;
    }
  
  // Redraw the screen, since something may have changed
  editor->draw();
  return true;
}

bool
EditorEvent::on_button_release(CL_InputDevice *device, const CL_Key &key)
{
  if (!is_enabled)
    return true;

  if (device == CL_Input::keyboards[0])
    {
    }
  else if (device == CL_Input::pointers[0])
    {
      if (key.id == 0)
	editor->panel->on_release();
    }
  else
    {
    }
  
  return true;
}

void
EditorEvent::editor_mark_all_objects()
{
  object_manager->delete_selection();
  
  for(ObjectManager::EditorObjIter i = object_manager->editor_objs.begin(); 
      i != object_manager->editor_objs.end(); 
      ++i) 
    {
      object_manager->current_objs.push_back(*i);
    }
}

void
EditorEvent::editor_toggle_background_color()
{
  object_manager->bg.r += 0.1;
  object_manager->bg.g += 0.1;
  object_manager->bg.b += 0.1;
      
  if (object_manager->bg.r > 1.0) 
    {
      object_manager->bg.r = 0.0;
      object_manager->bg.g = 0.0;
      object_manager->bg.b = 0.0;
    }
}

void
EditorEvent::editor_delete_selected_objects()
{
  editor->save_tmp_level ();
      
  for (ObjectManager::CurrentObjIter i = object_manager->current_objs.begin();
       i != object_manager->current_objs.end();
       i++)
    { 
      object_manager->editor_objs.erase(find(object_manager->editor_objs.begin(), object_manager->editor_objs.end(), *i));
    }
  
  object_manager->current_objs.erase(object_manager->current_objs.begin(), 
				     object_manager->current_objs.end());
}

void
EditorEvent::editor_start_current_level()
{
  disable();

  CL_MouseCursor::hide();
  loading_screen.draw();
  CL_MouseCursor::show();

  try {
    PingusGame game;
    std::string levelfile = editor->save_tmp_level();
    
    game.start_game(levelfile + ".plf",
		    levelfile + ".psm");
  }
  
  catch(PingusError err) {
    std::cout << "Editor: Error caught from Pingus: " << err.message << std::endl;
  }
  
  catch (CL_Error err) {
    std::cout << "Editor: Error caught from ClanLib: " << err.message << std::endl;
  }

  
  enable();
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

  dir = System::opendir(System::get_statdir() + "levels/dist/", "*.plf");

  for (System::Directory::iterator i = dir.begin(); i != dir.end(); i++)
    {
      std::cout << "dirs: " << System::get_statdir() + "levels/dist/" +  i->name << std::endl;
      
      temp_str = System::get_statdir() + "levels/dist/" + i->name;

      strings.push_back(temp_str.substr(0, temp_str.size() - 4));
    }

  reader.set_strings(&strings);

  disable();
  str = reader.read_string();
  enable();

  if (!str.empty()) 
    {
      try {
	object_manager->load_level(str);
	editor->last_level = str;
      }
      catch(PingusError err) {
	std::cout << " PingusError: " + err.message << std::endl;
	PingusMessageBox(" PingusError: " + err.message);
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

  StringReader reader("Input filename to save the file (without .plf!)", editor->last_level);

  dir = System::opendir(System::get_statdir() + "levels/dist/", "*.plf");

  for (System::Directory::iterator i = dir.begin(); i != dir.end(); i++)
    {
      std::cout << "dirs: " << System::get_statdir() + "levels/dist/" +  i->name << std::endl;
      str = System::get_statdir() + "levels/dist/" + i->name;

      strings.push_back(str.substr(0, str.size() - 4));
    }

  reader.set_strings(&strings);

  disable();
  str = reader.read_string();
  enable();

  if (!str.empty()) 
    {
      object_manager->save_level(str);
      editor->last_level = str;
    }
}

void
EditorEvent::editor_duplicate_current_selection()
{
  std::list<EditorObj*> new_objs;
  
  for (ObjectManager::CurrentObjIter i = object_manager->current_objs.begin(); 
       i != object_manager->current_objs.end();
       i++)
    {
      ObjectManager::EditorObjIter iter = find(object_manager->editor_objs.begin(), 
					       object_manager->editor_objs.end(), 
					       *i);
      EditorObj* obj = (*i)->duplicate();

      object_manager->editor_objs.insert(iter, obj);
      new_objs.push_back(obj);
    }

  object_manager->delete_selection();
  object_manager->add_to_selection(new_objs);
}

void
EditorEvent::editor_insert_new_object()
{
  editor->save_tmp_level();

  EditorObj* obj;
  try {
    disable();
    obj = editor->object_selector.get_obj(object_manager->x_offset, object_manager->y_offset);
    enable();
  }
  
  catch (CL_Error err) {
    std::cout << "Editor: Error caught from ClanLib: " << err.message << std::endl;
    enable();
    obj = 0;
  }
      
  if (obj) {
    object_manager->editor_objs.push_back(obj);
  } else {
    std::cout << "Something went wrong while inserting" << std::endl;
  }
}

void
EditorEvent::editor_new_level()
{
  editor->last_level = System::get_statdir() + "levels/dist/";
  object_manager->new_level();
}

void 
EditorEvent::editor_exit()
{
  std::cout << "Exit editor" << std::endl;
  editor->save_tmp_level();
  editor->quit = true;
}

void
EditorEvent::editor_mark_or_move_object()
{
  EditorObj* obj = object_manager->select_object(CL_Mouse::get_x(), CL_Mouse::get_y());
  
  if (obj)
    {
      if (object_manager->object_selected(obj))
	{
	  editor->interactive_move_object();
	}
      else
	{
	  if (!CL_Keyboard::get_keycode(CL_KEY_LSHIFT))
	    object_manager->delete_selection();
	  
	  object_manager->add_to_selection(obj);
	}
    }
  else
    {
      object_manager->delete_selection();
    }
}

void 
EditorEvent::editor_display_help()
{
  /*  CL_Display::clear_display();
      font->print_center(CL_Display::get_width() / 2, 20, "Help Screen");*/
}

/* EOF */
