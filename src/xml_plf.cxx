//  $Id: xml_plf.cxx,v 1.27 2002/09/28 19:31:06 torangan Exp $
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
#include "xml_helper.hxx"
#include "xml_plf.hxx"
#include "globals.hxx"
#include "system.hxx"
#include "pingus_error.hxx"
#include "string_converter.hxx"
#include "worldobj_data_factory.hxx"
#include "worldobjsdata/entrance_data.hxx"
#include "worldobjsdata/exit_data.hxx"
#include "worldobjsdata/hotspot_data.hxx"
#include "worldobjsdata/liquid_data.hxx"
#include "worldobjsdata/worldobj_group_data.hxx"

using Actions::action_from_string;

XMLPLF::XMLPLF (const std::string& filename)
{
  //  std::cout << "----- Parsing .xml file" << std::endl;
  //std::cout << "--- Checksum: " << std::flush;
  std::string str = System::checksum (filename);
  //std::cout << str << std::endl;

  doc = xmlParseFile(filename.c_str());

  if (doc == NULL)
    PingusError::raise("XMLPLF: Couldn't open \"" + filename + "\"");

  parse_file();
}

XMLPLF::~XMLPLF()
{
  xmlFreeDoc(doc);
  /*
  // Free all the allocated memory
  for(vector<BackgroundData*>::iterator i = backgrounds.begin ();
  i != backgrounds.end ();
  i++)
  delete *i;

  for(vector<WorldObjData*>::iterator i = worldobjs_data.begin ();
  i != worldobjs_data.end ();
  i++)
  delete *i;
  */
}

void
XMLPLF::parse_file()
{
  //std::cout << "parsing file" << std::endl;
  xmlNodePtr cur = doc->ROOT;

  cur = XMLhelper::skip_blank(cur);

  if (cur && XMLhelper::equal_str(cur->name, "pingus-level"))
    {
      //std::cout << "parse_file...." << std::endl;

      cur = XMLhelper::skip_blank(cur);

      if (!cur->children)
	std::cout << "XMLPLF: node: " << cur->name << std::endl;
      
      cur = cur->children;
      cur = XMLhelper::skip_blank(cur);
      
      while (cur)
	{
	  if (xmlIsBlankNode(cur)) 
	    {
	      cur = cur->next;
	      continue;
	    }

	  // FIXME: This can mostly be unified with the
	  // WorldObjDataFactory, exit, backgrounds, etc. are all
	  // WorldObjs
	  if (XMLhelper::equal_str(cur->name, "global"))
	    {
	      parse_global(cur);
	    }
	  else if (XMLhelper::equal_str(cur->name, "action-list"))
	    {
	      parse_actions(cur);
	    }
	  else if (XMLhelper::equal_str(cur->name, "background"))
	    {
	      parse_background(cur);
	    }
	  else if (XMLhelper::equal_str(cur->name, "groundpiece"))
	    {
#ifndef OLD_GROUNDPIECES
	      // FIXME: This is *not* backward compatible and wreck the levels
	      // worldobjs_data.push_back(WorldObjDataFactory::instance()->create (doc, cur));

	      // This probally is backward compatible
	      //groundpieces.push_back(WorldObjsData::GroundpieceData (doc, cur));
	      worldobjs_data.push_back(new WorldObjsData::GroundpieceData (doc, cur));
#else
	      parse_groundpiece(cur);
#endif
	    }
	  else if (XMLhelper::equal_str(cur->name, "exit"))
	    {
	      worldobjs_data.push_back (new WorldObjsData::ExitData(doc, cur));
	    }
	  else if (XMLhelper::equal_str(cur->name, "entrance"))
	    {
	      worldobjs_data.push_back (new WorldObjsData::EntranceData(doc, cur));
	    }
	  else if (XMLhelper::equal_str(cur->name, "trap"))
	    {
	      parse_traps(cur);
	    }
	  else if (XMLhelper::equal_str(cur->name, "hotspot"))
	    {
	      worldobjs_data.push_back(new WorldObjsData::HotspotData(doc, cur));
	    }
	  else if (XMLhelper::equal_str(cur->name, "liquid"))
	    {
	      worldobjs_data.push_back(new WorldObjsData::LiquidData(doc, cur));
	    }
	  else if (XMLhelper::equal_str(cur->name, "worldobj"))
	    {
	      worldobjs_data.push_back(WorldObjDataFactory::instance()->create(doc, cur));
	    }
	  else if (XMLhelper::equal_str(cur->name, "group"))
	    {
	      //worldobjs_data.push_back (new WorldObjsData::WorldObjGroupData (doc, cur));
	      parse_group (cur);
	    }
	  else if (XMLhelper::equal_str(cur->name, "start-position"))
	    {
	      parse_start_pos(cur);
	    }
	  else if (XMLhelper::equal_str(cur->name, "weather"))
	    {
	      worldobjs_data.push_back(WorldObjDataFactory::instance()->create (doc, cur));
	    }	  
	  else
	    {
	      printf("XMLPLF: Unhandled: %s\n", reinterpret_cast<const char*>(cur->name));
	    }
	  cur = cur->next;
	}
      //puts("global done");
    } else {
      PingusError::raise("XMLPLF: This is no valid Pingus level");
    }
}

