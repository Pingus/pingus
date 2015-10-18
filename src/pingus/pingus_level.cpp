// Pingus - A free Lemmings clone
// Copyright (C) 2002 Ingo Ruhnke <grumbel@gmail.com>
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

#include "pingus/pingus_level.hpp"

#include <stdexcept>

#include "pingus/globals.hpp"
#include "pingus/pingus_level_impl.hpp"
#include "util/log.hpp"
#include "util/pathname.hpp"
#include "util/raise_exception.hpp"
#include "util/system.hpp"

PingusLevel::PingusLevel() :
  impl(new PingusLevelImpl())
{
}

PingusLevel::PingusLevel(const Pathname& pathname) :
  impl(std::make_shared<PingusLevelImpl>())
{
  load("", pathname);
}

PingusLevel::PingusLevel(const std::string& resname, const Pathname& pathname) :
  impl(std::make_shared<PingusLevelImpl>())
{
  load(resname, pathname);
}

std::string
PingusLevel::get_checksum() const
{
  return impl->checksum;
}

void
PingusLevel::load(const std::string& resname,
                  const Pathname& pathname)
{
  impl->checksum = System::checksum(pathname);

  impl->resname = resname;
  ReaderObject reader_object = Reader::parse(pathname);

  if (reader_object.get_name() != "pingus-level")
  {
    raise_exception(std::runtime_error, "Error: " << pathname.str() << ": not a 'pingus-level' file");
  }
  else
  {
    ReaderMapping reader = reader_object.get_mapping();

    int version;
    if (reader.read_int("version", version))
      log_info("Levelfile Version: %1%", version);
    else
      log_info("Unknown Levelfile Version: %1%", version);

    ReaderMapping head;
    if (!reader.read_mapping("head", head))
    {
      raise_exception(std::runtime_error, "Error: (head) section not found in '" << pathname.str() << "'");
    }
    else
    {
      log_info("Reading head");
      head.read_string("levelname",        impl->levelname);
      head.read_string("description",      impl->description);
      head.read_size  ("levelsize",        impl->size);
      head.read_string("music",            impl->music);
      head.read_int   ("time",             impl->time);
      head.read_int   ("number-of-pingus", impl->number_of_pingus);
      head.read_int   ("number-to-save",   impl->number_to_save);
      head.read_colorf("ambient-light",    impl->ambient_light);
      head.read_string("author",           impl->author);

      log_info("Size: %1%x%2%", impl->size.width, impl->size.height);

      ReaderMapping actions;
      if (head.read_mapping("actions", actions))
      {
        std::vector<std::string> lst = actions.get_keys();
        for(std::vector<std::string>::iterator i = lst.begin(); i != lst.end(); ++i)
        {
          int count = 0;
          log_info("Actions: %1%", i->c_str());
          if (actions.read_int(i->c_str(), count))
            impl->actions[*i] = count;
        }
      }
      else
      {
        raise_exception(std::runtime_error,
                        "Error: (pingus-level head actions) not found in '" << pathname.str() << "'");
      }
    }

    ReaderCollection collection;
    if (reader.read_collection("objects", collection))
    {
      std::vector<ReaderObject> object_lst = collection.get_objects();
      for(auto i = object_lst.begin(); i != object_lst.end(); ++i)
      {
        impl->objects.push_back(*i);
      }
    }
  }
}

const std::string&
PingusLevel::get_levelname() const
{
  return impl->levelname;
}

const std::string&
PingusLevel::get_description() const
{
  return impl->description;
}

const Size&
PingusLevel::get_size() const
{
  return impl->size;
}

int
PingusLevel::get_number_of_pingus() const
{
  return impl->number_of_pingus;
}

int
PingusLevel::get_number_to_save() const
{
  return impl->number_to_save;
}

const std::map<std::string, int>&
PingusLevel::get_actions() const
{
  return impl->actions;
}

int
PingusLevel::get_time() const
{
  return impl->time;
}

const std::string&
PingusLevel::get_author() const
{
  return impl->author;
}

const std::string&
PingusLevel::get_music() const
{
  return impl->music;
}

const std::vector<ReaderObject>&
PingusLevel::get_objects() const
{
  return impl->objects;
}

const std::string
PingusLevel::get_resname() const
{
  return impl->resname;
}

const Color&
PingusLevel::get_ambient_light() const
{
  return impl->ambient_light;
}

/* EOF */
