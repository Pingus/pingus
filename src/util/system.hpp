// Pingus - A free Lemmings clone
// Copyright (C) 1999 Ingo Ruhnke <grumbel@gmail.com>
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

#ifndef HEADER_PINGUS_UTIL_SYSTEM_HPP
#define HEADER_PINGUS_UTIL_SYSTEM_HPP

#include <stdint.h>
#include <string>
#include <vector>

class Pathname;

/** A wrapper class around some system dependend functions (mostly
    POSIX stuff) */
class System
{
private:
  static std::string default_email;
  static std::string default_username;

  static std::string userdir;

public:
  enum FileType { DE_DIRECTORY, DE_FILE };

  struct DirectoryEntry
  {
    FileType type;
    std::string name;

    DirectoryEntry(const std::string&, FileType t = DE_FILE);
  };

  ///
  typedef std::vector<DirectoryEntry> Directory;
  typedef Directory::iterator DirectoryIter;

  /** Returns the content of the given directory, filtered by applying
      pattern. "." and ".." are removed from the output. */
  static Directory opendir(const std::string& pathname, const std::string& pattern = "*");
  static std::vector<std::string> opendir_recursive(const std::string& pathname);

  static std::string get_file_extension(const std::string& filename);
  static std::string cut_file_extension(const std::string& filename);

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

  /** Strips basename from pathnames (see dirname(3))
      @param filename The complete filename */
  static std::string dirname(std::string filename);

  static std::string find_userdir();

  /** Sets the directory to save users data to */
  static void set_userdir(const std::string&);

  /** Returns the directory where Pingus can store its user specific
      state and config data (savegames, config files, demos, etc.) */
  static std::string get_userdir();

  /** Returns the directory where Pingus can store things that needs
      to be cached, for example thumbnails */
  static std::string get_cachedir();

  /** Returns the Username how the current user or an empty string */
  static std::string get_username();

  /** Returns the EMail of the user or an empty string */
  static std::string get_email();

  /** Get the currently country code, in a two letter ISO 639 syntax */
  static std::string get_language();

  /** Return the modification time of a file */
  static uint64_t get_mtime(const std::string& filename);

  /** Removes all '..', double slashes and such from a pathname and
      makes it absolute */
  static std::string realpath(const std::string& pathname);

  /** Removes all '..', double slashes and such from a pathname */
  static std::string normalize_path(const std::string& pathname);

  /** Read a file and generate a checksum and return it. The checksum
      generation is very primitiv and should probably be replaced by CRC
      or md5. */
  static std::string checksum (std::string filename);
  static std::string checksum (const Pathname& pathname);

  /** Write \a content to the given filename atomically */
  static void write_file(const std::string& filename, const std::string& content);

private:
  System (const System&);
  System& operator= (const System&);
};

#endif

/* EOF */
