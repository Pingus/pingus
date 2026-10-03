// SPDX-FileCopyrightText: 2026 Ingo Ruhnke <grumbel@gmail.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#include "pingus/ecs/systems.hpp"

#include <functional>
#include <map>

#include <logmich/log.hpp>

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

void build_spike(World&, ecs::Registry& reg, ecs::Entity e, ObjectData const&)
{
  reg.emplace<TriggerZone>(e, 16.0f - 5.0f, 0.0f, 16.0f + 5.0f, 32.0f);
  reg.emplace<Spike>(e, Sprite("traps/spike"), AnimationClock::from_sprite("traps/spike"));
}

void build_fake_exit(World&, ecs::Registry& reg, ecs::Entity e, ObjectData const&)
{
  reg.emplace<TriggerZone>(e, -7.0f, -56.0f, 8.0f, 0.0f);
  FakeExit& fake_exit = reg.emplace<FakeExit>(e, Sprite("traps/fake_exit"), AnimationClock::from_sprite("traps/fake_exit"));
  // traps/fake_exit.sprite is marked looping, but the trap smashes once per
  // trigger; with a looping clock it never reset and kept smashing forever
  fake_exit.clock.set_loop(false);
  reg.emplace<SmallmapSymbol>(e, Sprite("core/misc/smallmap_exit"));
}

void build_guillotine(World&, ecs::Registry& reg, ecs::Entity e, ObjectData const&)
{
  reg.emplace<TriggerZone>(e, 38.0f, 90.0f, 42.0f, 98.0f);
  Guillotine& guillotine = reg.emplace<Guillotine>(e,
                                                   Sprite("traps/guillotinekill/left"),
                                                   Sprite("traps/guillotinekill/right"),
                                                   Sprite("traps/guillotineidle"),
                                                   AnimationClock::from_sprite("traps/guillotinekill/left"),
                                                   AnimationClock::from_sprite("traps/guillotineidle"));
  guillotine.kill_clock.set_loop(false);
  guillotine.idle_clock.set_loop(true);
}

void build_hammer(World&, ecs::Registry& reg, ecs::Entity e, ObjectData const&)
{
  reg.emplace<Hammer>(e, Sprite("traps/hammer"), AnimationClock::from_sprite("traps/hammer").frame_count());
}

void build_laser_exit(World&, ecs::Registry& reg, ecs::Entity e, ObjectData const&)
{
  reg.emplace<TriggerZone>(e, 34.0f, 43.0f, 34.0f + 10.0f, 43.0f + 20.0f);
  reg.emplace<LaserExit>(e, Sprite("traps/laser_exit"), AnimationClock::from_sprite("traps/laser_exit"));
}

void build_smasher(World&, ecs::Registry& reg, ecs::Entity e, ObjectData const&)
{
  reg.emplace<Smasher>(e, Sprite("traps/smasher"));
}

/** Owner ids are limited to the four players */
int clamp_owner_id(int owner_id)
{
  return (owner_id < 0 || owner_id > 3) ? 0 : owner_id;
}

void build_entrance(World&, ecs::Registry& reg, ecs::Entity e, ObjectData const& data)
{
  std::string const& direction_str = data.get<std::string>("direction");
  Entrance::Direction direction = Entrance::Direction::MISC;
  if (direction_str == "left") {
    direction = Entrance::Direction::LEFT;
  } else if (direction_str == "right") {
    direction = Entrance::Direction::RIGHT;
  } else if (direction_str != "misc") {
    log_error("unknown direction: '{}'", direction_str);
  }

  int const release_rate = data.get<int>("release-rate");
  // wait ~2sec at startup to allow a 'lets go' sound
  int const last_release = 150 - release_rate;

  reg.emplace<Owner>(e, clamp_owner_id(data.get<int>("owner-id")));
  reg.emplace<Entrance>(e, direction, release_rate, last_release);
  reg.emplace<SmallmapSymbol>(e, Sprite("core/misc/smallmap_entrance"));
}

void build_exit(World&, ecs::Registry& reg, ecs::Entity e, ObjectData const& data)
{
  int const owner_id = clamp_owner_id(data.get<int>("owner-id"));
  ResDescriptor const& desc = data.get<ResDescriptor>("surface");

  reg.emplace<Owner>(e, owner_id);
  reg.emplace<TriggerZone>(e, -1.0f, -5.0f, 1.0f, 5.0f);
  reg.emplace<Exit>(e, desc, Sprite(desc), Sprite("core/misc/flag" + std::to_string(owner_id)));
  reg.emplace<SmallmapSymbol>(e, Sprite("core/misc/smallmap_exit"));
}

