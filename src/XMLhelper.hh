//  $Id: XMLhelper.hh,v 1.2 2000/08/05 00:00:42 grumbel Exp $
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

#ifndef XMLHELPER_HH
#define XMLHELPER_HH

#include <string>
#include <gnome-xml/parser.h> 
#include "ResDescriptor.hh"
#include "Position.hh"
#include "Color.hh"

class XMLhelper
{
private:

public:
  static std::string XMLhelper::encode_entities(const std::string& arg_str);

  static ResDescriptor parse_surface(xmlDocPtr doc, xmlNodePtr cur);
  static Position      parse_position(xmlDocPtr doc, xmlNodePtr cur);
  static int           parse_int(xmlDocPtr doc, xmlNodePtr cur);
  static float         parse_float(xmlDocPtr doc, xmlNodePtr cur);
  static Color         parse_color(xmlDocPtr doc, xmlNodePtr cur);
};

#endif

/* EOF */
