//  $Id: pingus.hxx,v 1.9 2002/09/07 23:33:47 grumbel Exp $
// 
//  Pingus - A free Lemmings clone
//  Copyright (C) 2000 Ingo Ruhnke <grumbel@gmx.de>
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

#ifndef HEADER_PINGUS_WORLDMAP_PINGUS_HXX
#define HEADER_PINGUS_WORLDMAP_PINGUS_HXX

#include "../pingus.hxx"
#include <math.h>
#include <queue>
#include "node.hxx"

namespace boost {
  template <class T> class shared_ptr;
}

namespace pingus {
namespace worldmap {

/** This is the representation of the horde of Pingus which will walk
    on the worldmap */
class Pingus
{
private:
  Sprite sprite;
  CL_Vector pos;
  std::queue<Node*> targets;
  bool is_left;
  Node* current_node;

public:
  Pingus ();
  ~Pingus ();

  void draw (const CL_Vector& offset);
  void update (float delta);

  /** Let the pingu walk to the given node
      @param node The node to walk to */
  void walk_to (Node* node);

  /** @return If the pingu is currently between two nodes return
      true */
  bool is_walking ();

  /** Set the pingu to the position of a node */
  void set_position (boost::shared_ptr<Node> node);

  /** @return Current position of the pingu on the screen */
  CL_Vector get_pos () { return pos; }

  /** @return The node where the pingu is current on 

  FIXME: What should happen if the pingu is between two nodes? */
  Node* get_node ();
  
private:
  Pingus (const Pingus&);
  Pingus operator= (const Pingus&);
};

} // namespace worldmap
} // namespace pingus

#endif

/* EOF */
