# Level objects and pingus: schema, entities and systems

Level objects (groundpieces, traps, exits, backgrounds, …) are described
by a shared schema and live in the game as entities with components, as
do the pingus. Their behavior is implemented by systems. This replaced the old
`WorldObj` class hierarchy (`src/pingus/worldobjs/`) and
`WorldObjFactory`.

## Overview

| Part | Location | Role |
|------|----------|------|
| Object schema | `src/pingus/object_schema.{hpp,cpp}` | Every level object type, its properties, defaults and editor hints |
| Entity registry | `src/ecs/registry.hpp` | Generic, header-only entity/component storage |
| Components | `src/pingus/ecs/components.hpp` | Plain data structs, namespace `pingus::components` |
| Entity factory | `src/pingus/ecs/object_factory.cpp` | `ObjectData` → entity with components |
| Systems | `src/pingus/ecs/{systems,traps,objects,weather,pingus}.cpp` | Startup, per tick update, drawing |
| Animation timing | `src/pingus/animation_clock.{hpp,cpp}` | Sprite timing for game logic, without images |

(`src/pingus/components/` holds GUI widgets and is unrelated.)

The ground map (`GroundMap`) and the particle systems
(`src/pingus/particles/`) are not entities but plain services owned by
`World`, which updates and draws them explicitly.

## Pingus

Each pingu is an entity with `Transform` (position), `PinguState` (id,
owner, status, direction, velocity), `PinguBehavior` (the action state
machine) and, while it is alive and in the level, `ActivePingu`. The
`Pingu` component holds no state of its own: it is the interface the 22
actions, the GUI and the server use, and reads and writes the other
components. Actions are still classes (`src/pingus/actions/`) held by
`PinguBehavior`. They hold game state only: all animation timing is in
`AnimationClock`s (`DirectionalAnimationClock` for animations that only
advance in the facing direction). An action describes what the pingu
looks like through `get_look()`: a list of layers, each an animation of
the owner's animation set (`data/animsets/pingus/playerN.animset`) with
a frame and an optional offset, e.g. the walker's `walker` plus the
`floater-layer` overlay, or the bomber's `explosion` flash marked as
shown once. `systems::draw_pingus()` draws the looks, keeping the loaded
sprites in the `PinguView` component, so the simulation alone never
loads pingu sprites.

`PinguHolder` creates pingu entities, finds them by id, counts released,
exited and killed pingus, and iterates the active ones (`for_each()`).
`systems::update_pingus()` runs the actions and deactivates dead and
exited pingus; `systems::draw_pingus()` draws them at depth 50.

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
editor accesses properties by name (`LevelObj::get_property()` /
`set_property()`), loads and saves them automatically, and its property panel
(`ObjectProperties`) creates a widget for it from the property type:
an inputbox for numbers and strings, a combobox for strings with
`choices`, a checkbox for booleans, four inputboxes for colors. `label`
sets the panel label, `hidden` keeps a property out of the panel.

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
3. Sorts all objects, together with markers for the ground (z 0), the
   pingus (z 50) and the particles (z 1000), by z-index (stable). `systems::object_z_index()` applies the types that
   always used a fixed z (backgrounds, weather, switch doors).
4. Creates the entities in that order, so systems iterate them in
   z-order.
5. Runs startup per object in z-order: groundpieces draw themselves into
   the ground and collision map (and are destroyed afterwards), exits
   cut their shape out of it, teleporters and switches resolve their
   targets by `ObjectId`.

`World::object_order` keeps this combined z-sorted list of entities and
layer markers. Drawing walks it too, so the draw order is the same as
before the conversion.

## Per tick update

```
World::update()
  systems::update_spawners()   entrances release pingus
  systems::update_pingus()     pingus act and move
  particle systems update
  systems::update_objects()    traps, exits, teleporters, conveyor belts,
                               switch doors, ice blocks, weather,
                               backgrounds, decorative animation
```

Objects react to the positions the pingus moved to in the same tick.
(Before the conversion the update order followed the objects' z-index
relative to the pingus. This was replaced by the fixed order above,
which shifted level timing by a tick in some levels.)

## Animation sets

An animation set (`data/animsets/NAME.animset`, loaded by
`AnimationSet::get()`) lists the named animations an object can show,
each with left and right sprite resources, an optional draw offset and
an optional loop override:

```scheme
(pingus-animset
  (animations
    (idle (sprite "traps/guillotineidle") (loop #t))
    (kill (left "traps/guillotinekill/left")
          (right "traps/guillotinekill/right")
          (loop #f))))
```

The `AnimatedSprite` component refers to a set and holds the requested
animation, direction, frame and visibility. Game systems only set those
(the traps in `update_trap_animations()`), the render system loads the
sprites and draws them. The traps, teleporters and pingus use animation
sets, so their look can be changed without touching code.

### Effects

Animations can fire effects when they reach a step (the step of their
game timing, or the frame for purely visual animations; step 0 is the
start). Looping and restarted animations fire them again.

```scheme
(bomber (left ...) (right ...)
  (effects
    (sound (at-step 10) (name "plop") (volume 0.5))
    (particles (at-step 13) (kind "pingu") (offset 0 -5))
    (overlay (at-step 13) (animation "explosion") (offset -32 -48))))
```

The effect types are a fixed vocabulary implemented in C++
(`src/pingus/ecs/effects.cpp`): `sound`, `particles` (kinds `pingu`,
`smoke`; optional `count`, `offset`, `spread`) and `overlay` (another
animation of the set drawn once). There are no conditions or
expressions in the data. `systems::update_effects()` runs at the end of
every tick for the pingus and the `AnimatedSprite` entities, so pausing
and fast forward don't skip or repeat effects. Effects are presentation
only; anything that changes the game (craters, kills) stays in the game
logic.

## Game logic and presentation

Game logic never reads sprite state. Objects and pingu actions that time
their logic from an animation own an `AnimationClock` built from the
`.sprite` metadata. Logic advances and queries the clock, and drawing
copies the clock's frame onto the sprite (`AnimationClock::apply_to()`).

Game timing never comes from the art. Clocks the game logic depends on
(the bridger lays its brick at step 7 of 15, the bomber explodes at
step 13, a pingu dies when the splash animation ends, …) are created
with explicit values in the action or trap. The drawing code maps their
step proportionally onto the frames of the animation
(`AnimationClock::map_frame()`), so art with a different number of
frames changes only the look. Purely visual animations (walking,
falling, …) take their timing from the animation set
(`PinguAction::look_animation()`).

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

- Move the object type definitions from C++ into a data file.
- Turn the pingu actions from classes into data plus per action systems.
