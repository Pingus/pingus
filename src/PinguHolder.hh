//  $Id: PinguHolder.hh,v 1.8 2000/07/30 01:47:35 grumbel Exp $
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
#include "Pingu.hh"
#include "WorldObj.hh"

///
typedef std::list<Pingu*>::iterator PinguIter;

///
class PinguHolder : public std::list<Pingu*>, public WorldObj
{
private:
  /// The uniq id for the next Pingu
  int id_count;
  ///
  int total_size_count;
  ///
  int saved_pingus;

  /** A std::list holding all Pingus, the PinguHolder itself has only the
      active (not dead) ones */
  std::list<Pingu*> pingus;
public:
  ///
  PinguHolder();
  ///
  ~PinguHolder();
  ///
  void draw_offset(int, int, float s = 1.0);
  ///
  void push_back(Pingu*);
  ///
  int  total_size();
  ///
  int  get_saved() { return saved_pingus; }
  
  int get_z_pos() const { return 50; }
};

#endif

/* EOF */
