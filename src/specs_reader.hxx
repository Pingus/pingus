//  $Id: specs_reader.hxx,v 1.5 2002/09/27 11:26:44 torangan Exp $
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

#ifndef HEADER_PINGUS_SPECS_READER_HXX
#define HEADER_PINGUS_SPECS_READER_HXX

#include "pingus.hxx"
#include <map>
#include <fstream>
#include <string>

class SpecsReader
{
private:
  std::map<std::string, int> token_ids;
  std::map<int, std::string> values;

  std::ifstream in;

public:
  SpecsReader();
  
  int  open(const char*);
  void close();
  int  next_token();
  void register_token(int token_id, std::string token_value);
  std::string operator[](int t);
  
private:
  SpecsReader (const SpecsReader&);
  SpecsReader& operator= (const SpecsReader&);
};

#endif

/* EOF */