void build_teleporter(World&, ecs::Registry& reg, ecs::Entity e, ObjectData const& data)
{
  reg.emplace<TriggerZone>(e, -3.0f, -52.0f, 3.0f, 0.0f);
  reg.emplace<Teleporter>(e, Sprite("worldobjs/teleporter"),
                          AnimationClock::from_sprite("worldobjs/teleporter"),
                          data.get<std::string>("target-id"));
}

void build_teleporter_target(World&, ecs::Registry& reg, ecs::Entity e, ObjectData const& data)
{
  reg.emplace<ObjectId>(e, data.get<std::string>("id"));
  reg.emplace<TeleporterTarget>(e, Sprite("worldobjs/teleportertarget"),
                                AnimationClock::from_sprite("worldobjs/teleportertarget"));
}

void build_ice_block(World&, ecs::Registry& reg, ecs::Entity e, ObjectData const&)
{
  // "repeat" is part of the level format, but only a single block was
  // ever implemented
  auto cmap = std::make_shared<CollisionMask>("worldobjs/iceblock_cmap");
  reg.emplace<TriggerZone>(e, 0.0f, -4.0f,
                           static_cast<float>(cmap->get_width()),
                           static_cast<float>(cmap->get_height()));
  reg.emplace<IceBlock>(e, Sprite("worldobjs/iceblock"), cmap);
}

void build_conveyor_belt(World&, ecs::Registry& reg, ecs::Entity e, ObjectData const& data)
{
  int const width = data.get<int>("repeat");
  reg.emplace<TriggerZone>(e, 0.0f, -2.0f, 15.0f * static_cast<float>(width + 2), 10.0f);
  reg.emplace<ConveyorBelt>(e,
                            Sprite("worldobjs/conveyorbelt_left"),
                            Sprite("worldobjs/conveyorbelt_middle"),
                            Sprite("worldobjs/conveyorbelt_right"),
                            width,
                            static_cast<float>(data.get<int>("speed")));
}

void build_switch_door(World&, ecs::Registry& reg, ecs::Entity e, ObjectData const& data)
{
  int const height = data.get<int>("height");
  reg.emplace<ObjectId>(e, data.get<std::string>("id"));
  reg.emplace<SwitchDoor>(e,
                          Sprite("worldobjs/switchdoor_box"),
                          Sprite("worldobjs/switchdoor_tile"),
                          std::make_shared<CollisionMask>("worldobjs/switchdoor_box"),
                          std::make_shared<CollisionMask>("worldobjs/switchdoor_tile_cmap"),
                          height, height);
}

void build_switch(World&, ecs::Registry& reg, ecs::Entity e, ObjectData const& data)
{
  Sprite sprite("worldobjs/switchdoor_switch");
  reg.emplace<TriggerZone>(e, 0.0f, 0.0f,
                           static_cast<float>(sprite.get_width()),
                           static_cast<float>(sprite.get_height()));
  reg.emplace<SwitchDoorSwitch>(e, sprite, data.get<std::string>("target-id"));
}

void build_snow_generator(World&, ecs::Registry& reg, ecs::Entity e, ObjectData const& data)
{
  reg.emplace<SnowGenerator>(e, data.get<float>("intensity"));
}

void build_rain_generator(World&, ecs::Registry& reg, ecs::Entity e, ObjectData const&)
{
  reg.emplace<RainGenerator>(e);
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
    {"spike", build_spike},
    {"fake_exit", build_fake_exit},
    {"guillotine", build_guillotine},
    {"hammer", build_hammer},
    {"laser_exit", build_laser_exit},
    {"smasher", build_smasher},
    {"entrance", build_entrance},
    {"exit", build_exit},
    {"teleporter", build_teleporter},
    {"teleporter-target", build_teleporter_target},
    {"iceblock", build_ice_block},
    {"conveyorbelt", build_conveyor_belt},
    {"switchdoor-door", build_switch_door},
    {"switchdoor-switch", build_switch},
    {"snow-generator", build_snow_generator},
    {"rain-generator", build_rain_generator},
  };
  return builders;
}

} // namespace

float
object_z_index(ObjectData const& data)
{
  // These types never used the z-index from the level file
  static std::map<std::string, float> const fixed_z_index = {
    {"solidcolor-background", -10.0f},
    {"starfield-background", -10.0f},
    {"snow-generator", 1000.0f},
    {"rain-generator", 1000.0f},
    {"switchdoor-door", 100.0f},
    {"switchdoor-switch", 100.0f},
  };

  auto it = fixed_z_index.find(data.type().name);
  return it != fixed_z_index.end() ? it->second : data.get_z_index();
}

bool
is_solid_background(ObjectData const& data)
{
  std::string const& name = data.type().name;
  return name == "surface-background" || name == "solidcolor-background";
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
  reg.emplace<Transform>(entity, data.get_pos(), object_z_index(data));
  it->second(world, reg, entity, data);
  return entity;
}

} // namespace pingus::systems

/* EOF */
