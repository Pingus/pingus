//  $Id: xml_plf.cxx,v 1.30 2002/10/03 12:33:09 grumbel Exp $
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

XMLPLF::XMLPLF (const std::string& arg_filename)
{
  //  std::cout << "----- Parsing .xml file" << std::endl;
  //std::cout << "--- Checksum: " << std::flush;
  std::string str = System::checksum (arg_filename);
  //std::cout << str << std::endl;

  doc = xmlParseFile(arg_filename.c_str());

  if (doc == NULL)
    PingusError::raise("XMLPLF: Couldn't open \"" + arg_filename + "\"");

  parse_file();

  // FIXME: Dirty hack, should be replaced with a unified file namespace
  filename = System::basename(arg_filename);
  filename = filename.substr(0, filename.length()-4);
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
	  if (!XMLhelper::node_list_get_string(doc, cur->children, 1, button.number_of))
	    std::cout << "XMLPLF:parse_actions: no action number given" << std::endl;
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
	  std::string name;
	  std::string lang;

	  if (XMLhelper::node_list_get_string(doc, cur->children, 1, name)) {
	    if (XMLhelper::get_prop(cur, "lang", lang))
	      levelname[lang] = name;
	    else
	      levelname[default_language] = name;
	  }
	}
      else if (XMLhelper::equal_str(cur->name, "description"))
	{
	  std::string desc;
	  std::string lang;

	  if (XMLhelper::node_list_get_string(doc, cur->children, 1, desc)) {
	    if (XMLhelper::get_prop(cur, "lang", lang))		    
	      description[lang] = desc;
	    else
	      description[default_language] = desc;
	  }
	}
      else if (XMLhelper::equal_str(cur->name, "author"))
	{
	  XMLhelper::node_list_get_string(doc, cur->children, 1, author);
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
  xmlNodePtr child_cur = XMLhelper::skip_blank(cur->children);
  
  if (XMLhelper::equal_str(child_cur->name, "type"))
    {
      std::string name;
      if (XMLhelper::node_list_get_string(doc, child_cur->children, 1, name))
	{
	  worldobjs_data.push_back(WorldObjDataFactory::instance()->create(name, doc, cur));
	  return;
	}
    }

  std::cout << "XMLPLF::parse_traps: Invalid data structure" << std::endl;
}

/* EOF */
