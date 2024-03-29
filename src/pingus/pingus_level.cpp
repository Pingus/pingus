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

#include <logmich/log.hpp>

#include "pingus/globals.hpp"
#include "pingus/pingus_level_impl.hpp"
#include "util/pathname.hpp"
#include "util/raise_exception.hpp"
#include "util/system.hpp"

namespace pingus {

PingusLevel::PingusLevel() :
  impl(new PingusLevelImpl())
{
}

PingusLevel::PingusLevel(Pathname const& pathname) :
  impl(std::make_shared<PingusLevelImpl>())
{
  load("", pathname);
}

PingusLevel::PingusLevel(std::string const& resname, Pathname const& pathname) :
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
PingusLevel::load(std::string const& resname,
                  Pathname const& pathname)
{
  impl->checksum = System::checksum(pathname);

  impl->resname = resname;
  impl->doc = prio::ReaderDocument::from_file(pathname.get_sys_path());

  if (impl->doc.get_name() != "pingus-level")
  {
    raise_exception(std::runtime_error, "Error: " << pathname.str() << ": not a 'pingus-level' file");
  }
  else
  {
    ReaderMapping reader = impl->doc.get_mapping();

    int version;
    if (reader.read("version", version))
      log_info("Levelfile Version: {}", version);
    else
      log_info("Unknown Levelfile Version: {}", version);

    ReaderMapping head;
    if (!reader.read("head", head))
    {
      raise_exception(std::runtime_error, "Error: (head) section not found in '" << pathname.str() << "'");
    }
    else
    {
      log_info("Reading head");
      head.read("levelname",        impl->levelname);
      head.read("description",      impl->description);
      head.read("levelsize",        impl->size);
      head.read("music",            impl->music);
      head.read("time",             impl->time);
      head.read("number-of-pingus", impl->number_of_pingus);
      head.read("number-to-save",   impl->number_to_save);
      Colorf tmp_colorf;
      if (head.read("ambient-light", tmp_colorf)) {
        impl->ambient_light = tmp_colorf.to_color();
      }
      head.read("author",           impl->author);

      log_info("Size: {}x{}", impl->size.width(), impl->size.height());

      ReaderMapping actions;
      if (head.read("actions", actions))
      {
        std::vector<std::string> lst = actions.get_keys();
        for(std::vector<std::string>::iterator i = lst.begin(); i != lst.end(); ++i)
        {
          int count = 0;
          log_info("Actions: {}", i->c_str());
          if (actions.read(i->c_str(), count))
            impl->actions[*i] = count;
        }
      }
      else
      {
        raise_exception(std::runtime_error,
                        "Error: (pingus-level head actions) not found in '" << pathname.str() << "'");
      }
    }

    reader.read("objects", impl->objects);
  }
}

std::string const&
PingusLevel::get_levelname() const
{
  return impl->levelname;
}

std::string const&
PingusLevel::get_description() const
{
  return impl->description;
}

Size const&
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

std::string const&
PingusLevel::get_author() const
{
  return impl->author;
}

std::string const&
PingusLevel::get_music() const
{
  return impl->music;
}

ReaderCollection const&
PingusLevel::get_objects() const
{
  return impl->objects;
}

std::string const&
PingusLevel::get_resname() const
{
  return impl->resname;
}

Color const&
PingusLevel::get_ambient_light() const
{
  return impl->ambient_light;
}

} // namespace pingus

/* EOF */
