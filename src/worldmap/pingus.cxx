//  $Id: pingus.cxx,v 1.12 2002/10/13 20:25:00 torangan Exp $
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

#include "../graphic_context.hxx"
#include "pingus.hxx"
#include "../boost/smart_ptr.hpp"

namespace WorldMapNS {

Pingus::Pingus ()
  : Drawable("pingus"),
    sprite ("Pingus/walker0", "pingus", 20.0f, Sprite::RIGHT)
{
  sprite.set_align (-sprite.get_width()/2,  4 - sprite.get_height());
}

Pingus::~Pingus ()
{
}


void
Pingus::draw (GraphicContext& gc)
{
  gc.draw(sprite, pos);
}

void
Pingus::update (float delta)
{
  sprite.update (delta);
}

void
Pingus::update_walk (float delta)
{
#if TODO
  // Update the position
  edge_path_position += velocity * delta;

  if (edge_path_position > edge_path_length) // target reached
    {
      if (node_path.empty ()) // final target reached
        {
          current_node = target_node;
        }
      else // edge is traveled, now go to the next node
        {
          source_node = target_node;
          target_node = node_path.top ();
          node_path.pop ();

          edge_path_position = 0.0f;
          edge_path = get_edge (source_node, target_node)->data;
          edge_path_lenght   = calc_edge_path_length ();
        }
    }

  // Recalc pingu position on the screen
  pos = calc_pos ();
#else
UNUSED_ARG(delta);
#endif
}

bool
Pingus::walk_to_node (NodeId target)
{
#if TODO
  if (current_node) // pingu stands still
    {
      node_path = worldmap->find_path (current_node, target);
    }
  else // pingu between two nodes
    {
      node_path1 = worldmap->find_path (source_node, target);
      node_path2 = worldmap->find_path (target_node, target);
	
      // Select the shorter path
      if (length (node_path1) < length (node_path2))
        { // walk to source node, which means to reverse the pingu
          node_path = node_path1;

          // Reverse the pingu
          swap(target_node, source_node);
          std::reverse(edge_path.begin (), edge_path.end ());
          edge_path_position = edge_path_lenght - edge_path_position;
        }
      else
        { // walk to target_node
          node_path = node_path2;
        }	
    }
#else
  UNUSED_ARG(target);
#endif
  return false;
}

Vector
Pingus::calc_pos ()
{
#if TODO
  if (current_node) // pingu stands still
    {
      return current_node->get_pos ();
    }
  else // between two nodes
    {
      iterator current = edge_path.begin ();
      iterator next    = edge_path.begin () + 1;

      float comp_length = 0.0f;
      while (next != end)
        {
          float length = line_length (current, next);

          if (comp_length + length > edge_path_position) 
            {
              float perc = (edge_path_position - comp_length) // length to walk from current node
                / length;

              return interpol (current, next, perc);
            }

          ++current;
          ++next;
        }
      assert (!"This shouldn't happen, traveled bejoint target node");
      return target_node->get_pos ();
    }
#endif
  return Vector();
}

} // namespace WorldMapNS

/* EOF */
