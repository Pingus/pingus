//  $Id: Pingu.cc,v 1.56 2001/06/11 08:45:21 grumbel Exp $
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
#include <boost/smart_ptr.hpp>

using namespace boost;

#include "actions/Drown.hh"
#include "actions/Splashed.hh"
#include "actions/LaserKill.hh"

#include "globals.hh"
#include "World.hh"
#include "Pingu.hh"
#include "ActionHolder.hh"
#include "PingusResource.hh"
#include "PingusSound.hh"
#include "algo.hh"
#include "actions/smashed.hh"
#include "FVec.hh"

const float deadly_velocity = 20.0;
int   Pingu::id_counter = 0;

// Init a pingu at the given position while falling
Pingu::Pingu(const CL_Vector& arg_pos, int owner)
  : id (++id_counter),
    font (PingusResource::load_font("Fonts/numbers", "fonts")),
    walker ("Pingus/walker" + to_string(owner), "pingus"),
    faller ("Pingus/faller" + to_string(owner), "pingus"),
    tumble ("Pingus/tumble" + to_string(owner), "pingus"),
    owner_id (owner),
    pos (arg_pos)
{
  walker.set_align_center_bottom ();
  faller.set_align_center_bottom ();
  tumble.set_align_center_bottom ();

  falling = 4;
  status = alive;

  action_time = -1;

  environment = (PinguEnvironment)land;

  // Set the velocity to zero
  velocity.x = 0;
  velocity.y = 0;
}

Pingu::~Pingu()
{
}

World* 
Pingu::get_world()
{
  return world;
}

// Returns the x position of the pingu
int
Pingu::get_x()
{
  return int(pos.x);
}

// Returns the y position of the pingu
int
Pingu::get_y()
{
  return int(pos.y);
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
  pos.x = x;
  pos.y = y;
}

void 
Pingu::set_pos(const CL_Vector& arg_pos)
{
  pos = arg_pos;
}

// Set the action of the pingu (bridger, blocker, bomber, etc.)
// This function is used by external stuff, like the ButtonPanel, etc
int
Pingu::set_action(shared_ptr<PinguAction> act)
{
  assert(act.get());

  if (status == dead)
    {
      if (pingus_debug_flags & PINGUS_DEBUG_ACTIONS)
	std::cout << "Setting action to a dead pingu" << std::endl;
      return 0;
    }

  act->set_pingu(this);

  // Use the activation time of the action

  // FIXME: ^ XOR?!
  if (act->get_type() ^ (ActionType)ONCE) 
    {
      if (pingus_debug_flags & PINGUS_DEBUG_ACTIONS)
	{
	  std::cout << "Pingu: Found some persistant action" << std::endl;
	  std::cout << "Pingu: Action is FALL: " 
		    << int(act->get_type() & (ActionType)FALL) << std::endl;
	  std::cout << "Pingu: Action is WALL: " 
		    << int(act->get_type() & (ActionType)WALL) << std::endl;
	}
      
      for(std::vector<shared_ptr<PinguAction> >::iterator i = persist.begin(); i != persist.end(); i++)
	{
	  if ((*i)->name() == act->name()) 
	    {
	      if (pingus_debug_flags & PINGUS_DEBUG_ACTIONS)
		std::cout << "Not using action, we have allready" << std::endl;
	      return 0;
	    }
	}

      persist.push_back(act);
      return 1;
    } 
  else 
    {
      if (act->get_environment() == (PinguEnvironment)always)
	{
	  action = act;
	  return 1;
	}

      // if environment is bad return
      if (!(act->get_environment() & environment))
	{ 
	  return 0; 
	}
    
      if (act->activation_time() == -1)
	{ // Immediately activate the action
	  if (action.get() && (action->name() == act->name()))
	    {
	      if (pingus_debug_flags & PINGUS_DEBUG_ACTIONS)
		std::cout << "Pingu: Already have action" << std::endl;
	      return false;
	    }
	  action = act;
	  return 1;	    
	}
      else 
	{ // Use the activation time, given by t
	  if (sec_action.get() && sec_action->name() == act->name())
	    {
	      return false;
	    }
	  action_time = act->activation_time();
	  sec_action = act;
	  //PingusSound::play_wav("ohno");
	}
      return true;
    }
}

void 
Pingu::set_action (const std::string action_name)
{
  set_action (PinguActionFactory::create (action_name));
}

