//  $Id: Pingu.cc,v 1.18 2000/04/29 13:13:26 grumbel Exp $
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

#include <cassert>
#include <cstdio>
#include <cmath>
#include <ClanLib/core.h>

#include "globals.hh"
#include "Pingu.hh"
#include "ActionHolder.hh"
#include "PingusResource.hh"
#include "PingusSound.hh"
#include "algo.hh"
#include "actions/smashed.hh"
#include "FVec.hh"

const float deadly_velocity = 20.0;

// Create the static objects of the class
ColMap*             Pingu::colmap;
PinguMap*           Pingu::map;

bool                Pingu::init;
CL_Surface*         Pingu::walker;
CL_Surface*         Pingu::faller;
CL_Surface*         Pingu::tumble;
CL_Font*            Pingu::font;

CL_ResourceManager* Pingu::local_res_p;
ParticleHolder* Pingu::particle;

Pingu::Pingu()
{
}

// Init a pingu at the given position while falling
Pingu::Pingu(int x, int y)
{
  if (!init) 
    {
      init = true;

      local_res_p = PingusResource::get("pingus.dat");
      PinguAction::SetResourceManager(local_res());

      font   = CL_Font::load("Fonts/numbers", PingusResource::get("fonts.dat"));  
      //walker = CL_Surface::load("XMas/walker", local_res());
      walker = CL_Surface::load("Pingus/walker", local_res());
      faller = CL_Surface::load("Pingus/faller", local_res());
      tumble = CL_Surface::load("Pingus/tumble", local_res());
    }

  tumbling = false;
  falling = 4;
  status = alive;

  x_pos = x;
  y_pos = y;

  action = sec_action = 0;
  action_time = -1;

  // Init all animation timer
  walker_c.set_size(walker->get_num_frames() / 2);
  walker_c.set_speed(50);
  
  faller_c.set_size(faller->get_num_frames());
  tumble_c.set_size(tumble->get_num_frames());

  environment = (PinguEnvironment)land;

  // Set the velocity to zero
  velocity.x = 0;
  velocity.y = 0;
}

Pingu::~Pingu()
{
}

// Returns the x position of the pingu
int
Pingu::get_x()
{
  return x_pos;
}

// Returns the y position of the pingu
int
Pingu::get_y()
{
  return y_pos;
}

int
Pingu::get_id()
{
  return id;
}

// Returns environment
PinguEnvironment
Pingu::get_environment()
{
  return environment;
}

// Set the position of the pingu
void 
Pingu::set_pos(int x, int y)
{
  x_pos = x;
  y_pos = y;
}

// Static function which set the colmap for all pingus
void
Pingu::set_colmap(ColMap* a)
{
  colmap = a;
}

// Static function which set the map for all pingus
void
Pingu::set_map(PinguMap* a)
{
  map = a;
}

// Set the action of the pingu (bridger, blocker, bomber, etc.)
// This function is used by external stuff, like the ButtonPanel, etc
int
Pingu::set_action(PinguAction* act)
{
  assert(act);

  if (status == dead)
    {
      std::cout << "Setting action to a dead pingu" << std::endl;
      return 0;
    }

  act->set_pingu(this);

  // Use the activation time of the action

  // FIXME: ^ XOR?!
  if (act->get_type() ^ (ActionType)ONCE) 
    {
      if (verbose) 
	{
	  std::cout << "Pingu: Found some persistant action" << std::endl;
	  std::cout << "Pingu: Action is FALL: " 
		    << int(act->get_type() & (ActionType)FALL) << std::endl;
	  std::cout << "Pingu: Action is WALL: " 
		    << int(act->get_type() & (ActionType)WALL) << std::endl;
	}
      
      for(std::vector<PinguAction*>::iterator i = persist.begin(); i != persist.end(); i++)
	{
	  if ((*i)->name() == act->name()) 
	    {
	      std::cout << "Not using action, we have allready" << std::endl;
	      return 0;
	    }
	}

      persist.push_back(act);
      return 1;
    } 
  else 
    {
      // if environment is bad return
      if (!(act->get_environment() & environment))
	{ 
	  return 0; 
	}
    
      if (act->activation_time() == -1)
	{ // Immediately activate the action
	  if (action && (action->name() == act->name()))
	    {
	      if (verbose > 1) std::cout << "Allready have action" << std::endl;
	      return false;
	    }
	  action = act;
	  return 1;	    
	}
      else 
	{ // Use the activation time, given by t
	  if (sec_action && sec_action->name() == act->name())
	    {
	      return false;
	    }
	  action_time = act->activation_time();
	  sec_action = act;
	  PingusSound::play_wav(find_file(pingus_datadir, "sound/OHNO.WAV"));
	}
      return true;
    }
}

// Sets an action without any checking
int
Pingu::set_paction(PinguAction* act) 
{
  action = act;
  action->set_pingu(this);

  return true;
}

