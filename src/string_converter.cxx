//  $Id: string_converter.cxx,v 1.6 2002/09/04 14:55:11 torangan Exp $
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

#include <algorithm>
#include <stdio.h>
#include "string_converter.hxx"

using namespace std;

int 
StringConverter::to_int(const std::string& str)
{
  int i;
  if (sscanf(str.c_str(), "%d", &i) != 1)
    {
      throw Error("StringConverter: Couldn't convert: \"" + str + "\" to int");
    }
  return i;
}

float
StringConverter::to_float(const std::string& str)
{
  float i;
  if (sscanf(str.c_str(), "%f", &i) != 1)
    {
      throw Error("StringConverter: Couldn't convert: \"" + str + "\" to float");
    }
  return i;
}

std::string 
StringConverter::to_string (int i)
{
  char buffer[16];
  snprintf (buffer, 16, "%d", i);
  return buffer;
}

std::string 
StringConverter::to_string (long int i)
{
  char buffer[32];
  snprintf (buffer, 32, "%ld", i);
  return buffer;
}

std::string 
StringConverter::to_string (float i)
{
  char buffer[48];
  snprintf (buffer, 48, "%f", i);
  return buffer;
}

std::string string_upcase (const std::string& str)
{
  std::string name = str;
  std::transform(name.begin(), name.end(), name.begin(), ::tolower);
  return name;
}

std::string string_downcase (const std::string& str)
{
  std::string name = str;
  std::transform(name.begin(), name.end(), name.begin(), ::tolower);
  return name;
}

/* EOF */
