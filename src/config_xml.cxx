//  $Id: config_xml.cxx,v 1.2 2002/10/13 13:34:40 grumbel Exp $
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

#include <iostream>
#include "xml_helper.hxx"
#include "pingus_error.hxx"
#include "config_xml.hxx"

ConfigXML::ConfigXML(const std::string& arg_filename)
{
  xmlDocPtr doc = xmlParseFile(arg_filename.c_str());
  if (doc == NULL)
    PingusError::raise("ConfigXML: Couldn't open \"" + arg_filename + "\"");

  xmlNodePtr cur = doc->ROOT;

  if (cur && XMLhelper::equal_str(cur->name, "pingus-config"))
    {
      cur = cur->children;
      cur = XMLhelper::skip_blank(cur);

      parse_directory(doc, cur, "");
    }
  else
    {
      PingusError::raise("ConfigXML: Not a valid pingus-config file \"" + arg_filename + "\"");      
    }
}

ConfigXML::ConfigXML(xmlDocPtr doc, xmlNodePtr cur)
{
  parse_directory(doc, cur, "");
}

void 
ConfigXML::parse_directory(xmlDocPtr doc, xmlNodePtr cur, const std::string& prefix)
{
  while(cur)
    {
      std::string name = reinterpret_cast<const char*>(cur->name);
  
      if (is_directory(doc, cur))
        {
          parse_directory(doc, cur->children, prefix + name + "/");
        }
      else if (is_value(doc, cur))
        {
          std::string value;
          XMLhelper::node_list_get_string(doc, cur->children, 1, value);
          std::cout << "ConfigXML: Value: " << prefix << name << " = " << value << std::endl;
        }
      else if (xmlIsBlankNode(cur) || XMLhelper::equal_str(cur->name, "comment"))
        {
          // ignore blank nodes
        }
      else
        {
          std::cout << "ConfigXML: parse error 123: " << cur->name << std::endl;
        }
      
      cur = cur->next;
      cur = XMLhelper::skip_blank(cur);
    }
}

bool
ConfigXML::is_directory(xmlDocPtr doc, xmlNodePtr cur)
{
  return cur->children && (xmlIsBlankNode(cur->children) || !xmlNodeIsText(cur->children));
}

bool
ConfigXML::is_value(xmlDocPtr doc, xmlNodePtr cur)
{
  return cur->children && xmlNodeIsText(cur->children);
}

/* EOF */
