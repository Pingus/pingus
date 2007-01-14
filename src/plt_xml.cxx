//  $Id: plt_xml.cxx,v 1.13 2003/10/19 12:25:47 grumbel Exp $
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
#include <ClanLib/core.h>
#include "xml_file_reader.hxx"
#include "globals.hxx"
#include "pingus_error.hxx"
#include "xml_file_reader.hxx"
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
  CL_DomDocument doc(new CL_InputSource_File(filename), true);
  
  CL_DomElement root = doc.get_document_element();

  if (root.get_tag_name() != "pingus-level")
    {
      PingusError::raise("Error: " + filename + ": not a <pingus-world> file");
    }
  else
    {
      XMLFileReader reader(root);
      reader.read_string("name",        world_name);
      reader.read_string("description", description);
      //reader.read_desc  ("background", background_desc);
      
      FileReader levelreader;
      reader.read_section("level-list", levelreader);
      /** FIXME: Insert code here for parsing <level> tags */
    }
}

std::vector<std::string>
PLTXML::get_levels ()
{
  return level_list;
}

const std::string&
PLTXML::get_name ()
{
  return world_name;
}

const std::string&
PLTXML::get_description ()
{

  return description;
}


/* EOF */
