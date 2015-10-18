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

#ifndef HEADER_PINGUS_PINGUS_SAVEGAME_HPP
#define HEADER_PINGUS_PINGUS_SAVEGAME_HPP

#include <string>

#include "util/reader.hpp"

class Writer;

/** The Savegame class holds savegame informations for a single
    level */
class Savegame
{
public:
  enum S_Status { FINISHED,   // level is successfully finished
                  ACCESSIBLE, // level is accessible and can be played
                  NONE };     // level is not finished and cannot be accessed

private:
  std::string filename;
  S_Status    status;

  /** Time needed to finish the level, only valid if status == FINISHED */
  int needed_time;

  /** Number of Pingus that where saved while playing the level */
  int saved_pingus;

public:
  static std::string status_to_string(S_Status s);
  static S_Status string_to_status (std::string s);

  Savegame();
  Savegame(const ReaderMapping& node);
  Savegame(const std::string& filename,
           S_Status arg_status, int arg_time, int arg_saved_pingus);
  ~Savegame();

  void read_sexpr(const ReaderMapping& node);
  void write_sexpr(Writer& xml);

  std::string get_filename() const { return filename; }

  S_Status get_status() const { return status; }

  int get_needed_time() const  { return needed_time;  }
  int get_saved_pingus() const { return saved_pingus; }
};

#endif

/* EOF */
