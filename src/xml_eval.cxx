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
#include <ClanLib/core.h>
#include "xml_eval.hxx"
#include "string_converter.hxx"

namespace XMLEval {

std::map<std::string, int> variables;
std::map<std::string, CL_DomNode> functions;

int lookup(const std::string& name)
{
  int value = 0;
  
  if (from_string(name, value))
    {
      return value;
    }
  else
    {
      std::map<std::string, int>::iterator i = variables.find(name);

      if (i != variables.end())
        {
          return i->second;
        }
      std::cout << "Error: No variable named: '" << name << "'" << std::endl;
      return 0;
    }
}

void eval_block(CL_DomNode child)
{
  while(!child.is_null())
    { 
      if (child.is_element())
        eval(child);
                  
      child = child.get_next_sibling();
    }
}

void eval(const CL_DomNode& cur)
{
  CL_DomElement el = cur.to_element();

  if (cur.is_element())
    {
      if (cur.get_node_name() == "for")
        {         
          std::string var = el.get_attribute("name");

          for(int i = lookup(el.get_attribute("start")); 
              i <= lookup(el.get_attribute("end")); ++i)
            {
              variables[var] = i;
              eval_block(cur.get_first_child());
            }
        }
      else if (cur.get_node_name() == "newline")
        {
          std::cout << std::endl;
        }
      else if (cur.get_node_name() == "printvar")
        {
          std::cout << lookup(el.get_attribute("var")) << std::flush;          
        }
      else if (cur.get_node_name() == "print")
        {
          std::cout << el.get_attribute("string") << std::flush;
        }
      else if (cur.get_node_name() == "foobar")
        {
          std::cout << "Foobar" << std::endl;
        }
      else if (cur.get_node_name() == "block")
        {
          eval_block(cur.get_first_child());
        }
      else if (cur.get_node_name() == "set")
        {
          variables[el.get_attribute("name")] = lookup(el.get_attribute("var"));
        }
      else if (cur.get_node_name() == "function")
        {
          functions[el.get_attribute("name")] = el.get_first_child();
        }
      else if (cur.get_node_name() == "modulo")
        {
          variables[el.get_attribute("name")] 
            = lookup(el.get_attribute("name")) % lookup(el.get_attribute("var"));
        }
      else if (cur.get_node_name() == "add")
        {
          variables[el.get_attribute("name")] 
            = lookup(el.get_attribute("name")) + lookup(el.get_attribute("var"));
        }
      else if (cur.get_node_name() == "if-non-zero")
        {
          int var = lookup(el.get_attribute("var"));
          if (var != 0)
            eval_block(cur.get_first_child());
        }
      else if (cur.get_node_name() == "if-zero")
        {
          int var = lookup(el.get_attribute("var"));
          if (var == 0)
            eval_block(cur.get_first_child());
        }
      else if (cur.get_node_name() == "mult")
        {
          variables[el.get_attribute("name")]
            = lookup(el.get_attribute("name")) * lookup(el.get_attribute("val"));
        }
      else if (cur.get_node_name() == "funcall")
        {
          eval_block(functions[el.get_attribute("name")]);
        }
      else
        {
          std::cout << "Unknown command: " << cur.get_node_name() << std::endl;
        }
    }
  else
    {
      std::cout << "Unknown thingy" << std::endl;
    }
}

} // namespace XMLEval

int main(int argc, char** argv)
{
  try {
  CL_SetupCore::init();

  std::string filename = "test.xml";

  if (argc > 1)
    filename = argv[1];

  CL_DomDocument dom(new CL_InputSource_File(filename), true);

  CL_DomNode cur = dom.get_document_element();
  
  XMLEval::eval(cur);

  CL_SetupCore::deinit();
  } catch (CL_Error& err) {
    std::cout << "CL_Error: " << err.message << std::endl;
  }
  return 0;
}

/* EOF */
