//  $Id: ObjectManager.hh,v 1.13 2000/10/03 20:01:24 grumbel Exp $
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

#include "../Color.hh"
#include "EditorObj.hh"

class EditorObj;

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
  std::vector<BackgroundData*> backgrounds;
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

  /** All objects which are visible in the editor */ 
  std::list<EditorObj*> editor_objs;

  /** This list contains all objects, which are in the current
      selection */
  std::list<EditorObj*> current_objs;

  ///
  int x_offset;
  ///
  int y_offset;
  ///
  int move_offset;

  Color bg;
  PLF* plf;
  
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
  void draw_scroll_map(int x_pos, int y_pos, int arg_width, int arg_height);
  
  ///
  bool object_selected(EditorObj* c_obj);
  ///
  void move_obj();
  ///
  void delete_selection();
  ///
  void delete_all_objs();
  ///
  void rect_get_current_objs(int x1, int y1, int x2, int y2);

  ///
  int get_x_offset() { return x_offset; }
  ///
  int get_y_offset() { return y_offset; }
  ///
  int get_width() { return width; }
  ///
  int get_height() { return height; }
  ///
  void set_viewpoint(int x, int y);

  /** Return the currently selected object, if none is selected or
      multiple objects are selected return 0 */
  EditorObj* get_current_obj();
  
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

  /** Remove an object from the current selection */
  void unselect_object(EditorObj* c_obj);

  /** Add the object at the given position to the selection */
  EditorObj* select_object(int x, int y);
};

#endif

/* EOF */
