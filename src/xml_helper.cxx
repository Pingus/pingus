//  $Id: xml_helper.cxx,v 1.18 2002/09/28 11:52:22 torangan Exp $
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
#include "vector.hxx"
#include "string_converter.hxx"
#include "color.hxx"
#include "res_descriptor.hxx"
#include "xml_helper.hxx"

// Hack: if xmlIsBlankNode() is not present, we define an empty dummy
#ifdef NO_XMLISBLANKNODE
#error "int xmlIsBlankNode(xmlNodePtr node) missing from libxml"
int xmlIsBlankNode(xmlNodePtr node) { return 0; }
#endif

xmlNodePtr
XMLhelper::skip_blank (xmlNodePtr cur)
{
  if (xmlIsBlankNode(cur))
    return cur->next;
  else
    return cur;
}

bool
XMLhelper::equal_str (const xmlChar* comp, const char* orig)
{
  return ! strcmp(reinterpret_cast<const char*>(comp), orig);
}

char*
XMLhelper::get_prop (xmlNodePtr cur, const char* name)
{
  return reinterpret_cast<char*>(xmlGetProp(cur, reinterpret_cast<const xmlChar*>(name)));
}

std::string
XMLhelper::encode_entities (const std::string& arg_str)
{
  ///xmlEncodeEntitiesReentrant()
  std::string str = arg_str;
  std::string::size_type i;
  //  std::cout << "encode_xml: " << str << std::endl;
 
  i = str.find("<");
  if (i != std::string::npos)
    str.replace(i, 1, "&lt;");

  i = str.find(">");
  if (i != std::string::npos)
    str.replace(i, 1, "&gt;");

  //  std::cout << "encode_xml-done: " << str << std::endl;

  return str;
}

Vector
XMLhelper::parse_vector(xmlDocPtr doc, xmlNodePtr cur)
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

      char* ident = (char*)xmlNodeListGetString(doc, cur->children, 1);

      if (ident)
	{
	  //std::cout << "parse_position: ident = " << ident << std::endl;
	  if (XMLhelper::equal_str(cur->name, "x-pos")) {
	    pos.x = StringConverter::to_float(ident);
	  } else if (XMLhelper::equal_str(cur->name, "y-pos")) {
	    pos.y = StringConverter::to_float(ident);
	  } else if (XMLhelper::equal_str(cur->name, "z-pos")) {
	    pos.z = StringConverter::to_float(ident);
	  } else {
	    std::cout << "Unhandled position ident: " << ident << std::endl;
	  }
	  xmlFree(ident);
	}
      cur = cur->next;
    }
  return pos;
}

int
XMLhelper::parse_int(xmlDocPtr doc, xmlNodePtr cur)
{
  cur = cur->children;
  
  int number = 999;
  char* number_str = (char*)xmlNodeListGetString(doc, cur, 1);
  if (number_str) {
    number = StringConverter::to_int(number_str);
    xmlFree(number_str);
  } else {
    std::cout << "Error: XMLhelper: parse_int: Field empty" << std::endl;
  }
  return number;
}

float
XMLhelper::parse_float(xmlDocPtr doc, xmlNodePtr cur)
{
  cur = cur->children;
  
  float number = 3.1415927f;
  char* number_str = (char*)xmlNodeListGetString(doc, cur, 1);
  if (number_str) {
    number = StringConverter::to_float(number_str);
    xmlFree(number_str);
  } else {
    std::cout << "XMLhelper: parse_int: Field empty" << std::endl;
  }
  return number;
}

