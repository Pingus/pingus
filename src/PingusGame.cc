//  $Id: PingusGame.cc,v 1.7 2000/03/12 01:56:56 grumbel Exp $
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
#include "TrueServer.hh"
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
  if (client) {
    delete client;
  }
  if (server) {
    delete server;
  }
}

string
PingusGame::read_lastlevel_file()
{
  ifstream in;
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
  ofstream out;
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
PingusGame::start(std::string plf_filename, std::string psm_filename)
{
  if (verbose) std::cout << "PingusGame: start" << std::endl;

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
    
	std::cout << "PingusGame: Creating Server..." << std::endl;
	server = new TrueServer;
	std::cout << "PingusGame: Creating Server... done" << std::endl;

	std::cout << "PingusGame: Creating Client..." << std::endl;
	client = new Client(server);
	std::cout << "PingusGame: Creating Client.. done" << std::endl;
    
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

  if (verbose) std::cout << "PingusGame: start() done" << std::endl;
}

/* EOF */
