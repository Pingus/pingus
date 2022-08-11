// Pingus - A free Lemmings clone
// Copyright (C) 2007 Ingo Ruhnke <grumbel@gmail.com>
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

#ifndef HEADER_PINGUS_UTIL_PATHNAME_HPP
#define HEADER_PINGUS_UTIL_PATHNAME_HPP

#include <string>
#include <vector>
#include <sstream>

namespace pingus {

/** Simple class to allow a distinction of paths that refer to the
 *  filesystem and paths that refer to the datadir, it also hides
 *  path_manager from the rest of the code. */
class Pathname
{
public:
  /** Generate a temporary filename suitable for temporary files */
  static Pathname tmpfile(std::string const& prefix = "");

  static std::string join(std::string const& lhs, std::string const& rhs);

  enum Type {
    // The given pathname refers to the native file system
    SYSTEM_PATH,

    // The given pathname refers to a file in the datadir
    DATA_PATH,

    INVALID
  };

private:
  std::string pathname;
  Type type;

  void opendir_recursive(std::vector<Pathname>& result) const;

public:
  Pathname();
  explicit Pathname(std::string const& pathname, Type type = DATA_PATH);

  std::vector<Pathname> opendir(std::string const& pattern = "*") const;
  std::vector<Pathname> opendir_recursive() const;

  /** Check if the pathname has the extension \a ext (i.e. ".png") */
  bool has_extension(std::string const& ext) const;

  /** Return the pathname in a form suitable for the native file
      system (i.e. can be passed to ifstream and friends */
  std::string get_sys_path() const;

  /** Return the pathname as is */
  std::string get_raw_path() const;

  Type get_type() const;

  /** Converts the Pathname into human readable form, usefull for
      debugging output, but nothing else */
  std::string str() const;

  bool absolute() const;

  bool empty() const;

  bool exist() const;

  uint64_t mtime() const;

  bool operator<(Pathname const& rhs) const;
  bool operator==(Pathname const& rhs) const;
};

std::ostream& operator<< (std::ostream& os, Pathname const& p);

} // namespace pingus

#endif

/* EOF */
