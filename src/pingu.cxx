//  $Id: pingu.cxx,v 1.7 2002/06/21 16:50:20 torangan Exp $
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

#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <ClanLib/Display/Font/font.h>
#include "globals.hxx"
#include "world.hxx"
#include "pingu.hxx"
#include "pingus_resource.hxx"
#include "sound.hxx"
#include "col_map.hxx"
#include "pingu_action.hxx"
#include "pingu_action_factory.hxx"
#include "my_gettext.hxx"

const float deadly_velocity = 20.0;
int   Pingu::id_counter = 0;

// Init a pingu at the given position while falling
Pingu::Pingu(const CL_Vector& arg_pos, int owner)
  : countdown_action (0),
    id (++id_counter),
    font (PingusResource::load_font("Fonts/numbers", "fonts")),
    status (PS_ALIVE),
    environment (ENV_LAND),
    owner_id (owner),
    pos (arg_pos)
{
  action_time = -1;

  direction.left ();

  // Set the velocity to zero
  velocity.x = 0;
  velocity.y = 0;

  set_action("faller");
}

Pingu::~Pingu()
{
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
bool
Pingu::set_action(PinguAction* act)
{
  assert(act);

  if (status == PS_DEAD)
    {
      if (pingus_debug_flags & PINGUS_DEBUG_ACTIONS)
	std::cout << _("Setting action to a dead pingu") << std::endl;
      return false;
    }

  act->set_pingu(this);

  // check for persistent actions
  if (act->get_type() != (ActionType)ONCE) 
    {
      if (pingus_debug_flags & PINGUS_DEBUG_ACTIONS)
	{
	  std::cout << "Pingu: Found some persistant action" << std::endl;
	  std::cout << "Pingu: Action is FALL: " 
		    << int(act->get_type() & (ActionType)FALL) << std::endl;
	  std::cout << "Pingu: Action is WALL: " 
		    << int(act->get_type() & (ActionType)WALL) << std::endl;
	}
      
      for(std::vector<PinguAction*>::iterator i = persist.begin(); i != persist.end(); i++)
	{
	  if ((*i)->get_name() == act->get_name()) 
	    {
	      if (pingus_debug_flags & PINGUS_DEBUG_ACTIONS)
		std::cout << "Not using action, we have allready" << std::endl;
	      return false;
	    }
	}

      persist.push_back(act);
      return true;
    } 
  else 
    {
      if (act->get_environment() == (PinguEnvironment)ENV_ALWAYS)
	{
	  action = act;
	  return true;
	}

      // if environment is bad return
      if (!(act->get_environment() & environment))
	{ 
	  return false; 
	}
    
      if (act->activation_time() == -1)
	{ // Immediately activate the action
	  if (action && (action->get_name() == act->get_name()))
	    {
	      if (pingus_debug_flags & PINGUS_DEBUG_ACTIONS)
		std::cout << "Pingu: Already have action" << std::endl;
	      return false;
	    }
	  action = act;
	  return true;	    
	}
      else 
	{ // Use the activation time, given by t
	  if (countdown_action && countdown_action->get_name() == act->get_name())
	    {
	      return false;
	    }
	  action_time = act->activation_time();
	  countdown_action = act;
	  //PingusSound::play_sound("sounds/ohno.wav");
	}
      return true;
    }
}

void 
Pingu::set_action (const std::string& action_name)
{
  set_action (PinguActionFactory::instance ()->create (action_name));
}

void
Pingu::set_paction(const std::string& action_name) 
{
  set_paction (PinguActionFactory::instance ()->create (action_name));
}

// Sets an action without any checking
void
Pingu::set_paction(PinguAction* act) 
{
  assert(act);
  action = act;
  action->set_pingu(this);
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
  CL_Vector center = get_center_pos ();

  return (center.x + 16 > x && center.x - 16 < x &&
	  center.y + 16 > y && center.y - 16 < y);
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
  CL_Vector p = get_center_pos ();
  
  return sqrt(((p.x - x) * (p.x - x) + (p.y - y) * (p.y - y)));
}

void
Pingu::update_persistent(float /*delta*/)
{
  // set floater action if required
  if (environment == ENV_AIR && action->get_type() != (ActionType)FALL && rel_getpixel(0, -1) == ColMap::NOTHING) 
    {
      for (unsigned int i=0; i < persist.size(); ++i) 
	{
	  if (persist[i]->get_type() == (ActionType)FALL) 
	    {
              set_paction(persist[i]->get_name());
	    }
	}
    }
}

// Let the pingu do his job (i.e. walk his way)
// FIXME: This function is *much* too large, it needs a real cut down
// into smaller pieces.  
void
Pingu::update(float delta)
{
  if (status == PS_DEAD)
    return;

  // FIXME: Out of screen check is ugly
  /** The Pingu has hit the edge of the screen, a good time to let him
      die. */
  if (rel_getpixel(0, -1) == ColMap::OUTOFSCREEN) 
    {
      PingusSound::play_sound("sounds/die.wav");
      status = PS_DEAD;
      return;
    }

  if (action_time > -1) 
    --action_time;

  if ( !action_time && countdown_action) 
    {
      action = countdown_action;
      action->set_pingu(this);
    }
  
  if ( !action || action->is_finished) 
    {
      update_action(delta);
    }

  update_persistent(delta);
  action->update(delta);
}

// Check if the pingu is on ground and then do something.
void 
Pingu::update_action(float /*delta*/)
{
  std::cout << "Pingu: No action set, setting action." << std::endl;
  if (rel_getpixel(0,-1) == ColMap::NOTHING)
    set_action("faller");
  else
    set_action("walker");
}

// Draws the pingu on the screen with the given offset
void
Pingu::draw_offset(int x, int y, float s)
{
  char str[64];
  y += 2;

  action->draw_offset(x, y,s);
  
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

int
Pingu::rel_getpixel(int x, int y)
{
  return world->get_colmap()->getpixel(int(pos.x + (x * direction)), int((pos.y) - y));
}

void
Pingu::catch_pingu(Pingu* pingu)
{
  action->catch_pingu(pingu);
}

bool
Pingu::need_catch()
{
  if (status == PS_DEAD)
    return false;
  
  return action->need_catch();
}

void
Pingu::set_direction(Direction d)
{
  direction = d;
}

bool
Pingu::is_alive(void)
{
  if (status != PS_DEAD && status != PS_EXITED)
    return true;
  else 
    return false;
}

PinguAction*
Pingu::get_action()
{
  return action;
}

void
Pingu::apply_force(CL_Vector arg_v)
{
  velocity += arg_v;
  // Moving the pingu on pixel up, so that the force can take effect
  // FIXME: this should be handled by a state-machine
  pos.y -= 1; 
}

CL_Vector
Pingu::get_center_pos ()
{
  return pos + CL_Vector (0, -16); 
}

int 
Pingu::set_id(int i)
{
  return (id = i);
}

int 
Pingu::get_owner ()
{
  return owner_id;
}

bool 
Pingu::catchable ()
{
  return action->catchable ();
}

/* EOF */
