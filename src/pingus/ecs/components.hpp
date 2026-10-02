// SPDX-FileCopyrightText: 2026 Ingo Ruhnke <grumbel@gmail.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef HEADER_PINGUS_PINGUS_ECS_COMPONENTS_HPP
#define HEADER_PINGUS_PINGUS_ECS_COMPONENTS_HPP

#include <string>
#include <vector>

#include "engine/display/sprite.hpp"
#include "math/color.hpp"
#include "math/vector2f.hpp"
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

} // namespace pingus::components

#endif

/* EOF */
