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

#include "pingus/stat_manager.hpp"

#include <assert.h>

#include <strut/from_string.hpp>
#include <strut/to_string.hpp>

#include "util/reader.hpp"
#include "util/writer.hpp"
#include "util/system.hpp"

namespace pingus {

StatManager* StatManager::instance_ = nullptr;

StatManager*
StatManager::instance()
{
  assert(instance_);
  return instance_;
}

StatManager::StatManager(std::string const& arg_filename)
  : statfilename(System::get_userdir() + arg_filename),
    stats()
{
  load(statfilename);

  assert(instance_ == nullptr);
  instance_ = this;
}

StatManager::~StatManager()
{
  flush();
  instance_ = nullptr;
}

void
StatManager::load(std::string const& filename)
{
  if (!System::exist(filename))
  {
    // Create empty file
    save(filename);
  }

  auto doc = ReaderDocument::from_file(filename);
  if (doc.get_name() != "pingus-stats")
  {
    std::cerr << "Error: " << filename << ": not a (pingus-stats) file" << std::endl;
    return;
  }
  else
  {
    ReaderMapping mapping = doc.get_mapping();
    std::vector<std::string> const& section_names = mapping.get_keys();
    for(std::vector<std::string>::const_iterator i = section_names.begin();
        i != section_names.end(); ++i)
    {
      mapping.read(i->c_str(), stats[*i]);
    }
  }
}

void
StatManager::flush()
{
  save(statfilename);
}

void
StatManager::save(std::string const& filename)
{
  std::ostringstream out;
  Writer writer(out);

  writer.begin_object("pingus-stats");

  for (Table::iterator i = stats.begin(); i != stats.end(); ++i)
  {
    if (!i->second.empty())
      writer.write(i->first.c_str(), i->second);
  }

  writer.end_object();

  System::write_file(filename, out.str());
}

bool
StatManager::get_int(std::string const& name, int& value)
{
  std::string str;
  if (get_string(name, str))
    return strut::from_string(str, value);
  else
    return false;
}

bool
StatManager::get_bool(std::string const& name, bool& value)
{
  std::string str;
  if (get_string(name, str))
    return strut::from_string(str, value);
  else
    return false;
}

bool
StatManager::get_string(std::string const& name, std::string& value)
{
  Table::iterator i = stats.find(name);
  if (i == stats.end())
  {
    return false;
  }
  else
  {
    value = i->second;
    return true;
  }
}

void
StatManager::set_string(std::string const& name, std::string const& value)
{
  stats[name] = value;
  flush();
}

void
StatManager::set_int(std::string const& name, int value)
{
  stats[name] = strut::to_string(value);
  flush();
}

void
StatManager::set_bool(std::string const& name, bool value)
{
  stats[name] = strut::to_string(value);
  flush();
}

} // namespace pingus

/* EOF */
