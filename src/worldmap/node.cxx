//  $Id: node.cxx,v 1.6 2002/08/02 11:25:47 grumbel Exp $
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

#include "manager.hxx"
#include "../my_gettext.hxx"
#include "../path_manager.hxx"
#include "../system.hxx"
#include "../game_session.hxx"
#include "../game_session_result.hxx"
#include "../console.hxx"
#include "../xml_plf.hxx"
#include "../pingus_error.hxx"
#include "../sound.hxx"
#include "../pingus_resource.hxx"
#include "../globals.hxx"
#include "../screen_manager.hxx"
#include "node.hxx"

using namespace Pingus::WorldMap;

TubeNode::TubeNode (const TubeNodeData& data)
  : TubeNodeData (data),
    worldmap_name ("pacman.xml"),
    tube ("worldmap/tube", "core")
{
  tube.set_align (-16, -32);
}


void 
TubeNode::on_click ()
{
  std::cout << "Not Supported" << std::endl;
  WorldMapManager::instance ()->change_map (worldmap_name, link_node);
}

void 
TubeNode::draw (CL_Vector offset)
{
  tube.put_screen (pos + offset);
}

std::string
TubeNode::get_string ()
{
  return _("Tube");
}

LevelNode::LevelNode (const LevelNodeData& data)
  : LevelNodeData (data),
    green_dot ("worldmap/dot_green", "core"),
    red_dot ("worldmap/dot_red", "core"),
    invalid_dot ("worldmap/dot_invalid", "core"),
    dot_border ("Game/dot_border", "game"),
    green_flag ("worldmap/flaggreen", "core"),
    plf (0),
    invalid (false)
{
  //accessible = false;
  finished = false;

  green_flag.set_align (-24, -36);
  green_dot.set_align_center ();
  invalid_dot.set_align_center ();
  red_dot.set_align_center ();
  dot_border.set_align_center ();

  try 
    {
      plf = new XMLPLF (path_manager.complete (levelname));
    } 
  catch (PingusError& e) 
    {
      std::cout << "PingusWorldMapGraph: Caught PingusError (" << e.get_message () << ")" << std::endl;
      std::cout << "PingusWorldMapGraph: Failed to load '" << levelname << "', fallback to level1.xml" << std::endl;
      invalid = true;
    }
}

LevelNode::~LevelNode ()
{
  delete plf;
}

void 
LevelNode::on_click ()
{
  if (!invalid)
    {
      if (maintainer_mode) 
	std::cout << "Start a level...: " << levelname << std::endl;
		  
      PingusSound::play_sound(path_manager.complete("sounds/letsgo.wav"));
      /** We could reuse the plf here */
      
      ScreenManager::instance()->push_screen
	(new PingusGameSession (path_manager.complete(levelname)), true);
      /*
      if (game.get_result ().finished ())
	{
	  finished = true;	
	}
      else
	{
	  console.puts("Please try again!");
	}
      */
    }
  else
    {
      std::cout << "LevelNode::on_click (): Error: level is invalid" << std::endl;
    }

  //PingusResource::cleanup ();
}

void 
LevelNode::mark (bool value)
{
  finished = value;
}

void 
LevelNode::draw (CL_Vector offset)
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
LevelNode::get_string ()
{
  if (!invalid)
    return System::translate(plf->get_levelname ());
  else
    return _("invalid level");
}

PLF*
LevelNode::get_plf ()
{
  return plf;
}

/* EOF */