// Sets an action without any checking
int
Pingu::set_paction(shared_ptr<PinguAction> act) 
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
  if (x > pos.x + x_offset() && x < pos.x + x_offset() + 32
      && y > pos.y + y_offset() && y < pos.y + y_offset() + 32)
    {
      return true;
    } 
  else 
    {
      return false;
    }
}

bool
Pingu::is_inside (int x1, int y1, int x2, int y2)
{
  assert (x1 < x2);
  assert (y1 < y2);

  return (pos.x > x1 && pos.x < x2 
	  &&
	  pos.y > y1 && pos.y < y2);
}

// Returns the distance between the Pingu and a given coordinate
double
Pingu::dist (int x, int y)
{
  return sqrt(((pos.x - x) * (pos.x - x)
	       + (pos.y - 16 - y) * (pos.y - 16 - y)));
}

void
Pingu::update_persistent(float delta)
{
  if (environment == sky && action.get() == 0 && rel_getpixel(0, -1) == ColMap::NOTHING) 
    {
      for (unsigned int i=0; i < persist.size(); ++i) 
	{
	  if (persist[i]->get_type() & (ActionType)FALL) 
	    {
	      if (action.get() && persist[i]->name() == action->name()) 
		{
		  if (pingus_debug_flags & PINGUS_DEBUG_ACTIONS)
		    std::cout << "Pingu: Not using action, we already did." << std::endl;
		} 
	      else 
		{
		  set_paction(world->get_action_holder()->get_uaction(persist[i]->name()));
		}
	    }
	}
    }
}

// Let the pingu do his job (i.e. walk his way)
// FIXME: This function is *much* to large, it needs a real cut down
// into smaller pieces.  
void
Pingu::update(float delta)
{
  walker.update (delta);
  tumble.update (delta);
  faller.update (delta);

  if (status == dead) 
    return;
  
  if (action_time > -1) 
    --action_time;

  if (action_time == 0 && sec_action.get()) 
    {
      action = sec_action;
      action->set_pingu(this);
    }
  
  if (action.get() && action->is_finished) 
    {
      action.reset ();
    }

  /** The Pingu has hit the edge of the screen, a good time to let him
      die. */
  if (rel_getpixel(0, -1) == ColMap::OUTOFSCREEN) 
    {
      PingusSound::play_wav("die");
      status = dead;
      return;
    }

  update_persistent(delta);

  /** When we have an action evaluate it, else evaluate the normal
      walking */
  if (action.get()) 
    action->update(delta);
  else 
    update_normal(delta);
}

// Check if the pingu is on ground and then do something.
void 
Pingu::update_normal(float delta)
{
  if (rel_getpixel(0, -1) == ColMap::NOTHING)
    {
      update_falling(delta);
    }
  else 
    {
      update_walking(delta);
    }
}

// The Pingu is not on ground, so lets fall...
void
Pingu::update_falling(float delta)
{
  // Apply all forces
  velocity = ForcesHolder::apply_forces(pos, velocity);
    
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
	      pos.x++;
	      newp.x--;
	    }
	  else
	    {
	      pos.x--;
	      newp.x++;
	    }
	}

      if (fabs(newp.y) >= 1)
	{
	  if (newp.y > 0)
	    {
	      pos.y++;
	      newp.y--;
	    }
	  else 
	    {
	      pos.y--;
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
    }
  else // Ping is on ground
    {
      if (rel_getpixel(0, -1) == ColMap::WATER)
	set_paction(world->get_action_holder()->get_uaction("drown"));
      else
	{
	  // Did we stop too fast?
	  if (fabs(velocity.y) > deadly_velocity)
	    set_action(world->get_action_holder()->get_uaction("splashed"));
	  else if (fabs(velocity.x) > deadly_velocity)
	    {
	      if (pingus_debug_flags & PINGUS_DEBUG_ACTIONS)
		std::cout << "Pingu: x Smashed on ground, jumping" << std::endl;
	    }
	}
      // Reset the velocity
      velocity.x = 0;
      velocity.y = 0;
      falling = 0;
    }
}

