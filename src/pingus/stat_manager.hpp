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

#ifndef HEADER_PINGUS_PINGUS_STAT_MANAGER_HPP
#define HEADER_PINGUS_PINGUS_STAT_MANAGER_HPP

#include <map>
#include <string>

namespace pingus {

class StatManager
{
private:
  static StatManager* instance_;

  /** File from which it loads/saves the stats */
  std::string statfilename;
  typedef std::map<std::string, std::string> Table;
  Table stats;

public:
  static StatManager* instance();

public:
  StatManager(std::string const& filename);
  ~StatManager();

  bool get_int   (std::string const&, int& value);
  bool get_bool  (std::string const&, bool& value);
  bool get_string(std::string const&, std::string& vlaue);

  void set_string(std::string const& name, std::string const& value);
  void set_int   (std::string const& name, int value);
  void set_bool  (std::string const& name, bool value);

  /** Write the current status down */
  void flush();

  void load(std::string const& filename);
  void save(std::string const& filename);
private:
  StatManager (StatManager const&);
  StatManager& operator= (StatManager const&);
};

} // namespace pingus

#endif

/* EOF */
