//  $Id: pingu.cxx,v 1.23 2002/09/04 22:10:47 grumbel Exp $
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
#include "debug.hxx"
#include "string_converter.hxx"
#include "worldobj.hxx"

using namespace Actions;

const float deadly_velocity = 20.0;
int   Pingu::id_counter = 0;

// Init a pingu at the given position while falling
Pingu::Pingu (const CL_Vector& arg_pos, int owner)
            : action(0),
              countdown_action (0),
              wall_action(0),
              fall_action(0),
              id(++id_counter),
              font(PingusResource::load_font("Fonts/numbers", "fonts")),
              action_time(-1),
              owner_id(owner),
              status(PS_ALIVE),
	      pos_x(arg_pos.x),
	      pos_y(arg_pos.y),
              velocity(new CL_Vector(0, 0, 0))
{
  direction.left ();
  set_action(Faller);
}

Pingu::~Pingu ()
{
  delete velocity;
}

int
Pingu::get_id ()
{
  return id;
}

bool
Pingu::change_allowed (ActionName new_action)
{
  assert (action);
  return action->change_allowed (new_action);
}

void
Pingu::set_x (float x)
{
  //std::cout << "Pingu::set_x (" << x << ")" << std::endl;
  pos_x = x; 
}
  
void
Pingu::set_y (float y)
{ 
  //std::cout << "Pingu::set_y (" << y << ")" << std::endl;
  pos_y = y; 
}

void
Pingu::set_pos (float x, float y)
{
  set_x (x);
  set_y (y);
}

void
Pingu::set_pos (const CL_Vector& arg_pos)
{
  set_x (arg_pos.x);
  set_y (arg_pos.y);
}

void
Pingu::set_velocity (const CL_Vector& velocity_)
{
  *velocity = velocity_;
}

// Set the action of the pingu (bridger, blocker, bomber, etc.)
// This function is used by external stuff, like the ButtonPanel, etc
// When you select a function on the button panel and click on a
// pingu, this action will be called with the action name
bool
Pingu::request_set_action (PinguAction* act)
{
  bool ret_val = false;
  assert(act);

  if (status == PS_DEAD)
    {
      pout(PINGUS_DEBUG_ACTIONS) << _("Setting action to a dead pingu") << std::endl;
      ret_val =  false;
    }
  else
    {    
      switch (act->get_activation_mode()) {
  
        case INSTANT:
  
          if (act->get_type() == action->get_type()) 
	    {
	      pout(PINGUS_DEBUG_ACTIONS) << "Pingu: Already have action" << std::endl;
	      ret_val = false;
	    } 
          else if (action->change_allowed(act->get_type()))
	    {
	      pout(PINGUS_DEBUG_ACTIONS) << "setting instant action" << std::endl;
	      act->set_pingu(this);
	      set_action(act);
	      ret_val = true;
	    }
          else
	    {
	      pout(PINGUS_DEBUG_ACTIONS) << "change from action " << action->get_name () << " not allowed" << std::endl;
	      ret_val = false;
	    }
          break;
                
        case WALL_TRIGGERED:
    
          if (wall_action && wall_action->get_type() == act->get_type())
	    {
	      pout(PINGUS_DEBUG_ACTIONS) << "Not using wall action, we have already" << std::endl;
	      ret_val = false;
	    }
          else
	    {
	      pout(PINGUS_DEBUG_ACTIONS) << "Setting wall action" << std::endl;
	      wall_action = act;
	      ret_val = true;
	    }
          break;
    
        case FALL_TRIGGERED:
  
          if (fall_action && fall_action->get_type() == act->get_type())
	    {
	      pout(PINGUS_DEBUG_ACTIONS) << "Not using fall action, we have already" << std::endl;
	      ret_val = false;
	    }
          else
	    {
	      pout(PINGUS_DEBUG_ACTIONS) << "Setting fall action" << std::endl;
	      fall_action = act;
	      ret_val = true;
	    }
          break;

        case COUNTDOWN_TRIGGERED:
  
          if (countdown_action && countdown_action->get_type() == act->get_type())
	    {
	      pout(PINGUS_DEBUG_ACTIONS) << "Not using countdown action, we have already" << std::endl;
	      ret_val = false;
	      break;
	    }
      
          pout(PINGUS_DEBUG_ACTIONS) << "Setting countdown action" << std::endl;
          // We set the action and start the countdown
          action_time = act->activation_time();
          countdown_action = act;
          ret_val = true;
          break;
	  
        default:
          pout(PINGUS_DEBUG_ACTIONS) << "unknown action activation_mode" << std::endl;     
          assert(0);
          ret_val = false;
          break;
      }
    }

  if (ret_val == true) // Action successfull applied
    {
      act->on_successfull_apply (this);
    }
  else // Action failed to be set
    {
      act->on_failed_apply (this);
    }
    
  return ret_val;
}

