//  $Id: object_manager.hxx,v 1.14 2002/07/02 16:06:51 grumbel Exp $
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

#ifndef HEADER_PINGUS_EDITOR_OBJECT_MANAGER_HXX
#define HEADER_PINGUS_EDITOR_OBJECT_MANAGER_HXX

#include <map>
#include "../color.hxx"
#include "../action_data.hxx"
#include "editorobj.hxx"

class EditorView;

/// Manager for all level objects.
class ObjectManager
{
private:
  friend class EditorEvent;
  friend class Editor;
  
  /// Other Level data
  std::map<std::string, std::string> description;
  std::map<std::string, std::string> levelname;
  std::vector<ActionData> actions;

  int    number_to_save;
  int    number_of_pingus;
  int    level_time;
  int    height;
  int    width;
  int    start_x_pos;
  int    start_y_pos;
  std::string author;
  bool   quit;
  bool   playable;
  int    difficulty;
  std::string comment;

  /** All objects which are visible in the editor */ 
  std::vector<EditorObj*> editor_objs;

  int move_offset;

  Color bg;
  
public:
  typedef std::vector<EditorObj*>::iterator EditorObjIter;
  typedef std::vector<EditorObj*>::reverse_iterator EditorObjRIter;

  ObjectManager();
  ~ObjectManager();

  /// Create a new level
  void new_level ();
  ///
  void load_level (const std::string & filename);

  /// Save the current level in an xml file
  void save_level_xml (const std::string & filename);

  void draw(EditorView * view);

  void delete_all_objs();

  void add_object_group_from_file (const std::string& filename);

  std::vector<EditorObj*> rect_get_objs(int x1, int y1, int x2, int y2);

  /** FIXME: translation must be handled otherwise */
  std::string get_levelname () { return levelname["en"]; }
  std::string get_description () { return description["en"]; }

  // Get and Setter functions
  int get_width() { return width; }
  int get_height() { return height; }

  void set_width(int arg_width);
  void set_height(int arg_height);

  int get_number_to_save () { return number_to_save; }
  void set_number_to_save (int num) { number_to_save = num; }

  int get_number_of_pingus () { return number_of_pingus; }
  void set_number_of_pingus (int num) { number_of_pingus = num; }
    
  void set_leveltime (int num) { level_time = num; }
  int get_leveltime () { return level_time; }

  int get_difficulty () { return difficulty; }
  void set_difficulty (int num) { difficulty = num; }

  void set_playable (bool a) { playable = a; }
  bool get_playable () { return playable; }

  std::string get_author () { return author; }
  void set_author (std::string a) { author = a; }

  /** Return the currently selected object, if none is selected or
      multiple objects are selected return 0 */
  //boost::shared_ptr<EditorObj> get_current_obj();
  
  //void move_current_objs(float x, float y);
  //void drag_current_objs ();
  //void drop_current_objs ();

  //void add_to_selection(boost::shared_ptr<EditorObj> obj);
  //void add_to_selection(std::vector<boost::shared_ptr<EditorObj> > objs);

  bool raise_obj(EditorObj* obj);
  bool lower_obj(EditorObj* obj);

  /** Add a new object to the ObjectManager. The ObjectManager will
      take controll over the object and delele it, if its no longer
      needed! */
  void add (EditorObj*);

  /** Erase the object from the ObjectManager and delete it */
  void erase (EditorObj*);

  /** Erase objects from the ObjectManager and delete them */
  void erase (const std::vector<EditorObj*>&);

  /** Add the object at the given position to the selection */
  EditorObj* find_object(const CL_Vector & pos);

  std::vector<ActionData>* get_actions ();
};

#endif

/* EOF */
