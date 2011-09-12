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

#include "editor/editor_level.hpp"

#include <algorithm>

#include "editor/level_obj.hpp"
#include "editor/level_obj_factory.hpp"
#include "editor/generic_level_obj.hpp"
#include "editor/level_impl.hpp"
#include "pingus/pingus_level.hpp"
#include "pingus/prefab_file.hpp"
#include "util/log.hpp"
#include "util/system.hpp"
#include "util/sexpr_file_writer.hpp"

namespace Editor {

static bool LevelObjSort(const LevelObjPtr& a, const LevelObjPtr& b)
{
  return (a->get_pos().z < b->get_pos().z);
}

// Default constructor
EditorLevel::EditorLevel() :
  impl(new LevelImpl())
{
  set_impl_defaults();
}

/** simply replace the current level implementation with a new blank
    one */
void
EditorLevel::clear_impl()
{
  impl.reset(new LevelImpl);
}

/** assuming we have a valid implementation, set default values */
void
EditorLevel::set_impl_defaults()
{
  impl->levelname   = "";
  impl->description = "";
  impl->author = "";
  impl->music   = "";

  impl->ambient_light = Color(255,255,255);
  impl->size = Size(1920, 1200);

  impl->number_of_pingus = 50;
  impl->number_to_save   = 20;
  
  impl->actions.clear();
  impl->actions["basher"]  = 20;
  impl->actions["blocker"] = 20;
  impl->actions["bomber"]  = 20;
  impl->actions["bridger"] = 20;
  impl->actions["climber"] = 20;
  impl->actions["digger"]  = 20;
  impl->actions["floater"] = 20;
  impl->actions["jumper"]  = 20;
  impl->actions["miner"]   = 20;
  impl->actions["slider"]  = 20;
  
  impl->time = -1;
}

void
EditorLevel::clear() {
  clear_impl();
  set_impl_defaults();
}

// Default Destructor
EditorLevel::~EditorLevel()
{
}

Size
EditorLevel::get_size() const
{
  return impl->size;
}

void
EditorLevel::save_prefab(const std::string& filename)
{
  // Sort the level before saving, so that object order doesn't change
  // after a save/load cycle (load sort() too)
  sort();

  // Create new file (overwrite existing file)
  std::ostringstream out_file;

  SExprFileWriter fw(out_file);
        
  // Write header
  fw.begin_section("pingus-prefab");
  fw.write_int("version", 3);
 
  Vector3f level_center(static_cast<float>(get_size().width)/2.0f,
                        static_cast<float>(get_size().height)/2.0f);

  // Write the objects
  fw.begin_section("objects");
  for (auto it = impl->objects.begin(); it != impl->objects.end(); ++it)
  {
    LevelObjPtr obj = (*it)->duplicate(Vector2i(static_cast<int>(-level_center.x),
                                                static_cast<int>(-level_center.y)));
    obj->write_properties(fw);
  }
  fw.end_section();     // objects

  fw.end_section();     // pingus-prefab

  out_file << "\n\n;; EOF ;;" << std::endl;
        
  // Write the file
  System::write_file(filename, out_file.str());
}

// Save the level to a file.  Returns true if successful
void
EditorLevel::save_level(const std::string& filename)
{
  // Sort the level before saving, so that object order doesn't change
  // after a save/load cycle (load sort() too)
  sort();

  // Create new file (overwrite existing file)
  std::ostringstream out_file;

  SExprFileWriter fw(out_file);
        
  // Write header
  fw.begin_section("pingus-level");
  fw.write_int("version", 3);
  fw.begin_section("head");
  fw.write_string("license", "GPLv3+");
  fw.write_string("levelname", impl->levelname);
  fw.write_string("description", impl->description);
  fw.write_string("author", impl->author);
  fw.write_int("number-of-pingus", impl->number_of_pingus);
  fw.write_int("number-to-save", impl->number_to_save);
  fw.write_int("time", impl->time);
  fw.write_string("music", impl->music);
        
  // Write the list of actions to the file
  fw.begin_section("actions");
  for (auto i = impl->actions.begin(); i != impl->actions.end(); i++)
  {
    if (i->second > 0)
      fw.write_int(i->first.c_str(), i->second);
  }
  fw.end_section();     // actions

  fw.write_size("levelsize", impl->size);
  fw.end_section();     // head

  // Write the objects
  fw.begin_section("objects");
  for (auto it = impl->objects.begin(); it != impl->objects.end(); ++it)
  {
    (*it)->write_properties(fw);
  }
  fw.end_section();     // objects

  fw.end_section();     // pingus-level

  out_file << "\n\n;; EOF ;;" << std::endl;
        
  // Write the file
  System::write_file(filename, out_file.str());
}

void
EditorLevel::load_prefab(const Pathname& pathname)
{
  log_info("EditorLevel::load_level: " << pathname.str());

  // Get a new level implementation with default settings. It is a
  // good idea to set the level defaults first, in case the level file
  // doesn't set everything.
  clear();

  // Load the level from the file - we don't care what it's res_name is.
  PrefabFile prefab = PrefabFile::from_path(pathname);

  // FIMXE: there would be better way to handle prefab size, but it's
  // probably not worth the effort
  impl->size.width  = 1920;
  impl->size.height = 1200;

  // Get the objects
  const std::vector<FileReader>& objs = prefab.get_objects();
  for (auto i = objs.begin(); i != objs.end(); i++)
  {
    LevelObjPtr obj = LevelObjFactory::create(*i, impl.get());
    if (obj)
    {
      // move origin of the level to the center of it
      obj->set_pos(obj->get_pos() + Vector3f(static_cast<float>(impl->size.width)/2.0f, 
                                             static_cast<float>(impl->size.height)/2.0f));

      add_object(obj);
    }
  }

  sort(); 
}

// Load an existing level from a file
void
EditorLevel::load_level(const Pathname& pathname)
{
  log_info("EditorLevel::load_level: " << pathname.str());

  // Get a new level implementation with default settings. It is a
  // good idea to set the level defaults first, in case the level file
  // doesn't set everything.
  clear();

  // Load the level from the file - we don't care what it's res_name is.
  PingusLevel level(pathname);
        
  // Assign all of the level information to our LevelImpl
  impl->levelname        = level.get_levelname();
  impl->description      = level.get_description();
  impl->ambient_light    = level.get_ambient_light();
  impl->size             = level.get_size();
  impl->number_of_pingus = level.get_number_of_pingus();
  impl->number_to_save   = level.get_number_to_save();
  impl->actions          = level.get_actions();
  impl->time             = level.get_time();
  impl->author           = level.get_author();
  impl->music            = level.get_music();
        
  if (impl->music == "none")
    impl->music = "";

  // Get the objects
  std::vector<FileReader> objs = level.get_objects();
  for (std::vector<FileReader>::const_iterator i = objs.begin(); i != objs.end(); i++)
  {
    LevelObjPtr obj = LevelObjFactory::create(*i, impl.get());
    if (obj)
    {
      add_object(obj);
    }
  }

  sort();
}

void
EditorLevel::sort()
{
  // Sort by Z coordinate
  impl->objects.sort(LevelObjSort);
}

void
EditorLevel::set_description(const std::string& str)
{
  impl->description = str;
}

void
EditorLevel::set_levelname(const std::string& str)
{
  impl->levelname = str;
}

void
EditorLevel::set_author(const std::string& str)
{
  impl->author = str;
}

std::string
EditorLevel::get_description() const
{
  return impl->description;
}

std::string
EditorLevel::get_levelname() const
{
  return impl->levelname;
}

std::string
EditorLevel::get_author() const
{
  return impl->author;
}

int
EditorLevel::get_number_of_pingus() const
{
  return impl->number_of_pingus;
}

int
EditorLevel::get_number_to_save() const
{
  return impl->number_to_save;
}

void
EditorLevel::set_number_of_pingus(int i)
{
  impl->number_of_pingus = i;
}

void
EditorLevel::set_number_to_save(int i)
{
  impl->number_to_save = i;
}

void
EditorLevel::set_action(const std::string& actionname, int count)
{
  impl->actions[actionname] = count;
}

std::map<std::string, int>
EditorLevel::get_actions() const
{
  return impl->actions;
}

std::string
EditorLevel::get_music() const
{
  return impl->music;
}

int
EditorLevel::get_time() const
{
  return impl->time;
}

void
EditorLevel::set_time(int t)
{
  impl->time = t;
}

void
EditorLevel::set_music(const std::string& str)
{
  impl->music = str;
}

void
EditorLevel::set_size(const Size& s)
{
  impl->size = s;

  if (impl->size.width <= 0)
    impl->size.width = 1;

  if (impl->size.height <= 0)
    impl->size.height = 1;
}

void
EditorLevel::raise_object_to_top(LevelObjPtr obj)
{
  Objects::iterator it = std::find(impl->objects.begin(), impl->objects.end(), obj);
  if (it != impl->objects.end())
  {
    impl->objects.erase(it);
    impl->objects.push_back(obj);
  }
}

void
EditorLevel::lower_object_to_bottom(LevelObjPtr obj)
{
  Objects::iterator it = std::find(impl->objects.begin(), impl->objects.end(), obj);
  if (it != impl->objects.end())
  {
    impl->objects.erase(it);
    impl->objects.push_front(obj);
  }
}

struct OverlapsWith
{
  Rect rect;

