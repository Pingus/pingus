//  $Id$
//
//  Pingus - A free Lemmings clone
//  Copyright (C) 2002 Ingo Ruhnke <grumbel@gmx.de>
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

#include <vector>
#include <iostream>
#include <ClanLib/Core/IOData/inputsource_provider_file.h>
#include <ClanLib/Core/XML/dom_node.h>
#include <ClanLib/Core/XML/dom_node_list.h>
#include <ClanLib/Core/XML/dom_document.h>
#include <ClanLib/Core/XML/dom_element.h>
#include "pingus_error.hxx"
#include "xml_file_reader.hxx"
#include "pingus_level_impl.hxx"
#include "xml_pingus_level.hxx"

namespace Pingus {

XMLPingusLevel::XMLPingusLevel(const std::string& filename)
{
  CL_InputSourceProvider_File provider(".");
  CL_DomDocument doc(provider.open_source(filename), true);

  CL_DomElement root = doc.get_document_element();
  
  if (root.get_tag_name() != "pingus-level")
    {
      PingusError::raise("Error: " + filename + ": not a <pingus-level> file");
    }
  else
    {
      CL_DomNodeList lst = root.get_child_nodes();

      for(int i = 0; i < lst.get_length(); ++i)
        {
          CL_DomElement node = lst.item(i).to_element();

          if (node.get_tag_name() == "head")
            {
              XMLFileReader reader(node);
              reader.read_string("levelname",        impl->levelname);
              reader.read_string("description",      impl->description);
              reader.read_size  ("size",             impl->size);
              reader.read_string("music",            impl->music);
              reader.read_int   ("time",             impl->number_of_pingus);
              reader.read_int   ("difficulty",       impl->difficulty);
              reader.read_int   ("number-of-pingus", impl->number_of_pingus);
              reader.read_int   ("number-to-save",   impl->number_to_save);
              reader.read_string("author",           impl->author);
              
              /* FIXME:
                FileReader actions_reader;
                reader.read_section("actions", actions_reader);
              
                actions_reader.read_int()
              */
              impl->actions["basher"] = 5;
              impl->actions["jumper"] = 50;
            }
          else if (node.get_tag_name() == "objects")
            {
              CL_DomNodeList objects = node.get_child_nodes();
              for(int i = 0; i < lst.get_length(); ++i)
                {
                  impl->objects.push_back(XMLFileReader(objects.item(i).to_element()));
                }
            }
          else
            {
              std::cout << "Warning: Unknown element: " << node.get_tag_name() << std::endl;
            }
        }
    }
}

} // namespace Pingus

/* EOF */
