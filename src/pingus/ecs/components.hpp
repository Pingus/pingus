// SPDX-FileCopyrightText: 2026 Ingo Ruhnke <grumbel@gmail.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef HEADER_PINGUS_PINGUS_ECS_COMPONENTS_HPP
#define HEADER_PINGUS_PINGUS_ECS_COMPONENTS_HPP

#include <map>
#include <memory>
#include <string>
#include <vector>

#include <glm/glm.hpp>

#include "ecs/registry.hpp"
#include "engine/display/sprite.hpp"
#include "math/color.hpp"
#include "math/vector2f.hpp"
#include "pingus/animation_clock.hpp"
#include "pingus/animation_set.hpp"
#include "pingus/collision_mask.hpp"
#include "pingus/direction.hpp"
#include "pingus/groundtype.hpp"
#include "pingus/pingu.hpp"
#include "pingus/pingu_action.hpp"
#include "pingus/res_descriptor.hpp"

/** Components of the level object and pingu entities. Components are
    plain data, the behavior lives in the systems (see systems.hpp). */
namespace pingus::components {

/** Position and drawing depth of an object in the world */
struct Transform
{
  Vector2f pos;
  float z_index;
};

/** Sprite drawn at the entity's position */
struct SpriteRender
{
  Sprite sprite;

  /** Pass the entity's z-index on to the drawing request instead of
      drawing at z 0 */
  bool use_z_index = false;
};

/** Advance the SpriteRender animation every tick, for purely decorative
    animations that game logic never looks at */
struct LoopingSprite {};

/** Shows one animation of an AnimationSet at the entity's position. Game
    logic only picks the animation, direction and frame; the render
    system loads and draws the sprites. */
struct AnimatedSprite
{
  std::shared_ptr<AnimationSet const> set;
  std::string animation;
  Direction direction = {};
  int frame = 0;
  bool visible = true;

  /** Sprites loaded by the render system, by animation and direction */
  std::map<std::string, Sprite> sprites = {};
};

/** Drawn into the ground and collision map at startup, the entity is
    destroyed afterwards */
struct Groundpiece
{
  ResDescriptor desc;
  Groundtype::GPType gptype;
};

/** Water or lava, a tiled animated sprite that is water in the
    collision map */
struct Liquid
{
  Sprite sprite;

  /** Width in pixels */
  int width;
};

struct SolidColorBackground
{
  Color color;
};

/** Tiled, optionally scrolling and parallax background image */
struct SurfaceBackground
{
  Sprite sprite;
  float para_x;
  float para_y;
  float scroll_x;
  float scroll_y;

  /** Current scroll offset */
  float scroll_ox = 0.0f;
  float scroll_oy = 0.0f;
};

struct Star
{
  Sprite sprite;
  float x_pos;
  float y_pos;
  float x_add;
  float y_add;
};

struct StarfieldBackground
{
  std::vector<Star> stars;
};

/** Rectangle relative to the entity's position. A pingu is inside when
    its position lies strictly within, see pingus_in_zone(). */
struct TriggerZone
{
  float x1;
  float y1;
  float x2;
  float y2;
};

/** Symbol drawn for the entity on the small map */
struct SmallmapSymbol
{
  Sprite sprite;
};

/** Spikes come out when a pingu walks into the TriggerZone and kill the
    pingus close by at frame 3 */
struct Spike
{
  AnimationClock clock;
  bool killing = false;
};

/** Looks like an exit, smashes pingus in the TriggerZone */
struct FakeExit
{
  AnimationClock clock;
  bool smashing = false;
};

/** Kills one pingu in the TriggerZone, then animates the kill in the
    pingu's direction */
struct Guillotine
{
  AnimationClock kill_clock;
  AnimationClock idle_clock;
  Direction direction = {};
  bool killing = false;
};

/** Swings down and up continuously, splashes pingus below at the bottom */
struct Hammer
{
  int frame_count;
  bool down = true;
  int count = 0;
};

/** Zaps a pingu in the TriggerZone */
struct LaserExit
{
  AnimationClock clock;
  bool killing = false;
};

/** Smashes down when a pingu walks under it, splashing everything below */
struct Smasher
{
  bool smashing = false;
  bool downwards = false;
  int count = 0;
};

/** Player the object belongs to, in multiplayer levels */
struct Owner
{
  int owner_id;
};

/** Name other objects refer to the entity by ("id" in level files) */
struct ObjectId
{
  std::string id;
};

/** Releases the pingus of its owner */
struct Entrance
{
  enum class Direction { LEFT, MISC, RIGHT };

