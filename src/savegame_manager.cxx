//  $Id: savegame_manager.cxx,v 1.2 2003/03/28 12:06:32 grumbel Exp $
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
#include "system.hxx"
#include "xml_helper.hxx"
#include "savegame_manager.hxx"

SavegameManager* SavegameManager::instance_;

SavegameManager*
SavegameManager::instance()
{
  if (instance_)
    return instance_; 
  else
    return (instance_ = new SavegameManager(System::get_statdir() + "stat/savegame.xml")); 
}

SavegameManager::SavegameManager()
{
}

SavegameManager::SavegameManager(const std::string& arg_filename)
  : filename(arg_filename)
{
  xmlDocPtr doc = xmlParseFile(filename.c_str());
  
  if (!doc)
    {
      std::cout << "SavegameManager: Couldn't find savegame file '" << filename
                << "', starting with a empty one." << std::endl;
    }
  else
    {
      xmlNodePtr cur = doc->ROOT;
  
      cur = XMLhelper::skip_blank(cur);
  
      if (cur && XMLhelper::equal_str(cur->name, "pingus-savegame"))
        {
          cur = XMLhelper::skip_blank(cur);
          cur = cur->children;
          cur = XMLhelper::skip_blank(cur);

          while(cur)
            {
              if (XMLhelper::equal_str(cur->name, "level"))
                {
                  Savegame* savegame = new Savegame(doc, cur);
                  SavegameTable::iterator i = savegames.find(savegame->levelname);

                  if (i != savegames.end())
                    {
                      std::cout << "SavegameManager: name collision: " << savegame->levelname << std::endl;
                      delete i->second;
                      i->second = savegame;
                    }
                  else
                    {
                      //std::cout << "SavegameManager: Loading savegame for: " << savegame->levelname << std::endl;
                      savegames[savegame->levelname] = savegame;
                    }
                }
              else
                {
                  std::cout << "SavegameManager: Unknownen tag: " << cur->name << std::endl; 
                }
          
              cur = cur->next;
              cur = XMLhelper::skip_blank(cur);
            }
        }

  
      xmlFreeDoc(doc);
    }
}

Savegame*
SavegameManager::get(const std::string& levelname)
{
  SavegameTable::iterator i = savegames.find(levelname);
  if (i == savegames.end())
    {
      return 0;
    }
  else
    {
      return i->second;
    }
}

void
SavegameManager::store(Savegame& arg_savegame)
{
  Savegame* savegame = new Savegame(arg_savegame);
  SavegameTable::iterator i = savegames.find(savegame->levelname);
  if (i == savegames.end())
    {
      savegames[savegame->levelname] = savegame;
    }
  else
    {
      delete i->second;
      i->second = savegame;
    }
  
  flush();
}

void
SavegameManager::flush()
{
  std::ofstream xml(filename.c_str());
  xml << "<?xml version=\"1.0\"  encoding=\"ISO-8859-1\"?>\n\n"
      << "<pingus-savegame>\n";
    
  for(SavegameTable::iterator i = savegames.begin(); i != savegames.end(); ++i)
    {
      assert(i->second);
      i->second->write_xml(xml);
    }

  xml << "</pingus-savegame>\n" 
      << std::endl;
}

/* EOF */
