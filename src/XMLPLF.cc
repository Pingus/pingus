//  $Id: XMLPLF.cc,v 1.1 2000/07/30 01:47:36 grumbel Exp $
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

#include <cstdio>
#include <cstdlib>
#include "XMLPLF.hh"

#include "System.hh"
#include "PingusError.hh"
#include "StringConverter.hh"

XMLPLF::XMLPLF()
{
}

XMLPLF::XMLPLF(const string& filename)
{
  std::cout << "----- Parsing .xml file" << std::endl;

  doc = xmlParseFile(filename.c_str());

  if (doc == NULL)
    throw PingusError("Couldn't open \"" + filename + "\"");

  parse_file();
}

XMLPLF::~XMLPLF()
{
  xmlFreeDoc(doc);
}

void
XMLPLF::parse_file()
{
  xmlNodePtr cur = doc->root;

  if (cur != NULL && strcmp((const char*)cur->name, "pingus-level") == 0)
    {
      cur = cur->childs;
      while (cur != NULL)
	{
	  //puts("global loop");
	  if (strcmp((char*)cur->name, "global") == 0)
	    {
	      parse_global(cur->childs);
	    }
	  else if (strcmp((char*)cur->name, "action-list") == 0)
	    {
	      parse_actions(cur->childs);
	    }
	  else if (strcmp((char*)cur->name, "background") == 0)
	    {
	      parse_background(cur->childs);
	    }
	  else if (strcmp((char*)cur->name, "groundpiece") == 0)
	    {
	      parse_groundpiece(cur->childs);
	    }
	  else if (strcmp((char*)cur->name, "exit") == 0)
	    {
	      parse_exit(cur->childs);
	    }
	  else if (strcmp((char*)cur->name, "entrance") == 0)
	    {
	      parse_entrance(cur->childs);
	    }
	  else if (strcmp((char*)cur->name, "trap") == 0)
	    {
	      parse_traps(cur->childs);
	    }
	  else if (strcmp((char*)cur->name, "hotspot") == 0)
	    {
	      parse_hotspot(cur->childs);
	    }
	  else if (strcmp((char*)cur->name, "liquid") == 0)
	    {
	      parse_liquid(cur->childs);
	    }
	  else
	    {
	      printf("Unhandled: %s\n", (char*)cur->name);
	    }
	  cur = cur->next;
	}
    } else {
      throw PingusError("XMLPLF: This is no valid Pingus level");
    }
}

void
XMLPLF::parse_liquid(xmlNodePtr cur)
{
  LiquidData liquid;

  while (cur != NULL)
    {
      if (strcmp((char*)cur->name, "position") == 0)
	liquid.pos = parse_position(cur->childs);
      else if (strcmp((char*)cur->name, "surface") == 0)
	liquid.desc = parse_surface(cur->childs);
      else if (strcmp((char*)cur->name, "speed") == 0)
	liquid.speed = parse_int(cur->childs);
      else if (strcmp((char*)cur->name, "width") == 0)
	liquid.width = parse_int(cur->childs);
      else
	{
	  std::cout << "XMLPLF::parse_liquid: Unhandled: " << cur->name << std::endl;
	}	

      cur = cur->next;
    }
  liquids.push_back(liquid);
}

void 
XMLPLF::parse_background(xmlNodePtr cur)
{
  while (cur != NULL)
    {
      if (strcmp((char*)cur->name, "surface") == 0)
	{
	  background.desc = parse_surface(cur->childs);
	}
      cur = cur->next;
    }      
}

void 
XMLPLF::parse_actions(xmlNodePtr cur)
{
  while (cur != NULL)
    {
      char* number = (char*)xmlNodeListGetString(doc, cur->childs, 1);

      ActionData button((char*)cur->name, StringConverter::to_int(number));

      actions.push_back(button);

      free(number);
      cur = cur->next;
    }      
}

