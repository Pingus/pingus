//  $Id: pingu_holder.hxx,v 1.1 2002/06/12 19:06:12 grumbel Exp $
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

#ifndef PINGUHOLDER_HH
#define PINGUHOLDER_HH

#include <list>
#include <vector>
#include "worldobj.hxx"

class CL_Vector;
class Pingu;

typedef std::list<Pingu*>::iterator PinguIter;

/** This class holds all the penguins in the world */
class PinguHolder : public WorldObj
{
private:
  /// The uniq id for the next Pingu
  int id_count;

  /// ???
  int total_size_count;

  /// ???
  int saved_pingus;

  /** This vector holds all pingus which are ever allocated in the
      world, its used to free them all on the end of this class. */
  std::vector<Pingu*> all_pingus;

  /** A list holding all Pingus, the PinguHolder itself has only the
      active (not dead) ones */
  std::list<Pingu*> pingus;
  
public:
  PinguHolder();
  ~PinguHolder();

  void draw_offset(int, int, float s = 1.0);
  int  total_size();
  int  get_saved() { return saved_pingus; }
  
  /** Adds a pingu to the list of active pingus (the ones that are
      displayed and walk around) */
  void add (Pingu* pingu);

  /** Return a reference to a newly create Pingu, the PinguHolder will
      take care of the deletion. The caller *must* not delete the
      Pingu */
  Pingu* create_pingu (const CL_Vector& pos, int owner_id);

  float get_z_pos() const { return 50; }

  std::list<Pingu*>::iterator  begin () { return pingus.begin (); }
  std::list<Pingu*>::iterator  end ()   { return pingus.end (); }
  std::list<Pingu*>::size_type size ()  { return pingus.size (); }
};

#endif

/* EOF */
