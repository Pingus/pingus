//  $Id: pingus.cxx,v 1.19 2002/10/15 21:48:43 grumbel Exp $
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

#include <iostream>
#include <math.h>
#include "../boost/smart_ptr.hpp"
#include "../graphic_context.hxx"
#include "dot.hxx"
#include "pingus.hxx"

namespace WorldMapNS {

Pingus::Pingus (PathGraph* arg_path)
  : Drawable("pingus"),
    path(arg_path),
    sprite ("Pingus/walker0", "pingus", 20.0f, Sprite::RIGHT)
{
  pos.x = 320;
  pos.y = 200;

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
Pingus::update ()
{
  float delta = 0.025f;
  
  sprite.update ();
  if (current_node == NoNode)
    update_walk(delta);
}

void
Pingus::update_walk (float delta)
{
  float velocity = 50.0f;

  //std::cout << "Updating Walk: " << edge_path_position << "/" << edge_path_length << std::endl;
  // Update the edge_path_position
  edge_path_position += velocity * delta;
  
  if (edge_path_position > edge_path.length()) // target reached
    {
      if (node_path.empty ()) // final target reached
        {
          current_node = target_node;
        }
      else // edge is traveled, now go to the next node
        {
          update_edge_path();
        }
    }

  // Recalc pingu position on the screen
  pos = calc_pos ();
}

bool
Pingus::walk_to_node (NodeId target)
{
  if (current_node != NoNode) // pingu stands still
    {
      const PathfinderResult& res = path->get_path (current_node, target);

      std::cout << "XXXXXXX Path cost: " << res.cost << std::endl;
      node_path = res.path;

      // Simulate that we just reached current_node, then update the edge_path
      target_node = node_path.back(); // equal to current_node;
      node_path.pop_back();
      update_edge_path();

      current_node = NoNode;
      return true;
    }
  else // pingu between two nodes
    {
      if (target_node == target)
        {
          node_path.clear();
          return true;
        }
      else if (source_node == target)
        {
          // Reverse the pingu
          std::swap(target_node, source_node);
          edge_path.reverse();
          edge_path_position = edge_path.length() - edge_path_position;         
          node_path.clear();
          return true;
        }
      else
        {
          PathfinderResult node_path1 = path->get_path (source_node, target);
          PathfinderResult node_path2 = path->get_path (target_node, target);
	
          // Select the shorter path
          if (node_path1.cost + edge_path_position
              < node_path2.cost + (edge_path.length() - edge_path_position))
            { // walk to source node, which means to reverse the pingu
              node_path = node_path1.path;

              // Reverse the pingu
              std::swap(target_node, source_node);
              edge_path.reverse();
              edge_path_position = edge_path.length() - edge_path_position;
            }
          else
            { // walk to target_node
              node_path = node_path2.path;
            }
          // Pop the first element on the stack, since we are already targeting it
          node_path.pop_back();

          return true;
        }
    }
}

Vector
Pingus::calc_pos ()
{
  if (current_node != NoNode) // pingu stands still
    {
      return path->graph.resolve_node(current_node).data->get_pos ();
    }
  else // between two nodes
    {
      return edge_path.at(edge_path_position);
    }
}

void
Pingus::set_position (NodeId node)
{
  pos = path->get_dot(node)->get_pos();
  current_node = node;
}

float
Pingus::get_z_pos() const
{
  return 2000.0f;
}

void
Pingus::update_edge_path()
{
  // Update source and target nodes
  source_node = target_node;
  target_node = node_path.back ();
  node_path.pop_back (); // remove target node from list of nodes

  edge_path_position = 0.0f;
  edge_path.clear();

  Path* partial_path = path->graph.resolve_edge(source_node, target_node).data;
          
  edge_path.push_back(path->graph.resolve_node(source_node).data->get_pos());
  // Why do we need to reverse this?!
  edge_path.reverse_insert(*partial_path);
  edge_path.push_back(path->graph.resolve_node(target_node).data->get_pos());
}

bool
Pingus::is_walking()
{
  return current_node == NoNode;
}

} // namespace WorldMapNS

/* EOF */
