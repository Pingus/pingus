//  $Id: pingu_actions.cc,v 1.1 2000/02/04 23:45:19 mbn Exp $
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

#ifndef WIN32
#include <dlfcn.h>
#endif /* !WIN32 */

#include "../PLF.hh"

#include "basher.hh"
#include "blocker.hh"
#include "bomber.hh"
#include "bridger.hh"
#include "climber.hh"
#include "digger.hh"
#include "exiter.hh"
#include "floater.hh"
#include "miner.hh"
#include "Jumper.hh"
//#include "smashed.hh"

#include "pingu_actions.hh"

//#define DYNAMIC_ACTIONS 1
// makePinguAction() translate a action name into the according
// PinguAction object.
PinguAction*
makePinguAction(string action_name)
{
#ifdef DYNAMIC_ACTIONS
  // add some support for dynamic actions
  void *handle;
  PinguAction* (*makeAction)(void);
  char *error;
  
  handle = dlopen ("actions/.libs/libpingu_actions.so", RTLD_LAZY);
  if (!handle) {
    fputs (dlerror(), stderr);
    exit(1);
  }
  
  makeAction = dlsym(handle, "make_Basher__FV");
  return (*makeAction)();

#else /* not DYNAMIC_ACTIONS */
  if (action_name == "climber") {
    return new Climber;
  } else if (action_name == "blocker") {
    return new Blocker;
  } else if (action_name == "bomber") {
    return new Bomber;
  } else if (action_name == "bridger") {
    return new Bridger;
  } else if (action_name == "digger") {
    return new Digger;
  } else if (action_name == "miner") {
    return new Miner;
  } else if (action_name == "floater") {
    return new Floater;
  } else if (action_name == "basher") {
    return new Basher;
  } else if (action_name == "jumper") {
    return new Jumper;
  } else {
    cout << "makePinguAction: Action `" << action_name << "' unkown"
	 << endl;
    exit(EXIT_FAILURE);
  }
#endif
}

/* EOF */
