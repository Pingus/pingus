//  $Id: xml_file_reader.cxx,v 1.5 2003/10/18 23:17:27 grumbel Exp $
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

#include "vector.hxx"
#include "res_descriptor.hxx"
#include "debug.hxx"
#include "globals.hxx"
#include "xml_file_reader.hxx"

namespace Pingus {

XMLFileReader::XMLFileReader(xmlDocPtr doc_, xmlNodePtr node)
  : doc(doc_), section_node(node)
{
}

XMLFileReader::XMLFileReader()
  : doc(NULL), section_node(NULL)
{
}

void
XMLFileReader::init(xmlDocPtr doc_, xmlNodePtr node)
{
  doc = doc_;
  section_node = node;
}

xmlNodePtr
XMLFileReader::find_node(const char* name)
{
  if (doc == NULL)
    {
      perr(PINGUS_DEBUG_LOADING) << "XMLFileReader points to nothing, probally not inited" << std::endl;
      return NULL;
    }
  else if (section_node == NULL)
    {
      // Whole section is empty
      return NULL;
    }

  xmlNodePtr node = section_node->children;

  while (node)
    {
      if (xmlIsBlankNode(node))
	{
	  node = node->next;
	  continue;
	}

      if (XMLhelper::equal_str(node->name, name))
        {
          return node;
        }

      node = node->next;
    }
  return NULL;
}

bool
XMLFileReader::read_desc  (const char* name, ResDescriptor& value)
{
  xmlNodePtr node = find_node(name);

  if (node)
    {
      value = XMLhelper::parse_surface(doc, node);
      return true;
    }

  return false;
}

bool
XMLFileReader::read_color (const char* name, CL_Colorf& value)
{
  xmlNodePtr node = find_node(name);

  if (node)
    {
      value = XMLhelper::parse_color(doc, node);
      return true;
    }

  return false;
}

bool
XMLFileReader::read_int   (const char* name, int& value)
{
  xmlNodePtr node = find_node(name);

  if (node)
    {
      value = XMLhelper::parse_int(doc, node);
      return true;
    }

  return false;
}

bool
XMLFileReader::read_float (const char* name, float& value)
{
  xmlNodePtr node = find_node(name);

  if (node)
    {
      value = XMLhelper::parse_float(doc, node);
      return true;
    }

  return false;
}

bool
XMLFileReader::read_bool  (const char* name, bool& value)
{
  xmlNodePtr node = find_node(name);

  if (node)
    {
      value = XMLhelper::parse_bool(doc, node);
      return true;
    }

  return false;
}

bool
XMLFileReader::read_string(const char* name, std::string& value)
{
  xmlNodePtr node = find_node(name);

  if (node)
    {
      value = XMLhelper::parse_string(doc, node);
      return true;
    }

  return false;
}

bool
XMLFileReader::read_vector(const char* name, Vector& value)
{
  xmlNodePtr node = find_node(name);

  if (node)
    {
      value = XMLhelper::parse_vector(doc, node);
      return true;
    }

  return false;
}

bool
XMLFileReader::read_section(const char* name, XMLFileReader& value)
{
  xmlNodePtr node = find_node(name);
  if (node)
    {
      value.init(doc, node);
      return true;
    }
  return false;
}

} // namespace Pingus

/* EOF */
