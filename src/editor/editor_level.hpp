// Pingus - A free Lemmings clone
// Copyright (C) 2007 Jason Green <jave27@gmail.com>,
//                    Ingo Ruhnke <grumbel@gmail.com>
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef HEADER_PINGUS_EDITOR_EDITOR_LEVEL_HPP
#define HEADER_PINGUS_EDITOR_EDITOR_LEVEL_HPP

#include <list>
#include <map>
#include <memory>
#include <vector>

#include "math/size.hpp"
#include "editor/level_obj.hpp"

class Pathname;
class Writer;

namespace Editor {

class EditorScreen;
class LevelImpl;
class LevelObj;

class EditorLevel
{
public:
  static std::unique_ptr<EditorLevel> from_level_file(const Pathname& pathname);
  static std::unique_ptr<EditorLevel> from_prefab_file(const Pathname& pathname);

public:
  typedef std::list<LevelObjPtr> Objects;

  EditorLevel();
  ~EditorLevel();

  /** Save the level to a file.  Returns true if successful */
  void save_level(Writer& writer);
  void save_level(const std::string& filename);
  void save_prefab(const std::string& filename);

  Size get_size() const;
  void set_size(const Size& s);

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
  std::string get_music() const;

  void set_time(int);
  void set_music(const std::string&);

  /** Sorts the level according to the objects z-pos */
  void sort();

  void set_action(const std::string& actionname, int count);
  std::map<std::string, int> get_actions() const;

  void raise_object(LevelObjPtr obj);
  void lower_object(LevelObjPtr obj);

  void raise_object_to_top(LevelObjPtr obj);
  void lower_object_to_bottom(LevelObjPtr obj);

  Objects* get_objects();

  void add_object(LevelObjPtr obj);
  LevelObjPtr object_at (int x, int y);

private:
  std::unique_ptr<LevelImpl> impl;

private:
  EditorLevel (const EditorLevel&);
  EditorLevel& operator= (const EditorLevel&);
};

} // Editor namespace

#endif

/* EOF */
