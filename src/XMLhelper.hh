//  $Id: XMLhelper.hh,v 1.8 2000/09/30 22:20:03 grumbel Exp $
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

#include "../config.h"
#include <string>
#include <fstream>

// FIXME: This should be <libxml/parser.h>, but that doesn't work with
// libxml1.x :-/
#include <parser.h>

#include "ResDescriptor.hh"
#include "Position.hh"
#include "Color.hh"

// Some compatibility stuff
#ifdef LIBXML_2
#define ROOT children
#else // libxml 1.x
#define children childs
#define ROOT root
#endif

// Hack: if xmlIsBlankNode() is not present, we define an empty dummy
#ifdef NO_XMLISBLANKNODE
int xmlIsBlankNode(xmlNodePtr node) { return 0; }
#endif

class XMLhelper
{
private:

public:
  static std::string XMLhelper::encode_entities(const std::string& arg_str);

  /// A set of function to parse an xml file
  //@{
  static ResDescriptor parse_surface(xmlDocPtr doc, xmlNodePtr cur);
  static Position      parse_position(xmlDocPtr doc, xmlNodePtr cur);
  static std::string   parse_string(xmlDocPtr doc, xmlNodePtr cur);
  static int           parse_int(xmlDocPtr doc, xmlNodePtr cur);
  static float         parse_float(xmlDocPtr doc, xmlNodePtr cur);
  static Color         parse_color(xmlDocPtr doc, xmlNodePtr cur);
  //@}

  /// A set of functions to write data down to an xml file
  //@{
  /** Writes the given res_desc to the ofstream */
  static void write_desc_xml(std::ofstream* xml, ResDescriptor desc);
  /** Write a position to the xml ofstream */
  static void write_position_xml(std::ofstream* xml, Position pos);
  //@}
};

#endif

/* EOF */
