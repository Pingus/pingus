//  $Id: System.hh,v 1.2 2000/02/09 21:43:41 grumbel Exp $
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

class System
{
private:
  
public:
  struct DirectoryEntry
  {
    enum FileType { DIRECTORY, FILE } type;
    string name;

    DirectoryEntry(const string&);
  };

  typedef list<DirectoryEntry> Directory;
  
  static Directory opendir(const string& pathname, const string& pattern = "*");

  static string basename(string);
};

#endif

/* EOF */