PinguStatus
Pingu::get_status(void) const
{
  return status;
}

PinguStatus
Pingu::set_status(PinguStatus s)
{
  return (status = s);
}

// Returns true if the given koordinates are above the pingu
bool
Pingu::is_over(int x, int y)
{
  // FIXME: We should use the x/y_offset here
  if (x > x_pos - 16 && x < x_pos + 16
      && y > y_pos - 32 && y < y_pos)
    {
      return true;
    } 
  else 
    {
      return false;
    }
}

// Returns the distance between the Pingu and a given coordinate
double
Pingu::dist(int x, int y)
{
  return sqrt(double(((x_pos - x) * (x_pos - x)
		      + (y_pos - 16 - y) * (y_pos - 16 - y))));
}

void
Pingu::do_persistent()
{
  if (environment == sky && action == 0 && rel_getpixel(0, -1) == ColMap::NOTHING) 
    {
      for (unsigned int i=0; i < persist.size(); ++i) 
	{
	  if (persist[i]->get_type() & (ActionType)FALL) 
	    {
	      if (action && persist[i]->name() == action->name()) 
		{
		  std::cout << "Not using action, we have allready" << std::endl;
		} 
	      else 
		{
		  set_paction(ActionHolder::get_uaction(persist[i]->name()));
		}
	    }
	}
    }
}

// Let the pingu do his job (i.e. walk his way)
// FIXME: This function is *much* to large, it needs a real cut down
// into smaller pieces.  
void
Pingu::let_move(void)
{
  if (status == dead) 
    return;
  
  // Increase the AnimCounter's
  ++tumble_c;
  ++walker_c;

  if (action_time > -1) 
    --action_time;

  if (action_time == 0 && sec_action) 
    {
      action = sec_action;
      action->set_pingu(this);
    }
  
  if (action && action->is_finished) 
    {
      action = 0;
    }
    
  if (rel_getpixel(0, -1) == ColMap::OUTOFSCREEN) 
    {
      PingusSound::play_wav(find_file(pingus_datadir, "sound/DIE.WAV"));
      status = dead;
      return;
    }

  do_persistent();
  
  if (action) 
    { // if we have an action, let_move() it
      action->let_move();
    }
  else 
    { // if we have no action, let the pingu walk
      do_normal();
    }
}

PinguEnvironment
Pingu::check_enviroment()
{
  // FIXME: We need some code here...
}

// Check if the pingu is on ground and then do something.
void 
Pingu::do_normal()
{
  if (rel_getpixel(0, -1) == ColMap::NOTHING)
    {
      do_falling();
    }
  else 
    {
      do_walking();
    }
}

// The Pingu is not on ground, so lets fall...
void
Pingu::do_falling()
{
  // Apply all forces
  velocity = ForcesHolder::apply_forces(CL_Vector(x_pos, y_pos), velocity);
    
  CL_Vector newp = velocity;
	  
  // Update x and y by moving the penguin to it's target *slowly*
  // and checking if the penguin has hit the bottom at each loop
  while(rel_getpixel(0, -1) == ColMap::NOTHING
	&& (fabs(newp.x) >= 1 || fabs(newp.y) >= 1))
    {
      if (fabs(newp.x) >= 1)
	{ 
	  // Since the velocity might be a
	  // fraction stop when we are within 1 unit of the target
	  if (newp.x > 0)
	    {
	      x_pos++;
	      newp.x--;
	    }
	  else
	    {
	      x_pos--;
	      newp.x++;
	    }
	}

      if (fabs(newp.y) >= 1)
	{
	  if (newp.y > 0)
	    {
	      y_pos++;
	      newp.y--;
	    }
	  else 
	    {
	      y_pos--;
	      newp.y++;
	    }
	}
    }

  // Now that the Pingu is moved, check if he hits the ground.
  if (rel_getpixel(0, -1) == ColMap::NOTHING)
    { // if pingu is not on ground
      ++falling;
	  
      if (falling > 3) 
	environment = sky;
	  
      // If we are going fast enough to get smashed set falling to 80
      if (fabs(velocity.x) > deadly_velocity || fabs(velocity.y) > deadly_velocity)
	{
	  tumbling = true;
	}
    }
  else // Ping is on ground
    {
      // Did we stop too fast?
      if (std::fabs(velocity.y) > deadly_velocity)
	{
	  // FIXME: This is a LinuxTag Hack and should be replaced
	  // with a real ground smashing action! 
	  set_action(ActionHolder::get_uaction("splashed"));
	}
      else if (std::fabs(velocity.x) > deadly_velocity)
	{
	  std::cout << "x Smashed on ground, jumping" << std::endl;
	}

      // Reset the velocity
      velocity.x = 0;
      velocity.y = 0;
      falling = 0;
    }
}

