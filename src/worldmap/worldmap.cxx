//  $Id: worldmap.cxx,v 1.15 2002/10/01 19:53:45 grumbel Exp $
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
#include "worldmap.hxx"
#include "pingus.hxx"

namespace WorldMapNS {

#if TODO

WorldMap::WorldMap(const std::string& filename) 
{
  doc = xmlParseFile(filename.c_str());
  
  if (!doc) 
    {
      PingusError::raise (_("WorldMap: File not found: ") + filename);
    }    

  xmlNodePtr cur = doc->ROOT;
  cur = XMLhelper::skip_blank(cur);

  parse_file(cur);
}

void 
WorldMap::parse_file(xmlNodePtr cur)
{
  if (cur && XMLhelper::equal_str(cur->name, "pingus-worldmap"))
    {
      cur = cur->children;
      cur = XMLhelper::skip_blank(cur);
	  
      while (cur)
	{
	  if (XMLhelper::equal_str(cur->name, "graph"))
	    {
	      parse_graph(cur);
	    }
	  else if (XMLhelper::equal_str(cur->name, "objects"))
	    {
	      parse_objects(cur);
	    }
	  else if (XMLhelper::equal_str(cur->name, "properties"))
	    {
	      parse_properties(cur);
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
      PingusError::raise ("WorldMap:" + filename + ": not a Worldmap file");
    }
}

void
WorldMap::parse_objects(xmlNodePtr cur)
{
  cur = cur->children;
  cur = XMLhelper::skip_blank(cur);


}

void
WorldMap::parse_graph(xmlNodePtr cur)
{
  cur = cur->children;
  cur = XMLhelper::skip_blank(cur);


}

void
WorldMap::parse_properties(xmlNodePtr cur)
{
  cur = cur->children;
  cur = XMLhelper::skip_blank(cur);

  
}

void
WorldMap::draw (GraphicContext& gc)
{
  for (iterator drawables.begin (); i != drawables.end (); ++i)
    {
      i->draw (gc);
    }
}

void
WorldMap::update (float delta)
{
  for (iterator drawables.begin (); i != drawables.end (); ++i)
    {
      i->update (delta);
    }
}

#endif

WorldMap::WorldMap (std::string filename) 
  : green_dot ("worldmap/dot_green", "core"),
    red_dot ("worldmap/dot_red", "core"),
    dot_border ("Game/dot_border", "game"),
    green_flag ("worldmap/flaggreen", "core"),
    last_node (0)
{
  green_flag.set_align (-24, -36);
  green_dot.set_align_center ();
  red_dot.set_align_center ();
  dot_border.set_align_center ();

  graph_data.parse_file (filename);

  background = PingusResource::load_surface (graph_data.get_background ());
  
  pingu = new Pingus ();

  stat = boost::shared_ptr<PingusWorldMapStat>
    (new PingusWorldMapStat (System::basename(filename)));
  
  if (!stat->empty ())
    {
      for (GraphIter i = graph_data.nodes.begin ();
	   i != graph_data.nodes.end ();
	   ++i)
	{
	  (*i)->mark(stat->finished ((*i)->get_id ()));
      
	  if (!(*i)->accessible)
	    (*i)->accessible = stat->accessible ((*i)->get_id ());
	}
      
      pingu->set_position (*graph_data.nodes.begin ());
    }
  else
    {
      pingu->set_position (*graph_data.nodes.begin ());
    }
}

WorldMap::~WorldMap ()
{
  //delete graph;
  delete pingu;
}

void 
WorldMap::save ()
{
  std::cout << "PingusWorldMap:save()" << std::endl;
  stat->save (graph_data.nodes);
}

Vector 
WorldMap::get_offset ()
{
  // FIXME: Handling of background smaller than screen isn't handled
  assert (pingu);

  Vector offset = pingu->get_pos ();
  offset *= -1.0;

  if (CL_Display::get_width () <= int(background.get_width ()))
    {
      offset.x += float(CL_Display::get_width ())/2;
      // When offset is larger then zero the background wouldn't fill the
      // complet screen, so we reset.
      if (offset.x > 0) offset.x = 0;
      if (offset.x < float(CL_Display::get_width ()) - background.get_width ())
	offset.x = float(CL_Display::get_width ()) - background.get_width ();
    } 
  else
    {
      offset.x = (float(CL_Display::get_width ()) - background.get_width ()) / 2.0f;
    }

  if (CL_Display::get_height () <= int(background.get_height ()))
    {
      offset.y += float(CL_Display::get_height ())/2;
      if (offset.y > 0) offset.y = 0;
      if (offset.y < float(CL_Display::get_height ()) - background.get_height ())
	offset.y = float(CL_Display::get_height ()) - background.get_height ();
    }
  else
    {
      offset.y = (float(CL_Display::get_height ()) - background.get_height ()) / 2.0f;
    }
  
  return offset;
}

void
WorldMap::on_startup ()
{
  std::cout << "PingusWorldMap::init" << std::endl;
  if (!graph_data.get_music ().empty ())
    PingusSound:: play_music (path_manager.complete("music/" + graph_data.get_music ()));
}

void 
WorldMap::on_primary_button_press (int x, int y)
{
  Vector offset = get_offset ();

  {      
    NodePtr node
      = get_node ((int) (x - offset.x), (int) (y - offset.y));

    if (node.get() && !node->accessible)
      {
	PingusSound::play_sound("sounds/chink.wav");
      }
    else if (node.get() && node->accessible)
      {
	Node* pingus_node = pingu->get_node ();
	if (maintainer_mode)
	  {
	    std::cout << "Click on: " << node->get_id () << std::endl;
	    std::cout << "Pingu at: " << pingus_node->get_id () << std::endl;
	  }

	if (pingus_node && pingus_node->get_id () == node->get_id ())
	  {
	    node->on_click ();
		    
	    // FIXME: Ugly marking code... should be rewritten
	    for (std::list<int>::iterator k = node->get_links ().begin();
		 k != node->get_links ().end();
		 ++k)
	      {
		for (GraphIter i = graph_data.nodes.begin ();
		     i != graph_data.nodes.end ();
		     ++i)
		  {
		    if ((*i)->get_id () == *k)
		      (*i)->accessible = true;
		  }
	      }
		    
	    // Save the changes
	    save ();
      	  }
	else
	  {
	    pingu->walk_to (node.get ());
	  }
      }
    else
      {
	if (maintainer_mode)
	  std::cout << "no id clicked" << std::endl;
      }
  }


  /** 
      case CL_MOUSE_MIDDLEBUTTON:
      {
      if (maintainer_mode)
      {
      std::cout << "<position>" << std::endl;
      std::cout << "  <x-pos>" << key.x - offset.x << "</x-pos>" << std::endl;
      std::cout << "  <y-pos>" << key.y - offset.y << "</y-pos>" << std::endl;
      std::cout << "</position>" << std::endl;
      }
      }
      break;*/
}
 
void
WorldMap::draw (GraphicContext& gc)
{
  Vector offset = get_offset ();
  
  if (offset.x > 0)
    CL_Display::clear_display ();

  background.put_screen ((int) offset.x, (int) offset.y);

  if (last_node.get () && last_node->accessible)
    {
      dot_border.put_screen (last_node->get_pos () + offset);
      
      gc.print_center (Fonts::pingus_small, 
		       CL_Display::get_width ()/2, CL_Display::get_height () - 40,
		       last_node->get_string().c_str ());
    }

  graph_data.draw(offset);

  for (GraphIter i = graph_data.nodes.begin ();
       i != graph_data.nodes.end ();
       ++i)
    {
      (*i)->draw (get_offset ());
    }

  NodePtr node
    = get_node (CL_Mouse::get_x () - (int) offset.x,
		CL_Mouse::get_y () - (int) offset.y);

  // The mouse is over a node
  if (node.get ())
    {
      last_node = node;
      last_node_time = CL_System::get_time ();
    }
  else
    {
      if (last_node_time + 300 < CL_System::get_time ())
	{
	  last_node = NodePtr(0);
	}
    }
 
  pingu->draw (offset);
  UNUSED_ARG(gc);
}

void
WorldMap::update (float delta)
{
  pingu->update (delta);
}

boost::shared_ptr<Node>
WorldMap::get_node (int x, int y)
{
  for (GraphIter i = graph_data.nodes.begin ();
       i != graph_data.nodes.end ();
       i++)
    {
      Vector pos = (*i)->get_pos ();

      if (   pos.x - (int)(red_dot.get_width()/2) - 3 < x
	     && pos.x + (int)(red_dot.get_width()/2) + 3 > x
	     && pos.y - (int)(red_dot.get_width()/2) - 3 < y
	     && pos.y + (int)(red_dot.get_width()/2) + 3 > y)
	{
	  if (!(*i)->get_string ().empty ())
	    return *i;
	}
    }
  return NodePtr(0);
}

void 
WorldMap::set_pingus (int node_id)
{
  for (GraphIter i = graph_data.nodes.begin ();
       i != graph_data.nodes.end ();
       ++i)
    {
      if ((*i)->get_id () == node_id)
	{
	  pingu->set_position (*i);
	  return;
	}
    }
}

} // namespace WorldMapNS

/* EOF */
