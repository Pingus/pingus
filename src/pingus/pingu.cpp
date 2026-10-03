// Pingus - A free Lemmings clone
// Copyright (C) 1999 Ingo Ruhnke <grumbel@gmail.com>
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "pingus/pingu.hpp"

#include <sstream>
#include <utility>

#include <logmich/log.hpp>

#include "engine/display/scene_context.hpp"
#include "pingus/collision_map.hpp"
#include "pingus/fonts.hpp"
#include "pingus/globals.hpp"
#include "pingus/world.hpp"
#include "pingus/ecs/components.hpp"
#include "pingus/pingu_enums.hpp"

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
#include "pingus/actions/slider.hpp"
#include "pingus/actions/smashed.hpp"
#include "pingus/actions/splashed.hpp"
#include "pingus/actions/superman.hpp"
#include "pingus/actions/waiter.hpp"
#include "pingus/actions/walker.hpp"

using namespace pingus::actions;

namespace pingus {

using components::PinguBehavior;
using components::PinguState;
using components::Transform;

Pingu::Pingu(World& world, ecs::Entity entity) :
  m_world(&world),
  m_entity(entity),
  m_state(nullptr),
  m_behavior(nullptr),
  m_transform(nullptr)
{
}

void
Pingu::init()
{
  ecs::Registry& reg = m_world->get_registry();
  m_state = &reg.get<PinguState>(m_entity);
  m_behavior = &reg.get<PinguBehavior>(m_entity);
  m_transform = &reg.get<Transform>(m_entity);

  state().direction.left();
  // Initialisize the action, after this step the action ptr will
  // always be valid in the pingu class
  behavior().action = create_action(ActionName::FALLER);
}

unsigned int
Pingu::get_id() const
{
  return state().id;
}

bool
Pingu::change_allowed(ActionName::Enum new_action)
{
  assert (behavior().action);
  return behavior().action->change_allowed (new_action);
}

float
Pingu::get_x() const
{
  return transform().pos.x();
}

float
Pingu::get_y() const
{
  return transform().pos.y();
}

void
Pingu::set_x (float x)
{
  Transform& t = transform();
  t.pos = Vector2f(x, t.pos.y());
}

void
Pingu::set_y (float y)
{
  Transform& t = transform();
  t.pos = Vector2f(t.pos.x(), y);
}

void
Pingu::set_pos (float x, float y)
{
  set_x (x);
  set_y (y);
}

void
Pingu::set_pos (Vector2f const& arg_pos)
{
  set_x(arg_pos.x());
  set_y(arg_pos.y());
}

glm::vec2
Pingu::get_velocity() const
{
  return state().velocity;
}

void
Pingu::set_velocity (glm::vec2 const& velocity_)
{
  glm::vec2& velocity = state().velocity;
  velocity = velocity_;

  // crude terminal velocity
  velocity.x = std::clamp(velocity.x, -terminal_velocity, terminal_velocity);
  velocity.y = std::clamp(velocity.y, -terminal_velocity, terminal_velocity);
}

Direction&
Pingu::direction() const
{
  return state().direction;
}

// Set the action of the pingu (bridger, blocker, bomber, etc.)
// This function is used by external stuff, like the ButtonPanel, etc
// When you select a function on the button panel and click on a
// pingu, this action will be called with the action name
bool
Pingu::request_set_action(ActionName::Enum action_name)
{
  PinguBehavior& b = behavior();
  bool ret_val = false;

  if (state().status == PS_DEAD)
  {
    log_debug("Setting action to a dead pingu");
    ret_val =  false;
  }
  else
  {
    switch (PinguAction::get_activation_mode(action_name))
    {
      case INSTANT:

        if (action_name == b.action->get_type())
        {
          log_debug("Pingu: Already have action");
          ret_val = false;
        }
        else if (b.action->change_allowed(action_name))
        {
          log_debug("setting instant action");
          set_action(action_name);
          ret_val = true;
        }
        else
        {
          log_debug("change from action {} not allowed", b.action->get_name());
          ret_val = false;
        }
        break;

      case WALL_TRIGGERED:

        if (b.wall_action && b.wall_action->get_type() == action_name)
        {
          log_debug("Not using wall action, we have already");
          ret_val = false;
        }
        else
        {
          log_debug("Setting wall action");
          b.wall_action = create_action(action_name);
          ret_val = true;
        }
        break;

      case FALL_TRIGGERED:

        if (b.fall_action && b.fall_action->get_type() == action_name)
        {
          log_debug("Not using fall action, we have already");
          ret_val = false;
        }
        else
        {
          log_debug("Setting fall action");
          b.fall_action = create_action(action_name);
          ret_val = true;
        }
        break;

      case COUNTDOWN_TRIGGERED:
        {
          if (b.countdown_action && b.countdown_action->get_type() == action_name)
          {
            log_debug("Not using countdown action, we have already");
            ret_val = false;
            break;
          }

          log_debug("Setting countdown action");
          // We set the action and start the countdown
          std::shared_ptr<PinguAction> act = create_action(action_name);
          b.action_time = act->activation_time();
          b.countdown_action = act;
          ret_val = true;
        }
        break;

      default:
        log_debug("unknown action activation_mode");
        ret_val = false;
        assert(0);
        break;
    }
  }

  return ret_val;
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

  PinguBehavior& b = behavior();
  b.previous_action = b.action->get_type();
  b.action = std::move(act);
}

bool
Pingu::request_fall_action()
{
  PinguBehavior& b = behavior();
  if (b.fall_action)
  {
    set_action(b.fall_action);
    return true;
  }

  return false;
}

bool
Pingu::request_wall_action()
{
  PinguBehavior& b = behavior();
  if (b.wall_action)
  {
    set_action(b.wall_action);
    return true;
  }

  return false;
}

PinguAction*
Pingu::get_wall_action()
{
  return behavior().wall_action.get();
}

PinguAction*
Pingu::get_fall_action()
{
  return behavior().fall_action.get();
}

Pingu::PinguStatus
Pingu::get_status (void) const
{
  return state().status;
}

Pingu::PinguStatus
Pingu::set_status (PinguStatus s)
{
  return (state().status = s);
}

// Returns true if the given koordinates are above the pingu
bool
Pingu::is_over (float x, float y) const
{
  Vector2f center = get_center_pos();

  return (center.x() + 16 > x && center.x() - 16 < x &&
          center.y() + 16 > y && center.y() - 16 < y);
}

bool
Pingu::is_inside (float x1, float y1, float x2, float y2) const
{
  assert (x1 < x2);
  assert (y1 < y2);

  Vector2f const pos = get_pos();
  return (pos.x() > x1 && pos.x() < x2
          &&
          pos.y() > y1 && pos.y() < y2);
}

// Returns the distance between the Pingu and a given coordinate
float
Pingu::dist(float x, float y) const
{
  Vector2f p = get_center_pos();

  return std::sqrt(((p.x() - x) * (p.x() - x) +
                    (p.y() - y) * (p.y() - y)));
}

// Let the pingu do his job (i.e. walk his way)
void
Pingu::update()
{
  PinguState& s = state();
  PinguBehavior& b = behavior();

  if (s.status == PS_DEAD)
    return;

  // FIXME: Out of screen check is ugly
  /** The Pingu has hit the edge of the screen, a good time to let him
      die. */
  if (rel_getpixel(0, -1) == Groundtype::GP_OUTOFSCREEN)
  {
    //Sound::PingusSound::play_sound("die");
    s.status = PS_DEAD;
    return;
  }

  // if an countdown action is set, update the countdown time
  if (b.action_time > -1)
    --b.action_time;

  if (b.action_time == 0 && b.countdown_action)
  {
    set_action(b.countdown_action);
    // Reset the countdown action handlers
    b.countdown_action = std::shared_ptr<PinguAction>();
    b.action_time = -1;
    return;
  }

  // keep the action alive, it may replace itself during update()
  std::shared_ptr<PinguAction> action = b.action;
  action->update();
}

// Draws the pingu on the screen with the given offset
void
Pingu::draw(SceneContext& gc)
{
  PinguBehavior& b = behavior();

  b.action->draw(gc);

  if (b.action_time != -1)
  {
    // FIXME: some people preffer a 5-0 or a 9-0 countdown, not sure
    // FIXME: about that got used to the 50-0 countdown [counting is
    // FIXME: in ticks, should probally be in seconds]
    char str[16];
    snprintf(str, 16, "%d", b.action_time/3);
    gc.color().print_center(pingus::fonts::chalk_normal, Vector2i(get_xi(), get_yi() - 48), str);
  }
}

int
Pingu::rel_getpixel(int x, int y) const
{
  Vector2f const pos = get_pos();
  return m_world->get_colmap()->getpixel(static_cast<int>(pos.x() + static_cast<float>(x * direction())),
                                         static_cast<int>(pos.y() - static_cast<float>(y)));
}

void
Pingu::catch_pingu (Pingu* pingu)
{
  behavior().action->catch_pingu(pingu);
}

bool
Pingu::need_catch()
{
  if (state().status == PS_DEAD || state().status == PS_EXITED)
    return false;

  return behavior().action->need_catch();
}

void
Pingu::set_direction (Direction const& d)
{
  state().direction = d;
}

std::string
Pingu::get_name()
{
  return behavior().action->get_name();
}

ActionName::Enum
Pingu::get_action()
{
  return behavior().action->get_type();
}

ActionName::Enum
Pingu::get_previous_action() const
{
  return behavior().previous_action;
}

void
Pingu::apply_force (glm::vec2 const& arg_v)
{
  state().velocity += arg_v;
  // Moving the pingu on pixel up, so that the force can take effect
  // FIXME: this should be handled by a state-machine
  set_y(get_y() - 1);
}

Vector2f
Pingu::get_pos() const
{
  return transform().pos;
}

Vector2f
Pingu::get_center_pos() const
{
  return behavior().action->get_center_pos();
}

int
Pingu::get_owner() const
{
  return state().owner_id;
}

std::string
Pingu::get_owner_str() const
{
  std::ostringstream ostr;
  ostr << state().owner_id;
  return ostr.str();
}

bool
Pingu::catchable()
{
  return behavior().action->catchable();
}

std::shared_ptr<PinguAction>
Pingu::create_action(ActionName::Enum action_)
{
  switch(action_)
  {
    case ActionName::ANGEL:     return std::make_shared<Angel>(this);
    case ActionName::BASHER:    return std::make_shared<Basher>(this);
    case ActionName::BLOCKER:   return std::make_shared<Blocker>(this);
    case ActionName::BOARDER:   return std::make_shared<Boarder>(this);
    case ActionName::BOMBER:    return std::make_shared<Bomber>(this);
    case ActionName::BRIDGER:   return std::make_shared<Bridger>(this);
    case ActionName::CLIMBER:   return std::make_shared<Climber>(this);
    case ActionName::DIGGER:    return std::make_shared<Digger>(this);
    case ActionName::DROWN:     return std::make_shared<Drown>(this);
    case ActionName::EXITER:    return std::make_shared<Exiter>(this);
    case ActionName::FALLER:    return std::make_shared<Faller>(this);
    case ActionName::FLOATER:   return std::make_shared<Floater>(this);
    case ActionName::JUMPER:    return std::make_shared<Jumper>(this);
    case ActionName::LASERKILL: return std::make_shared<LaserKill>(this);
    case ActionName::MINER:     return std::make_shared<Miner>(this);
    case ActionName::SLIDER:    return std::make_shared<Slider>(this);
    case ActionName::SMASHED:   return std::make_shared<Smashed>(this);
    case ActionName::SPLASHED:  return std::make_shared<Splashed>(this);
    case ActionName::SUPERMAN:  return std::make_shared<Superman>(this);
    case ActionName::WAITER:    return std::make_shared<Waiter>(this);
    case ActionName::WALKER:    return std::make_shared<Walker>(this);
    default: assert(false && "Invalid action name provied"); return {};
  }
}

} // namespace pingus

/* EOF */