bool 
Pingu::request_set_action (ActionName action_name)
{
  return request_set_action (PinguActionFactory::instance ()->create (action_name));
}

void
Pingu::set_action (ActionName action_name) 
{
  set_action (PinguActionFactory::instance ()->create (action_name));
}

// Sets an action without any checking
void
Pingu::set_action (PinguAction* act) 
{
  assert(act);
  action = act;
  action->set_pingu(this);
}

bool
Pingu::request_fall_action ()
{
  if (fall_action)
    {
      set_action(fall_action);
      return true;
    }
    
  return false;
}

bool
Pingu::request_wall_action ()
{
  if (wall_action)
    {
      set_action(wall_action);
      return true;
    }

  return false;
}

PinguStatus
Pingu::get_status (void) const
{
  return status;
}

PinguStatus
Pingu::set_status (PinguStatus s)
{
  return (status = s);
}

// Returns true if the given koordinates are above the pingu
bool
Pingu::is_over (int x, int y)
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

  return (pos_x > x1 && pos_x < x2 
	  &&
	  pos_y > y1 && pos_y < y2);
}

// Returns the distance between the Pingu and a given coordinate
double
Pingu::dist (int x, int y)
{
  CL_Vector p = get_center_pos ();
  
  return sqrt(((p.x - x) * (p.x - x) + (p.y - y) * (p.y - y)));
}

// Let the pingu do his job (i.e. walk his way)
void
Pingu::update (float delta)
{
  if (status == PS_DEAD)
    return;

  // FIXME: Out of screen check is ugly
  /** The Pingu has hit the edge of the screen, a good time to let him
      die. */
  if (rel_getpixel(0, -1) == GroundpieceData::GP_OUTOFSCREEN) 
    {
      PingusSound::play_sound("sounds/die.wav");
      status = PS_DEAD;
      return;
    }

  // if an countdown action is set, update the countdown time
  if (action_time > -1) 
    --action_time;

  if (action_time == 0 && countdown_action) 
    {
      std::cout << "COUNTDOWNACTION SET: " << countdown_action->get_name () << std::endl;

      set_action(countdown_action);
      // Reset the countdown action handlers 
      countdown_action = 0;
      action_time = -1;
      return;
    }
  
  action->update(delta);
}

// Draws the pingu on the screen with the given offset
void
Pingu::draw (GraphicContext& gc)
{
  char str[16];

  action->draw (gc);
  
  if (action_time != -1) 
    {
      // FIXME: some people preffer a 5-0 or a 9-0 countdown, not sure
      // FIXME: about that got used to the 50-0 countdown [counting is
      // FIXME: in ticks, should probally be in seconds]
      snprintf(str, 16, "%d", action_time);
      
      font->print_center(static_cast<int>(pos_x), 
			 static_cast<int>(pos_y - 45) + 2, 
			 str);
    }
}

int
Pingu::rel_getpixel (int x, int y)
{
  return WorldObj::get_world()->get_colmap()->getpixel(static_cast<int>(pos_x + (x * direction)), 
                                                       static_cast<int>(pos_y - y));
}

void
Pingu::catch_pingu (Pingu* pingu)
{
  action->catch_pingu(pingu);
}

bool
Pingu::need_catch ()
{
  if (status == PS_DEAD || status == PS_EXITED)
    return false;
  
  return action->need_catch();
}

void
Pingu::set_direction (Direction d)
{
  direction = d;
}

bool
Pingu::is_alive (void)
{
  return (status != PS_DEAD && status != PS_EXITED);
}

PinguAction*
Pingu::get_action ()
{
  return action;
}

void
Pingu::apply_force (CL_Vector arg_v)
{
  *velocity += arg_v;
  // Moving the pingu on pixel up, so that the force can take effect
  // FIXME: this should be handled by a state-machine
  --pos_y; 
}

CL_Vector
Pingu::get_pos () const
{
  return CL_Vector(pos_x, pos_y, 0);
}

CL_Vector
Pingu::get_center_pos () const
{
  return CL_Vector(pos_x, pos_y) + CL_Vector (0, -16); 
}

int 
Pingu::set_id (int i)
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
