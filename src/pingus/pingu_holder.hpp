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

#include <list>

#include "pingus/worldobj.hpp"

class PingusLevel;
class Vector3f;
class Pingu;

typedef std::list<Pingu*>::iterator PinguIter;

/** This class holds all the penguins in the world */
class PinguHolder : public WorldObj
{
private:
  /** The total number of pingus that will get released in this
      level */
  int number_of_allowed;

  /** Number of pingus that made it to the exit, we cache this, since
      else we would have to iterate over the whole list and count them
      each time they are requested. */
  int number_of_exited;

  /** This vector holds all pingus which are ever allocated in the
      world, its used to free them all on the end of this class. */
  std::vector<Pingu*> all_pingus;

  /** A list holding all Pingus, the PinguHolder itself has only the
      active (not dead) ones */
  std::list<Pingu*> pingus;

public:
  PinguHolder(const PingusLevel&);
  ~PinguHolder();

  /*@{
    @name overloaded stuff for WorldObj
  */
  void draw (SceneContext& gc);

  /** Update all Pingus (this calls Pingu::update() which then calls
      PinguAction::update()) */
  void update();

  /** The z-pos at which the pingus gets draw.
      @return 50 */
  float get_z_pos() const;
  void set_pos(const Vector3f& p) { }
  Vector3f get_pos() const { return Vector3f(); }
  /*@}*/

  /** @return the number of pingus that have successfully exit this
      level */
  int  get_number_of_exited();

  /** @return the number of pingus that got killed */
  int  get_number_of_killed();

  /** @return the number of pingus that are still alive, this is shown
      in the PingusCounter panel as 'Out'. Exited pingus are *not*
      counted. FIXME: name should be different (out, active?!) */
  int  get_number_of_alive();

  /** @return the total number of pingus released, this is alive +
      killed + exited */
  int get_number_of_released();

  /** @return the maximal number of pingus that will get released in
      this level */
  int get_number_of_allowed();

  /** @return a reference to a newly create Pingu, the PinguHolder
      will take care of the deletion. The caller *must* not delete the
      Pingu. Might return 0 if all available pingus are already
      released */
  Pingu* create_pingu(const Vector3f& pos, int owner_id);

  /** Get a pingu by id, references to dead or exited Pingus are not
      returned, but 0 instead

      @return the pingu with the id, or 0 if none found or pingu is
      dead or exited */
  Pingu* get_pingu(unsigned int id);

  /** @return the id of the last pingu + 1 */
  unsigned int get_end_id();

  // FIXME: Dirty cruft, needs cleanup
  std::list<Pingu*>::iterator  begin () { return pingus.begin (); }
  std::list<Pingu*>::iterator  end ()   { return pingus.end (); }
  std::list<Pingu*>::size_type size ()  { return pingus.size (); }

private:
  PinguHolder (const PinguHolder&);
  PinguHolder& operator= (const PinguHolder&);
};

#endif

/* EOF */
