//  $Id: pingu_holder.hxx,v 1.12 2002/10/04 13:46:56 grumbel Exp $
//
//  Pingus - A free Lemmings clone
//  Copyright (C) 1999 Ingo Ruhnke <grumbel@gmx.de>
//
//  This program is free software; you can redistribute it and/or
//  modify it under the terms of the GNU General Public License
//  as published by the Free Software Foundation; either version 2
//  of the License, or (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

#ifndef HEADER_PINGUS_PINGU_HOLDER_HXX
#define HEADER_PINGUS_PINGU_HOLDER_HXX

#include <list>
#include <vector>
#include "worldobj.hxx"

class Vector;
class Pingu;

typedef std::list<Pingu*>::iterator PinguIter;

/** This class holds all the penguins in the world */
class PinguHolder : public WorldObj
{
private:
  /** The uniq id for the next Pingu, starts at 0 and is increased
      with every Pingu. Id's are not recycled if Pingus die. */
  int id_count;

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
  PinguHolder();
  ~PinguHolder();

  /*@{ 
    @name overloaded stuff for WorldObj 
  */
  void draw (GraphicContext& gc);

  /** Update all Pingus (this calls Pingu::update() which then calls
      PinguAction::update()) */
  void update();

  /** The z-pos at which the pingus gets draw. 
      @return 50 */
  float get_z_pos() const;
  /*@}*/

  /** @return the number of pingus that have successfully exit this
      level */
  int  get_number_of_exited();

  /** @return the number of pingus that got killed */
  int  get_number_of_killed();

  /** @return the number of pingus that are still alive, this is shown
      in the PingusCounter panel as 'Out' */
  int  get_number_of_alive();

  /** @return the total number of pingus released, this is alive +
      killed + exited */
  int get_number_of_released();

  /** Return a reference to a newly create Pingu, the PinguHolder will
      take care of the deletion. The caller *must* not delete the
      Pingu */
  Pingu* create_pingu(const Vector& pos, int owner_id);

  /** Get a pingu by id 
      @return the pingu with the id, or 0 if none found */
  Pingu* get_pingu(int id);

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
