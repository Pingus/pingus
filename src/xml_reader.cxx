//  $Id: xml_reader.cxx,v 1.1 2003/02/18 00:13:13 grumbel Exp $
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

#include "xml_helper.hxx"
#include "xml_reader.hxx"

XMLReader::XMLReader(xmlDocPtr d, xmlNodePtr n)
  : doc(d), root_node(n), current_node(n)
{
  if (xmlIsBlankNode(current_node))
    current_node = current_node->next;
}

xmlNodePtr
XMLReader::find_node(const char* name)
{
  xmlNodePtr cur = current_node;
  while (cur)
    {
      if (XMLhelper::equal_str(cur->name, name))
        return cur;
      cur = next_node(cur);
    }
  return NULL;
}
  
bool
XMLReader::read_vector(const char* name, Vector& vec)
{
  xmlNodePtr cur = find_node(name);
  if (cur && XMLhelper::equal_str(cur->name, name))
    {
      vec = XMLhelper::parse_vector(doc, cur);
      return true;
    }
  return false;
}

bool
XMLReader::read_string(const char* name, std::string& str)
{
  xmlNodePtr cur = current_node;
  while (cur)
    {
      if (XMLhelper::equal_str(cur->name, name))
        {
          str = XMLhelper::parse_string(doc, cur);
          return true;
        }
      cur = next_node(cur);
    }
  return false;
}

bool
XMLReader::read_resdesc(const char* name, ResDescriptor& resdesc)
{
  xmlNodePtr cur = current_node;
  while (cur)
    {
      if (XMLhelper::equal_str(cur->name, name))
        {
          resdesc = XMLhelper::parse_surface(doc, cur);
          return true;
        }
      cur = next_node(cur);
    }
  
  return false;
}

xmlNodePtr
XMLReader::next_node(xmlNodePtr node)
{
  xmlNodePtr cur = node->next;
  
  if (xmlIsBlankNode(cur))
    cur = node->next;

  return cur;
}

/* EOF */
