//  $Id: demo_session.cxx,v 1.1 2002/10/03 01:02:12 grumbel Exp $
//
//  Pingus - A free Lemmings clone
//  Copyright (C) 2002 Ingo Ruhnke <grumbel@gmx.de>
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

#include "xml_pdf.hxx"
#include "true_server.hxx"
#include "world.hxx"
#include "demo_player.hxx"
#include "demo_session.hxx"

DemoSession::DemoSession(const std::string& filename)
{
  // Load Demo file
  pdf    = new XMLPDF(filename);

  // Create server
  server = new TrueServer(pdf->get_plf());

  demo_player = new DemoPlayer(server, pdf);
}

DemoSession::~DemoSession()
{
  delete server;
  delete pdf;
}

/** Draw this screen */
bool
DemoSession::draw (GraphicContext& gc)
{
  server->get_world()->draw(gc);
}

/** Pass a delta to the screen */
void
DemoSession::update (const GameDelta& delta)
{
  // FIXME: Duplicate all timing code here?!
  server->update();
  demo_player->update();
}

/* EOF */
