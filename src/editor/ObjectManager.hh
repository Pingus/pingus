//  $Id: ObjectManager.hh,v 1.8 2000/07/30 01:47:37 grumbel Exp $
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

#ifndef OBJECTMANAGER_HH
#define OBJECTMANAGER_HH

#include "EditorObj.hh"

/// Manager for all level objects.
class ObjectManager
{
private:
  ///
  friend class EditorEvent;
  ///
  friend class Editor;
  
  /// Other Level data
  map<std::string, std::string> description;
  ///
  map<std::string, std::string> levelname;
  ///
  std::vector<ActionData> actions;
  ///
  BackgroundData background;
  ///
  int    number_to_save;
  ///
  int    number_of_pingus;
  ///
  int    level_time;
  ///
  int    height;
  ///
  int    width;
  ///
  int    start_x_pos;
  ///
  int    start_y_pos;
  ///
  std::string author;
  ///
  bool   quit;

  ///
  std::list<EditorObj*> editor_objs;
  ///
  std::list<EditorObj*> current_objs;

  ///
  int x_offset;
  ///
  int y_offset;
  ///
  int move_offset;

  ///
  struct RGB { 
    ///
    float r;
    ///
    float g;
    ///
    float b; 
  };
  
  ///
  RGB bg;
  
public:
  ///
  typedef std::list<EditorObj*>::iterator EditorObjIter;
  ///
  typedef std::list<EditorObj*>::reverse_iterator EditorObjRIter;
  ///
  typedef std::list<EditorObj*>::iterator CurrentObjIter;

  ///
  ObjectManager();
  ///
  ~ObjectManager();

  /// Create a new level
  void new_level ();
  ///
  void load_level (std::string filename);
  /// Save the current level (obsolete)
  void save_level (std::string filename);
  /// Save the current level in an xml file
  void save_level_xml (std::string filename);
  ///
  void draw();
  
  ///
  bool object_selected(EditorObj* c_obj);
  ///
  void move_obj();
  ///
  void delete_selection();
  ///
  void rect_get_current_objs(int x1, int y1, int x2, int y2);

  ///
  void move_current_objs(int x, int y);
  ///
  void add_to_selection(EditorObj* obj);
  ///
  void add_to_selection(std::list<EditorObj*> objs);

  ///
  bool raise_obj(EditorObj* obj);
  ///
  void raise_current_objs();
  ///
  bool lower_obj(EditorObj* obj);
  ///
  void lower_current_objs();

  ///
  void unselect_object(EditorObj* c_obj);
  ///
  EditorObj* select_object(int x, int y);
}///
;

#endif

/* EOF */
