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

#include "editor/editor_level.hpp"

#include <algorithm>
#include <sstream>

#include "editor/level_obj.hpp"
#include "editor/level_obj_factory.hpp"
#include "editor/generic_level_obj.hpp"
#include "pingus/pingus_level.hpp"
#include "pingus/prefab_file.hpp"
#include "util/log.hpp"
#include "util/system.hpp"
#include "util/writer.hpp"

namespace Editor {

class LevelImpl
{
public:
  // Set defaults in constructor
  LevelImpl() :
    resname(),
    levelname(),
    description(),
    ambient_light(255, 255, 255),
    size(1920, 1200),
    number_of_pingus(50),
    number_to_save(20),
    actions({
        { "basher",   20 },
        { "blocker",  20 },
        { "bomber",   20 },
        { "bridger",  20 },
        { "climber",  20 },
        { "digger",   20 },
        { "floater",  20 },
        { "jumper",   20 },
        { "miner",    20 },
        { "slider",   20 }
      }),
    time(-1),
    author(),
    music(),
    objects()
  {
  }

  /** Destructor */
  ~LevelImpl()
  {
  }

  std::string resname;

  std::string levelname;
  std::string description;

  Color    ambient_light;
  Size     size;

  int number_of_pingus;
  int number_to_save;

  std::map<std::string, int> actions;

  int time;

  std::string author;
  std::string music;

  EditorLevel::Objects objects;

private:
  LevelImpl (const LevelImpl&);
  LevelImpl& operator= (const LevelImpl&);
};

std::unique_ptr<EditorLevel>
EditorLevel::from_level_file(const Pathname& pathname)
{
  log_info("%1%", pathname.str());

  // Load the level from the file - we don't care what it's res_name is.
  PingusLevel plf(pathname);

  std::unique_ptr<EditorLevel> level(new EditorLevel);

  // Assign all of the level information to our LevelImpl
  level->impl->levelname        = plf.get_levelname();
  level->impl->description      = plf.get_description();
  level->impl->ambient_light    = plf.get_ambient_light();
  level->impl->size             = plf.get_size();
  level->impl->number_of_pingus = plf.get_number_of_pingus();
  level->impl->number_to_save   = plf.get_number_to_save();
  level->impl->actions          = plf.get_actions();
  level->impl->time             = plf.get_time();
  level->impl->author           = plf.get_author();
  level->impl->music            = plf.get_music();

  // remove obsolete "none" tag
  if (level->impl->music == "none")
  {
    level->impl->music = "";
  }

  // Get the objects
  auto objs = plf.get_objects();
  for (auto i = objs.begin(); i != objs.end(); i++)
  {
    LevelObjPtr obj = LevelObjFactory::create(*i);
    if (obj)
    {
      level->add_object(obj);
    }
  }

  level->sort();

  return level;
}

std::unique_ptr<EditorLevel>
EditorLevel::from_prefab_file(const Pathname& pathname)
{
  log_info("%1%", pathname.str());

  // Load the level from the file - we don't care what it's res_name is.
  PrefabFile prefab = PrefabFile::from_path(pathname);

  std::unique_ptr<EditorLevel> level(new EditorLevel);

  // FIMXE: there would be better way to handle prefab size, but it's
  // probably not worth the effort
  level->impl->size.width  = 1920;
  level->impl->size.height = 1200;

  // FIXME: overrides are getting ignored

  // Get the objects
  auto objs = prefab.get_objects();
  for (auto i = objs.begin(); i != objs.end(); i++)
  {
    LevelObjPtr obj = LevelObjFactory::create(*i);
    if (obj)
    {
      // move origin of the level to the center of it
      obj->set_pos(obj->get_pos() + Vector3f(static_cast<float>(level->impl->size.width)/2.0f,
                                             static_cast<float>(level->impl->size.height)/2.0f));

      level->add_object(obj);
    }
  }

  level->sort();

  return level;
}

static bool LevelObjSort(const LevelObjPtr& a, const LevelObjPtr& b)
{
  return (a->get_pos().z < b->get_pos().z);
}

// Default constructor
EditorLevel::EditorLevel() :
  impl(new LevelImpl)
{
}

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

  Writer fw(out_file);

  // Write header
  fw.begin_object("pingus-prefab");
  fw.write_int("version", 3);

  Vector3f level_center(static_cast<float>(get_size().width)/2.0f,
                        static_cast<float>(get_size().height)/2.0f);

  // Write the objects
  fw.begin_collection("objects");
  for (auto it = impl->objects.begin(); it != impl->objects.end(); ++it)
  {
    LevelObjPtr obj = (*it)->duplicate(Vector2i(static_cast<int>(-level_center.x),
                                                static_cast<int>(-level_center.y)));
    obj->write_properties(fw);
  }
  fw.end_collection();     // objects

  fw.end_object();     // pingus-prefab

  // Write the file
  System::write_file(filename, out_file.str());
}

void
EditorLevel::save_level(const std::string& filename)
{
  // Create new file (overwrite existing file)
  std::ostringstream out_file;

  Writer fw(out_file);
  save_level(fw);

  // Write the file
  System::write_file(filename, out_file.str());
}

void
EditorLevel::save_level(Writer& fw)
{
  // Sort the level before saving, so that object order doesn't change
  // after a save/load cycle (load sort() too)
  sort();

  // Write header
  fw.begin_object("pingus-level");
  fw.write_int("version", 3);
  fw.begin_mapping("head");
  fw.write_string("license", "GPLv3+");
  fw.write_string("levelname", impl->levelname);
  fw.write_string("description", impl->description);
  fw.write_string("author", impl->author);
  fw.write_int("number-of-pingus", impl->number_of_pingus);
  fw.write_int("number-to-save", impl->number_to_save);
  fw.write_int("time", impl->time);
  fw.write_string("music", impl->music);

  // Write the list of actions to the file
  fw.begin_mapping("actions");
  for (auto i = impl->actions.begin(); i != impl->actions.end(); i++)
  {
    if (i->second > 0)
      fw.write_int(i->first.c_str(), i->second);
  }
  fw.end_mapping();     // actions

  fw.write_size("levelsize", impl->size);
  fw.end_mapping();     // head

  // Write the objects
  fw.begin_collection("objects");
  for (auto it = impl->objects.begin(); it != impl->objects.end(); ++it)
  {
    (*it)->write_properties(fw);
  }
  fw.end_collection();     // objects

  fw.end_object();     // pingus-level
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
    log_error("couldn't find object: %1%", obj);
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
    log_error("couldn't find object: %1%", obj);
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
