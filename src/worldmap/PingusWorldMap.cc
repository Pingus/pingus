//  $Id: PingusWorldMap.cc,v 1.3 2000/09/20 14:31:11 grumbel Exp $
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
}

PingusWorldMap::~PingusWorldMap ()
{
  delete graph;
}

void 
PingusWorldMap::on_button_press (CL_InputDevice *device, const CL_Key &key)
{
  for (list<PingusWorldMapNode>::iterator i = graph_data.nodes.begin ();
       i != graph_data.nodes.end ();
       i++)
    if (i->pos.x_pos - (red_dot->get_width()/2) < key.x
	&& i->pos.x_pos + (red_dot->get_width()/2) > key.x
	&& i->pos.y_pos - (red_dot->get_height()/2) < key.y
	&& i->pos.y_pos + (red_dot->get_height()/2) > key.y)
      {
	std::cout << "Click on: " << i->id << std::endl;
	PingusGame game;
	game.start_game(find_file(pingus_datadir, "levels/" + i->levelname));
	return;
      }
  std::cout << "no id clicked" << std::endl;
}
 
void 
PingusWorldMap::on_button_release (CL_InputDevice *device, const CL_Key &key)
{
}

void
PingusWorldMap::draw ()
{
  background->put_screen (0,0, CL_Display::get_width (), CL_Display::get_height ());

  for (list<PingusWorldMapNode>::iterator i = graph_data.nodes.begin ();
       i != graph_data.nodes.end ();
       i++)
    red_dot->put_screen (i->pos.x_pos - (red_dot->get_width()/2),
			 i->pos.y_pos - (red_dot->get_height()/2));
}

void
PingusWorldMap::let_move ()
{
  
}

/* EOF */