// If the Pingu is on ground he can do his walking stuff here.
void 
Pingu::update_walking(float delta)
{
  environment = land;

  if (rel_getpixel(0,-1) & ColMap::WATER)
    {
      //PingusSound::play_wav("SPLASH");
      //status = dead;
      set_paction(world->get_action_holder()->get_uaction("drown"));
      if (pingus_debug_flags & PINGUS_DEBUG_ACTIONS)
	std::cout << "Pingu: Gluck..." << std::endl;
      return;
    }

  if (rel_getpixel(1, 0) == ColMap::NOTHING) 
    { // if infront is free
      pos.x += direction;    
    }
  else 
    { // if infront is a pixel 
      // Pingu is walking up the mountain 
      if (rel_getpixel(1,1) == ColMap::NOTHING) 
	{
	  pos.x += direction;
	  pos.y -= 1;
	} 
      else if (rel_getpixel(1,2) == ColMap::NOTHING)
	{
	  pos.x += direction;
	  pos.y -= 2;
	} 
      else if (rel_getpixel(1,2) & ColMap::BRIDGE) 
	{
	  pos.x += direction;
	  pos.y -=3;
	}
      else
	{ // WALL
	  for (unsigned int i=0; i < persist.size(); ++i) 
	    {
	      if (persist[i]->get_type() & (ActionType)WALL) 
		{
		  if (action.get() && persist[i]->name() == action->name()) 
		    {
		      if (pingus_debug_flags & PINGUS_DEBUG_ACTIONS)
			std::cout << "Pingu: Not using action, we already did." << std::endl;
		    } 
		  else 
		    {
		      if (pingus_debug_flags & PINGUS_DEBUG_ACTIONS)
			std::cout << "Pingu: We are in front of a wall, setting persistant action" << std::endl;
		      set_paction(world->get_action_holder()->get_uaction(persist[i]->name()));
		    }
		  return;
		}
	    }
	  direction.change();
	}
    }
  
  if (rel_getpixel(0, 26) != ColMap::NOTHING && !(rel_getpixel(0, 26) & ColMap::BRIDGE))
    {
      if (pingus_debug_flags & PINGUS_DEBUG_ACTIONS)
	std::cout << "Pingu: Head collision" << std::endl;
      direction.change();
      return;
    }
}

// Draws the pingu on the screen with the given offset
void
Pingu::draw_offset(int x, int y, float s)
{
  char str[64];
  y += 2;

  if (action.get()) 
    {
      action->draw_offset(x, y,s);
    } 
  else 
    {
      if (falling > 3) 
	{
	  if (is_tumbling ()) {
	    tumble.put_screen (int(pos.x + x), int(pos.y + y));
	  } else {
	    faller.put_screen (int(pos.x + x), int(pos.y + y));
	  }
	} 
      else // If not falling
	{
	  if (s == 1.0) 
	    {
	      if (direction.is_left ())
		walker.set_direction (Sprite::LEFT);
	      else
		walker.set_direction (Sprite::RIGHT);
	      
	      walker.put_screen(int(pos.x + x), int(pos.y + y));
	    } 
	}
    }

  if (action_time != -1) 
    {
      sprintf(str, "%d", action_time);
      
      if (s == 1.0) 
	{
	  font->print_center(int(pos.x + x), int(pos.y - 45) + y, str);
	} 
      else if (s > 1.0) 
	{
	  font->print_left(int((pos.x + x) * s), (int)((pos.y - 45 + y) * s) - (int(font->get_text_width(str) * s) / 2),
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
  //assert(colmap);
  return world->get_colmap()->getpixel(int(pos.x + (x * direction)), int((pos.y) - y));
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
  
  if (action.get())
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

shared_ptr<PinguAction>
Pingu::get_action()
{
  return action;
}

int 
Pingu::set_id(int i)
{
  return (id = i);
}

int
Pingu::x_offset(void)
{
  if (action.get())
    return action->x_offset();
  else
    return -16;
}

int
Pingu::y_offset(void)

{
  if (action.get()) 
    return action->y_offset();
  else 
    return -32;
}

void
Pingu::apply_force(CL_Vector arg_v)
{
  velocity += arg_v;
  // Moving the pingu on pixel up, so that the force can take effect
  pos.y -= 1; 
}

bool
Pingu::is_tumbling () const
{
  // If we are going fast enough to get smashed, start tumbling
  if (fabs(velocity.x) > deadly_velocity
      || fabs(velocity.y) > deadly_velocity)
    {
      return true;
    }
  else
    {
      return false;
    }
}

int 
Pingu::get_owner ()
{
  return owner_id;
}

/* EOF */
