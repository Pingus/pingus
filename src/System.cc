//  $Id: System.cc,v 1.25 2001/03/30 09:19:23 grumbel Exp $
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
#  include <windows.h>
#  include <fstream>
#endif

#include <cstdio>
#include <iostream>

#include "my_gettext.hh"

//#include "PingusError.hh" 
#include "StringConverter.hh"
#include "System.hh"

int System::verbose;
std::string System::default_email;
std::string System::default_username;
std::string System::default_language;


System::DirectoryEntry::DirectoryEntry(const std::string& n)
{
  name = n;
}

System::Directory
System::opendir(const std::string& pathname, const std::string& pattern)
{
  std::list<System::DirectoryEntry> dir_list;

#ifndef WIN32 
  DIR* dp;
  dirent* de; 
  
  dp = ::opendir(pathname.c_str());

  if (dp == 0)
    {
      std::cout << _("System: Couldn't open: ") << pathname << std::endl;
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
  WIN32_FIND_DATA coFindData;
  std::string FindFileDir = pathname + "\\" + pattern;
  std::string FileLocation;
  HANDLE hFind = FindFirstFile(TEXT(FindFileDir.c_str()),&coFindData);

  if (hFind == INVALID_HANDLE_VALUE)
    {
      std::cout << "System: Couldn't open: " << pathname << std::endl;
    }
  
  do
    {     
      dir_list.push_back(DirectoryEntry(coFindData.cFileName));
    }
  while (FindNextFile(hFind,&coFindData));

  FindClose(hFind);  
#endif

  return dir_list;
}

// Returns the basic filename without the path
std::string
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
#ifndef WIN32
  return !access(filename.c_str(), F_OK);
#else
  //don't know a better solution
  std::ifstream check(filename.c_str());
  if(!check) return false;
  return true;
#endif
}

void
System::create_dir(std::string directory)
{
#ifndef WIN32
  if (!exist(directory))
    {
      if (mkdir(directory.c_str(), S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IXGRP) != 0)
	{
	  throw Error(directory + ": " + strerror(errno));
	}
      else
	{
	  std::cout << _("Successfully created: ") << directory << std::endl;
	}
    }  
  else
    {
      if (verbose) std::cout << _("Found: ") << directory << std::endl;
    }
#else
  CreateDirectory(directory.c_str(), 0);
#endif
}

/** Change into the directory named dir, on error throw an PingusError */
void
System::change_dir (std::string dir)
{
  chdir (dir.c_str ());
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
  create_dir(statdir + "cache/");
  create_dir(statdir + "demos/");
  create_dir(statdir + "screenshots/");
  create_dir(statdir + "themes/");

  // create_dir(vardir);
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
      return std::string(homedir) + "/.pingus/";
    }
  else
    {
      throw Error(_("Enviroment variable $HOME not set, fix that and start again."));
    }
#endif
}

std::string 
System::get_cachedir()
{
  return get_statdir() + "cache/";
}

std::string
System::get_vardir()
{
#ifdef WIN32   
  return "var\\";
#else
  return "/var/games/pingus/";
#endif
}

std::string
System::get_tmpdir()
{
#ifdef WIN32   
  // FIXME: Warrning this hardcoded values are mostlikly wrong!
  return "c:\\windows\\temp\\";
#else
  return "/tmp/";
#endif
}

/** Returns the Username how the current user or an empty string */
std::string
System::get_username()
{
  if (default_username.empty())
    {
      char* username = getenv("USERNAME");

      if (username)
	return std::string(username);
      else
	return "";
    }
  else
    {
      return default_username;
    }
}

/** Returns the EMail of the user or an empty string */
std::string 
System::get_email()
{
  if (default_email.empty())
    {
      char* email = getenv("EMAIL");

      if (email)
	// FIXME: $EMAIL contains the complete from header, not only
	// the email address
	return std::string(email);
      else
	return "";
    }
  else
    {
      return default_email;
    }
}

std::string
System::get_language()
{
  char* lang = getenv("LANG");

  if (lang && strcmp(lang, "C") != 0)
    {
      if (strcmp(lang, "de_DE") == 0)
	return "de";
      else
	return lang; 
    }
  else
    return default_language;
}

std::string
System::translate(std::map<std::string, std::string> strs)
{
  std::string str = strs[System::get_language()];
  
  if (str.empty())
    {
      return strs[default_language];
    }
  else 
    {
      return str;
    }
}

/** Read file and create a checksum and return it */
std::string
System::checksum (std::string filename)
{
  FILE* in;
  int bytes_read;
  char buffer[4096];
  int checksum = 0;

  in = fopen(filename.c_str(), "r");

  if (!in)
    {
      std::cout << _("System::checksum: Couldn't open file: ") << filename << std::endl;
      return "";
    }

  do 
    {
      bytes_read = fread (buffer, sizeof (char), 4096, in);
      
      if (bytes_read == -1)
	{
	  throw Error (_("System:checksum: file read error"));
	}

      for (int i=0; i < bytes_read; i++)
	checksum = checksum * 17 + buffer[i];
    } 
  while (bytes_read != 0);

  fclose (in);

  return StringConverter::to_string(checksum);
}

/* EOF */
