//  $Id: worldmap.cxx,v 1.24 2002/10/17 16:06:21 grumbel Exp $
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

#include <assert.h>
#include <iostream>
#include <ClanLib/Core/System/system.h>
#include <ClanLib/Display/Display/display.h>
#include <ClanLib/Display/Input/mouse.h>
#include "../fonts.hxx"
#include "../path_manager.hxx"
#include "../system.hxx"
#include "../pingus_resource.hxx"
#include "../globals.hxx"
#include "../sound.hxx"
#include "../xml_helper.hxx"
#include "../pingus_error.hxx"
#include "../my_gettext.hxx"
#include "pingus.hxx"
#include "../globals.hxx"
#include "../debug.hxx"
#include "worldmap.hxx"
#include "drawable_factory.hxx"
#include "drawable.hxx"
#include "dot.hxx"
#include "path_graph.hxx"

namespace WorldMapNS {

struct z_pos_sorter
{
  bool operator()(Drawable* a, Drawable* b)
  {
    return a->get_z_pos() < b->get_z_pos();
  }
};

WorldMap::WorldMap(const std::string& arg_filename) 
  : filename(arg_filename)
{
  xmlDocPtr doc = xmlParseFile(filename.c_str());
  
  if (!doc) 
    {
      PingusError::raise (_("WorldMap: File not found: ") + filename);
    }    

  xmlNodePtr cur = doc->ROOT;
  cur = XMLhelper::skip_blank(cur);

  parse_file(doc, cur);

  pingus = new Pingus(path_graph);
  pingus->set_position(path_graph->lookup_node("leveldot_3"));

  add_drawable(pingus);
}

WorldMap::~WorldMap()
{
  //delete pingus;
  //delete path_graph;
}

void 
WorldMap::parse_file(xmlDocPtr doc, xmlNodePtr cur)
{
  if (cur && XMLhelper::equal_str(cur->name, "pingus-worldmap"))
    {
      cur = cur->children;
      cur = XMLhelper::skip_blank(cur);
	  
      while (cur)
	{
	  if (XMLhelper::equal_str(cur->name, "graph"))
	    {
	      parse_graph(doc, cur);
	    }
	  else if (XMLhelper::equal_str(cur->name, "objects"))
	    {
	      parse_objects(doc, cur);
	    }
	  else if (XMLhelper::equal_str(cur->name, "properties"))
	    {
	      parse_properties(doc, cur);
	    }
	  else
	    {
	      perr(PINGUS_DEBUG_WORLDMAP) << "WorldMap: Unknown node name: " << cur->name << std::endl;
	    }

	  cur = cur->next;
	  cur = XMLhelper::skip_blank(cur);
	}
    }
  else
    {
      PingusError::raise("WorldMap:" + filename + ": not a Worldmap file");
    }

  if (!path_graph)
    {
      PingusError::raise("WorldMap: " + filename + " missed Graph");
    }
}

void
WorldMap::parse_objects(xmlDocPtr doc, xmlNodePtr cur)
{
  cur = cur->children;
  cur = XMLhelper::skip_blank(cur);

  while (cur)
    {
      Drawable* drawable = DrawableFactory::create(doc, cur);

      if (drawable)
        {
          objects.push_back(drawable);
          drawables.push_back(drawable);
        }
      else
        {
          std::cout << "WorldMap::parse_objects: Parse Error" << std::endl;
        }
      
      cur = cur->next;
      cur = XMLhelper::skip_blank(cur);
    }      
}

void
WorldMap::parse_graph(xmlDocPtr doc, xmlNodePtr cur)
{
  path_graph = new PathGraph(this, doc, cur);
}

void
WorldMap::parse_properties(xmlDocPtr doc, xmlNodePtr cur)
{
  cur = cur->children;
  cur = XMLhelper::skip_blank(cur);

  UNUSED_ARG(doc);
}

void
WorldMap::draw (GraphicContext& gc)
{
  std::stable_sort(drawables.begin(), drawables.end(), z_pos_sorter());

  for (DrawableLst::iterator i = drawables.begin (); i != drawables.end (); ++i)
    {
      (*i)->draw (gc);
    }
}

void
WorldMap::update ()
{
  for (DrawableLst::iterator i = drawables.begin (); i != drawables.end (); ++i)
    {
      (*i)->update ();
    }
}

void
WorldMap::add_drawable(Drawable* drawable)
{
  drawables.push_back(drawable);
}

void
WorldMap::remove_drawable(Drawable* drawable)
{
  UNUSED_ARG(drawable);
}

void
WorldMap::set_pingus(NodeId id)
{
  UNUSED_ARG(id);
}

void
WorldMap::on_primary_button_press(int x, int y)
{
  std::cout << "<position>\n"
            << "  <x-pos>" << x << "</x-pos>\n"
            << "  <y-pos>" << y << "</y-pos>\n"
            << "  <z-pos>0</z-pos>\n"
            << "</position>\n\n" << std::endl;

  Dot* dot = path_graph->get_dot(x, y);
  if (dot)
    {
      std::cout << "Clicked on: " << dot->get_name() << std::endl;
      if (path_graph->lookup_node(dot->get_name()) == pingus->get_node())
        {
          std::cout << "Pingu is on node, issue on_click()" << std::endl;
          dot->on_click();
        }
      else
        {
          if (!pingus->walk_to_node(path_graph->lookup_node(dot->get_name())))
            {
              std::cout << "NO PATH TO NODE FOUND!" << std::endl;
            }
        }
    }
}

} // namespace WorldMapNS

/* EOF */
