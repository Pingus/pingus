//  $Id: xml_plf.cxx,v 1.40 2003/10/19 12:25:47 grumbel Exp $
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
#include "xmlgz_reader.hxx"
#include "xml_file_reader_old.hxx"
#include "xml_plf.hxx"
#include "globals.hxx"
#include "system.hxx"
#include "vector.hxx"
#include "pingus_error.hxx"
#include "worldobj_factory.hxx"
#include "worldobjs/groundpiece.hxx"

namespace Pingus {

using Actions::action_from_string;

XMLPLF::XMLPLF (const std::string& arg_filename)
{
  filename = arg_filename;
  std::string str = System::checksum (filename);

  doc = xmlgzParseFile(filename.c_str());

  if (doc == NULL)
    PingusError::raise("XMLPLF: Couldn't open \"" + filename + "\"");

  resname = System::basename(System::dirname(filename))
    + "/" + System::basename(filename.substr(0, filename.length()-7));

  parse_file();
}

XMLPLF::~XMLPLF()
{
  xmlFreeDoc(doc);

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

	  if (XMLhelper::equal_str(cur->name, "version"))
            {
              // FIXME: ignoring version at the moment
            }
	  // FIXME: This can mostly be unified with the
	  // WorldObjDataFactory, exit, backgrounds, etc. are all
	  // WorldObjs
	  else if (XMLhelper::equal_str(cur->name, "global"))
	    {
	      parse_global(cur);
	    }
	  else if (XMLhelper::equal_str(cur->name, "action-list"))
	    {
	      parse_actions(cur);
	    }
          else
            {
              parse_worldobjs(cur);
            }
         
	  cur = cur->next;
	}
      //puts("global done");
    } else {
      PingusError::raise("XMLPLF: This is no valid Pingus level");
    }
}

void
XMLPLF::parse_worldobjs(xmlNodePtr cur)
{
  if (XMLhelper::equal_str(cur->name, "background"))
    {
      parse_background(cur);
    }
  else if (XMLhelper::equal_str(cur->name, "groundpiece") ||
           XMLhelper::equal_str(cur->name, "exit")        ||
           XMLhelper::equal_str(cur->name, "hotspot")     ||
           XMLhelper::equal_str(cur->name, "liquid")      ||
           XMLhelper::equal_str(cur->name, "worldobj")    ||
           XMLhelper::equal_str(cur->name, "prefab")      ||
           XMLhelper::equal_str(cur->name, "weather")     ||
           XMLhelper::equal_str(cur->name, "entrance"))
    {
      worldobjs.push_back(WorldObjFactory::instance()->create(doc, cur));
    }
  else if (XMLhelper::equal_str(cur->name, "trap"))
    {
      parse_traps(cur);
    }
  else if (XMLhelper::equal_str(cur->name, "group"))
    {
      //worldobjs_data.push_back(WorldObjDataFactory::instance()->create(doc, cur));
      xmlNodePtr mycur = cur->children;
      while (mycur)
	{
	  if (xmlIsBlankNode(mycur))
	    {
	      mycur = mycur->next;
	      continue;
	    }

          parse_worldobjs(mycur);

          mycur = mycur->next;
          mycur = XMLhelper::skip_blank(mycur->next);
        }
    }
  else if (XMLhelper::equal_str(cur->name, "start-position"))
    {
      parse_start_pos(cur);
    }
  else
    {
      printf("XMLPLF: Unhandled: %s\n", reinterpret_cast<const char*>(cur->name));
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
      worldobjs.push_back(WorldObjFactory::instance()
			       ->create (type + "-background", doc, cur));
    }
  else
    {
      worldobjs.push_back(WorldObjFactory::instance()
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
  XMLFileReaderOld reader(doc, cur);

  reader.read_string("author", author);
  reader.read_int("number-of-pingus", number_of_pingus);
  reader.read_int("number-to-save", number_to_save);
  reader.read_int("time", max_time);
  reader.read_int("difficulty", difficulty);
  reader.read_bool("playable", playable);
  reader.read_string("comment", comment);
  reader.read_string("music", music);
  reader.read_int("width", width);
  reader.read_int("height", height);

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
      cur = cur->next;
    }
}

void
XMLPLF::parse_groundpiece(xmlNodePtr cur)
{
  worldobjs.push_back(new WorldObjs::Groundpiece(XMLFileReaderOld(doc, cur)));
}

void
XMLPLF::parse_traps(xmlNodePtr cur)
{
  xmlNodePtr child_cur = XMLhelper::skip_blank(cur->children);

  if (XMLhelper::equal_str(child_cur->name, "type"))
    {
      std::string name;
      if (XMLhelper::node_list_get_string(doc, child_cur->children, 1, name))
	{
	  worldobjs.push_back(WorldObjFactory::instance()->create(name, doc, cur));
	  return;
	}
    }

  std::cout << "XMLPLF::parse_traps: Invalid data structure" << std::endl;
}

} // namespace Pingus

/* EOF */
