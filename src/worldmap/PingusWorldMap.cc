//  $Id: PingusWorldMap.cc,v 1.17 2001/04/06 13:21:44 grumbel Exp $
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

PingusWorldMap::PingusWorldMap (std::string filename)
  : green_flag ("worldmap/flaggreen", "core")
{
  green_flag.set_align (-24, -36);
  green_dot  = PingusResource::load_surface ("Game/dot_green", "game");
  red_dot    = PingusResource::load_surface ("Game/dot_red", "game");
  dot_border = PingusResource::load_surface ("Game/dot_border", "game");

  graph_data.parse_file (filename);

  background = PingusResource::load_surface (graph_data.get_background ());

  background = Blitter::scale_surface (background, CL_Display::get_width (), CL_Display::get_height ());
  
  pingus = new PingusWorldMapPingus;
  pingus->set_position (&(*graph_data.nodes.begin ()));
}

PingusWorldMap::~PingusWorldMap ()
{
  delete graph;
  delete pingus;
}

void
PingusWorldMap::init ()
{
  std::cout << "PingusWorldMap::init" << std::endl;
  if (!graph_data.get_music ().empty ())
    PingusSound:: play_mod ("music/" + graph_data.get_music ());
}

void 
PingusWorldMap::on_button_press (CL_InputDevice *device, const CL_Key &key)
{
  if (device == CL_Input::pointers[0])
    {
      if (key.id == CL_MOUSE_LEFTBUTTON)
	{
	  PingusWorldMapNode* node = get_node (key.x, key.y);
	  //PingusGame game;
	  //game.start_game(find_file(pingus_datadir, "levels/" + i->levelname));

	  if (node && !node->accessible)
	    {
	      PingusSound::play_wav("chink");
	    }
	  else if (node && node->accessible)
	    {
	      PingusWorldMapNode* pingus_node = pingus->get_node ();
	      if (maintainer_mode)
		{
		  std::cout << "Click on: " << node->id << std::endl;
		  std::cout << "Pingu at: " << pingus_node->id << std::endl;
		}

	      if (pingus_node && pingus_node->id == node->id)
		{
		  start_level (pingus_node);
		}
	      else
		{
		  pingus->walk_to (node);
		}
	    }
	  else
	    {
	      if (maintainer_mode)
		std::cout << "no id clicked" << std::endl;
	    }
      	}
      else 
	{
	  if (maintainer_mode)
	    {
	      std::cout << "<position>" << std::endl;
	      std::cout << "  <x-pos>" << key.x << "</x-pos>" << std::endl;
	      std::cout << "  <y-pos>" << key.y << "</y-pos>" << std::endl;
	      std::cout << "</position>" << std::endl;
	    }
	}
    }
}
 
void 
PingusWorldMap::on_button_release (CL_InputDevice *device, const CL_Key &key)
{
}

void
PingusWorldMap::start_level (PingusWorldMapNode* node)
{
  assert (node);
  
  if (maintainer_mode) 
    std::cout << "Start a level...: " << node->levelname << std::endl;
		  
  PingusSound::play_wav("letsgo");

  CL_Target* target = CL_Display::get_target ();
		  
  if (!target)
    {
      console << "PingusWorldMap: Couldn't get CL_Target" << std::endl;
    }
  else
    {
      /* Fade out, fixme, doesn't work at the moment */
      CL_SurfaceProvider* provider = new TargetProvider (target);
      CL_Surface* sur = CL_Surface::create (provider);

      for (int y = 0; y < CL_Display::get_height(); 
	   y += CL_Display::get_height() / 40)
	{
	  CL_System::keep_alive ();
	  CL_Display::clear_display ();
	  sur->put_screen (0, y);
	  Display::flip_display ();
	}
		      
      delete sur;
      delete provider;

      PingusGameSession game (node->levelname);
      game.start ();
      if (game.get_results ().finished ())
	{
	  node->finished = true;
	  
	  for (list<int>::iterator k = node->links.begin();
	       k != node->links.end();
	       ++k)
	    {
	      for (list<PingusWorldMapNode>::iterator i = graph_data.nodes.begin ();
		   i != graph_data.nodes.end ();
		   ++i)
		{
		  if (i->id == *k)
		    i->accessible = true;
		}
	    }
	}
      else
	{
	  console.puts("Please try again!");
	}
    }
}

void
PingusWorldMap::draw ()
{
  // The node positions are based up on a 800x600 screen, so we need
  // to scale if the screen has another size
  float x_scale = CL_Display::get_width () / 800.0;
  float y_scale = CL_Display::get_height () / 600.0;

  //background->put_screen (0,0, CL_Display::get_width (), CL_Display::get_height ());
  background.put_screen (0,0);

  graph_data.draw();

  for (list<PingusWorldMapNode>::iterator i = graph_data.nodes.begin ();
       i != graph_data.nodes.end ();
       i++)
    {
      if (!i->levelname.empty())
	{
	  if (i->accessible) 
	    {
	      green_dot.put_screen ((i->pos.x - (red_dot.get_width()/2)) * x_scale,
				    (i->pos.y - (red_dot.get_height()/2)) * y_scale);
	      if (i->finished) {
		green_flag.put_screen (i->pos.x * x_scale,
				       i->pos.y * y_scale);
	      }
	    }
	  else
	    {
	      red_dot.put_screen ((i->pos.x - (red_dot.get_width()/2)) * x_scale,
				  (i->pos.y - (red_dot.get_height()/2)) * y_scale);
	    }
	}
    }
  
  PingusWorldMapNode* node = get_node (CL_Mouse::get_x (), CL_Mouse::get_y ());

  if (node)
    {
      dot_border.put_screen ((node->pos.x - (dot_border.get_width()/2)) * x_scale,
			     (node->pos.y - (dot_border.get_height()/2)) * y_scale);
    }

  pingus->draw ();
}

void
PingusWorldMap::update (float delta)
{
  pingus->update (delta);
}

PingusWorldMapNode* 
PingusWorldMap::get_node (int x, int y)
{
  float x_scale = 800.0 / CL_Display::get_width ();
  float y_scale = 600.0 / CL_Display::get_height ();

  for (list<PingusWorldMapNode>::iterator i = graph_data.nodes.begin ();
       i != graph_data.nodes.end ();
       i++)
    if (i->pos.x - (int)(red_dot.get_width()/2) - 3 < x * x_scale
	&& i->pos.x + (int)(red_dot.get_width()/2) + 3 > x * x_scale
	&& i->pos.y - (int)(red_dot.get_width()/2) - 3 < y * y_scale
	&& i->pos.y + (int)(red_dot.get_width()/2) + 3 > y * y_scale)
      {
	if (!i->levelname.empty ())
	  return &(*i);
      }
  return 0;
}

/* EOF */