  OverlapsWith(const Rect& rect_) :
    rect(rect_)
  {}

  bool operator()(LevelObjPtr obj) {
    return rect.is_overlapped(obj->get_rect());
  }
};

void
EditorLevel::raise_object(LevelObjPtr obj)
{
  Objects::iterator i = std::find(impl->objects.begin(), impl->objects.end(), obj);
  if (i == impl->objects.end())
  {
    log_error("couldn't find object: " << obj);
  }
  else
  {
    Objects::iterator j = i;
    ++j;
    j = std::find_if(j, impl->objects.end(), OverlapsWith(obj->get_rect()));

    if (j == impl->objects.end())
    {
      // object overlaps with no other object, no point in raising it
    }
    else
    {
      impl->objects.erase(i);
      impl->objects.insert(++j, obj);
    }
  }
}

void
EditorLevel::lower_object(LevelObjPtr obj)
{
  Objects::reverse_iterator i = std::find(impl->objects.rbegin(), impl->objects.rend(), obj);
  if (i == impl->objects.rend())
  {
    log_error("couldn't find object: " << obj);
  }
  else
  {
    Objects::reverse_iterator j = i;
    ++j;
    j = std::find_if(j, impl->objects.rend(), OverlapsWith(obj->get_rect()));

    if (j == impl->objects.rend())
    {
      // object overlaps with no other object, no point in lowering it
    }
    else
    {
      // the base() of base in one further then where the reverse
      // iterator was, so we have to move back to get the same
      // position
      impl->objects.erase(--(i.base()));
      impl->objects.insert(--(j.base()), obj);
    }
  }
}  

EditorLevel::Objects*
EditorLevel::get_objects()
{
  return &(impl->objects);
}

void 
EditorLevel::add_object(LevelObjPtr obj)
{
  impl->objects.push_back(obj);
}

LevelObjPtr
EditorLevel::object_at(int x, int y)
{
  // we travel reversly through the object list, so that we get the
  // top-most object
  for (auto i = (*get_objects()).rbegin (); i != (*get_objects()).rend (); ++i)
  {
    if ((*i)->is_at(x, y))
      return *i;
  }
  return LevelObjPtr();
}

} // namespace Editor

/* EOF */
