//  $Id: PingusGame.cc,v 1.12 2000/06/08 20:05:35 grumbel Exp $
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

#include "System.hh"
#include "globals.hh"
#include "PingusError.hh"
#include "PingusMessageBox.hh"
#include "algo.hh"
#include "Timer.hh"
#include "TrueServer.hh"
#include "DemoPlayer.hh"
#include "PingusGame.hh"

// A wrapper class around the client and the server, to allow a much
// easier start of the game. 

PingusGame::PingusGame()
{
  server = 0;
  client = 0;
}

PingusGame::~PingusGame()
{
  cout << "PingusGame:~PingusGame" << endl;
  if (client) {
    cout << "Deleting client" << endl;
    delete client;
  }
  if (server) {
    cout << "Server client" << endl;
    delete server;
  }
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
    if (verbose) std::cout << "PingusGame: Warrning: Couldn't open lastlevel file \"" << filename << "\", using default level1.plf" << std::endl;
    return find_file(pingus_datadir, "levels/level1.plf");
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
    if (verbose) std::cout << "Warrning: Couldn't write lastlevel file: " << filename << std::endl;
  } else {
    if (verbose) std::cout << "Writing lastlevel file: " << levelfile << std::endl;
    out <<  levelfile << std::endl;
    out.close();
  }
}

// Start the given level
void
PingusGame::start_game(std::string plf_filename, std::string psm_filename)
{
  if (verbose) std::cout << "PingusGame: start" << std::endl;

  std::cout << "Level: " << plf_filename << std::endl;

  try 
    {
      if (plf_filename.empty()) {
	plf_filename = read_lastlevel_file();
      }

      if (!plf_filename.empty())
	write_lastlevel_file(plf_filename);

      do {
	if (client)
	  delete client;
      
	if (server)
	  delete server;

	server = new TrueServer();
	client = new Client(server);
    
	if (psm_filename.empty()) 
	  {
	    client->start(plf_filename);
	  } 
	else 
	  {
	    client->start(plf_filename, psm_filename);
	  }
    
      } while (client->replay());
    }

  catch(PingusError err)
    {
      PingusMessageBox(" PingusError: " + err.message);
    }

  if (verbose) std::cout << "PingusGame: start_game() done" << std::endl;
}

void
PingusGame::start_demo(std::string pdm_filename)
{
  DemoPlayer player;
  
  try 
    {
      if (client)
	delete client;
      
      if (server)
	delete server;

      player.load(pdm_filename);
     
      server = new TrueServer();
      client = new Client(server);
      
      client->start(&player);
    }  

  catch(PingusError err)
    {
      PingusMessageBox(" PingusError: " + err.message);
    }

  if (verbose) std::cout << "PingusGame: start_demo() done" << std::endl;
}

/* EOF */
