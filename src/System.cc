//  $Id: System.cc,v 1.6 2000/03/12 01:38:49 grumbel Exp $
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
//  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

#ifndef WIN32
#  include <dirent.h>
#  include <fcntl.h>
#  include <fnmatch.h>
#  include <sys/stat.h>
#  include <sys/types.h>
#  include <unistd.h>
#  include <errno.h>
#else /* !WIN32 */
#  include <iostream>
#  include <windows.h>
#endif

#include "globals.hh"
#include "PingusError.hh" 
#include "System.hh"

System::DirectoryEntry::DirectoryEntry(const std::string& n)
{
  name = n;
}

System::Directory
System::opendir(const std::string& pathname, const std::string& pattern)
{
  list<System::DirectoryEntry> dir_list;

#ifndef WIN32 
  DIR* dp;
  dirent* de; 
  
  dp = ::opendir(pathname.c_str());

  if (dp == 0)
    {
      std::cout << "System: Couldn't open: " << pathname << std::endl;
    }
  else
    {
      while ((de = ::readdir(dp)) != 0) 
	{
	  if (fnmatch(pattern.c_str(), de->d_name, FNM_PATHNAME) == 0) 
	    {
	      if (de->d_type == 'd')
		{
		  dir_list.push_back(DirectoryEntry(de->d_name));
		}
	      else 
		{
		  dir_list.push_back(DirectoryEntry(de->d_name));
		}
	    }
	}
      
      closedir(dp);
    }
#else /* !WIN32 */
  // FIXME: This Windows code is untested and may not work!

  WIN32_FIND_DATA coFindData;
  std::string FindFileDir = pathname + "\\" + pattern;
  std::string FileLocation;
  HANDLE hFind = FindFirstFile(TEXT(FindFileDir.c_str()),&coFindData);

  if (hFind == INVALID_HANDLE_VALUE)
    {
      //BUG, errors aren't supported :)
      std::cout << "System: Error: No theme files found";
    }
  
  do
    {     
      dir_list.push_back(coFindData.cFileName);
    }
  while (FindNextFile(hFind,&coFindData));

  FindClose(hFind);  
#endif

  return dir_list;
}

// Returns the basic filename without the path
string
System::basename(std::string filename)
{
  // Should be replaced with STL
  
  const char* str = filename.c_str();
  int i;

  for(i = filename.size() - 1; i >= 0; --i) 
    {
      if (*(str + i) == '/') {
	break;
      }
    }
  
  return (str+i + 1);
}

bool
System::exist(std::string filename)
{
  return !access(filename.c_str(), F_OK);
}

void
System::create_dir(std::string directory)
{
  if (!exist(directory))
    {
      if (mkdir(directory.c_str(), S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IXGRP) != 0)
	{
	  throw PingusError(directory + ": " + strerror(errno));
	}
      else
	{
	  if (verbose) 
	    {
	      std::cout << "Successfully created: directory" << std::endl;
	    }
	}
    }
}

void
System::init_directories()
{
  std::string statdir  = get_statdir();
  std::string vardir   = get_vardir();
  
  create_dir(statdir);
  create_dir(statdir + "levels/");
  create_dir(statdir + "levels/dist");
  create_dir(statdir + "stat/");
  create_dir(statdir + "themes/");

  create_dir(vardir);
}
 
std::string
System::get_statdir()
{
#ifdef WIN32
  return "stat\\";
#else /* !WIN32 */
  char* homedir = getenv("HOME");

  if (homedir) 
    {
      throw PingusError("Enviroment variable $HOME not set, fix that and start again.");
    }
  else
    {
      return string(homedir) + "/.pingus/";
    }
#endif
}

std::string
System::get_vardir()
{
#ifdef WIN32   
  return "var\\";
#endif
  return "/var/games/pingus/";
}

/* EOF */
