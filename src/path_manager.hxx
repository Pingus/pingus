//  $Id: path_manager.hxx,v 1.1 2002/06/12 19:06:12 grumbel Exp $
// 
//  Pingus - A free Lemmings clone
//  Copyright (C) 2000 Ingo Ruhnke <grumbel@gmx.de>
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

#ifndef PATHMANAGER_HH
#define PATHMANAGER_HH

#include <string>
#include <list>

class PathManager
{
private:
  std::list<std::string> path_list;
  typedef std::list<std::string>::iterator PathIter;

  bool path_found;
  std::string base_path;
public:
  PathManager ();
  ~PathManager ();

  /** Search for a path which contains all the files given in
      file_list */
  bool find_path (std::list<std::string> file_list);

  /** Search for a path which contains the file 'file' */
  bool find_path (std::string file);

  /** Set the path directly without using find_path/add_path 
      Must include trailing slash */
  void set_path (std::string path);

  /** Add a path to the search list */
  void add_path (std::string path);

  /** Set the executable name (argv[0]), its use as a hint to find the
      correct path */
  void set_executable (std::string executable);

  std::string get_base_path () { return base_path; }

  /** Complete a releative path to the absolute path, the returned
      path contains a trailing slash */
  std::string complete (std::string relative_path);
};

extern PathManager path_manager;

#endif

/* EOF */
