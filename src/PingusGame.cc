//  $Id: PingusGame.cc,v 1.21 2001/05/18 19:17:08 grumbel Exp $
//
//  Pingus - A free Lemmings clone
//  Copyright (C) 1999 Ingo Ruhnke <grumbel@gmx.de>
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
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

#include <fstream>

#include "PingusLevelDesc.hh"
#include "System.hh"
#include "globals.hh"
#include "PingusError.hh"
#include "PingusMessageBox.hh"
#include "algo.hh"
#include "Timer.hh"
#include "TrueServer.hh"
#include "DemoPlayer.hh"
#include "Console.hh"
#include "PingusGame.hh"
#include "Controller.hh"
#include "GamepadController.hh"
#include "MouseController.hh"
#include "XMLPLF.hh"
#include "PLFPLF.hh"

using boost::shared_ptr;

// A wrapper class around the client and the server, to allow a much
// easier start of the game. 

PingusGame::PingusGame()
{
}

PingusGame::~PingusGame()
{
}

std::string
PingusGame::read_lastlevel_file()
{
  std::ifstream in;
  std::string filename;
  std::string levelfile;

  filename = System::get_statdir() + "stat/last_level";

  in.open(filename.c_str());

  if (!in) {
    if (verbose) std::cout << "PingusGame: Warning: Couldn't open lastlevel file \"" << filename << "\", using default level1.plf" << std::endl;
    return "levels/level1.plf";
  } else {
    in >> levelfile;
    if (verbose) std::cout << "PingusGame: Read lastlevel file: " << levelfile << std::endl;
    in.close();
    return levelfile;
  }
}

void
PingusGame::write_lastlevel_file(std::string levelfile)
{
  std::ofstream out;
  std::string filename;

  filename += System::get_statdir() + "stat/last_level";

  out.open(filename.c_str());
  
  if (!out) {
    if (verbose) std::cout << "Warning: Couldn't write lastlevel file: " << filename << std::endl;
  } else {
    if (verbose) std::cout << "Writing lastlevel file: " << levelfile << std::endl;
    out <<  levelfile << std::endl;
    out.close();
  }
}

// Start the given level
void
PingusGame::start_game (boost::shared_ptr<PLF> arg_plf)
{
  plf = arg_plf;
  if (verbose) std::cout << "PingusGame: start" << std::endl;

  boost::shared_ptr<Controller> controller;
  
  /*  if (CL_Input::joysticks.size () > 0)
    controller = boost::shared_ptr<Controller>(new GamepadController (CL_Input::joysticks[0]));
  else*/
    controller = boost::shared_ptr<Controller>(new MouseController ());
  
  PingusLevelDesc leveldesc(plf, controller);
  leveldesc.draw(PingusLevelDesc::LOADING);
	
  server = boost::shared_ptr<Server>(new TrueServer(plf));
  client = boost::shared_ptr<Client>(new Client(controller, server.get ()));
  
  leveldesc.draw(PingusLevelDesc::FINISHED);

  try {
    client->start();
  } catch(PingusError err) {
    PingusMessageBox(" PingusError: " + err.get_message ());
  }
  
  if (verbose) std::cout << "PingusGame: start_game() done" << std::endl;
}


void  
PingusGame::start_game(std::string plf_filename, std::string psm_filename)
{
  if (plf_filename.substr(plf_filename.size() - 4) == ".xml")
    {
      plf = shared_ptr<PLF>(new XMLPLF(plf_filename));
    }
  else // Assuming we are reading a .plf file
    {
      plf = shared_ptr<PLF>(new PLFPLF(plf_filename));

      // FIXME: dirty hack, should replace or merge the psm files
      {
	std::string filename = plf_filename.substr(0, plf_filename.size() - 4);
    
	if (verbose > 1) std::cout << "PSM: " << filename + ".psm" << std::endl;
    
	plf->set_psm_filename(filename + ".psm");
      }
    }
  start_game (plf);
}

/* EOF */
