//  Pingus - A free Lemmings clone
//  Copyright (C) 2002 Ingo Ruhnke <grumbel@gmx.de>
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

#include "pingus/pingus_level.hpp"

#include <iostream>
#include <stdexcept>

#include "pingus/debug.hpp"
#include "pingus/globals.hpp"
#include "pingus/pingus_level_impl.hpp"
#include "util/file_reader.hpp"
#include "util/pathname.hpp"

PingusLevel::PingusLevel()
  : impl(new PingusLevelImpl())  
{
}

PingusLevel::PingusLevel(const Pathname& pathname)
  : impl(new PingusLevelImpl())
{
  load("", pathname);
}

PingusLevel::PingusLevel(const std::string& resname,
                         const Pathname& pathname)
  : impl(new PingusLevelImpl())
{
  load(resname, pathname);
}

void
PingusLevel::load(const std::string& resname,
                  const Pathname& pathname)
{
  impl->resname = resname;
  FileReader reader = FileReader::parse(pathname);

  if (reader.get_name() != "pingus-level")
    {
      throw std::runtime_error("Error: " + pathname.str() + ": not a 'pingus-level' file");
    }
  else
    {
      int version;
      if (reader.read_int("version", version))
        pout(PINGUS_DEBUG_LOADING) << "Levelfile Version: " << version << std::endl;
      else
        pout(PINGUS_DEBUG_LOADING) << "Unknown Levelfile Version: " << version << std::endl;

      FileReader head;
      if (!reader.read_section("head", head))
        {
          throw std::runtime_error("Error: (head) section not found in '" + pathname.str() + "'");
        }
      else
        {
          pout(PINGUS_DEBUG_LOADING) << "Reading head" << std::endl;
          head.read_string("levelname",        impl->levelname);
          head.read_string("description",      impl->description);
          head.read_size  ("levelsize",        impl->size);
          head.read_string("music",            impl->music);
          head.read_int   ("time",             impl->time);
          head.read_int   ("difficulty",       impl->difficulty);
          head.read_int   ("number-of-pingus", impl->number_of_pingus);
          head.read_int   ("number-to-save",   impl->number_to_save);
          head.read_color ("ambient-light",    impl->ambient_light);
          head.read_string("author",           impl->author);

          pout(PINGUS_DEBUG_LOADING) << "Size: " << impl->size.width << " " << impl->size.height << std::endl;
          
          FileReader actions;
          if (head.read_section("actions", actions))
            {
              std::vector<std::string> lst = actions.get_section_names();
              for(std::vector<std::string>::iterator i = lst.begin(); i != lst.end(); ++i)
                {
                  int count = 0;
                  pout(PINGUS_DEBUG_LOADING) << "Actions: " << i->c_str() << std::endl;
                  if (actions.read_int(i->c_str(), count))
                    impl->actions[*i] = count;
                }
            }
          else
            {
              throw std::runtime_error("Error: (pingus-level head actions) not found in '" + pathname.str() + "'"); 
            }
        }
      
      FileReader objects;
      if (reader.read_section("objects", objects))
        {
          std::vector<FileReader> object_lst = objects.get_sections();
          for(std::vector<FileReader>::iterator i = object_lst.begin(); i != object_lst.end(); ++i)
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

int
PingusLevel::get_difficulty() const
{
  return impl->difficulty;
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

const std::vector<FileReader>&
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
