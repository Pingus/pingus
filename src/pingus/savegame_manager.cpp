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

#include "pingus/savegame_manager.hpp"

#include <sstream>

#include "util/log.hpp"
#include "util/reader.hpp"
#include "util/writer.hpp"
#include "util/system.hpp"
#include "util/mem.hpp"

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
    ReaderObject reader = Reader::parse(filename);
    if (reader.get_name() != "pingus-savegame")
    {
      log_error("%1%: not a 'pingus-savegame' file", filename);
    }
    else
    {
      ReaderCollection levels_collection;
      reader.get_mapping().read_collection("levels", levels_collection);
      for(auto const& level_object : levels_collection.get_objects())
      {
        auto savegame = util::make_unique<Savegame>(level_object.get_mapping());

        SavegameTable::iterator it = find(savegame->get_filename());
        if (it != savegames.end())
        {
          // overwrite duplicates, shouldn't happen, but harmless
          log_info("name collision: %1%", savegame->get_filename());
          *it = std::move(savegame);
        }
        else
        {
          savegames.push_back(std::move(savegame));
        }
      }
    }
  }
}

SavegameManager::~SavegameManager()
{
  savegames.clear();
  instance_ = 0;
}

Savegame*
SavegameManager::get(const std::string& filename_)
{
  SavegameTable::iterator i = find(filename_);
  if (i == savegames.end())
    return 0;
  else
    return (*i).get();
}

void
SavegameManager::store(Savegame const& arg_savegame)
{
  auto savegame = util::make_unique<Savegame>(arg_savegame);
  SavegameTable::iterator i = find(savegame->get_filename());
  if (i == savegames.end())
  {
    // don't know anything about the savegame
    savegames.push_back(std::move(savegame));
  }
  else
  {
    // already have such a savegame
    if ((*i)->get_status() == Savegame::FINISHED &&
        savegame->get_status() == Savegame::ACCESSIBLE)
    {
      // saved savegame is better then new game
    }
    else
    {
      // new game is better or equal, save it
      *i = std::move(savegame);
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
  Writer writer(out);

  writer.begin_object("pingus-savegame");
  writer.begin_collection("levels");
  for(SavegameTable::iterator i = savegames.begin(); i != savegames.end(); ++i)
  {
    (*i)->write_sexpr(writer);
  }
  writer.end_collection();
  writer.end_object(); // pingus-savegame

  System::write_file(filename, out.str());
}

/* EOF */
