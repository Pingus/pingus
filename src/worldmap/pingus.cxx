//  $Id: pingus.cxx,v 1.15 2002/10/14 10:19:31 grumbel Exp $
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

  if (edge_path_position > edge_path_length) // target reached
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

float 
Pingus::calc_edge_path_length()
{
  std::cout << "Edgepath size: " << edge_path.size() << std::endl;
  float length = 0;
  Path::iterator prev = edge_path.begin();
  for(Path::iterator next = prev + 1; next != edge_path.end(); ++next)
    {
      length += distance(*prev, *next);
      prev = next;
    }
  return length;
}

bool
Pingus::walk_to_node (NodeId target)
{
  if (current_node != NoNode) // pingu stands still
    {
      node_path = path->get_path (current_node, target);

      // Simulate that we just reached current_node, then update the edge_path
      target_node = node_path.back(); // equal to current_node;
      node_path.pop_back();
      update_edge_path();

      current_node = NoNode;
      return true;
    }
  else // pingu between two nodes
    {
      assert(false);
#if 0
      std::vector<NodeId> node_path1 = path->get_path (source_node, target);
      std::vector<NodeId> node_path2 = path->get_path (target_node, target);
	
      // Select the shorter path
      if (length (node_path1) < length (node_path2))
        { // walk to source node, which means to reverse the pingu
          node_path = node_path1;

          // Reverse the pingu
          std::swap(target_node, source_node);
          std::reverse(edge_path.begin (), edge_path.end ());
          edge_path_position = edge_path_lenght - edge_path_position;
        }
      else
        { // walk to target_node
          node_path = node_path2;
        }
#endif
      return false;
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
      Path::iterator current = edge_path.begin ();
      Path::iterator next    = edge_path.begin () + 1;

      float comp_length = 0.0f;
      while (next != edge_path.end())
        {
          float length = distance(*current, *next);

          if (comp_length + length > edge_path_position) 
            {
              // FIXME: this is incorrect, it causses speed miss match
              float perc = (edge_path_position - comp_length) // length to walk from current node
                / length;

              return interpolate (*current, *next, perc);
            }

          comp_length += length;

          ++current;
          ++next;
        }
      assert (!"This shouldn't happen, traveled bejoint target node");
      return path->graph.resolve_node(target_node).data->get_pos ();
    }
}

float
Pingus::distance(const Vector& a, const Vector& b)
{
  return fabsf(sqrt((a.x * b.x) + (a.y * b.y)));
}

Vector
Pingus::interpolate(const Vector& a, const Vector& b, float perc)
{
  Vector c = b - a;
  return a + (c * perc);
}

void
Pingus::set_position (NodeId node)
{
  pos = path->get_dot(node)->get_pos();
  current_node = node;
  std::cout << "Pingu Pos: " << pos << std::endl;
  walk_to_node(0);
}

float
Pingus::get_z_pos() const
{
  return 2000.0f;
}

void
Pingus:: update_edge_path()
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
  edge_path.insert(edge_path.end(), partial_path->rbegin(), partial_path->rend());
  edge_path.push_back(path->graph.resolve_node(target_node).data->get_pos());
  
  edge_path_length = calc_edge_path_length();
}

} // namespace WorldMapNS

/* EOF */
