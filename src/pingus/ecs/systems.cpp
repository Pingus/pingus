// SPDX-FileCopyrightText: 2026 Ingo Ruhnke <grumbel@gmail.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#include "pingus/ecs/systems.hpp"

#include "engine/display/scene_context.hpp"
#include "pingus/collision_map.hpp"
#include "pingus/components/smallmap.hpp"
#include "pingus/collision_mask.hpp"
#include "pingus/ecs/components.hpp"
#include "pingus/ecs/system_parts.hpp"
#include "pingus/globals.hpp"
#include "pingus/world.hpp"

namespace pingus::systems {

using namespace pingus::components;

namespace {

// Startup

void startup_groundpiece(World& world, ecs::Entity entity, Transform const& transform, Groundpiece const& groundpiece)
{
  // FIXME: using a CollisionMask is kind of unneeded here
  CollisionMask mask(groundpiece.desc);
  int const x = static_cast<int>(transform.pos.x());
  int const y = static_cast<int>(transform.pos.y());

  // FIXME: overdrawing of bridges and similar things aren't handled here
  if (groundpiece.gptype == Groundtype::GP_REMOVE) {
    world.remove(mask, x, y);
  } else {
    world.put(mask, x, y, groundpiece.gptype);
  }

  // Nothing left to do for a groundpiece once it is part of the ground
  world.get_registry().destroy(entity);
}

void startup_liquid(World& world, Transform const& transform, Liquid const& liquid)
{
  CollisionMask mask("liquids/water_cmap");
  for (int i = 0; i < liquid.width; ++i)
  {
    world.get_colmap()->put(mask,
                            static_cast<int>(transform.pos.x() + static_cast<float>(i)),
                            static_cast<int>(transform.pos.y()),
                            Groundtype::GP_WATER);
  }
}

// Update

void update_animated_sprites(ecs::Registry& reg)
{
  reg.each<SpriteRender, AnimatedSprite>([](ecs::Entity, SpriteRender& render, AnimatedSprite&) {
    render.sprite.update();
  });
}

void update_liquids(ecs::Registry& reg)
{
  reg.each<Liquid>([](ecs::Entity, Liquid& liquid) {
    liquid.sprite.update(0.033f);
  });
}

void update_surface_backgrounds(ecs::Registry& reg)
{
  reg.each<SurfaceBackground>([](ecs::Entity, SurfaceBackground& bg) {
    bg.sprite.update();

    if (!bg.sprite || globals::static_graphics) {
      return;
    }

    float const width = static_cast<float>(bg.sprite.get_width());
    float const height = static_cast<float>(bg.sprite.get_height());

    if (bg.scroll_x != 0.0f)
    {
      bg.scroll_ox += bg.scroll_x;
      if (bg.scroll_ox > width) {
        bg.scroll_ox -= width;
      } else if (-bg.scroll_ox > width) {
        bg.scroll_ox += width;
      }
    }

    if (bg.scroll_y != 0.0f)
    {
      bg.scroll_oy += bg.scroll_y;
      if (bg.scroll_oy > height) {
        bg.scroll_oy -= height;
      } else if (-bg.scroll_oy > height) {
        bg.scroll_oy += height;
      }
    }
  });
}

void update_starfields(World& world, ecs::Registry& reg)
{
  reg.each<StarfieldBackground>([&](ecs::Entity, StarfieldBackground& starfield) {
    for (Star& star : starfield.stars)
    {
      star.x_pos += star.x_add;
      star.y_pos += star.y_add;

      if (star.x_pos > static_cast<float>(world.get_width()))
      {
        star.x_pos = float(-globals::tile_size);
        star.y_pos = float(world.get_fx_random().next_int(world.get_height()));
      }
    }
  });
}

// Drawing

void draw_surface_background(World& world, SceneContext& gc, Transform const& transform, SurfaceBackground const& bg)
{
  if (!bg.sprite) {
    return;
  }

  Vector2i offset = gc.color().world_to_screen(Vector2i(0,0));
  offset -= geom::ioffset(gc.color().get_rect().left(),
                          gc.color().get_rect().top());

  int start_x = static_cast<int>((static_cast<float>(offset.x()) * bg.para_x) + bg.scroll_ox);
  int start_y = static_cast<int>((static_cast<float>(offset.y()) * bg.para_y) + bg.scroll_oy);

  if (start_x > 0) {
    start_x = (start_x % bg.sprite.get_width()) - bg.sprite.get_width();
  }

  if (start_y > 0) {
    start_y = (start_y % bg.sprite.get_height()) - bg.sprite.get_height();
  }

  for (int y = start_y; y < world.get_height(); y += bg.sprite.get_height())
  {
    for (int x = start_x; x < world.get_width(); x += bg.sprite.get_width())
    {
      gc.color().draw(bg.sprite, Vector2i(x - offset.x(), y - offset.y()), transform.z_index);
    }
  }
}

void draw_liquid(SceneContext& gc, Transform const& transform, Liquid const& liquid)
{
  int const x0 = static_cast<int>(transform.pos.x());
  for (int x = x0; x < x0 + liquid.width; x += liquid.sprite.get_width()) {
    gc.color().draw(liquid.sprite, Vector2f(static_cast<float>(x), transform.pos.y()));
  }
}

} // namespace

void
startup(World& world, ecs::Entity entity)
{
  ecs::Registry& reg = world.get_registry();
  Transform const& transform = reg.get<Transform>(entity);

  if (auto* groundpiece = reg.try_get<Groundpiece>(entity)) {
    startup_groundpiece(world, entity, transform, *groundpiece);
    return;
  }

  if (auto* liquid = reg.try_get<Liquid>(entity)) {
    startup_liquid(world, transform, *liquid);
  }

  startup_trap(world, entity);
  startup_level_object(world, entity);
}

void
update_spawners(World& world)
{
  update_entrances(world);
}

void
update_objects(World& world)
{
  ecs::Registry& reg = world.get_registry();

  update_traps(world);
  update_level_objects(world);
  update_weather(world);
  update_surface_backgrounds(reg);
  update_starfields(world, reg);
  update_liquids(reg);
  update_animated_sprites(reg);
}

void
draw(World& world, SceneContext& gc, ecs::Entity entity)
{
  ecs::Registry& reg = world.get_registry();
  if (!reg.valid(entity)) {
    return;
  }

  Transform const& transform = reg.get<Transform>(entity);

  if (auto* bg = reg.try_get<SolidColorBackground>(entity)) {
    gc.color().fill_screen(bg->color);
  }

  if (auto* bg = reg.try_get<SurfaceBackground>(entity)) {
    draw_surface_background(world, gc, transform, *bg);
  }

  if (auto* starfield = reg.try_get<StarfieldBackground>(entity)) {
    for (Star const& star : starfield->stars) {
      gc.color().draw(star.sprite, Vector2f(star.x_pos, star.y_pos));
    }
  }

  if (auto* liquid = reg.try_get<Liquid>(entity)) {
    draw_liquid(gc, transform, *liquid);
  }

  if (auto* render = reg.try_get<SpriteRender>(entity)) {
    gc.color().draw(render->sprite, transform.pos, render->use_z_index ? transform.z_index : 0.0f);
  }

  draw_trap(world, gc, entity);
  draw_level_object(world, gc, entity);
  draw_weather(world, gc, entity);
}

void
draw_smallmap(World& world, SmallMap& smallmap)
{
  world.get_registry().each<Transform, SmallmapSymbol>([&](ecs::Entity, Transform& transform, SmallmapSymbol& symbol) {
    smallmap.draw_sprite(symbol.sprite, transform.pos);
  });
}

} // namespace pingus::systems

/* EOF */
