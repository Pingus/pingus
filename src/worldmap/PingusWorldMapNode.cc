//  $Id: PingusWorldMapNode.cc,v 1.1 2002/06/04 21:23:42 grumbel Exp $
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

#include "PingusWorldMapManager.hh"
#include "PingusWorldMapNode.hh"
#include "../my_gettext.hh"
#include "../PathManager.hh"
#include "../System.hh"
#include "../PingusGameSession.hh"
#include "../Console.hh"
#include "../XMLPLF.hh"
#include "../PingusError.hh"
#include "PingusWorldMapNode.hh"

PingusWorldMapTubeNode::PingusWorldMapTubeNode (const PingusWorldMapTubeNodeData& data)
  : PingusWorldMapTubeNodeData (data),
    worldmap_name ("pacman.xml"),
    tube ("worldmap/tube", "core")
{
  tube.set_align (-16, -32);
}


void 
PingusWorldMapTubeNode::on_click ()
{
  std::cout << "Not Supported" << std::endl;
  PingusWorldMapManager::get_current ()->change_map (worldmap_name, link_node);
}

void 
PingusWorldMapTubeNode::draw (CL_Vector offset)
{
  tube.put_screen (pos + offset);
}

std::string
PingusWorldMapTubeNode::get_string ()
{
  return _("Tube");
}

PingusWorldMapLevelNode::PingusWorldMapLevelNode (const PingusWorldMapLevelNodeData& data)
  : PingusWorldMapLevelNodeData (data),
    green_dot ("worldmap/dot_green", "core"),
    red_dot ("worldmap/dot_red", "core"),
    invalid_dot ("worldmap/dot_invalid", "core"),
    dot_border ("Game/dot_border", "game"),
    green_flag ("worldmap/flaggreen", "core"),
    invalid (false)
{
  //accessible = false;
  finished = false;

  green_flag.set_align (-24, -36);
  green_dot.set_align_center ();
  invalid_dot.set_align_center ();
  red_dot.set_align_center ();
  dot_border.set_align_center ();
}

void 
PingusWorldMapLevelNode::on_click ()
{
  if (!invalid)
    {
      if (maintainer_mode) 
	std::cout << "Start a level...: " << levelname << std::endl;
		  
      PingusSound::play_sound(path_manager.complete("sounds/letsgo.wav"));
      PingusGameSession game (path_manager.complete(levelname));
      
      // Launch the game and wait until it is finished
      game.start ();

      if (game.get_result ().finished ())
	{
	  finished = true;	
	}
      else
	{
	  console.puts("Please try again!");
	}
    }
  else
    {
      std::cout << "PingusWorldMapLevelNode::on_click (): Error: level is invalid" << std::endl;
    }
}

void 
PingusWorldMapLevelNode::mark (bool value)
{
  finished = value;
}

void 
PingusWorldMapLevelNode::draw (CL_Vector offset)
{
  if (!levelname.empty())
    {
      if (!invalid)
	{
	  if (/* accessible */ true) 
	    {
	      green_dot.put_screen (pos + offset);
	      if (finished) {
		green_flag.put_screen (pos + offset);
	      }
	    }
	  else
	    {
	      red_dot.put_screen (pos + offset);
	    }
	}
      else
	{
	  invalid_dot.put_screen (pos + offset);
	}
    }
}

std::string
PingusWorldMapLevelNode::get_string ()
{
  boost::shared_ptr<PLF> plf = get_plf ();
  
  if (!invalid)
    return System::translate(plf->get_levelname ());
  else
    return _("invalid level");
}

boost::shared_ptr<PLF>
PingusWorldMapLevelNode::get_plf ()
{
  if (plf.get () == 0)
    {
      if (!invalid) 
	{
	  //console << "Loading " << levelname << std::endl;
	  try {
	    plf = boost::shared_ptr<PLF> (new XMLPLF (path_manager.complete (levelname)));
	  } catch (PingusError& e) {
	    std::cout << "PingusWorldMapGraph: Caught PingusError (" << e.get_message () << ")" << std::endl;
	    std::cout << "PingusWorldMapGraph: Failed to load '" << levelname << "', fallback to level1.xml" << std::endl;
	    //plf = boost::shared_ptr<PLF> (new XMLPLF (path_manager.complete ("levels/level1.xml")));
	    invalid = true;
	    return boost::shared_ptr<PLF>();
	  }
	}
      else
	{
	  return boost::shared_ptr<PLF>();
	}
    }
  
  return plf;
}

/* EOF */
