//  $Id: PingusGame.cc,v 1.2 2000/02/09 21:43:40 grumbel Exp $
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

#include "globals.hh"
#include "algo.hh"
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
  string filename;
  string levelfile;

  filename = pingus_homedir + "stat/last_level";

  in.open(filename.c_str());

  if (!in) {
    cout << "Warrning: Couldn't open lastlevel file \"" << filename << "\", using default level1.plf" << endl;
    return find_file(pingus_datadir, "levels/level1.plf");
  } else {
    in >> levelfile;
    cout << "Read lastlevel file: " << levelfile << endl;
    in.close();
    return levelfile;
  }
}

void
PingusGame::write_lastlevel_file(string levelfile)
{
  ofstream out;
  string filename;

  filename += pingus_homedir + "stat/last_level";

  out.open(filename.c_str());
  
  if (!out) {
    cout << "Warrning: Couldn't write lastlevel file: " << filename << endl;
  } else {
    cout << "Writing lastlevel file: " << levelfile << endl;
    out <<  levelfile << endl;
    out.close();
  }
}

// Start the given level
void
PingusGame::start(string plf_filename, string psm_filename)
{
  if (plf_filename.empty()) {
    plf_filename = read_lastlevel_file();
  }

  if (!plf_filename.empty())
    write_lastlevel_file(plf_filename);

  client = 0;
  server = 0;

  do {
    if (client)
      delete client;
      
    if (server)
      delete server;
    
    server = new TrueServer;
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

// Returns the results, which were created by the last level.
Result
PingusGame::get_result()
{
  return server->get_world()->get_result();
}

/* EOF */
