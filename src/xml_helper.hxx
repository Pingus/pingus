//  $Id: xml_helper.hxx,v 1.15 2002/10/01 23:10:41 grumbel Exp $
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

#ifndef HEADER_PINGUS_XML_HELPER_HXX
#define HEADER_PINGUS_XML_HELPER_HXX

#include "pingus.hxx"
#include <config.h>
#include <string>
#include <iosfwd>


// FIX: Under windows, it's necessary to use libxml/parser.h, or MS's
// compiler will fails.
#ifdef WIN32
  #define list
    #include <libxml/parser.h>
  #undef list
#else
  #ifdef LIBXML_2
    #include <libxml/parser.h>
  #else
    #include <parser.h>
  #endif
#endif

// Some compatibility stuff
#ifdef LIBXML_2
  #define ROOT children
#else // libxml 1.x
  #define children childs
  #define ROOT root
#endif

// Hack: if xmlIsBlankNode() is not present, we define an empty dummy
#ifdef NO_XMLISBLANKNODE
  int xmlIsBlankNode(xmlNodePtr node);
#endif

class Vector;
class Color;
class ResDescriptor;

class XMLhelper
{

public:
  static std::string encode_entities (const std::string& arg_str);

  /** @return if \a cur is a blank-node, goto the next, else return \a
      cur */
  static xmlNodePtr skip_blank (xmlNodePtr cur);
  
  /** Compare a xmlChar* string with a char* one 
      @return true if both strings are equal */
  static bool equal_str (const xmlChar* comp, const char* orig);
  
  /** get the propertie of the given name
      @param name  the name of the property
      @param value gets the value if the property exists or remains unchanged
      @return indicates whether the property existed  */
  static bool get_prop (xmlNodePtr cur, const char* name, std::string& value);

  /** get the propertie of the given name
      @param name  the name of the property
      @param value gets the value if the property exists or remains unchanged
      @return indicates whether the property existed  */
  static bool get_prop (xmlNodePtr cur, const char* name, float& value);

  /** get the propertie of the given name
      @param name  the name of the property
      @param value gets the value if the property exists or remains unchanged
      @return indicates whether the property existed  */
  static bool get_prop (xmlNodePtr cur, const char* name, int& value);

  /** get the propertie of the given name
      @param name  the name of the property
      @param value gets the value if the property exists or remains unchanged
      @return indicates whether the property existed  */
  static bool get_prop (xmlNodePtr cur, const char* name, bool& value);

  static bool node_list_get_string (xmlDocPtr doc, xmlNodePtr cur, int inLine, std::string& value);
  static bool node_list_get_string (xmlDocPtr doc, xmlNodePtr cur, int inLine, float& value);
  static bool node_list_get_string (xmlDocPtr doc, xmlNodePtr cur, int inLine, int& value);


  /// A set of function to parse an xml file
  //@{
  static ResDescriptor parse_surface (xmlDocPtr doc, xmlNodePtr cur);
  static Vector        parse_vector  (xmlDocPtr doc, xmlNodePtr cur);
  static std::string   parse_string  (xmlDocPtr doc, xmlNodePtr cur);
  static int           parse_int     (xmlDocPtr doc, xmlNodePtr cur);
  static float         parse_float   (xmlDocPtr doc, xmlNodePtr cur);
  static Color         parse_color   (xmlDocPtr doc, xmlNodePtr cur);
  //@}

  /// A set of functions to write data down to an xml file
  //@{
  /** Writes the given res_desc to the ofstream */
  static void write_desc_xml (std::ostream& xml, ResDescriptor desc);
  /** Write a Vector to an xml stream */
  static void write_vector_xml (std::ostream& xml, const Vector& pos);
  //@}

  static void init();
  
private:
  XMLhelper ();
  XMLhelper (const XMLhelper&);
  XMLhelper& operator= (const XMLhelper&);
};

#endif

/* EOF */