void
XMLPLF::parse_group (xmlNodePtr cur)
{
  cur = cur->children;
  
  while (cur)
    {
      if (XMLhelper::equal_str(cur->name, "background"))
	{
	  parse_background(cur);
	}
      else if (XMLhelper::equal_str(cur->name, "groundpiece"))
	{
	  parse_groundpiece(cur);
	}
      else if (XMLhelper::equal_str(cur->name, "exit"))
	{
	  worldobjs_data.push_back(new WorldObjsData::ExitData(doc, cur));
	}
      else if (XMLhelper::equal_str(cur->name, "entrance"))
	{
	  worldobjs_data.push_back(new WorldObjsData::EntranceData(doc, cur));
	}
      else if (XMLhelper::equal_str(cur->name, "trap"))
	{
	  parse_traps(cur);
	}
      else if (XMLhelper::equal_str(cur->name, "hotspot"))
	{
	  worldobjs_data.push_back(new WorldObjsData::HotspotData (doc, cur));
	}
      else if (XMLhelper::equal_str(cur->name, "liquid"))
	{
	  worldobjs_data.push_back(new WorldObjsData::LiquidData (doc, cur));
	}
      else if (XMLhelper::equal_str(cur->name, "worldobj"))
	{
	  worldobjs_data.push_back(WorldObjDataFactory::instance()->create (doc, cur));
	}
      else if (XMLhelper::equal_str(cur->name, "group"))
	{
	  //worldobjs_data.push_back (new WorldObjsData::WorldObjGroupData (doc, cur));
	  parse_group (cur);
	}
      else if (XMLhelper::equal_str(cur->name, "start-position"))
	{
	  parse_start_pos(cur);
	}
      else if (XMLhelper::equal_str(cur->name, "weather"))
	{
	  worldobjs_data.push_back(WorldObjDataFactory::instance()->create (doc, cur));
	}
      else
	{
	  printf("XMLPLF: Unhandled parse_group: %s\n", reinterpret_cast<const char*>(cur->name));
	}
      cur = cur->next;
    }
}

void
XMLPLF::parse_start_pos (xmlNodePtr cur)
{
  cur = cur->children;

  while (cur)
    {
      if (xmlIsBlankNode(cur)) 
	{
	  cur = cur->next;
	  continue;
	}
      
      if (XMLhelper::equal_str(cur->name, "position"))
	{
	  Vector pos = XMLhelper::parse_vector(doc, cur);
	  start_x_pos = static_cast<int>(pos.x);
	  start_y_pos = static_cast<int>(pos.y);
	}
      else
	{
	  std::cout << "XMLPLF::parse_start_pos: " << cur->name << std::endl;
	}
      cur = cur->next;
    }
}

void 
XMLPLF::parse_background (xmlNodePtr cur)
{
  // The allocated objects are delete'd in the destructor
  //FIXME: Repair me backgrounds.push_back(BackgroundData::create (doc, cur));

  std::string type;
  if (XMLhelper::get_prop(cur, "type", type))
    {
      worldobjs_data.push_back(WorldObjDataFactory::instance()
			       ->create (type + "-background", doc, cur));
    }
  else
    {
      worldobjs_data.push_back(WorldObjDataFactory::instance()
			       ->create ("surface-background", doc, cur));
    }
}

