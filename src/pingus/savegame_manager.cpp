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

#include "pingus/savegame_manager.hpp"

#include <sstream>

#include "lisp/parser.hpp"
#include "util/log.hpp"
#include "util/sexpr_file_reader.hpp"
#include "util/sexpr_file_writer.hpp"
#include "util/system.hpp"

SavegameManager* SavegameManager::instance_ = 0;

SavegameManager*
SavegameManager::instance()
{
  assert(instance_);
  return instance_;
}

SavegameManager::SavegameManager(const std::string& arg_filename) :
  filename(System::get_userdir() + arg_filename),
  savegames()
{
  assert(instance_ == 0);
  instance_ = this;

  if (!System::exist(filename))
  {
    log_info("%1%: savegame file not found", filename);
  }
  else
  {
    FileReader reader = FileReader::parse(filename);
    if (reader.get_name() != "pingus-savegame")
    {
      log_error("%1%: not a (pingus-savegame) file", filename);
    }
    else
    {
      const std::vector<FileReader>& sections = reader.get_sections();
      for(std::vector<FileReader>::const_iterator i = sections.begin();
          i != sections.end(); ++i)
      {
        Savegame* savegame = new Savegame(*i);
        SavegameTable::iterator j = find(savegame->get_filename());
        if (j != savegames.end())
        { // overwrite duplicates, shouldn't happen, but harmless
          log_info("name collision: %1%", savegame->get_filename());
          delete *j;
          *j = savegame;
        }
        else
        {
          savegames.push_back(savegame);
        }
      }
    }
  }
}

SavegameManager::~SavegameManager()
{
  for (SavegameTable::iterator i =  savegames.begin(); i !=  savegames.end (); ++i)
    delete *i;

  instance_ = 0;
}

Savegame*
SavegameManager::get(const std::string& filename_)
{
  SavegameTable::iterator i = find(filename_);
  if (i == savegames.end())
    return 0;
  else
    return *i;
}

void
SavegameManager::store(Savegame& arg_savegame)
{
  Savegame* savegame = new Savegame(arg_savegame);
  SavegameTable::iterator i = find(savegame->get_filename());
  if (i == savegames.end())
  { // don't know anything about the savegame
    savegames.push_back(savegame);
  }
  else
  { // already have such a savegame
    if ((*i)->get_status() == Savegame::FINISHED
        && savegame->get_status() == Savegame::ACCESSIBLE)
    { // saved savegame is better then new game
      delete savegame;
    }
    else
    { // new game is better or equal, save it
      delete *i;
      *i = savegame;
    }
  }

  flush();
}

SavegameManager::SavegameTable::iterator
SavegameManager::find(const std::string& filename_)
{
  //log_info("SavegameManager::find: \"" << filename << "\"");

  for(SavegameTable::iterator i = savegames.begin();
      i != savegames.end(); ++i)
    if ((*i)->get_filename() == filename_)
      return i;

  return savegames.end();
}

void
SavegameManager::flush()
{
  std::ostringstream out;
  SExprFileWriter writer(out);

  writer.begin_section("pingus-savegame");

  for(SavegameTable::iterator i = savegames.begin(); i != savegames.end(); ++i)
  {
    (*i)->write_sexpr(writer);
  }

  writer.end_section(); // pingus-savegame

  System::write_file(filename, out.str());
}

/* EOF */
