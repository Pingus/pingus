//  $Id: string_reader.hxx,v 1.8 2003/10/18 23:17:27 grumbel Exp $
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

#ifndef HEADER_PINGUS_EDITOR_STRING_READER_HXX
#define HEADER_PINGUS_EDITOR_STRING_READER_HXX

#include "../pingus.hxx"
#include <list>
#include <string>
#include <ClanLib/display.h>

namespace Pingus {
namespace EditorNS {

class StringReader
{
private:
  std::list<std::string>* strings;
  std::list<std::string*> completions;
  std::string current_string;
  std::string default_string;
  bool finished;
  std::string description;
  CL_Font font;

public:
  StringReader ();
  StringReader (const std::string &, const std::string &);
  virtual ~StringReader ();

  void   set_strings (std::list<std::string>*);
  std::string read_string ();
  void draw ();
  std::string find_uniq ();
  std::string while_eq (const std::string& a, const std::string& b);
  void complete_string ();

private:
  StringReader (const StringReader&);
  StringReader& operator= (const StringReader&);
};

} // namespace EditorNS
} // namespace Pingus

#endif

/* EOF */
