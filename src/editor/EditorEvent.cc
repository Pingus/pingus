//  $Id: EditorEvent.cc,v 1.35 2000/12/30 23:54:06 grumbel Exp $
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

#include <vector>

#include "../globals.hh"
#include "../PingusGame.hh"
#include "../PingusMessageBox.hh"
#include "../globals.hh"
#include "../System.hh"
#include "../PingusError.hh"
#include "../System.hh"
#include "../Loading.hh"
#include "../Display.hh"
#include "../backgrounds/SurfaceBackgroundData.hh"
#include "ObjectManager.hh"
#include "EditorObjGroup.hh"
#include "StringReader.hh"
#include "EditorEvent.hh"

EditorEvent::EditorEvent()
{
  enable();
}

EditorEvent::~EditorEvent()
{
  
}

void
EditorEvent::set_editor(Editor* e)
{
  editor = e;
  editor->last_level = System::get_statdir() + "levels/dist/";
  object_manager = editor->object_manager;
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

void
EditorEvent::on_button_press(CL_InputDevice *device, const CL_Key& key)
{
  if (!is_enabled)
    return;

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
	  {
	    // FIXME
	    std::cout << "EditorEvent: Background settings is currently not supported" << std::endl;
	    /*
	    SurfaceBackgroundData* sur_bg;
	    if ((sur_bg = dynamic_cast<SurfaceBackgroundData*>(*(object_manager->backgrounds.begin()))) != 0)
	      sur_bg->desc.res_name = editor->object_selector->get_background();
	    */
	  }
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

	  // Lower all object in the current selection
	case CL_KEY_PAGEDOWN:
	  object_manager->lower_current_objs();
	
	  if (CL_Keyboard::get_keycode(CL_KEY_RSHIFT)) 
	    {
	      for (ObjectManager::CurrentObjIter i = object_manager->current_objs.begin(); 
		   i != object_manager->current_objs.end(); 
		   ++i)
		{
		  (*i)->set_position_offset(0, 0, -50);
		}
	    }
	  else if (CL_Keyboard::get_keycode(CL_KEY_RCTRL))
	    {
	      for (ObjectManager::CurrentObjIter i = object_manager->current_objs.begin(); 
		   i != object_manager->current_objs.end(); 
		   ++i)
		{
		  (*i)->set_position_offset(0, 0, -5);
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
		  (*i)->set_position_offset(0, 0, 50);
		}
	    }
	  else if (CL_Keyboard::get_keycode(CL_KEY_RCTRL)) 
	    {
	      for (ObjectManager::CurrentObjIter i = object_manager->current_objs.begin(); 
		   i != object_manager->current_objs.end(); 
		   ++i)
		{
		  (*i)->set_position_offset(0, 0, 5);
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
	    editor->panel->on_click();
	  else if (editor->scroll_map->mouse_over (key.x, key.y))
	    editor->scroll_map->on_button_press(device, key);
	  else
	    editor_mark_or_move_object();
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
}

void
EditorEvent::on_button_release(CL_InputDevice *device, const CL_Key &key)
{
  if (!is_enabled)
    return;

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
  
  return;
}

void
EditorEvent::editor_convert_group_to_selection()
{
  if (object_manager->current_objs.size() == 1)
    {
      boost::shared_ptr<EditorObj> obj = *(object_manager->current_objs.begin());
      EditorObjGroup* group = dynamic_cast<EditorObjGroup*>(obj.get());

      if (group)
	{      
	  std::list<boost::shared_ptr<EditorObj> >* objs = group->get_objs();
	  
	  object_manager->editor_objs.erase(std::find(object_manager->editor_objs.begin(), 
						      object_manager->editor_objs.end(),
						      obj));
	  object_manager->delete_selection();

	  for(std::list<boost::shared_ptr<EditorObj> >::iterator i = objs->begin();
	      i != objs->end();
	      i++)
	    {
	      object_manager->editor_objs.push_back(*i);
	      object_manager->current_objs.push_back(*i);
	    }

	  objs->clear();
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
  if (object_manager->current_objs.size() > 1)
    {
      boost::shared_ptr<EditorObjGroup> group(new EditorObjGroup());
      std::vector<std::list<boost::shared_ptr<EditorObj> >::iterator> to_erase;

      // We need to collect the objects out of the editor_objs list to keep the correct sorting
      for (ObjectManager::EditorObjIter j = object_manager->editor_objs.begin();
	   j != object_manager->editor_objs.end();
	   j++)
	{
	  for (ObjectManager::CurrentObjIter i = object_manager->current_objs.begin();
	       i != object_manager->current_objs.end();
	       i++)
	    { 
	      if (*j == *i)
		{
		  group->push_back(*i);
		  to_erase.push_back(std::find(object_manager->editor_objs.begin(), object_manager->editor_objs.end(), *i));
		}
	    }
	}

      for(std::vector<std::list<boost::shared_ptr<EditorObj> >::iterator>::iterator i = to_erase.begin();
	  i != to_erase.end();
	  i++)
	object_manager->editor_objs.erase(*i);

      object_manager->editor_objs.push_back(group);
      object_manager->delete_selection();
      object_manager->add_to_selection(group);
    }
  else
    {
      std::cout << "EditorEvent::editor_convert_selection_to_group: Not enough objects selected for a group" << std::endl;
    }
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
  object_manager->bg.red   += 0.1;
  object_manager->bg.green += 0.1;
  object_manager->bg.blue  += 0.1;
      
  if (object_manager->bg.red > 1.0) 
    {
      object_manager->bg.red   = 0.0;
      object_manager->bg.green = 0.0;
      object_manager->bg.blue  = 0.0;
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
      object_manager->editor_objs.erase(std::find(object_manager->editor_objs.begin(), object_manager->editor_objs.end(), *i));
    }
  
  object_manager->delete_selection();
}

void
EditorEvent::editor_start_current_level()
{
  disable();

  loading_screen.draw();

  try {
    PingusGame game;
    std::string levelfile = editor->save_tmp_level();
    /*
      game.start_game(levelfile + ".plf",
      levelfile + ".psm");*/
    game.start_game(levelfile);
  }
  
  catch(PingusError err) {
    std::cout << "Editor: Error caught from Pingus: " << err.get_message () << std::endl;
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

  dir = System::opendir(System::get_statdir() + "levels/dist/", "*");

  for (System::Directory::iterator i = dir.begin(); i != dir.end(); i++)
    {
      std::cout << "dirs: " << System::get_statdir() + "levels/dist/" +  i->name << std::endl;
      
      temp_str = System::get_statdir() + "levels/dist/" + i->name;

      strings.push_back(temp_str);
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

  StringReader reader("Input filename to save the file (without .plf!)", editor->last_level);

  dir = System::opendir(System::get_statdir() + "levels/dist/", "*");

  for (System::Directory::iterator i = dir.begin(); i != dir.end(); i++)
    {
      std::cout << "dirs: " << System::get_statdir() + "levels/dist/" +  i->name << std::endl;
      str = System::get_statdir() + "levels/dist/" + i->name;

      strings.push_back(str);
    }

  reader.set_strings(&strings);

  disable();
  str = reader.read_string();
  enable();

  if (!str.empty()) 
    {
      if (str.substr(str.size() - 4) ==  ".plf")
	object_manager->save_level(str);
      else if (str.substr(str.size() - 4) ==  ".xml")
	object_manager->save_level_xml(str);	
      else
	object_manager->save_level_xml(str + ".xml");

      editor->last_level = str;
    }
}

void
EditorEvent::editor_duplicate_current_selection()
{
  std::list<boost::shared_ptr<EditorObj> > new_objs;
  
  for (ObjectManager::CurrentObjIter i = object_manager->current_objs.begin(); 
       i != object_manager->current_objs.end();
       i++)
    {
      ObjectManager::EditorObjIter iter = std::find(object_manager->editor_objs.begin(), 
						    object_manager->editor_objs.end(), 
						    *i);
      
      boost::shared_ptr<EditorObj> obj = (*i)->duplicate();
      if (obj.get())
	{
	  object_manager->editor_objs.insert(iter, obj);
	  new_objs.push_back(obj);
	}
    }

  object_manager->delete_selection();
  object_manager->add_to_selection(new_objs);
}

void
EditorEvent::editor_insert_new_object()
{
  editor->save_tmp_level();

  std::list<boost::shared_ptr<EditorObj> > objs;
  try 
    {
      disable();
      objs = editor->object_selector->get_obj(object_manager->x_offset, object_manager->y_offset);
      enable();
    }
  
  catch (CL_Error err) {
    std::cout << "Editor: Error caught from ClanLib: " << err.message << std::endl;
    enable();
  }
      
  if (!objs.empty ()) 
    {
      for (std::list<boost::shared_ptr<EditorObj> >::iterator i = objs.begin (); i != objs.end (); i++)
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
  boost::shared_ptr<EditorObj> obj = object_manager->select_object(CL_Mouse::get_x(), CL_Mouse::get_y());
  
  if (obj.get())
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
