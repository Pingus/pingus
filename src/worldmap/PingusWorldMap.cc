//  $Id: PingusWorldMap.cc,v 1.39 2002/06/06 16:24:50 grumbel Exp $
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

#include "../PathManager.hh"
#include "../PingusError.hh"
#include "../System.hh"
#include "../globals.hh"
#include "../Display.hh"
#include "../TargetProvider.hh"
#include "../PingusResource.hh"
#include "../globals.hh"
#include "../algo.hh"
#include "../PingusGameSession.hh"
#include "../blitter.hh"
#include "../PingusSound.hh"
#include "../Console.hh"
#include "PingusWorldMap.hh"

using namespace Pingus::WorldMap;

WorldMap::WorldMap (std::string filename) :
  font (PingusResource::load_font ("Fonts/pingus_small", "fonts")),
  green_dot ("worldmap/dot_green", "core"),
  red_dot ("worldmap/dot_red", "core"),
  dot_border ("Game/dot_border", "game"),
  green_flag ("worldmap/flaggreen", "core"),
  catch_input (true),
  do_quit (false),
  last_node (0)
{
  green_flag.set_align (-24, -36);
  green_dot.set_align_center ();
  red_dot.set_align_center ();
  dot_border.set_align_center ();

  graph_data.parse_file (filename);

  background = PingusResource::load_surface (graph_data.get_background ());

  //background = Blitter::scale_surface (background, CL_Display::get_width (), CL_Display::get_height ());
  
  pingus = new PingusWorldMapPingus;

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
      
      pingus->set_position (*graph_data.nodes.begin ());
    }
  else
    {
      pingus->set_position (*graph_data.nodes.begin ());
    }
}

WorldMap::~WorldMap ()
{
  //delete graph;
  delete pingus;
}

void 
WorldMap::save ()
{
  std::cout << "PingusWorldMap:save()" << std::endl;
  stat->save (graph_data.nodes);
}

CL_Vector 
WorldMap::get_offset ()
{
  // FIXME: Handling of background smaller than screen isn't handled
  assert (pingus);

  CL_Vector offset = pingus->get_pos ();
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
WorldMap::init ()
{
  std::cout << "PingusWorldMap::init" << std::endl;
  if (!graph_data.get_music ().empty ())
    PingusSound:: play_music (path_manager.complete("music/" + graph_data.get_music ()));
}

void 
WorldMap::disable_button_events ()
{
  catch_input = false;
}
  
void 
WorldMap::enable_button_events ()
{
  catch_input = true;
}

void 
WorldMap::on_button_press (CL_InputDevice *device, const CL_Key &key)
{
  if (!catch_input) return;

  if (device == CL_Input::keyboards[0])
    {
      switch(key.id)
	{
	case CL_KEY_ESCAPE:
	  do_quit = true;
	  break;
	default:
	  break;
	}
    }
  else if (device == CL_Input::pointers[0])
    {
      CL_Vector offset = get_offset ();
      
      switch (key.id)
	{
	case CL_MOUSE_LEFTBUTTON:
	  {
	    NodePtr node
	      = get_node ((int) (key.x - offset.x), (int) (key.y - offset.y));

	    if (node.get() && !node->accessible)
	      {
		PingusSound::play_sound("sounds/chink.wav");
	      }
	    else if (node.get() && node->accessible)
	      {
		Pingus::WorldMap::Node* pingus_node = pingus->get_node ();
		if (maintainer_mode)
		  {
		    std::cout << "Click on: " << node->get_id () << std::endl;
		    std::cout << "Pingu at: " << pingus_node->get_id () << std::endl;
		  }

		if (pingus_node && pingus_node->get_id () == node->get_id ())
		  {
		    disable_button_events ();
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
      
		    enable_button_events ();
		  }
		else
		  {
		    pingus->walk_to (node.get ());
		  }
	      }
	    else
	      {
		if (maintainer_mode)
		  std::cout << "no id clicked" << std::endl;
	      }
	  }
	  break;
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
	  break;
	case CL_MOUSE_RIGHTBUTTON:
	  {
	    //Node* node = get_node (key.x - offset.x, key.y - offset.y);

	    /*if (node) {
	      std::cout << "Node: " << node->id << std::endl;
	      } else {
	      std::cout << "No node selected" << std::endl;
	      }*/
	  }
	  break;
	}
    }
}
 
void 
WorldMap::on_button_release (CL_InputDevice * /*device*/, const CL_Key & /*key*/)
{
  if (!catch_input) return;
}

void
WorldMap::start_level (Pingus::WorldMap::Node* /*node*/)
{

}

void
WorldMap::draw ()
{
  CL_Vector offset = get_offset ();
  
  if (offset.x > 0)
    CL_Display::clear_display ();

  background.put_screen ((int) offset.x, (int) offset.y);

  if (last_node.get () && last_node->accessible)
    {
      dot_border.put_screen (last_node->get_pos () + offset);
      
      font->print_center (CL_Display::get_width ()/2, CL_Display::get_height () - 40,
			  last_node->get_string().c_str ());

      /*
      if (last_node->finished)
	font->print_center (CL_Display::get_width ()/2, CL_Display::get_height () - 20,
			    "100%");
      else
	font->print_center (CL_Display::get_width ()/2, CL_Display::get_height () - 20,
			    "0%");
      */
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
 
  pingus->draw (offset);
}

void
WorldMap::update (float delta)
{
  pingus->update (delta);
}

boost::shared_ptr<Pingus::WorldMap::Node>
WorldMap::get_node (int x, int y)
{
  for (GraphIter i = graph_data.nodes.begin ();
       i != graph_data.nodes.end ();
       i++)
    {
      CL_Vector pos = (*i)->get_pos ();

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
       i++)
    {
      if ((*i)->get_id () == node_id)
	{
	  pingus->set_position (*i);
	  return;
	}
    }
}

bool
WorldMap::do_exit () 
{
  return do_quit; 
}

/* EOF */
