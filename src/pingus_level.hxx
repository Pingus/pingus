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

#ifndef HEADER_PINGUS_LEVEL_HXX
#define HEADER_PINGUS_LEVEL_HXX

#include <ClanLib/Core/System/sharedptr.h>
#include <ClanLib/Core/Math/size.h>
#include <string>
#include <vector>
#include <map>

namespace Pingus {

class FileReader;
class PingusLevelImpl;

/** This class represents a Pingus Level, its metadata and its body
    (groundpieces, worldobjs, etc.) alike. */
class PingusLevel
{
private:
public:
  PingusLevel(const std::string& file);

  /** Returns the name of the current level, {\em not} the level file name. */
  const std::map<std::string, std::string>& get_levelname() const;

  /** Returns the description of the level, which fit to the current
      language */
  const std::map<std::string, std::string>& get_description() const;

  /** Returns the width of the level */
  const CL_Size&     get_size() const;

  /** Returns the number of Pingus, which are going to be released in
      this level. */
  int         get_number_of_pingus() const;

  /** Returns the number of pingus you need to save in this level. */
  int         get_number_to_save() const;

  /** Number of Actions that are available to the user (ActionName, number of them) */
  const std::map<std::string, int>&    get_actions() const;

  /** Returns the time you have to complete a level */
  int         get_time() const;

  /** Returns the difficulty of the current level */
  int         get_difficulty() const;

  /** Returns the name of the author, who creates this level */
  const std::string& get_author() const;

  /** Returns the ResDescriptor where the music for this level can be
      found. The ResDescriptor points normaly to a file. */
  const std::string& get_music() const;

  /** Returns the body of this file */
  const std::vector<FileReader>& get_body() const;

private:
  CL_SharedPtr<PingusLevelImpl> impl;
};

} // namespace Pingus

#endif

/* EOF */
