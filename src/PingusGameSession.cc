//  $Id: PingusGameSession.cc,v 1.10 2001/08/15 22:01:45 grumbel Exp $
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

#include "XMLPLF.hh"
#include "PLFPLF.hh"
#include "Client.hh"
#include "TrueServer.hh"
#include "MouseController.hh"

#include "PingusGameSession.hh"

using boost::shared_ptr;
using boost::dummy_ptr;

static 
boost::shared_ptr<PLF>
create_plf (std::string filename)
{
  if (filename.length () > 4)
    {
      //std::cout << "Extension is: " << filename.substr(filename.size() - 4) << std::endl;

      if (filename.substr(filename.size() - 4) == ".xml")
	return boost::shared_ptr<PLF> (new XMLPLF (filename));
      else
	return boost::shared_ptr<PLF> (new PLFPLF (filename));
    }
  else
    return boost::shared_ptr<PLF> (new XMLPLF (filename));
}

bool
PingusGameSessionResult::finished ()
{
  // FIXME: Always success
  return true;
}

float
PingusGameSessionResult::percentage_saved ()
{
  return 100.0;
}

float 
PingusGameSessionResult::percentage_time ()
{
  return 100.0;
}

PingusGameSession::PingusGameSession (std::string arg_filename)
  : filename (arg_filename),
    plf(create_plf (filename)),
    server (new TrueServer (plf)),
    client (new Client(shared_ptr<Controller>(new MouseController ()), 
		       server.get ()))
{
}

PingusGameSession::~PingusGameSession ()
{
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
