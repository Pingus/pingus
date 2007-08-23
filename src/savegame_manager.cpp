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

#include <assert.h>
#include <fstream>
#include "system.hpp"
#include "pingus_error.hpp"
#include "sexpr_file_reader.hpp"
#include "sexpr_file_writer.hpp"
#include "savegame_manager.hpp"
#include "lisp/lisp.hpp"
#include "lisp/parser.hpp"

SavegameManager* SavegameManager::instance_ = 0;

SavegameManager*
SavegameManager::instance()
{
  if (instance_)
    return instance_;
  else
    return (instance_ = new SavegameManager("savegames/savegames.scm"));
}

void SavegameManager::deinit()
{
  delete instance_;
  instance_ = 0;
}

SavegameManager::SavegameManager(const std::string& arg_filename)
  : filename(System::get_statdir() + arg_filename)
{
  boost::shared_ptr<lisp::Lisp> sexpr;

  try {
    sexpr = lisp::Parser::parse(filename);
  }
  catch (const std::runtime_error& e) {
    std::cerr << "SavegameManager: " << e.what() << std::endl;
    return;
  }
  if (!sexpr)
    {
      std::cerr << "SavegameManager: Couldn't find savegame file '" <<
        filename << "', starting with an empty one." << std::endl;
      return;
    }

  SExprFileReader reader(sexpr->get_list_elem(0));
  if (reader.get_name() != "pingus-savegame")
    {
      std::cerr << "Error: " << filename << ": not a (pingus-savegame) file" << std::endl;
      return;
    }

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
  std::ofstream out(filename.c_str());
  SExprFileWriter sfw(out);

  sfw.begin_section("pingus-savegame");

  for(SavegameTable::iterator i = savegames.begin(); i != savegames.end(); ++i)
    {
      assert(i->second);
      i->second->write_sexpr(sfw);
    }

  sfw.end_section();	// pingus-savegame
}


/* EOF */
