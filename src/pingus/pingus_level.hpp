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

#ifndef HEADER_PINGUS_PINGUS_PINGUS_LEVEL_HPP
#define HEADER_PINGUS_PINGUS_PINGUS_LEVEL_HPP

#include <map>

#include "util/reader.hpp"

class Pathname;
class PingusLevelImpl;

/** This class represents a Pingus Level, its metadata and its body
    (groundpieces, worldobjs, etc.) alike. */
class PingusLevel
{
public:
  PingusLevel();

  PingusLevel(const Pathname& pathname);

  PingusLevel(const std::string& resname,
              const Pathname& pathname);

  /** Returns a short checksum for the level file */
  std::string get_checksum() const;

  /** Returns the name of the current level, {\em not} the level file name. */
  const std::string& get_levelname() const;

  /** Returns the description of the level, which fit to the current
      language */
  const std::string& get_description() const;

  /** Returns the width of the level */
  const Size&  get_size() const;

  /** Returns the number of Pingus, which are going to be released in
      this level. */
  int         get_number_of_pingus() const;

  /** Returns the number of pingus you need to save in this level. */
  int         get_number_to_save() const;

  /** Number of Actions that are available to the user (ActionName, number of them) */
  const std::map<std::string, int>&    get_actions() const;

  /** Returns the time you have to complete a level */
  int         get_time() const;

  /** Returns the name of the author, who creates this level */
  const std::string& get_author() const;

  /** Returns the ResDescriptor where the music for this level can be
      found. The ResDescriptor points normaly to a file. */
  const std::string& get_music() const;

  /** Returns the light to be used in this level */
  const Color& get_ambient_light() const;

  /** Returns the body of this file */
  const std::vector<ReaderObject>& get_objects() const;

  /** Return the 'resource name' of the level ('snow22-grumbel', etc. ) */
  const std::string get_resname() const;

private:
  void load(const std::string& resname,
            const Pathname& pathname);

protected:
  std::shared_ptr<PingusLevelImpl> impl;
};

#endif

/* EOF */