void 
XMLPLF::parse_entrance(xmlNodePtr cur)
{
  EntranceData entrance;
  
  while (cur != NULL)
    {
      if (strcmp((char*)cur->name, "type") == 0)
	{
	  char* name = (char*)xmlNodeListGetString(doc, cur->childs, 1); 
	  entrance.type = name;
	  free(name);
	}
      else if (strcmp((char*)cur->name, "position") == 0)
	{
	  entrance.pos = parse_position(cur->childs);
	}
      else if (strcmp((char*)cur->name, "release-rate") == 0)
	{
	  char* release_rate = (char*)xmlNodeListGetString(doc, cur->childs, 1);
	  entrance.release_rate = StringConverter::to_int(release_rate);
	  free(release_rate);
	}
      else if (strcmp((char*)cur->name, "direction") == 0)
	{
	  char* direction = (char*)xmlNodeListGetString(doc, cur->childs, 1);

	  if (strcmp(direction, "left") == 0)
	    entrance.direction = EntranceData::LEFT;
	  else if (strcmp(direction, "right") == 0)
	    entrance.direction = EntranceData::RIGHT;
	  else if (strcmp(direction, "misc") == 0)
	    entrance.direction = EntranceData::MISC;
	  
	  free(direction);
	}
      else
	{
	  printf("unhandeld: %s\n", (char*)cur->name);
	}	
      cur = cur->next;	
    }   
  entrances.push_back(entrance);
}

void 
XMLPLF::parse_exit(xmlNodePtr cur)
{
  ExitData exit;

  while (cur != NULL)
    {
      if (strcmp((char*)cur->name, "position") == 0)
	{
	  exit.pos = parse_position(cur->childs);
	}
      else if (strcmp((char*)cur->name, "surface") == 0)
	{
	  exit.desc = parse_surface(cur->childs);
	}
      cur = cur->next;	
    }

  exits.push_back(exit);
}

void
XMLPLF::parse_global(xmlNodePtr cur)
{
  while (cur != NULL)
    {
      if (strcmp((char*)cur->name, "levelname") == 0)
	{
	  char* name = (char*)xmlNodeListGetString(doc, cur->childs, 1);
	  char* lang = (char*)xmlGetProp(cur, (xmlChar*)"lang");

	  if (name) {
	    if (lang)
	      levelname[lang] = name;
	    else
	      levelname[System::get_language()] = name;
	  }

	  if (name) free(name);
	  if (lang) free(lang);
	}
      else if (strcmp((char*)cur->name, "description") == 0)
	{
	  char* desc = (char*)xmlNodeListGetString(doc, cur->childs, 1);
	  char* lang = (char*)xmlGetProp(cur, (xmlChar*)"lang");

	  if (desc) {
	    if (lang)		    
	      description[lang] = desc;
	    else
	      description[System::get_language()] = desc;
	  }

	  if (desc) free(desc);	  
	  if (lang) free(lang);
	}
      else if (strcmp((char*)cur->name, "author") == 0)
	{
	  char* tmp_author = (char*)xmlNodeListGetString(doc, cur->childs, 1);
	  if (tmp_author) {
	    author = tmp_author;
	    free(tmp_author);
	  }
	  std::cout << "Author: " << author << " -----------------------" << std::endl;
	}
      else if (strcmp((char*)cur->name, "number-of-pingus") == 0)
	{
	  number_of_pingus = parse_int(cur->childs);	  
	}
      else if (strcmp((char*)cur->name, "number-to-save") == 0)
	{
	  char* number = (char*)xmlNodeListGetString(doc, cur->childs, 1);
	  if (number) {
	    number_to_save = StringConverter::to_int(number);
	    free(number);
	  }
	}
      else if (strcmp((char*)cur->name, "height") == 0)
	{
	  char* number = (char*)xmlNodeListGetString(doc, cur->childs, 1);
	  if (number) {
	    height = StringConverter::to_int(number);
	    free(number);
	  }
	}
      else if (strcmp((char*)cur->name, "width") == 0)
	{
	  char* number = (char*)xmlNodeListGetString(doc, cur->childs, 1);
	  if (number) {
	    width = StringConverter::to_int(number);
	    free(number);
	  }	  
	}
      else
	{
	  std::cout << "XMLPLF: global: unhandeld: " << cur->name << std::endl;
	}

      cur = cur->next;
    }
}

