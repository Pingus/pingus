//  $Id: PingusGameSession.cc,v 1.15 2002/06/09 13:03:11 grumbel Exp $
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

#include "Client.hh"
#include "TrueServer.hh"
#include "MouseController.hh"
#include "System.hh"
#include "PingusGameSession.hh"
#include "PingusGameSessionResult.hh"

using boost::shared_ptr;

PingusGameSession::PingusGameSession (std::string arg_filename)
  : filename (arg_filename),
    plf(PLF::create (filename)),
    server (new TrueServer (plf)),
    client (new Client(shared_ptr<Controller>(new MouseController ()), 
		       server))
{
}

PingusGameSession::~PingusGameSession ()
{
  delete client;
  delete server;
  delete plf;
}

void 
PingusGameSession::start ()
{
  std::cout << "PingusGameSession: Giving control to the client..." << std::endl;
  client->display();
  std::cout << "PingusGameSession: Got control back from the client." << std::endl;
}

PingusGameSessionResult
PingusGameSession::get_result ()
{
  return PingusGameSessionResult ();
}

/* EOF */
