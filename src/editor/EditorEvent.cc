//  $Id: EditorEvent.cc,v 1.1 2000/02/04 23:45:19 mbn Exp $
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

#include "../PingusGame.hh"
#include "../globals.hh"
#include "StringReader.hh"
#include "EditorEvent.hh"

EditorEvent::EditorEvent()
{
  enabled = true;
}

EditorEvent::~EditorEvent()
{
  
}

void
EditorEvent::set_editor(Editor* e)
{
  editor = e;
  object_manager = &(editor->object_manager);
}

bool
EditorEvent::on_button_press(CL_InputDevice *device, const CL_Key &key)
{
  if (!enabled)
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
	  editor->checkpoint = string(tmpnam(0)) + ".pingus";
	  cout << "Setting checkpoint: " << editor->checkpoint << endl;
	  object_manager->save_level(editor->checkpoint);
	  break;
	
	  // Restore a previously created checkpoint.
	case CL_KEY_F8:
	  editor->save_tmp_level();
	  if (!editor->checkpoint.empty()) 
	    {
	      cout << "Restoring checkpoint: " << editor->checkpoint << endl;
	      object_manager->load_level(editor->checkpoint);
	    } 
	  else 
	    {
	      cout << "No checkpoint set, no restoring done. " << endl;
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
	    string tmp_str;
	    char str[1024];
	    int  theight, twidth;

	    sprintf(str, "%d", object_manager->width);
	    tmp_str = editor->read_string("Input Width: ", str);
	
	    if (sscanf(tmp_str.c_str(), "%d", &twidth) == 1) 
	      {
		if ((twidth % tile_size) != 0)
		  {
		    twidth += (tile_size - (twidth % tile_size));
		    cout << "Editor: Width not a multiple of " 
			 << tile_size << ", fixing width to " << twidth
			 << endl; 
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
		    cout << "Editor: Height not a multiple of " 
			 << tile_size << ", fixing height to " << theight 
			 << endl; 
		  }
	   
		object_manager->height = theight;
	      }
	  }
	  break;

	case CL_KEY_KP_PLUS:
	  editor_duplicate_current_selection();
	  break;
	  
	default:
	  cout << "EdiorEvent: Unknown key pressed: id=" << key.id << " ascii=" << key.ascii << endl;
	}
    }
  else if (device == CL_Input::pointers[0])
    {
      // cout << "Mouse Button Pressed" << endl;

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
	  //cout << "Middle button pressed" << endl;
	  editor->rect_get_current_objs();
	  break;
	case 2:
	  //cout << "Right button pressed" << endl;
	  editor->scroll();
	  break;
	}
    }
  else
    {
      cout << "Unkown input device" << endl;
    }
  
  // Redraw the screen, since something may have changed
  editor->draw();
  return true;
}

