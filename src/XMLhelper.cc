//  $Id: XMLhelper.cc,v 1.3 2000/08/05 18:52:22 grumbel Exp $
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

#include "StringConverter.hh"
#include "XMLhelper.hh"

std::string
XMLhelper::encode_entities(const std::string& arg_str)
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



Position
XMLhelper::parse_position(xmlDocPtr doc, xmlNodePtr cur)
{
  Position pos;
  cur = cur->childs;  
  while (cur != NULL)
    {
      char* ident = (char*)xmlNodeListGetString(doc, cur->childs, 1);

      if (ident)
	{
	  //std::cout << "parse_position: ident = " << ident << std::endl;
	  if (strcmp((char*)cur->name, "x-pos") == 0) {
	    pos.x_pos = StringConverter::to_int(ident);
	  } else if (strcmp((char*)cur->name, "y-pos") == 0) {
	    pos.y_pos = StringConverter::to_int(ident);
	  } else if (strcmp((char*)cur->name, "z-pos") == 0) {
	    pos.z_pos = StringConverter::to_int(ident);
	  } else {
	    std::cout << "Unhandled position ident: " << ident << std::endl;
	  }
	  free(ident);
	}
      cur = cur->next;
    }
  return pos;
}

int
XMLhelper::parse_int(xmlDocPtr doc, xmlNodePtr cur)
{
  cur = cur->childs;
  
  int number = 999;
  char* number_str = (char*)xmlNodeListGetString(doc, cur, 1);
  if (number_str) {
    number = StringConverter::to_int(number_str);
    free(number_str);
  } else {
    std::cout << "XMLhelper: parse_int: Field empty" << std::endl;
  }
  return number;
}

float
XMLhelper::parse_float(xmlDocPtr doc, xmlNodePtr cur)
{
  cur = cur->childs;
  
  float number = 3.1415927;
  char* number_str = (char*)xmlNodeListGetString(doc, cur, 1);
  if (number_str) {
    number = StringConverter::to_float(number_str);
    free(number_str);
  } else {
    std::cout << "XMLhelper: parse_int: Field empty" << std::endl;
  }
  return number;
}

Color
XMLhelper::parse_color(xmlDocPtr doc, xmlNodePtr cur)
{
  Color color;
  cur = cur->childs;

  while (cur != NULL)
    {  
      if (strcmp((char*)cur->name, "red") == 0)
	{
	  color.red = parse_float(doc, cur);
	}
      else if (strcmp((char*)cur->name, "green") == 0)
	{
	  color.green = parse_float(doc, cur);
	}
      else if (strcmp((char*)cur->name, "blue") == 0)
	{
	  color.blue = parse_float(doc, cur);
	}
      else if (strcmp((char*)cur->name, "alpha") == 0)
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
  cur = cur->childs;  
  while (cur != NULL)
    {
      char* type = (char*)xmlGetProp(cur, (xmlChar*)"type");
      
      if (type)
	{
	  if (strcmp(type, "file") == 0)
	    {
	      desc.type = ResDescriptor::FILE;	 
	      xmlNodePtr ccur = cur->childs;
	      desc.type = ResDescriptor::RESOURCE;
	      while (ccur != NULL)
		{
		  if (strcmp((char*)ccur->name, "resource-file") == 0)
		    {
		      char* filename = (char*)xmlNodeListGetString(doc, ccur->childs, 1);
		      if (filename) 
			{
			  desc.res_name = filename;
			  free(filename);
			}		      
		    }
		}
	    }
	  else if (strcmp(type, "datafile") == 0)
	    {
	      xmlNodePtr ccur = cur->childs;
	      desc.type = ResDescriptor::RESOURCE;
	      while (ccur != NULL)
		{
		  if (strcmp((char*)ccur->name, "resource-datafile") == 0)
		    {
		      char* datafile = (char*)xmlNodeListGetString(doc, ccur->childs, 1);
		      if (datafile) 
			{
			  desc.datafile = datafile;
			  free(datafile);
			}
		      else
			{
			  std::cout << "XMLhelper: parse_surface() Empty" << std::endl;
			}
		    }
		  else if (strcmp((char*)ccur->name, "resource-ident") == 0)
		    {
		      char* ident = (char*)xmlNodeListGetString(doc, ccur->childs, 1);
		      if (ident) 
			{
			  desc.res_name = ident;
			  free(ident);		  
			}
		    }
		  ccur = ccur->next;
		}
	    }
	  else
	    {
	      std::cout << "XMLhelper: Unhandled resource type: " << type << std::endl;	  
	    }
	  free(type);
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
  cur = cur->childs;

  char* str = (char*)xmlNodeListGetString(doc, cur, 1);
  if (str) 
    {
      ret_str = str;
      free(str);
      return ret_str;
    }
  else
    {  
      std::cout << "XMLhelper::parse_string: Field empty" << std::endl;
      return "";
    }
}

/* EOF */
