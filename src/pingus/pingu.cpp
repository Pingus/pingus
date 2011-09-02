//  Pingus - A free Lemmings clone
//  Copyright (C) 1999 Ingo Ruhnke <grumbel@gmx.de>
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//  
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//  
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "pingus/pingu.hpp"

#include <sstream>

#include "engine/display/scene_context.hpp"
#include "pingus/collision_map.hpp"
#include "pingus/debug.hpp"
#include "pingus/fonts.hpp"
#include "pingus/globals.hpp"
#include "pingus/world.hpp"
#include "pingus/worldobj.hpp"

#include "pingus/actions/angel.hpp"
#include "pingus/actions/basher.hpp"
#include "pingus/actions/blocker.hpp"
#include "pingus/actions/boarder.hpp"
#include "pingus/actions/bomber.hpp"
#include "pingus/actions/bridger.hpp"
#include "pingus/actions/climber.hpp"
#include "pingus/actions/digger.hpp"
#include "pingus/actions/drown.hpp"
#include "pingus/actions/exiter.hpp"
#include "pingus/actions/faller.hpp"
#include "pingus/actions/floater.hpp"
#include "pingus/actions/jumper.hpp"
#include "pingus/actions/laser_kill.hpp"
#include "pingus/actions/miner.hpp"
#include "pingus/actions/rocket_launcher.hpp"
#include "pingus/actions/slider.hpp"
#include "pingus/actions/smashed.hpp"
#include "pingus/actions/splashed.hpp"
#include "pingus/actions/superman.hpp"
#include "pingus/actions/waiter.hpp"
#include "pingus/actions/walker.hpp"

using namespace Actions;

// Init a pingu at the given position while falling
Pingu::Pingu (int arg_id, const Vector3f& arg_pos, int owner) :
  action(),
  countdown_action(),
  wall_action(),
  fall_action(),
  previous_action(ActionName::FALLER),
  id(arg_id),
  action_time(-1),
  owner_id(owner),
  status(PS_ALIVE),
  pos_x(arg_pos.x),
  pos_y(arg_pos.y),
  velocity(0, 0, 0),
  direction()
{
  direction.left ();

  // Initialisize the action, after this step the action ptr will
  // always be valid in the pingu class
  action = create_action(ActionName::FALLER);
}

Pingu::~Pingu ()
{
}

unsigned int
Pingu::get_id ()
{
  return id;
}

bool
Pingu::change_allowed(ActionName::Enum new_action)
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
Pingu::set_pos (const Vector3f& arg_pos)
{
  set_x (arg_pos.x);
  set_y (arg_pos.y);
}

void
Pingu::set_velocity (const Vector3f& velocity_)
{
  velocity = velocity_;
}

// Set the action of the pingu (bridger, blocker, bomber, etc.)
// This function is used by external stuff, like the ButtonPanel, etc
// When you select a function on the button panel and click on a
// pingu, this action will be called with the action name
bool
Pingu::request_set_action(std::shared_ptr<PinguAction> act)
{
  bool ret_val = false;
  assert(act);

  if (status == PS_DEAD)
  {
    pout(PINGUS_DEBUG_ACTIONS) << "Setting action to a dead pingu" << std::endl;
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
        ret_val = false;
        assert(0);
        break;
    }
  }

  if (ret_val) // Action successfull applied
  {
    act->on_successfull_apply();
  }
  else // Action failed to be set
  {
    act->on_failed_apply(this);
  }

  return ret_val;
}

bool
Pingu::request_set_action (ActionName::Enum action_name)
{
  return request_set_action(create_action(action_name));
}

void
Pingu::set_action (ActionName::Enum action_name)
{
  set_action(create_action(action_name));
}

