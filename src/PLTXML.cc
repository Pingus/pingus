//  $Id: PLTXML.cc,v 1.5 2000/09/30 21:34:42 grumbel Exp $
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

#include "globals.hh"
#include "PingusError.hh"
#include "XMLhelper.hh"
#include "PLTXML.hh"

PLTXML::PLTXML()
{
}

PLTXML::~PLTXML()
{
}


///
void
PLTXML::parse(std::string filename)
{
  doc = xmlParseFile(filename.c_str());
  if (doc)
    {
      parse_file();
      xmlFreeDoc(doc);
    }
  else
    {
      throw PingusError("PLTXML: Couldn't load : " + filename);
    }
}

void 
PLTXML::parse_background(xmlNodePtr cur)
{
  cur = cur->children;

  while (cur != NULL)
    {
      if (strcmp((char*)cur->name, "surface") == 0)
	{
	  background.desc = XMLhelper::parse_surface(doc, cur);
	}
      else if (strcmp((char*)cur->name, "color") == 0)
	{
	  background.color = XMLhelper::parse_color(doc, cur);
	}
      else if (strcmp((char*)cur->name, "para-x") == 0)
	{
	  background.para_x = XMLhelper::parse_float(doc, cur);
	}
      else if (strcmp((char*)cur->name, "para-y") == 0)
	{
	  background.para_y = XMLhelper::parse_float(doc, cur);
	}
      else if (strcmp((char*)cur->name, "scroll-x") == 0)
	{
	  background.scroll_x = XMLhelper::parse_float(doc, cur);
	}
      else if (strcmp((char*)cur->name, "scroll-y") == 0)
	{
	  background.scroll_y = XMLhelper::parse_float(doc, cur);
	}
      else if (strcmp((char*)cur->name, "stretch-x") == 0)
	{
	  background.stretch_x = XMLhelper::parse_float(doc, cur);
	}
      else if (strcmp((char*)cur->name, "stretch-y") == 0)
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
  char* lang = (char*)xmlGetProp(cur, (xmlChar*)"lang");

  if (desc) {
    if (lang)		    
      description[lang] = desc;
    else
      description[default_language] = desc;
  }

  if (desc) free(desc);	  
  if (lang) free(lang);
}

void 
PLTXML::parse_world_name(xmlNodePtr cur)
{
  char* name = (char*)xmlNodeListGetString(doc, cur->children, 1);
  char* lang = (char*)xmlGetProp(cur, (xmlChar*)"lang");

  if (name) 
    {
      if (lang)
	world_name[lang] = name;
      else
	world_name[default_language] = name;
    }

  if (name) free(name);
  if (lang) free(lang);
}

void
PLTXML::parse_level_list(xmlNodePtr cur)
{
  cur = cur->children;

  while (cur != NULL)
    {
      if (strcmp((char*)cur->name, "level") == 0)
	{
	  char* name = (char*)xmlNodeListGetString(doc, cur->children, 1);
	  if (name)
	    {
	      level_list.push_back(name);
	      free(name);
	    }
	}
      else
	{
	  std::cout << "PLTXML: Unhandeld: " << cur->name << std::endl;
	}
      cur = cur->next;
    }  
}

void
PLTXML::parse_file()
{
  xmlNodePtr cur = doc->ROOT;

  if (cur != NULL && strcmp((const char*)cur->name, "pingus-world") == 0)
    {
      cur = cur->children;

      while (cur != NULL)
	{
	  if (strcmp((char*)cur->name, "description") == 0)
	    {
	      parse_description(cur);
	    }
	  else if (strcmp((char*)cur->name, "background") == 0)
	    {
	      parse_background(cur);
	    }
	  else if (strcmp((char*)cur->name, "world-name") == 0)
	    {
	      parse_world_name(cur);
	    }
	  else if (strcmp((char*)cur->name, "level-list") == 0)
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
PLTXML::get_levels()
{
  return level_list;
}

map<std::string,std::string> 
PLTXML::get_name()
{
  return world_name;
}

map<std::string,std::string> 
PLTXML::get_description()
{

  return description;
}

SurfaceBackgroundData
PLTXML::get_background()
{
  return background;
}

/* EOF */
