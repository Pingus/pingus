//  $Id: PingusWorldMap.cc,v 1.4 2000/09/21 15:23:57 grumbel Exp $
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

#include "../PingusResource.hh"
#include "../globals.hh"
#include "../algo.hh"
#include "../PingusGame.hh"
#include "PingusWorldMap.hh"

PingusWorldMap::PingusWorldMap (std::string filename)
{
  green_dot  = PingusResource::load_surface ("Game/dot_green", "game");
  red_dot    = PingusResource::load_surface ("Game/dot_red", "game");

  graph_data.parse_file (filename);

  background = PingusResource::load_surface (graph_data.get_background ());

  pingus = new PingusWorldMapPingus;

  pingus->set_position (*graph_data.nodes.begin ());
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
	  for (list<PingusWorldMapNode>::iterator i = graph_data.nodes.begin ();
	       i != graph_data.nodes.end ();
	       i++)
	    if (i->pos.x_pos - (red_dot->get_width()/2) - 3 < key.x
		&& i->pos.x_pos + (red_dot->get_width()/2) + 3 > key.x
		&& i->pos.y_pos - (red_dot->get_width()/2) - 3 < key.y
		&& i->pos.y_pos + (red_dot->get_width()/2) + 3 > key.y)
	      {
		std::cout << "Click on: " << i->id << std::endl;
		//PingusGame game;
		//game.start_game(find_file(pingus_datadir, "levels/" + i->levelname));
		pingus->walk_to (*i);
		return;
	      }
	  std::cout << "no id clicked" << std::endl;
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
    red_dot->put_screen (i->pos.x_pos - (red_dot->get_width()/2),
			 i->pos.y_pos - (red_dot->get_height()/2));

  pingus->draw ();
}

void
PingusWorldMap::let_move ()
{
  pingus->let_move ();
}

/* EOF */