// Sets an action without any checking
void
Pingu::set_action(std::shared_ptr<PinguAction> act)
{
  assert(act);

  previous_action = action->get_type();

  action = act;
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

Pingu::PinguStatus
Pingu::get_status (void) const
{
  return status;
}

Pingu::PinguStatus
Pingu::set_status (PinguStatus s)
{
  return (status = s);
}

// Returns true if the given koordinates are above the pingu
bool
Pingu::is_over (int x, int y)
{
  Vector3f center = get_center_pos ();

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
float
Pingu::dist(int x, int y)
{
  Vector3f p = get_center_pos ();

  return Math::sqrt(((p.x - static_cast<float>(x)) * (p.x - static_cast<float>(x)) + 
                     (p.y - static_cast<float>(y)) * (p.y - static_cast<float>(y))));
}

// Let the pingu do his job (i.e. walk his way)
void
Pingu::update()
{
  if (status == PS_DEAD)
    return;

  // FIXME: Out of screen check is ugly
  /** The Pingu has hit the edge of the screen, a good time to let him
      die. */
  if (rel_getpixel(0, -1) == Groundtype::GP_OUTOFSCREEN)
  {
    //Sound::PingusSound::play_sound("die");
    status = PS_DEAD;
    return;
  }

  // if an countdown action is set, update the countdown time
  if (action_time > -1)
    --action_time;

  if (action_time == 0 && countdown_action)
  {
    set_action(countdown_action);
    // Reset the countdown action handlers
    countdown_action = std::shared_ptr<PinguAction>();
    action_time = -1;
    return;
  }

  action->update();
}

// Draws the pingu on the screen with the given offset
void
Pingu::draw(SceneContext& gc)
{
  char str[16];

  action->draw(gc);

  if (action_time != -1)
  {
    // FIXME: some people preffer a 5-0 or a 9-0 countdown, not sure
    // FIXME: about that got used to the 50-0 countdown [counting is
    // FIXME: in ticks, should probally be in seconds]
    snprintf(str, 16, "%d", action_time/3);

    gc.color().print_center(Fonts::chalk_normal, Vector2i(static_cast<int>(pos_x), static_cast<int>(pos_y) - 48), str);
  }
}

int
Pingu::rel_getpixel(int x, int y)
{
  return WorldObj::get_world()->get_colmap()->getpixel(static_cast<int>(pos_x + static_cast<float>(x * direction)),
                                                       static_cast<int>(pos_y - static_cast<float>(y)));
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

std::string
Pingu::get_name()
{
  return action->get_name();
}

ActionName::Enum
Pingu::get_action ()
{
  return action->get_type();
}

void
Pingu::apply_force (Vector3f arg_v)
{
  velocity += arg_v;
  // Moving the pingu on pixel up, so that the force can take effect
  // FIXME: this should be handled by a state-machine
  --pos_y;
}

Vector3f
Pingu::get_pos () const
{
  return Vector3f(pos_x, pos_y, 0);
}

Vector3f
Pingu::get_center_pos () const
{
  return Vector3f(pos_x, pos_y) + Vector3f(0, -16);
}

int
Pingu::get_owner ()
{
  return owner_id;
}

std::string
Pingu::get_owner_str ()
{
  std::ostringstream ostr;
  ostr << owner_id;
  return ostr.str();
}

bool
Pingu::catchable ()
{
  return action->catchable ();
}

std::shared_ptr<PinguAction>
Pingu::create_action(ActionName::Enum action_)
{
  switch(action_)
  {
    case ActionName::ANGEL:     return std::shared_ptr<PinguAction>(new Angel(this));
    case ActionName::BASHER:    return std::shared_ptr<PinguAction>(new Basher(this));
    case ActionName::BLOCKER:   return std::shared_ptr<PinguAction>(new Blocker(this));
    case ActionName::BOARDER:   return std::shared_ptr<PinguAction>(new Boarder(this));
    case ActionName::BOMBER:    return std::shared_ptr<PinguAction>(new Bomber(this));
    case ActionName::BRIDGER:   return std::shared_ptr<PinguAction>(new Bridger(this));
    case ActionName::CLIMBER:   return std::shared_ptr<PinguAction>(new Climber(this));
    case ActionName::DIGGER:    return std::shared_ptr<PinguAction>(new Digger(this));
    case ActionName::DROWN:     return std::shared_ptr<PinguAction>(new Drown(this));
    case ActionName::EXITER:    return std::shared_ptr<PinguAction>(new Exiter(this));
    case ActionName::FALLER:    return std::shared_ptr<PinguAction>(new Faller(this));
    case ActionName::FLOATER:   return std::shared_ptr<PinguAction>(new Floater(this));
    case ActionName::JUMPER:    return std::shared_ptr<PinguAction>(new Jumper(this));
    case ActionName::LASERKILL: return std::shared_ptr<PinguAction>(new LaserKill(this));
    case ActionName::MINER:     return std::shared_ptr<PinguAction>(new Miner(this));
    case ActionName::ROCKETLAUNCHER: return std::shared_ptr<PinguAction>(new RocketLauncher(this));
    case ActionName::SLIDER:    return std::shared_ptr<PinguAction>(new Slider(this));
    case ActionName::SMASHED:   return std::shared_ptr<PinguAction>(new Smashed(this));
    case ActionName::SPLASHED:  return std::shared_ptr<PinguAction>(new Splashed(this));
    case ActionName::SUPERMAN:  return std::shared_ptr<PinguAction>(new Superman(this));
    case ActionName::WAITER:    return std::shared_ptr<PinguAction>(new Waiter(this));
    case ActionName::WALKER:    return std::shared_ptr<PinguAction>(new Walker(this));
    default: assert(!"Invalid action name provied");
  }
}

/* EOF */
