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

#ifndef HEADER_PINGUS_PINGUS_PINGU_HOLDER_HPP
#define HEADER_PINGUS_PINGUS_PINGU_HOLDER_HPP

#include <vector>

#include "ecs/registry.hpp"
#include "math/vector2f.hpp"
#include "pingus/ecs/components.hpp"
#include "pingus/pingu.hpp"

namespace pingus {

class PingusLevel;
class World;

/** Creates the pingu entities of a World and keeps track of them: lookup
    by id, iteration over the active pingus and the released, exited and
    killed counts. The per tick update and drawing of the pingus are done
    by the systems in pingus/ecs/pingus.cpp. */
class PinguHolder
{
private:
  World& world;

  /** Maximum number of pingus that can be released */
  int number_of_allowed;

  int number_of_exited;

  /** Number of pingus with an ActivePingu component */
  int number_of_active;

  /** All pingus ever created, the pingu id is the index */
  std::vector<ecs::Entity> pingus;

public:
  PinguHolder(World& world, PingusLevel const& plf);

  PinguHolder(PinguHolder const&) = delete;
  PinguHolder& operator=(PinguHolder const&) = delete;

  /** Create a new pingu at the given position, returns nullptr when all
      pingus allowed by the level are released */
  Pingu* create_pingu(Vector2f const& pos, int owner_id);

  /** @return the pingu with the given id, nullptr if it doesn't exist
      or isn't alive */
  Pingu* get_pingu(unsigned int id) const;

  /** Call func(Pingu&) for each active pingu, in creation order */
  template<typename Func>
  void for_each(Func&& func)
  {
    get_registry().each<Pingu, components::ActivePingu>(
      [&](ecs::Entity, Pingu& pingu, components::ActivePingu&) {
        func(pingu);
      });
  }

  /** Remove a pingu that died or exited from the active pingus, called
      by the pingu update system */
  void deactivate(Pingu& pingu);

  /** @return the number of pingus that have successfully exited this level */
  int get_number_of_exited() const;

  /** @return the number of pingus that have been killed */
  int get_number_of_killed() const;

  /** @return the number of pingus that are still alive and in the level */
  int get_number_of_alive() const;

  /** @return the total number of pingus released */
  int get_number_of_released() const;

  /** @return the maximum number of pingus that can be released */
  int get_number_of_allowed() const;

  /** @return the id after the highest id handed out so far */
  unsigned int get_end_id() const;

private:
  ecs::Registry& get_registry();
};

} // namespace pingus

#endif

/* EOF */
