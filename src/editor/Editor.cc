//  $Id: Editor.cc,v 1.24 2001/05/13 18:45:08 grumbel Exp $
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
#include <algorithm>
#include <ClanLib/core.h>

#include "../algo.hh"
#include "../globals.hh"
#include "../PingusError.hh"
#include "../PingusMessageBox.hh"
#include "../Display.hh"
#include "../PingusResource.hh"
#include "../System.hh"
#include "Editor.hh"

Editor::Editor ()
{
  move_x = 0;
  move_y = 0;
  event_handler_ref_counter = 0;

  EditorObj::set_editor(this);

  event      = new EditorEvent;
  panel      = new Panel;
  scroll_map = new ScrollMap;
  object_manager  = new ObjectManager;
  status_line     = new StatusLine;
  object_selector = new ObjectSelector;
  
  event->set_editor(this);
  
  font = PingusResource::load_font("Fonts/courier_small", "fonts");
  panel->init();
  status_line->set_current_objs(&object_manager->current_objs);
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

      on_button_press_slot = CL_Input::sig_button_press.connect(CL_CreateSlot(event, &EditorEvent::on_button_press));
      on_button_release_slot = CL_Input::sig_button_release.connect(CL_CreateSlot(event, &EditorEvent::on_button_release));

      if (verbose) std::cout << "done: " << event_handler_ref_counter << std::endl;
    }
  else
    {
      if (verbose) std::cout << "Editor: event_handler allready installed" << std::endl;
    }
}

void
Editor::unregister_event_handler()
{
  event_handler_ref_counter--;
  if (verbose) std::cout << "Editor: unregestering event handler" << event << "... " << std::flush; 

  //CL_Input::chain_button_release.remove(event);
  //CL_Input::chain_button_press.remove(event);

  CL_Input::sig_button_press.disconnect (on_button_press_slot);
  CL_Input::sig_button_release.disconnect (on_button_release_slot);

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

  while (!quit) 
    {
      CL_System::keep_alive();
      move_objects();
      draw();
    }

  unregister_event_handler();

  Display::hide_cursor();
}


void
Editor::draw ()
{
  CL_Display::clear_display();
  object_manager->draw();
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

  status_line->draw(object_manager->x_offset, object_manager->y_offset);
  scroll_map->draw();

  Display::flip_display(true);
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
	  object_manager->x_offset += (mouse_x - CL_Mouse::get_x()) / 5;
	  object_manager->y_offset += (mouse_y - CL_Mouse::get_y()) / 5;
	  
	  /*cout << "ObjectManager: "
	       << "X: " << object_manager.x_offset 
	       << " Y: " << object_manager.y_offset << std::endl;
	  */
	  draw();
	}
    }
  
  if (verbose) std::cout << "finished" << std::endl;
}

std::string
Editor::read_string (std::string prefix, std::string default_str)
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
  font->print_left(20, 400, "For informations on the editor, have a look at the info pages.");
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
Editor::rect_get_current_objs()
{
  int x1 = CL_Mouse::get_x();
  int y1 = CL_Mouse::get_y();
  int x2 = CL_Mouse::get_x();
  int y2 = CL_Mouse::get_y();

  std::cout << "Editor: Selecting rectangle... " << std::flush;
  
  while (CL_Mouse::middle_pressed())
    {
      CL_System::keep_alive();

      x2 = CL_Mouse::get_x();
      y2 = CL_Mouse::get_y();

      // Draw the screen
      CL_Display::clear_display();
      object_manager->draw();
      Display::draw_rect(x1, y1, x2, y2,
			 0.0, 1.0, 0.0, 1.0);
      panel->draw();
      status_line->draw(object_manager->x_offset, object_manager->y_offset);
      Display::flip_display(true);
    }
  
  object_manager->rect_get_current_objs(x1, y1, x2, y2);

  std::cout << "finished" << std::endl;
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
      && CL_Keyboard::get_keycode(CL_KEY_RSHIFT))
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
  int mouse_x = CL_Mouse::get_x();
  int mouse_y = CL_Mouse::get_y();

  CL_System::keep_alive();
  
  while (CL_Mouse::left_pressed()) 
    {
      object_manager->move_current_objs(CL_Mouse::get_x() - mouse_x,
				       CL_Mouse::get_y() - mouse_y);
      draw();
      mouse_x = CL_Mouse::get_x();
      mouse_y = CL_Mouse::get_y();
      CL_System::keep_alive();
    }
}