  Direction direction;
  int release_rate;
  int last_release;

  /** For Direction::MISC, alternate between right (first) and left */
  bool last_was_right = false;
};

/** Lets pingus of its owner in the TriggerZone exit */
struct Exit
{
  ResDescriptor desc;
  Sprite sprite;
  Sprite flag;
};

/** Moves pingus in the TriggerZone to its target */
struct Teleporter
{
  AnimationClock clock;
  std::string target_id;
  ecs::Entity target = ecs::null_entity;
};

struct TeleporterTarget
{
  AnimationClock clock;
};

/** Solid ground that melts while pingus walk on it */
struct IceBlock
{
  Sprite sprite;
  std::shared_ptr<CollisionMask> cmap;
  float thickness = 1.0f;
  bool finished = false;
  int last_contact = 0;
};

/** Moves pingus in the TriggerZone to the left */
struct ConveyorBelt
{
  Sprite left;
  Sprite middle;
  Sprite right;
  int width;
  float speed;
};

/** Door that opens when its switch is triggered */
struct SwitchDoor
{
  Sprite box;
  Sprite tile;
  std::shared_ptr<CollisionMask> box_cmap;
  std::shared_ptr<CollisionMask> tile_cmap;
  int height;
  int current_height;
  bool opening = false;
};

/** Opens the door with the given id when a pingu enters the TriggerZone */
struct SwitchDoorSwitch
{
  Sprite sprite;
  std::string target_id;
  ecs::Entity door = ecs::null_entity;
  bool triggered = false;
};

/** Adds snow particles, intensity is the number of flakes per tick */
struct SnowGenerator
{
  float intensity;
};

/** Adds rain particles and the occasional thunder flash */
struct RainGenerator
{
  bool do_thunder = false;
  float thunder_count = 0.0f;
  float waiter_count = 0.0f;
};

// Pingus, see Pingu and PinguHolder

/** Identity and physical state of a pingu, its position is in the
    entity's Transform */
struct PinguState
{
  /** Unique id, used to refer to the pingu in demo files */
  unsigned int id;

  /** Player the pingu belongs to, in multiplayer levels */
  int owner_id;

  Pingu::PinguStatus status = Pingu::PS_ALIVE;
  Direction direction = {};
  glm::vec2 velocity = {0.0f, 0.0f};
};

/** The pingu's action state machine */
struct PinguBehavior
{
  /** The action currently in control */
  std::shared_ptr<PinguAction> action = {};

  /** Action taking over once action_time reaches 0 (bomber) */
  std::shared_ptr<PinguAction> countdown_action = {};

  /** Actions taking over when the pingu hits a wall or starts falling */
  std::shared_ptr<PinguAction> wall_action = {};
  std::shared_ptr<PinguAction> fall_action = {};

  /** Type of the action before the current one */
  ActionName::Enum previous_action = ActionName::FALLER;

  /** Ticks until countdown_action is triggered, -1 for none */
  int action_time = -1;
};

/** Pingus that are alive and in the level, removed once a pingu died or
    exited */
struct ActivePingu {};

/** Drawing state of a pingu, used by draw_pingus(): the owner's
    animation set, the loaded sprites and which "once" layers of the
    current action's look were already shown */
struct PinguView
{
  std::shared_ptr<AnimationSet const> set = {};
  std::map<std::string, Sprite> sprites = {};

  /** The action shown_once belongs to */
  std::shared_ptr<PinguAction> action = {};
  std::vector<std::string> shown_once = {};

  /** Reused for PinguAction::get_look() */
  PinguLook look = {};
};

} // namespace pingus::components

#endif

/* EOF */
