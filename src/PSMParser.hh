//  $Id: PSMParser.hh,v 1.7 2000/05/24 15:39:56 grumbel Exp $
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

#ifndef PSMPARSER_HH
#define PSMPARSER_HH

#include <string>
#include <vector>
#include <fstream>

#include <ClanLib/core.h>

#include "ResDescriptor.hh"

struct surface_data 
{
  CL_Surface* surface;
  CL_Resource* resource;
  ResDescriptor res_desc;
  int x_pos;
  int y_pos;
  std::string res_name;
  std::string name;
  std::string type_str; 
  enum Type { SOLID, TRANSPARENT, GROUND, BRIDGE, WATER, LAVA, NOTHING } type ; 
};

struct PSMEOF {};

struct PSMParseError 
{
  std::string message;

  PSMParseError(std::string str) { 
    message = str; 
  }
};

class PSMParser
{
private:
  std::vector<surface_data> surface;
  bool file_parsed;
  std::ifstream in;
  int lines;
public:
  PSMParser();
  ~PSMParser();
  
  void   parse(std::string filename);
  void   load_surfaces(void);
  char   get_char(void);
  char   get_atom(void);
  int    get_int(void);
  std::string get_string(void);
  void   expect(char);
  ResDescriptor get_resdesc(void);
  void   jump_spaces(void);
  std::vector<surface_data> get_surfaces(void);
};

#endif

/* EOF */
