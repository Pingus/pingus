//  $Id: Editor.hh,v 1.2 2000/02/09 21:43:43 grumbel Exp $
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

#ifndef EDITOR_HH
#define EDITOR_HH

#include <string>
#include <list>
#include <ClanLib/core.h>

#include "../PSMParser.hh"
#include "../PLF.hh"
#include "ObjectSelector.hh"
#include "StatusLine.hh"
#include "EditorEvent.hh"
#include "ObjectManager.hh"
#include "Panel.hh"

class EditorEvent;
class Panel;

class Editor
{
private:
  EditorEvent* event;
  friend class EditorEvent;
  
  int event_handler_ref_counter;

  CL_Font* font;
  bool quit;

  int move_x;
  int move_y;
  string checkpoint;
  string last_level;
  bool always_animate;

  Panel*  panel;
  ObjectManager object_manager;
  StatusLine status_line;
  ObjectSelector object_selector;
  
public:
  Editor ();
  ~Editor ();

  string read_string (string prefix = "", string default_str = "");
  void edit ();
  //  EditorObj* get_current_obj();
  void draw ();
  void draw_noflip();
  void interactive_move_object();
  void move_objects();
  //  bool object_selected(EditorObj* c_obj);
  void rect_get_current_objs();
  //  void delete_selection();
  //  void unmark(EditorObj* c_obj);
  bool mouse_moved();
  void scroll();

  void register_event_handler();
  void unregister_event_handler();

  void enable_event_handler();
  void disable_event_handler();

  /*
  bool raise_obj(EditorObj*);
  bool lower_obj(EditorObj*);

  void raise_current_objs();
  void lower_current_objs();
  */
  string save_tmp_level ();
  /*  void   save_level (string);
  void   save_plf (string);
  void   save_psm (string);

  void load_level ();*/
  //  void interactive_load();
};

#endif

/* EOF */