// If the Pingu is on ground he can do his walking stuff here.
void 
Pingu::do_walking()
{
  environment = land;

 if (rel_getpixel(0,-1) & ColMap::WATER)
   {
     std::cout << "Blub, blub, blub, blub...." << std::endl;
     PingusSound::play_wav(find_file(pingus_datadir, "sound/SPLASH.WAV"));
     status = dead;
     return;
   }

  if (rel_getpixel(1, 0) == ColMap::NOTHING) 
    { // if infront is free
      x_pos += direction;    
    }
  else 
    { // if infront is a pixel 
      // Pingu is walking up the mountain 
      if (rel_getpixel(1,1) == ColMap::NOTHING) 
	{
	  x_pos += direction;
	  y_pos -= 1;
	} 
      else if (rel_getpixel(1,2) == ColMap::NOTHING)
	{
	  x_pos += direction;
	  y_pos -= 2;
	} 
      else if (rel_getpixel(1,2) & ColMap::BRIDGE) 
	{
	  x_pos += direction;
	  y_pos -=3;
	}
      else
	{ // WALL
	  for (unsigned int i=0; i < persist.size(); ++i) 
	    {
	      if (persist[i]->get_type() & (ActionType)WALL) 
		{
		  if (action && persist[i]->name() == action->name()) 
		    {
		      if (verbose) std::cout << "Not using action, we have allready" << std::endl;
		    } 
		  else 
		    {
		      if (verbose) std::cout << "We are infront of a wall, setting persistant action" << std::endl;
		      set_paction(ActionHolder::get_uaction(persist[i]->name()));
		    }
		  return;
		}
	    }
	  direction.change();
	}
    }

}

// Draws the pingu on the screen with the given offset
void
Pingu::draw_offset(int x, int y, float s) const
{
  char str[64];
  y += 2;
  
  if (action) 
    {
      action->draw_offset(x, y,s);
    } 
  else 
    {
      if (falling > 3) 
	{
	  CL_Surface* surf;
	  
	  if (tumbling) {
	    surf = tumble;
	  } else {
	    surf = faller;
	  }
	  
	  if (s == 1.0) 
	    {
	      surf->put_screen(x_pos + x - 16, y_pos + y - 32,
			       tumble_c);
	    } 
	  else 
	    {
	      surf->put_screen((x_pos + x - 16) * s , (y_pos + y - 32) * s,
			       s, s, tumble_c);
	    }
	} 
      else // If not falling
	{
	  if (s == 1.0) 
	    {
	      walker->put_screen(x_pos + x - 16, y_pos + y - walker->get_height(), 
				 walker_c
				 + ((direction.is_left() ? 0 :
				     walker_c.size())));
	    } 
	  else 
	    {
	      walker->put_screen((x_pos + x - 16) * s, (y_pos + y - walker->get_height()) * s, 
				 s, s,
				 walker_c
				 + ((direction.is_left() ? 0 :
				     walker_c.size())));
	    }
	}
    }

  if (action_time != -1) 
    {
      sprintf(str, "%d", action_time);
      
      if (s == 1.0) 
	{
	  font->print_center(x_pos + x, y_pos - 45 + y, str);
	} 
      else if (s > 1.0) 
	{
	  font->print_left(int((x_pos + x) * s), (int)((y_pos - 45 + y) * s) - (int(font->get_text_width(str) * s) / 2),
			   int(s), int(s),
			   str);
	}
      else 
	{
	  // Don't draw the font if the zoom is smaller than default (1.0)
	}
    }
}

// Returns the `color' of the colmap in the walking direction 
int
Pingu::rel_getpixel(int x, int y)
{
  assert(colmap);
  return colmap->getpixel(x_pos + (x * direction), (y_pos) - y);
}

// Let the pingu catch another pingu, so that an action can be aplied
// (i.e. let a blocker change the direction f another pingu)
void
Pingu::catch_pingu(Pingu* pingu)
{
  action->catch_pingu(pingu);
}

// Returns true if the pingu needs to catch another pingu
bool
Pingu::need_catch()
{
  if (status == dead)
    return false;
  
  if (action)
    return action->need_catch();
  else 
    return false;
}

void
Pingu::set_direction(Direction d)
{
  direction = d;
}

bool
Pingu::is_alive(void)
{
  if (status != dead && status != exited
      && status != not_catchable) 
    return true;
  else 
    return false;
}

PinguAction*
Pingu::get_action()
{
  return action;
}

CL_ResourceManager*
Pingu::local_res()
{
  return local_res_p;
}

void
Pingu::SetParticleHolder(ParticleHolder* p)
{
  particle = p;
}

int 
Pingu::set_id(int i)
{
  return (id = i);
}

int
Pingu::x_offset(void)
{
  if (action)
    {
      return action->x_offset();
    }
  else
    {
      return -16;
    }
}

int
Pingu::y_offset(void)

{
  if (action) 
    {
      return action->y_offset();
    }
  else 
    {
      return -32;
    }
}

/* EOF */
