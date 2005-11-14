//  $Id: savegame_manager.cxx,v 1.9 2003/10/18 23:17:27 grumbel Exp $
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
#include <assert.h>
#include <ClanLib/core.h>
#include "system.hxx"
#include "pingus_error.hxx"
#include "xml_file_reader.hxx"
#include "savegame_manager.hxx"

namespace Pingus {

SavegameManager* SavegameManager::instance_ = 0;

SavegameManager*
SavegameManager::instance()
{
  if (instance_)
    return instance_;
  else
    return (instance_ = new SavegameManager(System::get_statdir() + "savegames/savegames.xml"));
}

void SavegameManager::deinit()
{
	delete instance_;
	instance_ = 0;
}

SavegameManager::SavegameManager(const std::string& arg_filename)
  : filename(arg_filename)
{
  try 
    {
      CL_DomDocument doc(new CL_InputSource_File(filename), true);
      CL_DomElement root = doc.get_document_element();
      if (root.get_tag_name() != "pingus-savegame")
        {
          PingusError::raise("Error: " + filename + ": not a <pingus-savegame> file");
        }
      else
        {
          XMLFileReader reader(root);
          const std::vector<FileReader>& sections = reader.get_sections();
          for(std::vector<FileReader>::const_iterator i = sections.begin();
              i != sections.end(); ++i)
            {
              Savegame* savegame = new Savegame(*i);
              SavegameTable::iterator j = savegames.find(savegame->levelname);

              if (j != savegames.end())
                {
                  std::cout << "SavegameManager: name collision: " << savegame->levelname << std::endl;
                  delete j->second;
                  j->second = savegame;
                }
              else
                {
                  //std::cout << "SavegameManager: Loading savegame for: " << savegame->levelname << std::endl;
                  savegames[savegame->levelname] = savegame;
                }
            }
        }
    } 
  catch (...) 
    {
      std::cout << "SavegameManager: Couldn't find savegame file '" << filename
                << "', starting with a empty one." << std::endl;
      // FIXME: Unlock the first level
      Savegame savegame;
      savegame.status = Savegame::ACCESSIBLE;
      savegame.levelname = "tutorial/digger-tutorial2-grumbel";
      store(savegame);
    }
}

SavegameManager::~SavegameManager()
{
  for (SavegameTable::iterator i =  savegames.begin(); i !=  savegames.end (); ++i)
    delete i->second;
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
      if (i->second->status == Savegame::FINISHED
          && savegame->status == Savegame::ACCESSIBLE)
        { // saved game is better then new game
          delete savegame;
        }
      else
        { // new game is better or equal, save it
          delete i->second;
          i->second = savegame;
        }
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

} // namespace Pingus

/* EOF */
