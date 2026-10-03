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

#ifndef HEADER_PINGUS_PINGUS_PINGU_HPP
#define HEADER_PINGUS_PINGUS_PINGU_HPP

#include <memory>
#include <string>

#include <glm/glm.hpp>

#include "ecs/registry.hpp"
#include "math/vector2f.hpp"
#include "pingus/direction.hpp"
#include "pingus/action_name.hpp"
#include "fwd.hpp"

namespace pingus {

namespace components {
struct PinguBehavior;
struct PinguState;
struct Transform;
} // namespace components

/** Interface to one of the penguins walking around in the World.

    Each pingu is an entity, its state lives in the components
    Transform (position), PinguState (id, owner, status, direction,
    velocity) and PinguBehavior (the action state machine). Pingu is the
    component that gives the actions, the GUI and the server a
    convenient interface to them; it holds no state of its own besides
    its entity. The per tick update and drawing are done by the systems
    in pingus/ecs/pingus.cpp. */
class Pingu
{
public:
  /** The Pingus Status shows the current status of a Pingu, as
      displayed in the PingusCounter pannel. PS_DEAD are pingus that got
      killed, PS_ALIVE are pingus that are still active in the world and
      PS_EXITED are pingus that successfully finished a level

      FIXME: different subvalues of PS_DEAD might be usefull (drowned,
      FIXME: splashed, smashed, etc.) */
  enum PinguStatus { PS_ALIVE, PS_EXITED, PS_DEAD };

private:
  World* m_world;
  ecs::Entity m_entity;

  /** The entity's components, cached by init(). Pingu entities are never
      destroyed during a level and the registry keeps component addresses
      stable, so these stay valid. */
  components::PinguState* m_state;
  components::PinguBehavior* m_behavior;
  components::Transform* m_transform;

private:
  void set_action(std::shared_ptr<PinguAction>);
  std::shared_ptr<PinguAction> create_action(ActionName::Enum action);

  components::PinguState& state() const { return *m_state; }
  components::PinguBehavior& behavior() const { return *m_behavior; }
  components::Transform& transform() const { return *m_transform; }

public:
  /** Create the interface for the pingu entity, the entity needs its
      PinguState, PinguBehavior and Transform components, see
      PinguHolder::create_pingu() */
  Pingu(World& world, ecs::Entity entity);

  /** Give the pingu its initial action, needs to be called once the
      Pingu has its final address in the registry */
  void init();

  World* get_world() const { return m_world; }
  ecs::Entity get_entity() const { return m_entity; }

  /** Return the logical pingus position, this is the position which
      is used for collision detection to the ground (the pingus
      feet) */
  Vector2f get_pos() const;

  /** Returns the visible position of the pingu, the graphical center
      of the pingu. */
  Vector2f get_center_pos() const;

  float get_x() const;
  float get_y() const;

  int get_xi() const { return static_cast<int>(get_x()); }
  int get_yi() const { return static_cast<int>(get_y()); }

  /** Checks if this action allows to be overwritten with the given new action */
  bool change_allowed (ActionName::Enum new_action);

  /// Return the status of the pingu
  PinguStatus get_status (void) const;

  PinguStatus set_status (PinguStatus);

  /** The descriptive name of the action, this is used in the
      CaputreRectangle, so it can contain more than just the name
      (number of blocks, etc.) */
  std::string get_name();

  /// Returns the unique id of the pingu
  unsigned int get_id (void) const;

  /// Set the pingu to the given coordinates
  void set_pos (float x, float y);
  void set_pos (int x, int y) { set_pos(static_cast<float>(x), static_cast<float>(y)); }

  void set_x (float x);
  void set_y (float y);

  /// Set the pingu to the given coordinates
  void set_pos (Vector2f const& arg_pos);

  glm::vec2 get_velocity() const;
  void set_velocity (glm::vec2 const& velocity_);

  /** The direction the pingu is walking in */
  Direction& direction() const;

  // Set the pingu in the gives direction
  void set_direction (Direction const& d);

  /** Request an action to be set to the pingu, if its a persistent
      action, it will be hold back for later execution, same with a
      timed action, normal action will be applied if the current
      action allows that. */
  bool request_set_action (ActionName::Enum action_name);

  /** Set an action without any checking, the action will take
      instantly control. */
  void set_action (ActionName::Enum action_name);

  /// set the wall action if we have one
  bool request_wall_action();

  /// set the fall action if we have one
  bool request_fall_action();

  PinguAction* get_wall_action();
  PinguAction* get_fall_action();

  /** Returns the `color' of the colmap in the walking direction
      Examples:
      (0, -1) is the pixel under the pingu
      (1, 0)  is the pixel in front of the pingu
  */
  int  rel_getpixel (int x, int y) const;

  /** Let the pingu catch another pingu, so that an action can be
      applied (i.e. let a blocker change the direction f another
      pingu) */
  void catch_pingu (Pingu* pingu);

  /** Returns true if the pingu needs to catch another pingu */
  bool need_catch();

  void draw (SceneContext& gc);
  void apply_force(glm::vec2 const&);

  void update();

  /** @return The owner_id of the owner, only used in multiplayer
      configurations, ought to be 0 in single player */
  int get_owner() const;

  /** @return The owner_id as a string. Only used in multiplayer
      configurations, ought to be "0" in single player */
  std::string get_owner_str() const;

  bool   is_over (float x, float y) const;

  bool   is_inside (float x1, float y1, float x2, float y2) const;

  float dist (float x, float y) const;

  /** Return true if the pingu can be caught with the mouse and
      another action can be applied, false otherwise (exiter,
      splashed, etc.) */
  bool catchable();

  /** @return the name of the action the Pingu currently has */
  ActionName::Enum get_action();

  /** @return the action that was active before the action returned by
      get_action() took place. This is used in a few situations where
      an action needs to now what the Pingu was doing before the
      action took place (faller->bomber translation is different
      then Walker->bomber, etc.). */
  ActionName::Enum get_previous_action() const;
};

} // namespace pingus

#endif

/* EOF */
