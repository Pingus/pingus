//  $Id: Pingu.cc,v 1.1 2000/02/04 23:45:18 mbn Exp $
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
#include "actions/smashed.hh"
#include "FVec.hh"

// Create the static objects of the class
ColMap*             Pingu::colmap;
PinguMap*           Pingu::map;

bool                Pingu:: init;
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
  v.x = 0;
  v.y = 0;
  old_v = v;
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
// The `t' parameter set the amount of time which have to be passed
// before the action is set. A value of `-1' activates the action
// directly.
int
Pingu::set_action(PinguAction* act, int t)
{
  assert(act);

  if (status == dead)
    return 0;

  act->set_pingu(this);

  // Use the activation time of the action
  if (t == -2)
    {
      //action_time = act->activation_time();
      t = act->activation_time();
    }

  if (act->get_type() ^ (ActionType)ONCE) 
    {
      cout << "Pingu: Found some persistant action" << endl;
      cout << "Pingu: Action is FALL: " 
	   << int(act->get_type() & (ActionType)FALL) << endl;
      cout << "Pingu: Action is WALL: " 
	   << int(act->get_type() & (ActionType)WALL) << endl;

      for(vector<PinguAction*>::iterator i = persist.begin(); i != persist.end(); i++)
	{
	  if ((*i)->name() == act->name()) 
	    {
	      cout << "Not using action, we have allready" << endl;
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
    
      if (t == -1)
	{ // Immediately activate the action
	  if (action)
	    {
	      if (act->name() == action->name())
		{
		  cout << "Allready have action" << endl;
		  return false;
		}
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
	  action_time = t;
	  sec_action = act;
	}
      return true;
    }
}

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
      status = dead;
      return;
    }

  if (environment == sky && action == 0 && rel_getpixel(0, -1) == ColMap::NOTHING) 
    {
      for (unsigned int i=0; i < persist.size(); ++i) 
	{
	  if (persist[i]->get_type() & (ActionType)FALL) 
	    {
	      if (action && persist[i]->name() == action->name()) 
		{
		  cout << "Not using action, we have allready" << endl;
		} 
	      else 
		{
		  set_paction(ActionHolder::get_uaction(persist[i]->name()));
		}
	    }
	}
    }
  
  if (action) 
    { // if we have an action, let_move() it
      action->let_move();
    }
  else 
    { // if we have no action, let the pingu walk
      // Forces are only applied if the pingu isn't going something else
      if (force_vectors)
	{
	  // Apply forces
	  CL_Vector tmpvec;
	  tmpvec.x = x_pos;
	  tmpvec.y = y_pos;
      
	  // Apply all forces
	  v = ForcesHolder::apply_forces(tmpvec,v);
    
	  // Update x and y by moving the penguin to it's target *slowly*
	  // and checking if the penguin has hit the bottom at each loop
	  CL_Vector newp = v;
	  
	  while((rel_getpixel(0, -1) == ColMap::NOTHING) 
		&& !((newp.x < 1 && newp.x > -1) && (newp.y < 1 && newp.y > -1)))
	    {
	      if (!(newp.x < 1 && newp.x > -1)) 
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

	      if (!(newp.y < 1 && newp.y > -1)) 
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
	      /*
	      if ((rel_getpixel(0, -1) != ColMap::NOTHING) 
		  || ((newp.x < 1 && newp.x > -1) && (newp.y < 1 && newp.y > -1))) 
		{
		  break;
		  }*/
	    }
    
	  if (rel_getpixel(0, -1) == ColMap::NOTHING)
	    { // if pingu is not on ground
	      ++falling;
	      old_v = v;
	  
	      if (falling > 3) 
		environment = sky;
	  
	      // If we are going fast enough to get smashed set falling to 80
	      if (((v.x > 15) || (v.x < - 15))
		  || ((v.y > 15) || (v.y < - 15))) 
		{
		  falling = 80;
		}
	    } 
	  else
	    {
	      // Did we stop too fast?
	      if (((v.x - old_v.x > 15) || (v.x - old_v.x < - 15))
		  || ((v.y - old_v.y > 15) || (v.y - old_v.y < - 15))) 
		{
		  // FIXME: This is a LinuxTag Hack and should be replaced
		  // with a real ground smashing action! 
		  set_action(new Smashed);
		}
	  
	      // Reset the velocity
	      v.x = 0;
	      v.y = 0;
	  
	      falling = 0;
	    }
	}

      if (rel_getpixel(0, -1) == ColMap::NOTHING)
	{ // if pingu is not on ground
	  // Replaced by the force vectors
	  if (!force_vectors)
	    {
	      ++y_pos;
	      if (rel_getpixel(0, -1) == ColMap::NOTHING)
		++y_pos;
	      ++falling;
	      if (falling > 3) {
		environment = sky;
	      }
	    }
	} 
      else
	{ // if pingu is on ground
	  if (!force_vectors) 
	    {
	      if (falling > 75) 
		{
		  // FIXME: This is a LinuxTag Hack and should be replaced
		  // with a real ground smashing action! 
		  set_action(new Smashed);
		}
	      falling = 0;
	    }
	  environment = land;

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
	      else if (rel_getpixel(1,2) == ColMap::BRIDGE) 
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
			      cout << "Not using action, we have allready" << endl;
			    } 
			  else 
			    {
			      cout << "We are infront of a wall, setting persistant action" << endl;
			      set_paction(ActionHolder::get_uaction(persist[i]->name()));
			    }
			  return;
			} 
		    }
		  direction.change();
		}
	    }
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
	  
	  if (falling > 75)
	    surf = tumble;
	  else
	    surf = faller;
	  
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
