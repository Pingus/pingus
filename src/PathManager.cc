//  $Id: PathManager.cc,v 1.1 2001/06/16 15:01:53 grumbel Exp $
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

#include "System.hh"
#include "PathManager.hh"

PathManager path_manager;

PathManager::PathManager ()
  : path_found (false)
{
}

PathManager::~PathManager ()
{
}

void 
PathManager::add_path (std::string path)
{
  path_list.push_back (path);
}

std::string 
PathManager::complete (std::string relative_path)
{
  //return base_path + relative_path;
  // FIXME: correct handling is disabled cause we use the change_dir () hack in PingusMain.cc
  return relative_path;
}

bool 
PathManager::find_path (std::list<std::string> file_list)
{
  for (PathIter i = path_list.begin (); !path_found && i != path_list.end (); ++i)
    {
      bool found_file = true;
      for (PathIter f = file_list.begin (); found_file && f != file_list.end (); ++f)
	{
	  if (!System::exist(*i + "/" + *f))
	    found_file = false;
	}
      if (found_file)
	{
	  path_found = true;
	  base_path = *i;
	  return true;
	}
    }
  return false;
}

/** Search for a path which contains the file 'file' */
bool 
PathManager::find_path (std::string file)
{
  for (PathIter i = path_list.begin (); !path_found && i != path_list.end (); ++i)
    {
      if (System::exist(*i + "/" + file))
	{
	  path_found = true;
	  base_path = *i;
	  return true;
	}
    }
  return false;
}

void 
PathManager::set_path (std::string path)
{
  base_path = path;
}

/** The value of pingus_datadir is the main directory from where all
    core data is loaded. The pathname of this directory *must* be
    ending with a trailing slash. On startup the game will chdir()
    into this directory.

    Example Value: ../data/
    
   -d, --datadir 
extern std::string pingus_datadir; 
*/

/* EOF */
