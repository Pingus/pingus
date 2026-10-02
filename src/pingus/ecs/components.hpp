// SPDX-FileCopyrightText: 2026 Ingo Ruhnke <grumbel@gmail.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef HEADER_PINGUS_PINGUS_ECS_COMPONENTS_HPP
#define HEADER_PINGUS_PINGUS_ECS_COMPONENTS_HPP

#include <string>
#include <vector>

#include "engine/display/sprite.hpp"
#include "math/color.hpp"
#include "math/vector2f.hpp"
#include "pingus/animation_clock.hpp"
#include "pingus/direction.hpp"
#include "pingus/groundtype.hpp"
#include "pingus/res_descriptor.hpp"

/** Components of level object entities. Components are plain data, the
    behavior lives in the systems (see systems.hpp). */
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
struct AnimatedSprite {};

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

/** Covers the whole screen, levels without one get a default background */
struct SolidBackground {};

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
  Sprite sprite;
  AnimationClock clock;
  bool killing = false;
};

/** Looks like an exit, smashes pingus in the TriggerZone */
struct FakeExit
{
  Sprite sprite;
  AnimationClock clock;
  bool smashing = false;
};

/** Kills one pingu in the TriggerZone, then animates the kill in the
    pingu's direction */
struct Guillotine
{
  Sprite sprite_kill_left;
  Sprite sprite_kill_right;
  Sprite sprite_idle;
  AnimationClock kill_clock;
  AnimationClock idle_clock;
  Direction direction = {};
  bool killing = false;
};

/** Swings down and up continuously, splashes pingus below at the bottom */
struct Hammer
{
  Sprite sprite;
  int frame_count;
  bool down = true;
  int count = 0;
};

/** Zaps a pingu in the TriggerZone */
struct LaserExit
{
  Sprite sprite;
  AnimationClock clock;
  bool killing = false;
};

/** Smashes down when a pingu walks under it, splashing everything below */
struct Smasher
{
  Sprite sprite;
  bool smashing = false;
  bool downwards = false;
  int count = 0;
};

} // namespace pingus::components

#endif

/* EOF */
