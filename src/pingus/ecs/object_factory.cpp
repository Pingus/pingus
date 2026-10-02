// SPDX-FileCopyrightText: 2026 Ingo Ruhnke <grumbel@gmail.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#include "pingus/ecs/systems.hpp"

#include <functional>
#include <map>

#include "pingus/ecs/components.hpp"
#include "pingus/object_schema.hpp"
#include "pingus/resource.hpp"
#include "pingus/world.hpp"

namespace pingus::systems {

using namespace pingus::components;

namespace {

using Builder = std::function<void (World&, ecs::Registry&, ecs::Entity, ObjectData const&)>;

void build_groundpiece(World&, ecs::Registry& reg, ecs::Entity e, ObjectData const& data)
{
  reg.emplace<Groundpiece>(e, data.get<ResDescriptor>("surface"),
                           Groundtype::string_to_type(data.get<std::string>("type")));
}

void build_hotspot(World&, ecs::Registry& reg, ecs::Entity e, ObjectData const& data)
{
  // "parallax" is read but was never implemented for drawing
  reg.emplace<SpriteRender>(e, Sprite(data.get<ResDescriptor>("surface")), true);
  reg.emplace<AnimatedSprite>(e);
}

void build_liquid(World&, ecs::Registry& reg, ecs::Entity e, ObjectData const& data)
{
  Sprite sprite(data.get<ResDescriptor>("surface"));
  int const width = data.get<int>("repeat") * sprite.get_width();
  reg.emplace<Liquid>(e, sprite, width);
}

void build_solidcolor_background(World&, ecs::Registry& reg, ecs::Entity e, ObjectData const& data)
{
  reg.emplace<SolidColorBackground>(e, data.get<Color>("colori"));
  reg.emplace<SolidBackground>(e);
}

Sprite create_surface_background_sprite(World& world, ObjectData const& data)
{
  ResDescriptor const& desc = data.get<ResDescriptor>("surface");
  Color const color = data.get<Color>("colori");
  bool const stretch_x = data.get<bool>("stretch-x");
  bool const stretch_y = data.get<bool>("stretch-y");
  bool const keep_aspect = data.get<bool>("keep-aspect");

  if (!stretch_x && !stretch_y && color.a == 0)
  {
    // FIXME: would be nice to allow surface manipulation with
    // animated sprites, but it's not that easy to do
    return Sprite(desc);
  }

  Surface surface = Resource::load_surface(desc);

  if (color.a != 0 && surface.is_indexed())
  {
    if (surface.has_colorkey()) {
      surface = surface.convert_to_rgba();
    } else {
      surface = surface.convert_to_rgb();
    }
  }

  surface.fill(color);

  if (stretch_x && stretch_y)
  {
    surface = surface.scale(world.get_width(), world.get_height());
  }
  else if (stretch_x && !stretch_y)
  {
    if (keep_aspect)
    {
      float aspect = static_cast<float>(surface.get_height()) / static_cast<float>(surface.get_width());
      surface = surface.scale(world.get_width(), static_cast<int>(static_cast<float>(world.get_width()) * aspect));
    }
    else
    {
      surface = surface.scale(world.get_width(), surface.get_height());
    }
  }
  else if (!stretch_x && stretch_y)
  {
    if (keep_aspect)
    {
      float aspect = static_cast<float>(surface.get_width()) / static_cast<float>(surface.get_height());
      surface = surface.scale(static_cast<int>(static_cast<float>(world.get_height()) * aspect), world.get_height());
    }
    else
    {
      surface = surface.scale(surface.get_width(), world.get_height());
    }
  }

  return Sprite(surface);
}

void build_surface_background(World& world, ecs::Registry& reg, ecs::Entity e, ObjectData const& data)
{
  reg.emplace<SurfaceBackground>(e, create_surface_background_sprite(world, data),
                                 data.get<float>("para-x"), data.get<float>("para-y"),
                                 data.get<float>("scroll-x"), data.get<float>("scroll-y"));
  reg.emplace<SolidBackground>(e);
}

Star create_star(World& world, char const* sprite_name)
{
  Random& rng = world.get_fx_random();
  Star star{Sprite(sprite_name), 0.0f, 0.0f, 0.0f, 0.0f};
  star.x_pos = float(rng.next_int(world.get_width()));
  star.y_pos = float(rng.next_int(world.get_height()));
  star.x_add = static_cast<float>(rng.next_int(5)) + 1.0f;
  star.y_add = 0.0f;
  return star;
}

void build_starfield_background(World& world, ecs::Registry& reg, ecs::Entity e, ObjectData const& data)
{
  StarfieldBackground& starfield = reg.emplace<StarfieldBackground>(e);
  for (int i = 0; i < data.get<int>("small-stars"); ++i) {
    starfield.stars.push_back(create_star(world, "game/stars/small_star"));
  }
  for (int i = 0; i < data.get<int>("middle-stars"); ++i) {
    starfield.stars.push_back(create_star(world, "game/stars/middle_star"));
  }
  for (int i = 0; i < data.get<int>("large-stars"); ++i) {
    starfield.stars.push_back(create_star(world, "game/stars/large_star"));
  }
}

std::map<std::string, Builder> const& get_builders()
{
  static std::map<std::string, Builder> const builders = {
    {"groundpiece", build_groundpiece},
    {"hotspot", build_hotspot},
    {"liquid", build_liquid},
    {"solidcolor-background", build_solidcolor_background},
    {"surface-background", build_surface_background},
    {"starfield-background", build_starfield_background},
  };
  return builders;
}

} // namespace

bool
is_entity_type(ObjectTypeDef const& type)
{
  return get_builders().contains(type.name);
}

ecs::Entity
create_object(World& world, ObjectData const& data)
{
  auto it = get_builders().find(data.type().name);
  if (it == get_builders().end()) {
    throw std::runtime_error("create_object(): no entity builder for '" + data.type().name + "'");
  }

  ecs::Registry& reg = world.get_registry();
  ecs::Entity const entity = reg.create();
  reg.emplace<Transform>(entity, data.get_pos(), data.get_z_index());
  it->second(world, reg, entity, data);
  return entity;
}

} // namespace pingus::systems

/* EOF */