Color
XMLhelper::parse_color(xmlDocPtr doc, xmlNodePtr cur)
{
  Color color;
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
      
      char* type = get_prop(cur, "type");
      
      if (type)
	{
	  if (strcmp(type, "file") == 0)
	    {
	      desc.type = ResDescriptor::RD_FILE;	 
	      xmlNodePtr ccur = cur->children;
	      desc.type = ResDescriptor::RD_RESOURCE;
	      while (ccur != NULL)
		{
		  if (xmlIsBlankNode(cur)) 
		    {
		      cur = cur->next;
		      continue;
		    }

		  if (XMLhelper::equal_str(ccur->name, "resource-file"))
		    {
		      char* filename = (char*)xmlNodeListGetString(doc, ccur->children, 1);
		      if (filename) 
			{
			  desc.res_name = filename;
			  desc.type = ResDescriptor::RD_FILE;
			  xmlFree(filename);
			}       
		    }
		  else if (XMLhelper::equal_str(ccur->name, "modifier"))
		    {
		      char* ident = (char*)xmlNodeListGetString(doc, ccur->children, 1);
		      if (ident) 
			{
			  //std::cout << "Seen: modifier: " << ident << std::endl;

			  desc.modifier = ResourceModifierNS::rs_from_string(ident);
			  xmlFree(ident);
			}
		    }
		  else
		    {
		      std::cout << "XMLhelper::parse_surface: "
				<< ": unhandled: " << ccur->name << std::endl;
		    }
		  ccur = ccur->next;
		}
	    }
	  else if (strcmp(type, "datafile") == 0)
	    {
	      xmlNodePtr ccur = cur->children;
	      desc.type = ResDescriptor::RD_RESOURCE;
	      while (ccur != NULL)
		{
		  if (xmlIsBlankNode(ccur)) 
		    {
		      ccur = ccur->next;
		      continue;
		    }

		  if (XMLhelper::equal_str(ccur->name, "resource-datafile"))
		    {
		      char* datafile = (char*)xmlNodeListGetString(doc, ccur->children, 1);
		      if (datafile) 
			{
			  desc.datafile = datafile;
			  xmlFree(datafile);
			}
		      else
			{
			  std::cout << "XMLhelper: parse_surface() Empty" << std::endl;
			}
		    }
		  else if (XMLhelper::equal_str(ccur->name, "resource-ident"))
		    {
		      char* ident = (char*)xmlNodeListGetString(doc, ccur->children, 1);
		      if (ident) 
			{
			  desc.res_name = ident;
			  xmlFree(ident);		  
			}
		    }
		  else if (XMLhelper::equal_str(ccur->name, "modifier"))
		    {
		      //std::cout << "Modifier!!!!!" << std::endl;
		      char* ident = (char*)xmlNodeListGetString(doc, ccur->children, 1);
		      if (ident) 
			{
			  //std::cout << "Seen: modifier: " << ident << std::endl;
		      
			  desc.modifier = ResourceModifierNS::rs_from_string(ident);
			  xmlFree(ident);
			}
		    }
		  else
		    {
		      std::cout << "XMLHelper:parse_surface2: unhandled " << ccur->name << std::endl;
		    }
		  ccur = ccur->next;
		}
	    }
	  else
	    {
	      std::cout << "XMLhelper: Unhandled resource type: " << type << std::endl;	  
	    }
	  xmlFree(type);
	}
      cur = cur->next;
    }

  //std::cout << "XML: parse_surface(): " << desc.res_name << " " << desc.datafile  << std::endl;

  return desc;
}

std::string 
XMLhelper::parse_string(xmlDocPtr doc, xmlNodePtr cur)
{
  std::string ret_str;
  cur = cur->children;

  char* str = (char*)xmlNodeListGetString(doc, cur, 1);
  if (str) 
    {
      ret_str = str;
      xmlFree(str);
      return ret_str;
    }
  else
    {  
      std::cout << "XMLhelper::parse_string: Field empty" << std::endl;
      return "";
    }
}


void
XMLhelper::write_desc_xml(std::ostream& xml, ResDescriptor desc)
{
  xml << "  <surface><resource type=\"";
  switch (desc.type)
    {
    case ResDescriptor::RD_FILE:
      xml << "file\">\n"
	     << "    <resource-file>"
	     << desc.res_name
	     << "</resource-file>\n";
      break;
    case ResDescriptor::RD_RESOURCE:
      xml << "datafile\">\n"
	     << "    <resource-datafile>"
	     << desc.datafile
	     << "</resource-datafile>\n"
	     << "  <resource-ident>"
	     << desc.res_name
	     << "</resource-ident>\n";
      break;
    default:
      std::cout << "EditorObj::save_desc_xml(): Unhandled resource type" << std::endl;
      break;
    }

  xml << "    <modifier>" << ResourceModifierNS::rs_to_string(desc.modifier) << "</modifier>" << std::endl;
  xml << "  </resource></surface>" << std::endl;
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

/* EOF */


