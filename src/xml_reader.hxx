//  $Id: xml_reader.hxx,v 1.1 2003/02/18 00:13:13 grumbel Exp $
// 
//  Pingus - A free Lemmings clone
//  Copyright (C) 2002 Ingo Ruhnke <grumbel@gmx.de>
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

#ifndef HEADER_PINGUS_XML_READER_HXX
#define HEADER_PINGUS_XML_READER_HXX

#include <string>
#include "libxmlfwd.hxx"
#include "res_descriptor.hxx"
#include "vector.hxx"

/** */
class XMLReader
{
private:
  xmlDocPtr  doc;
  xmlNodePtr root_node;
  xmlNodePtr current_node;
public:
  XMLReader(xmlDocPtr d, xmlNodePtr n);
  
  bool read_vector (const char*, Vector&);
  bool read_string (const char*, std::string&);
  bool read_resdesc(const char*, ResDescriptor&);
private:
  xmlNodePtr next_node(xmlNodePtr n);
  xmlNodePtr find_node(const char*);
  
  XMLReader (const XMLReader&);
  XMLReader& operator= (const XMLReader&);
};

#endif

/* EOF */
