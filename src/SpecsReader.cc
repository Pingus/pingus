//  $Id: SpecsReader.cc,v 1.2 2000/02/09 21:43:41 grumbel Exp $
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

#include "PingusError.hh"
#include "globals.hh"
#include "SpecsReader.hh"

#ifdef WIN32
using namespace std;
#endif /* WIN32 */

SpecsReader::SpecsReader()
{
}

void 
SpecsReader::register_token(int token_id, string token_string)
{
  if (verbose > 2)
    cout << "SpecsReader: Register token: " 
	 << token_id << " - " << token_string << endl;

  token_ids[token_string] = token_id;
}
 
int
SpecsReader::next_token()
{
  string token;
  in >> token;
  
  if (token == "//") {
    // Jumps to the end of the current line
    if (verbose > 2)
      cout << "SpecsReader: Found comment, ignoring line" << endl;
    in.ignore(255, '\n');
    return next_token();
  }

  if (token == "/*") {
    if (verbose > 2)
      cout << "SpecsReader: Long Comment, ignoring until '*/'" << endl;
 
    while (true) {
      in >> token;
      if (token == "*/")
	break;
    }

    return next_token();
  }

  return token_ids[token];
}

string 
SpecsReader::operator[](int token_id)
{
  string str = values[token_id];
  
  if (str.empty())
    str = "<undef>";

  return str;
}

int
SpecsReader::open(const char* filename)
{
  in.open(filename);

  if (!in) {
    string mes;

    mes += "SpecsReader: Couldn't open '";
    mes += filename;
    mes += "'";

    throw PingusError(mes);
  }

  while(!in.eof()) {
    if (verbose > 2)
      cout << "SpecsReader: Getting next token pair: " << endl;
    
    int n_token = next_token();
    string token;
    
    in >> token;
 
    if (verbose > 2)
      cout << "Token: " << token << endl;

    values[n_token] = token;
  }
  return true;
}

void
SpecsReader::close()
{
  if (verbose > 2)
    cout << "SpecsReader: Closing" << endl;
  in.close();
}

/* EOF */
