//  $Id: PingusGameSession.cc,v 1.12 2002/06/07 14:50:34 torangan Exp $
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
#include "System.hh"
#include "PingusGameSession.hh"

using boost::shared_ptr;

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

boost::shared_ptr<PLF>
PingusGameSession::create_plf (std::string pathname)
{
  std::string extension = System::extension (pathname);

  if (extension == "xml")
    return boost::shared_ptr<PLF> (new XMLPLF (filename));
  else if (extension == "plf")
    return boost::shared_ptr<PLF> (new PLFPLF (filename));
  else // filename does not have an extension, default to xml
    return boost::shared_ptr<PLF> (new XMLPLF (filename));
}

/* EOF */
