//  $Id$
//
//  Pingus - A free Lemmings clone
//  Copyright (C) 2002 Ingo Ruhnke <grumbel@gmx.de>
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

#include <iostream>
#include "file_reader.hxx"
#include "pingus_level.hxx"
#include "pingus_error.hxx"
#include "pingus_level_impl.hxx"

PingusLevel::PingusLevel()
  : impl(new PingusLevelImpl())  
{
}


PingusLevel::PingusLevel(const std::string& resname,
                         const std::string& filename)
  : impl(new PingusLevelImpl())
{
  impl->resname = resname;
  FileReader reader = FileReader::parse(filename);

  if (reader.get_name() != "pingus-level")
    {
      PingusError::raise("Error: " + filename + ": not a 'pingus-level' file");
    }
  else
    {
      int version;
      if (reader.read_int("version", version))
        std::cout << "Levelfile Version: " << version << std::endl;
      else
        std::cout << "Unknown Levelfile Version: " << version << std::endl;

      FileReader head;
      if (reader.read_section("head", head))
        {
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
          
          FileReader actions;
          if (reader.read_section("actions", actions))
            {
              std::vector<std::string> lst = reader.get_section_names();
              for(std::vector<std::string>::iterator i = lst.begin(); i != lst.end(); ++i)
                {
                  int count = 0;
                  if (actions.read_int(i->c_str(), count))
                    impl->actions[*i] = count;
                }
            }
        }
      
      FileReader objects;
      if (reader.read_section("objects", objects))
        {
          std::vector<FileReader> object_lst = reader.get_sections();
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
