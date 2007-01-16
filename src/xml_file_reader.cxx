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

#include <iostream>
#include <ClanLib/Display/color.h>
#include <ClanLib/Core/Math/size.h>
#include <ClanLib/Core/XML/dom_element.h>
#include <ClanLib/Core/XML/dom_node_list.h>
#include <ClanLib/Core/XML/dom_named_node_map.h>
#include <ClanLib/Core/System/clanstring.h>
#include "file_reader_impl.hxx"
#include "vector.hxx"
#include "res_descriptor.hxx"
#include "resource_modifier.hxx"
////#include "xml_file_reader.hxx"


class XMLFileReaderImpl : public FileReaderImpl
{
private:
  CL_DomElement  root;
  CL_DomNodeList childs;

  CL_DomElement get_node_by_name(const char* name) const
  {
    for(int i = 0; i < childs.get_length(); ++i)
      {
        if (childs.item(i).is_element())
          {
            if (childs.item(i).to_element().get_tag_name() == name)
              {
                return childs.item(i).to_element();
              }
          }
      }
    return CL_DomElement();
  }
public:

  XMLFileReaderImpl(CL_DomElement element) 
    : root(element),
      childs(root.get_child_nodes())
  { 
  }

  std::string get_name() const
  {
    return root.get_tag_name();
  }

  bool read_int(const char* name, int& value) const
  {
    CL_DomElement node = get_node_by_name(name);
    return !node.is_null() && CL_String::from(node.get_first_child().get_node_value(), value);
  }
  
  bool read_float (const char* name, float& value) const
  {
    CL_DomElement node = get_node_by_name(name);
    return !node.is_null() && CL_String::from(node.get_first_child().get_node_value(), value);
  }

  bool read_bool  (const char* name, bool& value) const
  {
    CL_DomElement node = get_node_by_name(name);
    return !node.is_null() && CL_String::from(node.get_first_child().get_node_value(), value);
  }

  bool read_string(const char* name, std::string& value) const
  {
    CL_DomElement node = get_node_by_name(name);
    if (!node.is_null())
      {
        value = node.get_first_child().get_node_value();
        return true;
      }
    else
      {
        return false;
      }
  }

  bool read_vector(const char* name, Vector& value) const
  {
    CL_DomElement node = get_node_by_name(name);
    if (node.is_element())
      {
        XMLFileReader reader(node.to_element());
        reader.read_float("x", value.x);
        reader.read_float("y", value.y);
        reader.read_float("z", value.z);
        return true;
      }
    else
      {
        return false;
      }
  }

  bool read_color(const char* name, Colorf& value) const
  {
    CL_DomElement node = get_node_by_name(name);
    if (node.is_element())
      {
        XMLFileReader reader(node.to_element());
        reader.read_float("red",   value.red);
        reader.read_float("green", value.green);
        reader.read_float("blue",  value.blue);
        reader.read_float("alpha", value.alpha);
        return true;
      }
    else
      {
        return false;
      }
  }

  bool read_size(const char* name, CL_Size& value) const
  {
    CL_DomElement node = get_node_by_name(name);
    if (node.is_element())
      {
        XMLFileReader reader(node.to_element());
        reader.read_int("width",  value.width);
        reader.read_int("height", value.height);
        return true;
      }
    else
      {
        return false;
      }   
  }

  bool read_desc(const char* name, ResDescriptor& value) const
  {
    CL_DomElement node = get_node_by_name(name);
    if (!node.is_null())
      {
        using ResourceModifierNS::rs_from_string;

        XMLFileReader reader(node);
        reader.read_string("image",    value.res_name);
        reader.read_enum  ("modifier", value.modifier, rs_from_string);

        return true;
      }
    else
      {
        return false;
      }
   
  }

  bool read_section(const char* name, FileReader& value) const
  {
    CL_DomElement node = get_node_by_name(name);
    if (!node.is_null())
      {
        value = XMLFileReader(node);
        return true;
      }
    else
      {
        return false;
      }
  }

  std::vector<FileReader> get_sections() const
  {
    std::vector<FileReader> lst;
    for(int i = 0; i < childs.get_length(); ++i)
      {
        if (childs.item(i).is_element())
          lst.push_back(XMLFileReader(childs.item(i).to_element()));
      }
    return lst;
  }

  std::vector<std::string> get_section_names() const
  {
    std::vector<std::string> lst;
    for(int i = 0; i < childs.get_length(); ++i)
      {
        if (childs.item(i).is_element())
          lst.push_back(childs.item(i).to_element().get_tag_name());
      }
    return lst;
  }
};

XMLFileReader::XMLFileReader(CL_DomElement element)
  : FileReader(SharedPtr<FileReaderImpl>(new XMLFileReaderImpl(element)))
{
}


/* EOF */
