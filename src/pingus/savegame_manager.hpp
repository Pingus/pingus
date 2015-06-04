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

#ifndef HEADER_PINGUS_PINGUS_SAVEGAME_MANAGER_HPP
#define HEADER_PINGUS_PINGUS_SAVEGAME_MANAGER_HPP

#include "pingus/savegame.hpp"

class SavegameManager
{
private:
  static SavegameManager* instance_;
  typedef std::vector<std::unique_ptr<Savegame> > SavegameTable;

  std::string   filename;
  SavegameTable savegames;

  SavegameTable::iterator find(const std::string& filename);

public:
  static SavegameManager* instance();
  SavegameManager(const std::string& filename);
  ~SavegameManager();

  Savegame* get(const std::string& filename);

  void store(Savegame const& savegame);

  /** Write the current savegames down to file */
  void flush();

private:
  SavegameManager (const SavegameManager&);
  SavegameManager& operator= (const SavegameManager&);
};

#endif

/* EOF */
