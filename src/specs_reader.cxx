//  $Id: specs_reader.cxx,v 1.2 2002/08/16 15:13:59 torangan Exp $
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

#include <iostream>

#include "pingus_error.hxx"
#include "globals.hxx"
#include "specs_reader.hxx"

SpecsReader::SpecsReader()
{
}

void 
SpecsReader::register_token(int token_id, std::string token_string)
{
  if (verbose > 2)
    std::cout << "SpecsReader: Register token: " 
	 << token_id << " - " << token_string << std::endl;

  token_ids[token_string] = token_id;
}
 
int
SpecsReader::next_token()
{
  std::string token;
  in >> token;
  
  if (token == "//") {
    // Jumps to the end of the current line
    if (verbose > 2)
      std::cout << "SpecsReader: Found comment, ignoring line" << std::endl;
    in.ignore(255, '\n');
    return next_token();
  }

  if (token == "/*") {
    if (verbose > 2)
      std::cout << "SpecsReader: Long Comment, ignoring until '*/'" << std::endl;
 
    while (true) {
      in >> token;
      if (token == "*/")
	break;
    }

    return next_token();
  }

  return token_ids[token];
}

std::string 
SpecsReader::operator[](int token_id)
{
  std::string str = values[token_id];
  
  if (str.empty())
    str = "<undef>";

  return str;
}

int
SpecsReader::open(const char* filename)
{
  in.open(filename);

  if (!in) {
    std::string mes;

    mes += "SpecsReader: Couldn't open '";
    mes += filename;
    mes += "'";

    PingusError::raise(mes);
  }

  while(!in.eof()) {
    if (verbose > 2)
      std::cout << "SpecsReader: Getting next token pair: " << std::endl;
    
    int n_token = next_token();
    std::string token;
    
    in >> token;
 
    if (verbose > 2)
      std::cout << "Token: " << token << std::endl;

    values[n_token] = token;
  }
  return true;
}

void
SpecsReader::close()
{
  if (verbose > 2)
    std::cout << "SpecsReader: Closing" << std::endl;
  in.close();
}

/* EOF */
