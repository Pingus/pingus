//  $Id: System.cc,v 1.4 2000/02/11 21:26:37 grumbel Exp $
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
#  include <fnmatch.h>
#  include <sys/types.h>
#else /* !WIN32 */
#  include <iostream>
#  include <windows.h>
#endif
 
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
      std::cout << "Couldn't open: " << pathname << std::endl;
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
      std::cout << "Error: No theme files found";
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
  //cout << "Getting basename of: " << str << std::endl;

  for(i = filename.size() - 1; i >= 0; --i) 
    {
      if (*(str + i) == '/') {
	break;
      }
    }
  
  //cout << "Basename: " << (str+i + 1) << std::endl;
  return (str+i + 1);
}

/* EOF */
