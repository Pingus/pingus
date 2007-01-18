//  $Id: true_server.cxx,v 1.21 2003/10/18 23:17:27 grumbel Exp $
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
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

#include "goal_manager.hxx"
#include "true_server.hxx"


TrueServer::TrueServer(const PingusLevel& arg_plf)
  : Server(arg_plf)
{
  fast_forward = false;
  pause = false;
}

TrueServer::~TrueServer()
{
}

void
TrueServer::update()
{
  if (fast_forward && !pause)
    {
      // To let the game run faster we just update it multiple
      // times
      for (int i = 0; i < 4; ++i)
	{
          Server::update();
	}
    }
  else
    {
      if (!pause)
        Server::update();
    }
}

void
TrueServer::set_fast_forward(bool value)
{
  fast_forward = value;
}

bool
TrueServer::get_fast_forward()
{
  return fast_forward;
}

void
TrueServer::set_pause(bool value)
{
  pause = value;
}

bool
TrueServer::get_pause()
{
  return pause;
}

void
TrueServer::set_finished ()
{
  goal_manager->set_abort_goal();
  set_pause(false);
}


/* EOF */
