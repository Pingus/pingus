//  $Id: StringConverter.hh,v 1.6 2001/06/16 15:01:53 grumbel Exp $
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

#ifndef STRINGCONVERTER_HH
#define STRINGCONVERTER_HH

#include <string>
#include <stdexcept>

#ifdef HAVE_SSTREAM
#include <sstream>
#else
#include <strstream>
#endif

template <class T>
std::string to_string(const T& any)
{
#ifdef HAVE_SSTREAM
  std::ostringstream oss;
  oss << any;
  return oss.str();
#else
  std::ostrstream oss;
  oss << any << std::ends;
  std::string temp(oss.str());
  oss.freeze(false); // tell the ostrstream that it should free the memory
  return temp;
#endif
}

template <class T>
void from_string(const std::string& rep, T& x)
{
 // this is necessary so that if "x" is not modified if the conversion fails
  T temp;
#ifdef HAVE_SSTREAM
  std::istringstream iss(rep);
#else
  std::istrstream iss(rep.c_str());
#endif
  iss >> temp;
  if (iss.fail())
    throw std::invalid_argument
      ("Exception: 'failed to extract type T from rep' " __FILE__ ": " + rep);
  x = temp;
}

class StringConverter
{
private:
  struct Error {
    std::string message;
    
    Error (const std::string& str)
      : message (str) {}
  };

public:
  static int   to_int(const std::string&);
  static float to_float(const std::string&);

  static std::string to_string (int i);
  static std::string to_string (long int i);
  static std::string to_string (float i);
};

#endif

/* EOF */