void
Editor::edit_current_objs()
{
  boost::shared_ptr<EditorObj> obj = object_manager->get_current_obj();

  if (obj.get())
    obj->gui_edit_obj();
  else
    std::cout << "No single object selected" << std::endl;
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
	      level_time = 9000;
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

/***********************************************
$Log: Editor.cc,v $
Revision 1.24  2001/05/13 18:45:08  grumbel
Some more spelling error fixes by Felix

Revision 1.23  2001/03/18 17:45:05  grumbel
q&d ported pingus to the newest clanlib cvs, some things like mouse button events are not fully working yet

Revision 1.22  2000/12/16 23:11:22  grumbel
replaced most pointers with smart_ptr's, this might fix some memory holes and is probally a good start to clean up the dirty object generation code

Revision 1.21  2000/10/30 16:17:50  grumbel
- added support to disable gnu gettext
- added support for the teleporter in the editor (mostly untested)
- fixed some resource names to fit the new directory structure

I am now starting to move to the new directory structure and to the png
files, this will take some time, so expect some trouble when you do a
cvs update

Revision 1.20  2000/09/12 11:11:36  grumbel
Reinserted the signal disconnect()'s

Revision 1.19  2000/08/28 00:34:39  grumbel
Added support for multiple background types and multiple background layers
Removed some .disconnect() cause they segfault here

Revision 1.18  2000/08/11 21:17:54  grumbel
Added a level map into the ScrollMap, but its needs some optimizations

Revision 1.17  2000/08/11 01:07:34  grumbel
Some more fixes for the scrollmap, it work now basically.

Revision 1.16  2000/08/09 14:39:37  grumbel
Updated Pingus to use ClanLib 0.5 CVS, it will no longer work with ClanLib 0.4

Revision 1.15  2000/07/30 01:47:37  grumbel
XML support, currently not activated

Revision 1.14  2000/07/04 22:59:13  grumbel
Fixed scrolling to be no longer fast-forward depended, thanks to Alan Cox for finding this and some other bugs :-)
Added support for reading datafile or scriptfile (--use-datafile, --use-scriptfile)

Revision 1.13  2000/06/25 20:22:18  grumbel
Rewrote some parts of the resource management, to make it possible to transparently read real files instead of datafiles

Revision 1.12  2000/06/12 20:31:31  grumbel
Fixed handling of transparent spots in the editor
Added a faster screenshot function (only for 16bit)
Added global fps display

Revision 1.11  2000/06/10 07:57:00  grumbel
Added wrapper around CL_MouseCursor and added an option to disable the software cursor, due to probable bugs in CL_MouseCursor

Revision 1.10  2000/04/24 13:15:42  grumbel
Added  Felix Natter's namespace clean ups

Revision 1.9  2000/04/14 18:28:27  grumbel
Fixed ColMap again, bridger should work correctly again

Revision 1.8  2000/03/12 17:08:40  grumbel
Misc fixes I forgot yesterday

Revision 1.7  2000/02/28 03:50:43  grumbel
Some mouse cursor changes, to keep up with ClanLib

Revision 1.6  2000/02/25 02:35:27  grumbel
Added a more or less useless small map, which should be used to scroll around in the level, misc other changes/fixes

Revision 1.5  2000/02/15 13:09:51  grumbel
Misc cleanups, changes some messages to only appear when verbose > 0

Revision 1.4  2000/02/12 20:53:45  grumbel
Changed the rectangle selection in the editor to use non filled rects

Revision 1.3  2000/02/11 16:58:28  grumbel
Added correct namespaces

Revision 1.2  2000/02/09 21:43:43  grumbel
CVS should be up to date again...

Revision 1.42  2000/02/03 09:36:59  grumbel
Misc fixes

Revision 1.41  2000/01/29 15:50:52  grumbel
Added some buttons and added some other function, the editor should work now mostly

Revision 1.40  2000/01/28 20:44:38  grumbel
Most of the input handling is now event based, added some functionality to the new panel

Revision 1.39  2000/01/25 13:09:47  grumbel
Added a button panel

Revision 1.38  2000/01/16 16:39:25  grumbel
Added some handling for key releases

Revision 1.37  2000/01/15 20:19:14  grumbel
Changed most the Keyboard Input stuff to use EventChains

Revision 1.36  2000/01/14 18:48:10  grumbel
Misc code cleanups and fixes, moved all struct ???_data to there own files.

Revision 1.35  2000/01/11 17:43:01  grumbel
Changed the inputbuffers to the new ClanLib style, still a bit buggy

Revision 1.34  2000/01/10 20:38:20  grumbel
Changed the handling of how new actions are created (use get_uaction() now), it should be now safer and faster

Revision 1.33  2000/01/08 16:42:41  grumbel
Fixed the lowering and raising of objects

Revision 1.32  2000/01/08 00:49:44  grumbel
Added a teleporter, done by David Turner

Revision 1.31  2000/01/07 23:16:03  grumbel
Fixed objected duplication, it should work now again

Revision 1.30  2000/01/07 20:24:32  grumbel
Changed all GPL statements to a standard look & feel ;-)

Revision 1.29  2000/01/06 22:42:18  grumbel
Misc fixes

Revision 1.28  2000/01/05 22:23:11  grumbel
Misc changes and fixes

Revision 1.27  2000/01/04 00:00:08  grumbel
Misc changes and fixes

Revision 1.26  1999/12/21 12:02:03  grumbel
Misc fixes

Revision 1.25  1999/12/16 21:22:13  grumbel
Commited fix for the bridger and misc other cleanups

Revision 1.24  1999/12/13 22:49:49  grumbel
Fixed a bug in the 'buttons' section

Revision 1.23  1999/12/12 03:05:30  grumbel
Added some cvs keywords, misc other fixes
************************************************/

/* EOF */
