# Level objects: schema, entities and systems

Level objects (groundpieces, traps, exits, backgrounds, …) are described
by a shared schema and live in the game as entities with components.
Their behavior is implemented by systems. This replaced the old
`WorldObj` class hierarchy (`src/pingus/worldobjs/`) and
`WorldObjFactory`.

## Overview

| Part | Location | Role |
|------|----------|------|
| Object schema | `src/pingus/object_schema.{hpp,cpp}` | Every level object type, its properties, defaults and editor hints |
| Entity registry | `src/ecs/registry.hpp` | Generic, header-only entity/component storage |
| Components | `src/pingus/ecs/components.hpp` | Plain data structs, namespace `pingus::components` |
| Entity factory | `src/pingus/ecs/object_factory.cpp` | `ObjectData` → entity with components |
| Systems | `src/pingus/ecs/{systems,traps,objects,weather}.cpp` | Startup, per tick update, drawing |
| Animation timing | `src/pingus/animation_clock.{hpp,cpp}` | Sprite timing for game logic, without images |

(`src/pingus/components/` holds GUI widgets and is unrelated.)

The ground map, the pingus (`PinguHolder`) and the particle systems are
not entities. They are still `WorldObj`s, each with an explicit pointer
to their `World`.

## Object schema

`ObjectSchema` lists every level object type with its properties (name,
type, default value and older aliases such as `width` for `repeat`),
along with editor hints (the placeholder sprite for objects without a
surface, rotation support). `ObjectData` holds the property values of
one object. It reads them from a level file and writes them back in the
established order.

The game, the editor (`GenericLevelObj`, `LevelObjFactory`) and the
tools all use the schema, so each type is described once. Defaults are
the values the game uses when a property is missing.

To add a property, add it to the type in `ObjectSchema::ObjectSchema()`
and read it with `data.get<T>("name")` in the type's entity builder. The
editor loads and saves it automatically. It only shows a widget for it
if the property maps to one of the `HAS_*` flags in
`generic_level_obj.cpp`.

## Registry

`pingus::ecs::Registry` is deliberately minimal: entities are ids,
components are plain structs (at most one of each type per entity),
stored in per-type deques indexed by entity id.

- Ids are never reused, and `each<A, B>(func)` visits entities in
  creation order, so systems are deterministic.
- References to components stay valid when components are added to
  other entities. They don't survive destroying their own entity.

A level has at most a few hundred entities, so simple storage and stable
iteration order matter more than cache layout.

## Loading a level

`World::init_worldobjs()`:

1. Expands `group` and `prefab` objects (applying prefab overrides and
   offsets).
2. Reads every object into an `ObjectData` through the schema.
3. Sorts all objects, together with the remaining `WorldObj`s, by
   z-index (stable). `systems::object_z_index()` applies the types that
   always used a fixed z (backgrounds, weather, switch doors).
4. Creates the entities in that order, so the system iteration order is
   the old z-sorted update order.
5. Runs startup per object in z-order: groundpieces draw themselves into
   the ground and collision map (and are destroyed afterwards), exits
   cut their shape out of it, teleporters and switches resolve their
   targets by `ObjectId`.

`World::object_order` keeps the combined z-sorted list of `WorldObj`s and
entities. Drawing walks this list too, so the draw order is the same as
before the conversion.

## Per tick update

```
World::update()
  systems::update_objects()       objects at or below the pingus' z (50)
  WorldObj::update() for each     ground, pingus, particles
  systems::update_after_pingus()  objects above the pingus' z
```

The two phases keep the old z-sorted `WorldObj` update order, which the
level timing depends on. For example, entrances at z 0 release pingus
before the pingus move, and switch doors (fixed z 100) update after
them. See `Phase` in `system_parts.hpp`.

Within a phase the systems run per object type: traps, then entrances,
exits, teleporters, conveyor belts, switch doors, ice blocks, then
weather. Purely decorative updates (backgrounds, liquids, animated
sprites) run once per tick, in the first phase.

## Game logic and presentation

Game logic never reads sprite state. Objects and pingu actions that time
their logic from an animation own an `AnimationClock` built from the
`.sprite` metadata. Logic advances and queries the clock, and drawing
copies the clock's frame onto the sprite (`AnimationClock::apply_to()`).

Random numbers come from `World::get_game_random()` (gameplay, seeded
from the level, unused so far) and `World::get_fx_random()` (visual
effects only), never from `rand()`.

## Adding a level object type

1. Add the type and its properties to `ObjectSchema`.
2. Add the component struct(s) to `components.hpp`.
3. Add a builder to `get_builders()` in `object_factory.cpp`.
4. Add update, startup and draw code to the matching systems file and
   call it from the dispatch in `systems.cpp`.

## Testing

`extra/pingus-headless` (built with `-DBUILD_EXTRA=ON`) runs the
simulation without a display:

```sh
# simulation results for all demos and levels, diff before/after a change
find data -name '*.pingus-demo' -o -path 'data/levels/*.pingus' | sort \
  | xargs -d '\n' build/extra/pingus-headless > results.txt

# render the world at given ticks to PNGs (offscreen, software renderer,
# byte-identical between runs)
build/extra/pingus-headless -s shots/ -T 1,400,1500 data/levels/tutorial/*.pingus
```

ctest runs the tutorial levels through it (`test_pingus_headless`). Unit
tests for the registry, the schema, `AnimationClock` and `Random` are in
`tests/`.

During the conversion, every step was checked by diffing simulation
results for all 682 demos and levels, and renderings of 21 levels
covering all object types, against the previous step.

## Possible next steps

- Pingus as entities, with the action state machine as a component.
- Move the object type definitions from C++ into a data file.
- Let the editor build its property panel from the schema instead of the
  `HAS_*` flags.
- Simplify the two-phase update into one fixed system order, as a
  deliberate gameplay timing change.