void 
XMLPLF::parse_actions (xmlNodePtr cur)
{
  cur = cur->children;

  while (cur)
    {
      if (xmlIsBlankNode(cur)) 
	{
	  cur = cur->next;
	  continue;
	}
 
      ActionData button;
      button.name = action_from_string(reinterpret_cast<const char*>(cur->name));

      if (!XMLhelper::get_prop(cur, "count", button.number_of))
	{
	  //std::cout << "XMLPLF::parse_actions (): No 'count' given, fallback to the old format" << std::endl;
	  char* number = reinterpret_cast<char*>(xmlNodeListGetString(doc, cur->children, 1));
	  if (number) {
	    //std::cout << "xmlNoder..Result: " << number << std::endl;
	    button.number_of = StringConverter::to_int(number);
	  } else {
	    std::cout << "XMLPLF:parse_actions: no action number given" << std::endl;
	  }
	  xmlFree(number);
	}

      actions.push_back(button);
      
      cur = cur->next;
    }      
}

void
XMLPLF::parse_global (xmlNodePtr cur)
{
  cur = cur->children;
  while (cur)
    {
      if (xmlIsBlankNode(cur)) 
	{
	  cur = cur->next;
	  continue;
	}

      if (XMLhelper::equal_str(cur->name, "levelname"))
	{
	  char* name = reinterpret_cast<char*>(xmlNodeListGetString(doc, cur->children, 1));
	  std::string lang;

	  if (name) {
	    if (XMLhelper::get_prop(cur, "lang", lang))
	      levelname[lang] = name;
	    else
	      levelname[default_language] = name;
	  }

	  if (name)
	    xmlFree(name);
	}
      else if (XMLhelper::equal_str(cur->name, "description"))
	{
	  char* desc = reinterpret_cast<char*>(xmlNodeListGetString(doc, cur->children, 1));
	  std::string lang;

	  if (desc) {
	    if (XMLhelper::get_prop(cur, "lang", lang))		    
	      description[lang] = desc;
	    else
	      description[default_language] = desc;
	  }

	  if (desc)
	    xmlFree(desc);	  
	}
      else if (XMLhelper::equal_str(cur->name, "author"))
	{
	  char* tmp_author = reinterpret_cast<char*>(xmlNodeListGetString(doc, cur->children, 1));
	  if (tmp_author) {
	    author = tmp_author;
	    xmlFree(tmp_author);
	  }
	  //std::cout << "Author: " << author << " -----------------------" << std::endl;
	}
      else if (XMLhelper::equal_str(cur->name, "number-of-pingus"))
	{
	  number_of_pingus = XMLhelper::parse_int(doc, cur);
	}
      else if (XMLhelper::equal_str(cur->name, "difficulty"))
	{
	  difficulty = XMLhelper::parse_int(doc, cur);
	}
      else if (XMLhelper::equal_str(cur->name, "playable"))
	{
	  playable = XMLhelper::parse_int(doc, cur);
	}
      else if (XMLhelper::equal_str(cur->name, "comment"))
	{
	  comment = XMLhelper::parse_string(doc, cur);
	}
      else if (XMLhelper::equal_str(cur->name, "number-to-save"))
	{
	  number_to_save = XMLhelper::parse_int(doc, cur);
	}
      else if (XMLhelper::equal_str(cur->name, "time"))
	{
	  max_time = XMLhelper::parse_int(doc, cur);
	}
      else if (XMLhelper::equal_str(cur->name, "height"))
	{
	  height = XMLhelper::parse_int(doc, cur);
	}
      else if (XMLhelper::equal_str(cur->name, "width"))
	{
	  width = XMLhelper::parse_int(doc, cur);
	}
      else
	{
	  std::cout << "XMLPLF: global: Unhandled: " << cur->name << std::endl;
	}

      cur = cur->next;
    }
}

void 
XMLPLF::parse_groundpiece (xmlNodePtr cur)
{
  groundpieces.push_back(WorldObjsData::GroundpieceData (doc, cur));
}

void
XMLPLF::parse_traps (xmlNodePtr cur)
{
  xmlNodePtr cur_ = XMLhelper::skip_blank(cur->children);
  
  if (XMLhelper::equal_str(cur_->name, "type"))
    {
      char* name = reinterpret_cast<char*>(xmlNodeListGetString(doc, cur_->children, 1));
      if (name)
	{
	  worldobjs_data.push_back(WorldObjDataFactory::instance()->create (name, doc, XMLhelper::skip_blank(cur)));
	  xmlFree(name);
	  return;
	}
    }

  std::cout << "XMLPLF::parse_traps: Invalid data structure" << std::endl;
}

/* EOF */