bool
EditorEvent::on_button_release(CL_InputDevice *device, const CL_Key &key)
{
  if (!enabled)
    return true;

  if (device == CL_Input::keyboards[0])
    {
      switch (key.id)
	{
	case CL_KEY_LEFT:
	  editor->move_x = 0;
	  break;   
	case CL_KEY_RIGHT:
	  editor->move_x = 0;
	  break;
	case CL_KEY_UP:
	  editor->move_y = 0;
	  break;
	case CL_KEY_DOWN:
	  editor->move_y = 0;
	  break;
	}
    }
  else if (device == CL_Input::pointers[0])
    {
      if (key.id == 0)
	{
	  // cout << "Mouse release: " << key.x << key.y << endl;
	  editor->panel->on_release();
	}
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
  enabled = false;

  PingusGame game;
  string levelfile = editor->save_tmp_level();
	
  game.start(levelfile + ".plf",
	     levelfile + ".psm");
  
  enabled = true;
}

void 
EditorEvent::editor_load_level()
{
  string str;
  StringReader reader("Input filename to load the file (without .plf!)", "");
  cout << "Loading level, input filename" << endl;
  //string str = editor->read_string("Input filename to load the file (without .plf!)");
  enabled = false;
  str = reader.read_string();  
  enabled = true;

  if (!str.empty()) 
    {
      object_manager->load_level(str);
    }
}

void
EditorEvent::editor_save_level_as()
{
  cout << "Saving level, input filename" << endl;
  string str = editor->read_string("Input filename to save the file (without .plf!)");
  if (!str.empty()) 
    {
      object_manager->save_level(str);
    }
}

void
EditorEvent::editor_duplicate_current_selection()
{
  for (ObjectManager::CurrentObjIter i = object_manager->current_objs.begin(); 
       i != object_manager->current_objs.end();
       i++)
    {
      ObjectManager::EditorObjIter iter = find(object_manager->editor_objs.begin(), 
					       object_manager->editor_objs.end(), 
					       *i);
      cout << "Editor: Inserting Object: " << *iter << endl;
      object_manager->editor_objs.insert(iter, (*i)->duplicate());
    }
}

void
EditorEvent::editor_insert_new_object()
{
  editor->save_tmp_level();

  EditorObj* obj;
  try {
    enabled = false;
    obj = editor->object_selector.get_obj(object_manager->x_offset, object_manager->y_offset);
    enabled = true;
  }
  
  catch (CL_Error err) {
    cout << "Editor: Error caught from ClanLib: " << err.message << endl;
    enabled = true;
    obj = 0;
  }
      
  if (obj) {
    object_manager->editor_objs.push_back(obj);
  } else {
    cout << "Something went wrong while inserting" << endl;
  }
}

void
EditorEvent::editor_new_level()
{
  object_manager->new_level();
}

void 
EditorEvent::editor_exit()
{
  cout << "Exit editor" << endl;
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

#if 0
/*
      if (CL_Mouse::left_pressed()) 
	{
	  EditorObj* c_obj = get_current_obj();
	
	  if (c_obj == 0) 
	    {
	      if (!CL_Keyboard::get_keycode(CL_KEY_LSHIFT))
		delete_selection();
	    } 
	  else
	    {
	      if (object_selected(c_obj)) 
		{
		  if (mouse_moved()) 
		    {
		      move_obj();
		    } 
		  else 
		    {
		      if (CL_Keyboard::get_keycode(CL_KEY_LSHIFT)) {
			unmark(c_obj);
		      } else {
			delete_selection();
			current_objs.push_back(c_obj);
		      }
		    }
		} 
	      else 
		{
		  if (CL_Keyboard::get_keycode(CL_KEY_LSHIFT)) 
		    {
		      current_objs.push_back(c_obj);
		    }
		  else 
		    {
		      delete_selection();
		      current_objs.push_back(c_obj);
		    }
		  while(CL_Mouse::left_pressed())
		    CL_System::keep_alive();
		}
	    }
	  draw();
	}

      if (CL_Mouse::middle_pressed()) {
      rect_get_current_objs();
      }

      if (CL_Mouse::right_pressed()) {
	scroll();
      }

      if (mouse_x != CL_Mouse::get_x()
	  || mouse_y != CL_Mouse::get_y()) 
	{
	  draw();
	  mouse_x = CL_Mouse::get_x();
	  mouse_y = CL_Mouse::get_y();
	}
    
      if (CL_Keyboard::get_keycode(CL_KEY_RSHIFT)) 
	{
	  move_offset = 10;
	}
      else 
	{
	  move_offset = 1;
	  }*/
      /*
	if (CL_Keyboard::get_keycode(CL_KEY_LEFT)) 
	{
	for (CurrentObjIter i = current_objs.begin(); i != current_objs.end(); ++i)
	(*i)->x_pos -= move_offset;
	draw();
	} 
	else if (CL_Keyboard::get_keycode(CL_KEY_RIGHT)) 
	{
	for (CurrentObjIter i = current_objs.begin(); i != current_objs.end(); ++i)
	(*i)->x_pos += move_offset;
	draw();
	}
    
	if (CL_Keyboard::get_keycode(CL_KEY_UP)) 
	{
	for (CurrentObjIter i = current_objs.begin(); i != current_objs.end(); ++i)
	(*i)->y_pos -= move_offset;
	draw();
	} 
	else if (CL_Keyboard::get_keycode(CL_KEY_DOWN)) 
	{
	for (CurrentObjIter i = current_objs.begin(); i != current_objs.end(); ++i)
	(*i)->y_pos += move_offset;
	draw();
	}
      */
#endif    

/* EOF */
