//  $Id: stat_manager.cxx,v 1.9 2003/10/20 19:28:54 grumbel Exp $
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
#include <fstream>
#include <ClanLib/core.h>
#include "xml_helper.hxx"
#include "system.hxx"
#include "pingus_error.hxx"
#include "xml_file_reader.hxx"
#include "stat_manager.hxx"

namespace Pingus {

StatManager* StatManager::instance_ = 0;

StatManager*
StatManager::instance()
{
  if (instance_)
    return instance_;
  else
    return (instance_ = new StatManager(System::get_statdir() + "savegames/variables.xml"));
}

void
StatManager::init()
{
  StatManager::instance();
}

void
StatManager::deinit()
{
  instance()->flush();
  delete instance_;
  instance_ = 0;
}

StatManager::StatManager(const std::string& arg_filename)
  : statfilename(arg_filename)
{
  load(statfilename);
}

StatManager::~StatManager()
{
}

void
StatManager::load(const std::string& filename)
{
  CL_InputSourceProvider_File provider(".");
  CL_DomDocument doc(provider.open_source(filename), true);
      
  CL_DomElement root = doc.get_document_element();
      
  if (root.get_tag_name() != "pingus-stats")
    {
      PingusError::raise("Error: " + filename + ": not a <pingus-stats> file");
    }
  else
    {
      XMLFileReader reader(root);
      const std::vector<std::string>& section_names = reader.get_section_names();
      for(std::vector<std::string>::const_iterator i = section_names.begin();
          i != section_names.end(); ++i)
        {
          reader.read_string(i->c_str(), stats[*i]);
        }
    }
}

void
StatManager::flush()
{
  save(statfilename);
}

void
StatManager::save(const std::string& filename)
{
  std::ofstream xml(filename.c_str());

  xml << "<?xml version=\"1.0\"  encoding=\"ISO-8859-1\"?>\n\n"
      << "<pingus-stats>\n";

  for (Table::iterator i = stats.begin(); i != stats.end(); ++i)
    {
      if (!i->second.empty())
        xml << "  <" << i->first << ">" << i->second << "</" << i->first << ">" << std::endl;
    }

  xml << "</pingus-stats>\n";
}

bool
StatManager::get_int(const std::string& name, int& value)
{
  std::string str;
  if (get_string(name, str))
    return CL_String::from(str, value);
  else
    return false;
}

bool
StatManager::get_bool(const std::string& name, bool& value)
{
  std::string str;
  if (get_string(name, str))
    return CL_String::from(str, value);
  else
    return false;
}

bool
StatManager::get_string(const std::string& name, std::string& value)
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
StatManager::set_string(const std::string& name, const std::string& value)
{
  stats[name] = value;
  flush();
}

void
StatManager::set_int(const std::string& name, int value)
{
  stats[name] = CL_String::to(value);
  flush();
}

void
StatManager::set_bool(const std::string& name, bool value)
{
  stats[name] = CL_String::to(value);
  flush();
}

} // namespace Pingus

/* EOF */