void 
XMLPLF::parse_groundpiece(xmlNodePtr cur)
{
  SurfaceData surface;

  char* type = (char*)xmlGetProp(cur, (xmlChar*)"type");
  surface.type == SurfaceData::GROUND;
  if (type)
    {
      if (strcmp(type, "solid") == 0) 
	surface.type == SurfaceData::SOLID;
      else if (strcmp(type, "transparent") == 0) 
	surface.type == SurfaceData::TRANSPARENT;
      else if (strcmp(type, "ground") == 0) 
	surface.type == SurfaceData::GROUND;
      else if (strcmp(type, "bridge") == 0) 
	surface.type == SurfaceData::BRIDGE;
      else
	std::cout << "XMLPLF: Unhandeld groundpiece type: " << type << std::endl;
      free(type);
    }

  while (cur != NULL)
    {
      if (strcmp((char*)cur->name, "position") == 0)
	{
	  surface.pos = parse_position(cur->childs);
	  //	  fprintf(stderr, "  %d : %d : %d;\n",
		  //		  pos.x_pos, pos.y_pos, pos.z_pos);	    
	}
      else if (strcmp((char*)cur->name, "surface") == 0)
	{
	  surface.desc = parse_surface(cur->childs);
	}	
      else
	{
	  printf("unhandled: %s\n", (char*)cur->name);
	}
      cur = cur->next;	
    }
  groundpieces.push_back(surface);
}

void
XMLPLF::parse_traps(xmlNodePtr cur)
{
  Position pos;
  TrapData trap;

  while (cur != NULL)
    {
      if (strcmp((char*)cur->name, "type") == 0)
	{
	  char* name = (char*)xmlNodeListGetString(doc, cur->childs, 1);
	  if (name)
	    {
	      std::cout << "parse_trap: name = " << name << std::endl;
	      trap.type = name;
	      free(name);
	    }
	} 
      else if (strcmp((char*)cur->name, "position") == 0) 
	{
	  trap.pos = parse_position(cur->childs);
	}

      cur = cur->next;
    }
  traps.push_back(trap);
}  

void
XMLPLF::parse_hotspot(xmlNodePtr cur)
{
  Position pos;
  HotspotData hotspot;

  while (cur != NULL)
    {
      if (strcmp((char*)cur->name, "surface") == 0)
	{
	  hotspot.desc = parse_surface(cur->childs);
	} 
      else if (strcmp((char*)cur->name, "position") == 0) 
	{
	  hotspot.pos = parse_position(cur->childs);
	}
      else if (strcmp((char*)cur->name, "speed") == 0) 
	{
	  hotspot.speed = parse_int(cur->childs);
	}
      else if (strcmp((char*)cur->name, "speed") == 0) 
	{
	  hotspot.para = parse_int(cur->childs);
	}
      else
	{
	  std::cout << "XMLPLF: parse_hotspot: unhandeld: " << cur->name << std::endl;
	}
      cur = cur->next;
    }
  hotspots.push_back(hotspot);
}

Position
XMLPLF::parse_position(xmlNodePtr cur)
{
  Position pos;
  
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
XMLPLF::parse_int(xmlNodePtr cur)
{
  int number = 999;
  char* number_str = (char*)xmlNodeListGetString(doc, cur->childs, 1);
  if (number_str) {
    number = StringConverter::to_int(number_str);
    free(number_str);
  } else {
    std::cout << "XMLPLF: parse_int: Field empty" << std::endl;
  }
  return number;
}

ResDescriptor 
XMLPLF::parse_surface(xmlNodePtr cur)
{
  ResDescriptor desc;
  
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
			  std::cout << "XMLPLF: parse_surface() Empty" << std::endl;
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
	      std::cout << "XMLPLF: Unhandled resource type: " << type << std::endl;	  
	    }
	  free(type);
	}
      cur = cur->next;
    }

  //std::cout << "XML: parse_surface(): " << desc.res_name << " " << desc.datafile  << std::endl;

  return desc;
}

/* EOF */
