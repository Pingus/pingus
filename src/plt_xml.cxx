//  $Id: plt_xml.cxx,v 1.9 2002/09/28 19:31:06 torangan Exp $
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
#include "pingus_error.hxx"
#include "xml_helper.hxx"
#include "plt_xml.hxx"

PLTXML::PLTXML ()
{
}

PLTXML::~PLTXML ()
{
}


void
PLTXML::parse (std::string filename)
{
  doc = xmlParseFile(filename.c_str());
  if (doc)
    {
      parse_file();
      xmlFreeDoc(doc);
    }
  else
    {
      PingusError::raise("PLTXML: Couldn't load : " + filename);
    }
}

void 
PLTXML::parse_background (xmlNodePtr cur)
{
  cur = cur->children;

  while (cur)
    {
      if (XMLhelper::equal_str(cur->name, "surface"))
	{
	  background.desc = XMLhelper::parse_surface(doc, cur);
	}
      else if (XMLhelper::equal_str(cur->name, "color"))
	{
	  background.color = XMLhelper::parse_color(doc, cur);
	}
      else if (XMLhelper::equal_str(cur->name, "para-x"))
	{
	  background.para_x = XMLhelper::parse_float(doc, cur);
	}
      else if (XMLhelper::equal_str(cur->name, "para-y"))
	{
	  background.para_y = XMLhelper::parse_float(doc, cur);
	}
      else if (XMLhelper::equal_str(cur->name, "scroll-x"))
	{
	  background.scroll_x = XMLhelper::parse_float(doc, cur);
	}
      else if (XMLhelper::equal_str(cur->name, "scroll-y"))
	{
	  background.scroll_y = XMLhelper::parse_float(doc, cur);
	}
      else if (XMLhelper::equal_str(cur->name, "stretch-x"))
	{
	  background.stretch_x = XMLhelper::parse_float(doc, cur);
	}
      else if (XMLhelper::equal_str(cur->name, "stretch-y"))
	{
	  background.stretch_y = XMLhelper::parse_float(doc, cur);
	}
      else
	{
	  std::cout << "PLTXML::parse_background(): Unhandled: " << cur->name << std::endl;
	}
      cur = cur->next;
    }    
}

void 
PLTXML::parse_description(xmlNodePtr cur)
{
  char* desc = (char*)xmlNodeListGetString(doc, cur->children, 1);
  std::string lang;
  
  if (desc) {
    if (XMLhelper::get_prop(cur, "lang", lang))
      description[lang] = desc;
    else
      description[default_language] = desc;
  }

  if (desc) xmlFree(desc);	  
}

void 
PLTXML::parse_world_name (xmlNodePtr cur)
{
  char* name = (char*)xmlNodeListGetString(doc, cur->children, 1);
  std::string lang;

  if (name) 
    {
      if (XMLhelper::get_prop(cur, "lang", lang))
	world_name[lang] = name;
      else
	world_name[default_language] = name;
    }

  if (name) xmlFree(name);
}

void
PLTXML::parse_level_list (xmlNodePtr cur)
{
  cur = cur->children;

  while (cur)
    {
      if (XMLhelper::equal_str(cur->name, "level"))
	{
	  char* name = (char*)xmlNodeListGetString(doc, cur->children, 1);
	  if (name)
	    {
	      level_list.push_back(name);
	      xmlFree(name);
	    }
	}
      else
	{
	  std::cout << "PLTXML: Unhandled: " << cur->name << std::endl;
	}
      cur = cur->next;
    }  
}

void
PLTXML::parse_file()
{
  xmlNodePtr cur = doc->ROOT;

  if (cur && XMLhelper::equal_str(cur->name, "pingus-world"))
    {
      cur = cur->children;

      while (cur)
	{
	  if (XMLhelper::equal_str(cur->name, "description"))
	    {
	      parse_description(cur);
	    }
	  else if (XMLhelper::equal_str(cur->name, "background"))
	    {
	      parse_background(cur);
	    }
	  else if (XMLhelper::equal_str(cur->name, "world-name"))
	    {
	      parse_world_name(cur);
	    }
	  else if (XMLhelper::equal_str(cur->name, "level-list"))
	    {
	      parse_level_list(cur);
	    }
	  else
	    {
	      std::cout << "PLTXML: Unhandled: " << cur->name << std::endl;
	    }
	  cur = cur->next;
	}
    }
}

std::vector<std::string> 
PLTXML::get_levels ()
{
  return level_list;
}

std::map<std::string,std::string> 
PLTXML::get_name ()
{
  return world_name;
}

std::map<std::string,std::string> 
PLTXML::get_description ()
{

  return description;
}

SurfaceBackgroundData
PLTXML::get_background ()
{
  return background;
}

/* EOF */
