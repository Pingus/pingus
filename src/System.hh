//  $Id: System.hh,v 1.9 2000/06/20 20:30:53 grumbel Exp $
// 
//  Pingus - A free Lemmings clone
//  Copyright (C) 1999 Ingo Ruhnke <grumbel@gmx.de>
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
//  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
//  02111-1307, USA. 

#ifndef SYSTEM_HH
#define SYSTEM_HH

#include <string>
#include <list>

/** A wrapper class around some system dependend functions (mostly
    POSIX stuff) */
class System
{
private:
  
public:
  ///
  struct DirectoryEntry
  {
    ///
    enum FileType { DIRECTORY, FILE } ///
 type;
    ///
    std::string name;

    ///
    DirectoryEntry(const std::string&);
  };

  ///
  typedef std::list<DirectoryEntry> Directory;
  
  ///
  static Directory opendir(const std::string& pathname, const std::string& pattern = "*");

  /** Check if a file is avaiblable (no checking for permissens is currently performed)
      @param filename The name of the file
      @return True, if the file exist, false otherwise */
  static bool exist(std::string filename);

  /** Creates directory if it does not already exist.
      @param dir The name of the directory */
  static void create_dir(std::string dir);
  
  /** Check if all needed directories are available, if not then create 
      them. */
  static void init_directories();

  /** Strips directory from filenames
      @param filename The complete filename */
  static std::string basename(std::string filename);

  /** Returns the directory were Pingus can store its user specific
      state and config data (savegames, config files, demos, etc.) */
  static std::string get_statdir();

  /** Returns the directory were Pingus can store its system wide
      variable game data (highscores, cache images, ...) */
  static std::string get_vardir();

  /** Asks different variables to check were the directory for
      temporary files is located 
      @return The location of the tmp directory */
  static std::string get_tmpdir();
};

#endif

/* EOF */
