//  $Id: PingusWorldMap.cc,v 1.5 2000/09/25 16:29:43 grumbel Exp $
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

#include "../Display.hh"
#include "../TargetProvider.hh"
#include "../PingusResource.hh"
#include "../globals.hh"
#include "../algo.hh"
#include "../PingusGame.hh"
#include "PingusWorldMap.hh"

PingusWorldMap::PingusWorldMap (std::string filename)
{
  green_dot  = PingusResource::load_surface ("Game/dot_green", "game");
  red_dot    = PingusResource::load_surface ("Game/dot_red", "game");
  dot_border = PingusResource::load_surface ("Game/dot_border", "game");

  graph_data.parse_file (filename);

  background = PingusResource::load_surface (graph_data.get_background ());

  pingus = new PingusWorldMapPingus;

  pingus->set_position (&(*graph_data.nodes.begin ()));
}

PingusWorldMap::~PingusWorldMap ()
{
  delete graph;
  delete pingus;
}

void 
PingusWorldMap::on_button_press (CL_InputDevice *device, const CL_Key &key)
{
  if (device == CL_Input::pointers[0])
    {
      if (key.id == 0)
	{
	  PingusWorldMapNode* node = get_node (key.x, key.y);
	  //PingusGame game;
	  //game.start_game(find_file(pingus_datadir, "levels/" + i->levelname));
	  if (node)
	    {
	      PingusWorldMapNode* pingus_node = pingus->get_node ();
	      std::cout << "Click on: " << node->id << std::endl;

	      if (pingus_node && pingus_node->id == node->id)
		{
		  std::cout << "Start a level...: " << node->levelname << std::endl;
		  
		  CL_Target* target = CL_Display::get_target ();
		  if (target)
		    {
		      std::cout << "red:   " << target->get_red_mask () << std::endl;
		      std::cout << "green: " << target->get_green_mask () << std::endl;
		      std::cout << "blue:  " << target->get_blue_mask () << std::endl;
		      std::cout << "alpha: " << target->get_alpha_mask () << std::endl;

		      CL_SurfaceProvider* provider = new TargetProvider (target);
		      CL_Canvas* canvas = new CL_Canvas (target->get_width (), target->get_height ());
		      CL_Surface* sur = CL_Surface::create (provider);

		      sur->put_target (0, 0, 0, canvas);
		      
		      CL_Surface* sur2 = CL_Surface::create (canvas);

		      for (int y = 0; y < CL_Display::get_width(); 
			   y += CL_Display::get_width() / 40)
			{
			  CL_System::keep_alive ();
			  // CL_Display::clear_display ();
			  sur->put_screen (0, y);
			  sur2->put_screen (0, 0);
			  Display::flip_display ();
			}
		    }		  
		}
	      else
		{
		  pingus->walk_to (node);
		}
	    }
	  else
	    {
	      std::cout << "no id clicked" << std::endl;
	    }
      	}
      else 
	{
	  std::cout << "<position>" << std::endl;
	  std::cout << "  <x-pos>" << key.x << "</x-pos>" << std::endl;
	  std::cout << "  <y-pos>" << key.y << "</y-pos>" << std::endl;
	  std::cout << "</position>" << std::endl;
	}
    }
}
 
void 
PingusWorldMap::on_button_release (CL_InputDevice *device, const CL_Key &key)
{
}

void
PingusWorldMap::draw ()
{
  background->put_screen (0,0, CL_Display::get_width (), CL_Display::get_height ());

  graph_data.draw();

  for (list<PingusWorldMapNode>::iterator i = graph_data.nodes.begin ();
       i != graph_data.nodes.end ();
       i++)
    {
      if (!i->levelname.empty())
	{
	  if (i->accessible) 
	    {
	      green_dot->put_screen (i->pos.x_pos - (red_dot->get_width()/2),
				     i->pos.y_pos - (red_dot->get_height()/2));
	    }
	  else
	    {
	      red_dot->put_screen (i->pos.x_pos - (red_dot->get_width()/2),
				   i->pos.y_pos - (red_dot->get_height()/2));
	    }
	}
    }
  
  PingusWorldMapNode* node = get_node (CL_Mouse::get_x (), CL_Mouse::get_y ());

  if (node)
    {
      dot_border->put_screen (node->pos.x_pos - (dot_border->get_width()/2),
			      node->pos.y_pos - (dot_border->get_height()/2));
    }

  pingus->draw ();
}

void
PingusWorldMap::let_move ()
{
  pingus->let_move ();
}

PingusWorldMapNode* 
PingusWorldMap::get_node (int x, int y)
{
  for (list<PingusWorldMapNode>::iterator i = graph_data.nodes.begin ();
       i != graph_data.nodes.end ();
       i++)
    if (i->pos.x_pos - (red_dot->get_width()/2) - 3 < x
	&& i->pos.x_pos + (red_dot->get_width()/2) + 3 > x
	&& i->pos.y_pos - (red_dot->get_width()/2) - 3 < y
	&& i->pos.y_pos + (red_dot->get_width()/2) + 3 > y)
      {
	if (!i->levelname.empty ())
	  return &(*i);
      }
  return 0;
}

/* EOF */
