//  $Id: xml_helper.cxx,v 1.34 2004/03/31 18:17:38 torangan Exp $
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

#include <iostream>
#include "globals.hxx"
#include "vector.hxx"
#include "res_descriptor.hxx"
#include "xml_helper.hxx"

// Hack: if xmlIsBlankNode() is not present, we define an empty dummy
#ifdef NO_XMLISBLANKNODE
#error "int xmlIsBlankNode(xmlNodePtr node) missing from libxml"
int xmlIsBlankNode(xmlNodePtr node) { return 0; }
#endif

namespace Pingus {

std::ostream& operator<<(std::ostream& s, xmlNode& node)
{
#if LIBXML_VERSION >= 20506
  xmlChar* path = xmlGetNodePath(&node);
  s << path;
  xmlFree(path);
  return s;
#else
  xmlNodePtr cur = &node;

  while (cur != NULL)
    {
      s << cur->name << "/";
      cur = cur->parent;
    }

  return s;
#endif
}

xmlNodePtr
XMLhelper::skip_blank (xmlNodePtr cur)
{
  if (cur && xmlIsBlankNode(cur))
    return cur->next;
  else
    return cur;
}

bool
XMLhelper::equal_str (const xmlChar* comp, const char* orig)
{
  return ! strcmp(reinterpret_cast<const char*>(comp), orig);
}

bool
XMLhelper::get_prop (xmlNodePtr cur, const char* name, std::string& value)
{
  char* retval = reinterpret_cast<char*>(xmlGetProp(cur, reinterpret_cast<const xmlChar*>(name)));

  if (!retval)
    return false;

  value = retval;
  xmlFree(retval);
  return true;
}

bool
XMLhelper::get_prop (xmlNodePtr cur, const char* name, float& value)
{
  char* retval = reinterpret_cast<char*>(xmlGetProp(cur, reinterpret_cast<const xmlChar*>(name)));

  if (!retval)
    return false;

  value = strtod(retval, reinterpret_cast<char**>(NULL));
  xmlFree(retval);
  return true;
}

bool
XMLhelper::get_prop (xmlNodePtr cur, const char* name, int& value)
{
  char* retval = reinterpret_cast<char*>(xmlGetProp(cur, reinterpret_cast<const xmlChar*>(name)));
  if (!retval)
    return false;

  value = strtol(retval, reinterpret_cast<char**>(NULL), 10);
  xmlFree(retval);
  return true;
}

bool
XMLhelper::get_prop (xmlNodePtr cur, const char* name, bool& value)
{
  char* retval = reinterpret_cast<char*>(xmlGetProp(cur, reinterpret_cast<const xmlChar*>(name)));

  if (!retval)
    return false;

  value = strtol(retval, reinterpret_cast<char**>(NULL), 10);
  xmlFree(retval);
  return true;
}

bool
XMLhelper::node_list_get_string (xmlDocPtr doc, xmlNodePtr cur, int inLine, std::string& value)
{
  xmlChar* retval = xmlNodeListGetString(doc, cur, inLine);
  if (!retval)
    return false;

  value = xmlChar2string(retval);
  xmlFree(retval);
  return true;
}

bool
XMLhelper::node_list_get_string (xmlDocPtr doc, xmlNodePtr cur, int inLine, float& value)
{
  char * retval = reinterpret_cast<char*>(xmlNodeListGetString(doc, cur, inLine));
  if (!retval)
    return false;

  value = strtod(retval, reinterpret_cast<char**>(NULL));
  xmlFree(retval);
  return true;
}

bool
XMLhelper::node_list_get_string (xmlDocPtr doc, xmlNodePtr cur, int inLine, int& value)
{
  char * retval = reinterpret_cast<char*>(xmlNodeListGetString(doc, cur, inLine));
  if (!retval)
    return false;

  value = strtol(retval, reinterpret_cast<char**>(NULL), 10);
  xmlFree(retval);
  return true;
}

std::string
XMLhelper::encode_entities (const std::string& arg_str)
{
  //xmlEncodeEntitiesReentrant()
  std::string str;
	for (unsigned int i=0; i < arg_str.size(); ++i)
	  {
			switch (arg_str[i])
			  {
				  case '\'': str += "&apos;"; break;
					case '"' : str += "&quot;"; break;
				  case '<' : str += "&lt;"  ; break;
					case '>' : str += "&gt;"  ; break;
					case '&' : str += "&amp;" ; break;
					default: str += arg_str[i];
				}
		}
	
  return str;
}

Vector
XMLhelper::parse_vector (xmlDocPtr doc, xmlNodePtr cur)
{
  Vector pos;
  cur = cur->children;
  while (cur)
    {
      if (xmlIsBlankNode(cur))
	{
	  cur = cur->next;
	  continue;
	}

      float ident;
      if (node_list_get_string(doc, cur->children, 1, ident))
	{
	  //std::cout << "parse_position: ident = " << ident << std::endl;
	  if (XMLhelper::equal_str(cur->name, "x-pos")) {
	    pos.x = ident;
	  } else if (XMLhelper::equal_str(cur->name, "y-pos")) {
	    pos.y = ident;
	  } else if (XMLhelper::equal_str(cur->name, "z-pos")) {
	    pos.z = ident;
	  } else {
	    std::cout << "Unhandled position ident: " << ident << std::endl;
	  }
	}
      cur = cur->next;
    }
  return pos;
}

bool
XMLhelper::parse_bool (xmlDocPtr doc, xmlNodePtr cur)
{
  return static_cast<bool>(parse_int(doc, cur));
}

int
XMLhelper::parse_int (xmlDocPtr doc, xmlNodePtr cur)
{
  cur = cur->children;

  int number = 999;
  if (!node_list_get_string(doc, cur, 1, number)) {
    std::cout << "Error: XMLhelper: parse_int: Field empty: " << *cur << std::endl;
  }
  return number;
}

float
XMLhelper::parse_float (xmlDocPtr doc, xmlNodePtr cur)
{
  cur = cur->children;

  float number = 3.1415927f;
  if (!node_list_get_string(doc, cur, 1, number)) {
    std::cout << "XMLhelper: parse_int: Field empty: " << *cur << std::endl;
  }
  return number;
}

CL_Colorf
XMLhelper::parse_color (xmlDocPtr doc, xmlNodePtr cur)
{
  CL_Colorf color;
  cur = cur->children;

  while (cur)
    {
      if (xmlIsBlankNode(cur))
	{
	  cur = cur->next;
	  continue;
	}

      if (XMLhelper::equal_str(cur->name, "red"))
	{
	  color.red = parse_float(doc, cur);
	}
      else if (XMLhelper::equal_str(cur->name, "green"))
	{
	  color.green = parse_float(doc, cur);
	}
      else if (XMLhelper::equal_str(cur->name, "blue"))
	{
	  color.blue = parse_float(doc, cur);
	}
      else if (XMLhelper::equal_str(cur->name, "alpha"))
	{
	  color.alpha = parse_float(doc, cur);
	}
      else
	{
	  std::cout << "XMLhelper: Unhandled color ident: " << cur->name << std::endl;
	}
      cur = cur->next;
    }
  return color;
}

ResDescriptor
XMLhelper::parse_surface(xmlDocPtr doc, xmlNodePtr cur)
{
  ResDescriptor desc;
  cur = cur->children;
  while (cur)
    {
      if (xmlIsBlankNode(cur))
	{
	  cur = cur->next;
	  continue;
	}

      std::string type;
      xmlNodePtr ccur = cur->children;
      while (ccur)
        {
          if (xmlIsBlankNode(ccur))
            {
              ccur = ccur->next;
              continue;
            }

          if (XMLhelper::equal_str(ccur->name, "resource-datafile"))
            {
              if (!node_list_get_string(doc, ccur->children, 1, desc.datafile))
                std::cout << "XMLhelper: parse_surface() Empty" << std::endl;
            }
          else if (XMLhelper::equal_str(ccur->name, "resource-ident"))
            {
              node_list_get_string(doc, ccur->children, 1, desc.res_name);
            }
          else if (XMLhelper::equal_str(ccur->name, "modifier"))
            {
              //std::cout << "Modifier!!!!!" << std::endl;
              std::string ident;
              if (node_list_get_string(doc, ccur->children, 1, ident))
                {
                  //std::cout << "Seen: modifier: " << ident << std::endl;

                  desc.modifier = ResourceModifierNS::rs_from_string(ident);
                }
            }
          else
            {
              std::cout << "XMLHelper:parse_surface2: unhandled " << ccur->name << std::endl;
            }
          ccur = ccur->next;
        }
      cur = cur->next;
    }

  return desc;
}

std::string
XMLhelper::parse_string (xmlDocPtr doc, xmlNodePtr cur)
{
  std::string ret_str;

  if (!node_list_get_string(doc,cur->children, 1, ret_str) && maintainer_mode)
    {
      std::cout << "XMLhelper::parse_string: Field empty: " << *cur << std::endl;
    }

  return ret_str;
}


void
XMLhelper::write_desc_xml (std::ostream& xml, ResDescriptor desc)
{
  xml << "  <surface>"
      << "    <name>" << desc.res_name << "</name>\n"
      << "    <modifier>" << ResourceModifierNS::rs_to_string(desc.modifier) << "</modifier>\n"
      << "  </surface>" << std::endl;
}

void
XMLhelper::write_vector_xml(std::ostream& xml, const Vector& pos)
{
  xml << "  <position>\n"
      << "    <x-pos>" << pos.x << "</x-pos>\n"
      << "    <y-pos>" << pos.y << "</y-pos>\n"
      << "    <z-pos>" << pos.z << "</z-pos>\n"
      << "  </position>\n";
}

void
XMLhelper::init()
{
  xmlLineNumbersDefault(1);
}

void
XMLhelper::deinit()
{
}

int
XMLhelper::get_line(xmlNodePtr cur)
{
#ifdef WIN32
  return -1;
#else
  return XML_GET_LINE(cur);
#endif
}

std::string
XMLhelper::xmlChar2string(const xmlChar* in)
{
  int chars   = xmlUTF8Strlen(in);
  int in_len  = xmlUTF8Strsize(in, chars);
  int out_len = chars + 1;
  unsigned char* out = new unsigned char[out_len];

  //std::cout << "OUT: in_len: " << in_len << " out_len: " << out_len << std::endl;
  int ret = UTF8Toisolat1(out, &out_len, in,  &in_len);

  if (ret != 0)
    {
      std::cout << "Error: XMLhelper: Encoding failed: ret: " << ret
                << " in: " << in_len
                << " out: " << out_len
                << " str: " << reinterpret_cast<const char*>(in)
                << std::endl;
      delete[] out;
      return reinterpret_cast<const char*>(in);
    }
  else
    {
      out[out_len] = '\0';

      //std::cout << "IN:  in_len: " << in_len << " out_len: " << out_len
      //        << " => " << out << std::endl;

      std::string ret_str = reinterpret_cast<char*>(out);
      delete[] out;
      return ret_str;
    }
}

} // namespace Pingus
/* EOF */
