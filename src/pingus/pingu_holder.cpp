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

#include "pingus/pingu.hpp"
#include "pingus/pingus_level.hpp"

PinguHolder::PinguHolder(const PingusLevel& plf) :
  number_of_allowed(plf.get_number_of_pingus()),
  number_of_exited(0),
  all_pingus(),
  pingus()
{
}

PinguHolder::~PinguHolder()
{
  for(std::vector<Pingu*>::iterator i = all_pingus.begin();
      i != all_pingus.end(); ++i)
    delete *i;
}

Pingu*
PinguHolder::create_pingu (const Vector3f& pos, int owner_id)
{
  if (number_of_allowed > get_number_of_released())
  {
    // We use all_pingus.size() as pingu_id, so that id == array
    // index
    Pingu* pingu = new Pingu(static_cast<unsigned int>(all_pingus.size()), pos, owner_id);

    // This list will deleted
    all_pingus.push_back (pingu);

    // This list holds the active pingus
    pingus.push_back(pingu);

    return pingu;
  }
  else
  {
    return 0;
  }
}

void
PinguHolder::draw (SceneContext& gc)
{
  // Draw all walkers
  for(std::list<Pingu*>::iterator pingu = pingus.begin();
      pingu != pingus.end();
      ++pingu)
  {
    if ((*pingu)->get_action() == ActionName::WALKER)
      (*pingu)->draw (gc);
  }

  // Draw all non-walkers, so that they are easier spotable

  // FIXME: This might be usefull, but looks kind of ugly in the game
  // FIXME: Bridgers where walkers walk behind are an example of
  // FIMME: uglyness. Either we rip this code out again or fix the
  // FIXME: bridger so that it looks higher and better with walkers
  // FIXME: behind him.
  for(std::list<Pingu*>::iterator pingu = pingus.begin();
      pingu != pingus.end();
      ++pingu)
  {
    if ((*pingu)->get_action() != ActionName::WALKER)
      (*pingu)->draw (gc);
  }
}

void
PinguHolder::update()
{
  PinguIter pingu = pingus.begin();

  while(pingu != pingus.end())
  {
    (*pingu)->update();

    // FIXME: The draw-loop is not the place for things like this,
    // this belongs in the update loop
    if ((*pingu)->get_status() == Pingu::PS_DEAD)
    {
      // Removing the dead pingu and setting the iterator back to
      // the correct possition, no memory hole since pingus will
      // keep track of the allocated Pingus
      pingu = pingus.erase(pingu);
    }
    else if ((*pingu)->get_status() == Pingu::PS_EXITED)
    {
      number_of_exited += 1;
      pingu = pingus.erase(pingu);
    }
    else
    {
      // move to the next Pingu
      ++pingu;
    }
  }
}

Pingu*
PinguHolder::get_pingu(unsigned int id_)
{
  if (id_ < all_pingus.size())
  {
    Pingu* pingu = all_pingus[id_];

    assert(pingu->get_id() == id_);

    if (pingu->get_status() == Pingu::PS_ALIVE)
      return pingu;
    else
      return 0;
  }
  else
  {
    return 0;
  }
}

float
PinguHolder::get_z_pos() const
{
  return 50;
}

int
PinguHolder::get_number_of_exited()
{
  return number_of_exited;
}

int
PinguHolder::get_number_of_killed()
{
  return static_cast<int>(all_pingus.size()) - static_cast<int>(pingus.size()) - get_number_of_exited();
}

int
PinguHolder::get_number_of_alive()
{
  return static_cast<int>(pingus.size());
}

int
PinguHolder::get_number_of_released()
{
  return static_cast<int>(all_pingus.size());
}

int
PinguHolder::get_number_of_allowed()
{
  return number_of_allowed;
}

unsigned int
PinguHolder::get_end_id()
{
  return static_cast<unsigned int>(all_pingus.size());
}

/* EOF */
