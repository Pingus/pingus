//  $Id: PSMParser.cc,v 1.8 2000/05/24 15:39:56 grumbel Exp $
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

#include <fstream>
#include <cstdio>

#include "globals.hh"
#include "PingusResource.hh"
#include "PingusError.hh"

#include "PSMParser.hh"

using namespace std;

PSMParser::PSMParser()
{
  lines = 0;
}

PSMParser::~PSMParser()
{
  
}

void
PSMParser::parse(string filename)
{
  try {
    if (verbose > 1) cout << "PSMParser: Filename: " << filename << endl;
    
    in.open(filename.c_str());

    if (!in) 
      throw PingusError("Cannot open: " + filename);
    
    surface_data temp;
    
    temp.res_name = "global.dat";
    
    while(!in.eof()) {
      jump_spaces();
      temp.type_str = get_string();
      
      if (temp.type_str == "solid") { 
	temp.type = surface_data::SOLID;
      } else if (temp.type_str == "ground") {
	temp.type = surface_data::GROUND;
      } else if (temp.type_str == "transparent") {
	temp.type = surface_data::TRANSPARENT;
      } else {
	throw PSMParseError("Object type is not valid: " + temp.type_str);	
      }
      expect(':');

      jump_spaces();
      temp.res_desc = get_resdesc();
      temp.name = get_string();
      expect(':');

      jump_spaces();
      temp.x_pos = get_int();
      expect(':');

      jump_spaces();
      temp.y_pos = get_int();
      expect(';');

      surface.push_back(temp);
    }
  }
  catch (PSMParseError err) {
    cout << "PSMParseError occured: " << err.message << " at line: " << lines << endl;
    throw PingusError(err.message);
  }
  catch (PSMEOF) {}
  file_parsed = true;
  in.close();
}

void
PSMParser::load_surfaces(void)
{
  assert(file_parsed);

  for (vector<surface_data>::size_type i=0; i < surface.size(); ++i) {
    if (verbose > 1) {
      cout << "Surface: " << "(" << surface[i].res_desc.res_name << ":" << surface[i].res_desc.filename << ") " 
	   << surface[i].name << endl;
    }
    surface[i].surface = CL_Surface::load(surface[i].name.c_str(), 
					      PingusResource::get(surface[i].res_name));
  }
}

vector<surface_data>
PSMParser::get_surfaces(void)
{
  return surface;
}

char
PSMParser::get_char(void)
{
  char c;
  c = in.get();

  if (in.eof()) {
    throw PSMEOF();
  }

  if (c == '\n')
    ++lines;

  return c;
}

char
PSMParser::get_atom(void)
{
  return get_char();
}

void
PSMParser::jump_spaces()
{
  char c;
  while (isspace(c = get_char()));
  in.putback(c);
}

string
PSMParser::get_string(void)
{
  string str;
  char   c;

  while (!isspace(c = get_atom()) && c != ':') {
    str += c;
  }
  
  if (c == ':')
    in.putback(c);

  if (str.empty())
    throw PSMParseError("String is empty!");

  return str;
}

int
PSMParser::get_int(void)
{
  string str;
  char   c;
  int    i;

  if ((c = get_atom()) == '-')
    str += c;
  else
    in.putback(c);

  while (isdigit(c = get_atom())) {
    str += c;
  }

  in.putback(c);

  if (sscanf(str.c_str(), "%d", &i) != 1)
    throw PSMParseError("Couldn't convert: " + str + " to int");

  return i;
}

ResDescriptor
PSMParser::get_resdesc(void)
{
  ResDescriptor temp;
  char c;
  
  temp.res_name = "";
  temp.filename = "";

  jump_spaces();

  if ((c = get_atom()) != '(') {
    in.putback(c);
    jump_spaces();
    return ResDescriptor();
  } else {
    jump_spaces();
    while ((c = get_atom()) != ')' && c != ':') {
      temp.res_name += c;
    }
    
    if (c == ')') {
      jump_spaces();
      return ResDescriptor(temp.res_name, "global.dat");      
    } else { // c == ':'
      while ((c = get_atom()) != ')') {
	temp.filename += c;
      }
      jump_spaces();
      return ResDescriptor(temp.res_name, temp.filename);
    }
  }
}

void
PSMParser::expect(char b)
{
  char c;
  jump_spaces();
  c = get_atom();
  if (c == b) {
    return;
  } else {
    throw PSMParseError(string("Didn't found expected char: '") + b + "', found '" + c +"' instead.");
    return;
  }
}

/* EOF */
