//  $Id: PinguHolder.hh,v 1.14 2001/08/13 21:35:37 grumbel Exp $
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
#include "boost/smart_ptr.hpp"

#include "Pingu.hh"
#include "WorldObj.hh"

class Pingu;
///
typedef std::list<boost::shared_ptr<Pingu> >::iterator PinguIter;

///
class PinguHolder : public WorldObj
{
private:
  /// The uniq id for the next Pingu
  int id_count;
  ///
  int total_size_count;
  ///
  int saved_pingus;

  /** A list holding all Pingus, the PinguHolder itself has only the
      active (not dead) ones */
  std::list<boost::shared_ptr<Pingu> > pingus;
  
public:
  PinguHolder();
  ~PinguHolder();

  void draw_offset(int, int, float s = 1.0);
  int  total_size();
  int  get_saved() { return saved_pingus; }
  void add (boost::shared_ptr<Pingu> pingu);
  float get_z_pos() const { return 50; }

  std::list<boost::shared_ptr<Pingu> >::iterator begin () { return pingus.begin (); }
  std::list<boost::shared_ptr<Pingu> >::iterator end () { return pingus.end (); }
  std::list<boost::shared_ptr<Pingu> >::size_type size () { return pingus.size (); }
};

#endif

/* EOF */
