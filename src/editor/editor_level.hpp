//  Pingus - A free Lemmings clone
//  Copyright (C) 2007 Jason Green <jave27@gmail.com>,
//                     Ingo Ruhnke <grumbel@gmx.de>
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//  
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//  
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef HEADER_PINGUS_EDITOR_EDITOR_LEVEL_HPP
#define HEADER_PINGUS_EDITOR_EDITOR_LEVEL_HPP

#include "editor/level_impl.hpp"

class Pathname;

namespace Editor {

class LevelObj;
class EditorScreen;

class EditorLevel
{
public:
  /** Construct new blank level */
  EditorLevel();

  /** Destructor */
  ~EditorLevel();

  /** get a new, blank level implementation without any valid settings */
  void clear_impl();

  /** Set valid default level implementation settings, assuming an
      existing implementation */
  void set_impl_defaults();

  /** get a new implementation with default settings */
  void clear();

  /** Verify that level is valid */
  bool is_valid();

  /** Save the level to a file.  Returns true if successful */
  bool save_level(const std::string& filename);

  /** Load an existing level from a file */
  void load_level(const Pathname& pathname);

  Size get_size() const;
  void set_size(const Size& s);
        
  /** Return LevelImpl */
  LevelImpl* get_level_impl() { return impl; }

  void set_description(const std::string& str);
  void set_levelname(const std::string& str);
  void set_author(const std::string& str);

  std::string get_description() const;
  std::string get_levelname() const;
  std::string get_author() const;

  void set_number_of_pingus(int i);
  void set_number_to_save(int i);

  int get_number_of_pingus() const;
  int get_number_to_save() const;

  int get_time() const;
  int get_difficulty() const;
  std::string get_music() const;

  void set_time(int);
  void set_difficulty(int);
  void set_music(const std::string&);

  /** Sorts the level according to the objects z-pos */
  void sort();

  void set_action(const std::string& actionname, int count); 
  std::map<std::string, int> get_actions() const;

  void raise_object(LevelObj* obj);
  void lower_object(LevelObj* obj);

  void raise_object_to_top(LevelObj* obj);
  void lower_object_to_bottom(LevelObj* obj);

  std::vector<LevelObj*>* get_objects();

  void add_object(LevelObj* obj);
  LevelObj* object_at (int x, int y);

private:
  LevelImpl* impl;

private:
  EditorLevel (const EditorLevel&);
  EditorLevel& operator= (const EditorLevel&);
};

} // Editor namespace

#endif

/* EOF */
