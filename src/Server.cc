//  $Id: Server.cc,v 1.2 2000/02/09 21:43:41 grumbel Exp $
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

#include <cstdio>
#include <cstdlib>

#include "globals.hh"
#include "Server.hh"
#include "GameTime.hh"
#include "Pingu.hh"
#include "PingusError.hh"

Server::Server()
{
  demo_mode = false;
  get_next_event = true;
  finished = false;
}

Server::~Server()
{
}

World*
Server::get_world()
{
  return world;
}

void
Server::let_move()
{
  static PingusEvent event;
  
  if (!demo_mode) {
    return;
  }
  
  if (get_next_event) {
    // Getting next event from file
    get_next_event = false;    
    event = demo_in->get_next_event();
  }
  
  // Check if the time for the event is right
  if (GameTime::get_time() == event.game_time) {
    process_event(event.str);
    get_next_event = true;
  } else if (GameTime::get_time() >= event.game_time) {
    // BUG: If this is reached the demo code is buggy
    cout << "Demo out of sync!: " 
	 << "GameTime: " << GameTime::get_time()
	 << " EventTime: " << event.game_time
	 << endl;
    get_next_event = true;
  }
}

// Some simple event management
void
Server::send_event(string event)
{
  if (record_demo) {
    string str;
    char temp[256];
    sprintf(temp, "%d", GameTime::get_time());
    str += temp;
    str += ":";
    str += event;
    demo_out.set_next_event(str);
  }
  process_event(event);
}

void
Server::process_event(string event)
{
  char* event_str = strdup(event.c_str());
  string token;
  const char delimiters[] = ":";

  token = strtok(event_str, delimiters); // Get GameTime

  if (token == "armageddon")
    {
      world->armageddon();
      cout << endl;
    }
  else if (token == "Pingu")
    {
      int pingu_id;
      PinguHolder* pingus;
      
      string action;
      
      token = strtok(NULL, delimiters); // Get Pingu id
      pingu_id = atoi(token.c_str());

      token = strtok(NULL, delimiters); // Get action name
      action = token;
    
      pingus = world->get_pingu_p();
      
      //BUG: This could need some optimization
      for(PinguIter pingu=pingus->begin(); pingu != pingus->end(); ++pingu) 
	{
	  if ((*pingu)->get_id() == pingu_id) 
	    {
	      PinguAction* tmp_action = action_holder.get_action(action);
	      
	      if (tmp_action)
		{
		  if (!(*pingu)->set_action(tmp_action))
		    {
		      action_holder.push_action(action);
		    }
		}
	    }
	}
    }
  else 
    {
      cout << "Couldn't ident token: \""  << token << "\"" << endl;
      cout << "Unknown Event: " << event << endl;
    }
  
  free(event_str);
}

bool
Server::is_finished()
{
  if (finished) {
    cout << "Finished" << endl;
    return true;
  } else {
    return world->is_finished();
  }
}

void
Server::set_finished()
{
  finished = true;
}

void
Server::set_demo(string s)
{
  demo_mode = true;
  demo_file = s;
  demo_in->open(demo_file, (PingusDemoMode)play);

  cout << "Set_demo(): " << demo_file << " file opened" << endl;
}

void 
Server::set_record_file(string file)
{
  demo_out.open(file.c_str(), (PingusDemoMode)record);
};

ActionHolder*
Server::get_action_holder()
{
  return &action_holder;
}

/* EOF */
