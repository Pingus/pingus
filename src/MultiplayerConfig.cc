//  $Id: MultiplayerConfig.cc,v 1.5 2002/01/13 15:24:18 grumbel Exp $
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

#include "Display.hh"
#include "MultiplayerGame.hh"
#include "MultiplayerConfig.hh"

MultiplayerConfig::MultiplayerConfig ()
{
}

MultiplayerConfig::~MultiplayerConfig ()
{
}

void 
MultiplayerConfig::display ()
{
  CL_Display::clear_display (1.0, 0.0, 0.0);
  Display::flip_display ();
  CL_System::keep_alive ();

  start ();
}

void 
MultiplayerConfig::start ()
{
  std::cout << "MultiplayerConfig::start ()" << std::endl;
  MultiplayerGame game;
  game.start ();
}

/* EOF */
