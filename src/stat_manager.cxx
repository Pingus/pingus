//  $Id: stat_manager.cxx,v 1.8 2003/04/19 10:23:17 torangan Exp $
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
#include "string_converter.hxx"
#include "xml_helper.hxx"
#include "system.hxx"
#include "stat_manager.hxx"

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
  xmlDocPtr doc = xmlParseFile(filename.c_str());
  if (doc)
    {
            xmlNodePtr cur = doc->ROOT;

      cur = XMLhelper::skip_blank(cur);

      if (cur && XMLhelper::equal_str(cur->name, "pingus-stats"))
        {
          cur = XMLhelper::skip_blank(cur);
          cur = cur->children;
          cur = XMLhelper::skip_blank(cur);

          while(cur)
            {
              std::string name  = (const char*)(cur->name);
              std::string value = XMLhelper::parse_string(doc, cur);
              //std::cout << "Stat: " << name << " = " << value << std::endl;
              stats[name] = value;

              cur = cur->next;
              cur = XMLhelper::skip_blank(cur);
            }
        }
      else
        {
          std::cout << "StatManager: stat file '" << filename
                    << "' is corrupt or invalid, assuming an empty one." << std::endl;
        }

      xmlFreeDoc(doc);
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
    return from_string(str, value);
  else
    return false;
}

bool
StatManager::get_bool(const std::string& name, bool& value)
{
  std::string str;
  if (get_string(name, str))
    return from_string(str, value);
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
  stats[name] = to_string(value);
  flush();
}

void
StatManager::set_bool(const std::string& name, bool value)
{
  stats[name] = to_string(value);
  flush();
}

/* EOF */
