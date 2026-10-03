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

#include "pingus/pingu_holder.hpp"

#include "pingus/pingus_level.hpp"
#include "pingus/world.hpp"

namespace pingus {

PinguHolder::PinguHolder(World& world_, PingusLevel const& plf) :
  world(world_),
  number_of_allowed(plf.get_number_of_pingus()),
  number_of_exited(0),
  number_of_active(0),
  pingus()
{
}

ecs::Registry&
PinguHolder::get_registry()
{
  return world.get_registry();
}

Pingu*
PinguHolder::create_pingu(Vector2f const& pos, int owner_id)
{
  if (number_of_allowed <= get_number_of_released()) {
    return nullptr;
  }

  ecs::Registry& reg = get_registry();
  ecs::Entity const entity = reg.create();

  // The id is the index in 'pingus'
  reg.emplace<components::Transform>(entity, pos, 50.0f);
  reg.emplace<components::PinguState>(entity, static_cast<unsigned int>(pingus.size()), owner_id);
  reg.emplace<components::PinguBehavior>(entity);
  reg.emplace<components::ActivePingu>(entity);
  Pingu& pingu = reg.emplace<Pingu>(entity, world, entity);
  pingu.init();

  pingus.push_back(entity);
  number_of_active += 1;

  return &pingu;
}

Pingu*
PinguHolder::get_pingu(unsigned int id) const
{
  if (id >= pingus.size()) {
    return nullptr;
  }

  Pingu& pingu = world.get_registry().get<Pingu>(pingus[id]);
  assert(pingu.get_id() == id);

  if (pingu.get_status() == Pingu::PS_ALIVE) {
    return &pingu;
  } else {
    return nullptr;
  }
}

void
PinguHolder::deactivate(Pingu& pingu)
{
  ecs::Registry& reg = get_registry();
  if (!reg.has<components::ActivePingu>(pingu.get_entity())) {
    return;
  }

  reg.remove<components::ActivePingu>(pingu.get_entity());
  number_of_active -= 1;

  if (pingu.get_status() == Pingu::PS_EXITED) {
    number_of_exited += 1;
  }
}

int
PinguHolder::get_number_of_exited() const
{
  return number_of_exited;
}

int
PinguHolder::get_number_of_killed() const
{
  return get_number_of_released() - number_of_active - number_of_exited;
}

int
PinguHolder::get_number_of_alive() const
{
  return number_of_active;
}

int
PinguHolder::get_number_of_released() const
{
  return static_cast<int>(pingus.size());
}

int
PinguHolder::get_number_of_allowed() const
{
  return number_of_allowed;
}

unsigned int
PinguHolder::get_end_id() const
{
  return static_cast<unsigned int>(pingus.size());
}

} // namespace pingus

/* EOF */
