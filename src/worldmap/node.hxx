//  $Id: node.hxx,v 1.11 2002/10/24 13:01:50 grumbel Exp $
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

#ifndef HEADER_PINGUS_WORLDMAP_NODE_HXX
#define HEADER_PINGUS_WORLDMAP_NODE_HXX

#include "../sprite.hxx"
#include "graph.hxx"
#include "node_data.hxx"

#error "This file should not be used! Remove it from the projects file list!"

class Vector;
class PLF;

namespace WorldMapNS {

/** An node in the WorldmapGraph on which the Pingu walks */
class Node 
{
public:
  /** should links or nodes be accessible? */
  bool accessible;
  
  Node () {}
  virtual ~Node () {}
  
  virtual void on_click () =0;
  virtual void mark (bool /*value*/) {}
  virtual void draw (Vector /*offset*/) {}
  virtual std::string get_string () =0;

  virtual int  get_id () =0;
  virtual Vector get_pos () =0;
  virtual std::list<int>& get_links () =0;
      
private:
  Node (const Node&);
  Node& operator= (const Node&);
};

/** A wrap/beam object which brings you to the next worldmap 
    FIXME: Multi-Inheritage is evil */
class TubeNode : public Node, 
		 public TubeNodeData
{
public:
  std::string worldmap_name;
  Sprite tube;
  int link_node;
public:  
  TubeNode (const TubeNodeData&);
  void on_click ();
  void draw (Vector offset);
  std::string get_string ();

  int  get_id ()               { return NodeData::get_id();    }
  Vector get_pos ()         { return NodeData::get_pos();   }
  std::list<int>& get_links () { return NodeData::get_links(); }

private:
  TubeNode (const TubeNode&);
  TubeNode& operator= (const TubeNode&);
};

/** The entrance to a level 
    FIXME: Multi-Inheritage is evil */
class LevelNode :  public Node,
		   public LevelNodeData
{
private:
  Sprite green_dot;
  Sprite red_dot;
  Sprite invalid_dot;
  Sprite dot_border;
  Sprite green_flag;

  PLF* plf;
  
  /** true if the level is invalid, which means that the levelfile
      could not be loaded or had errors. false is the default */
  bool invalid;

public:
  bool finished;
  PLF* get_plf ();

  LevelNode (const LevelNodeData&);
  virtual ~LevelNode ();
  void on_click ();
  void mark (bool value);
  void draw (Vector offset);
  std::string get_string ();

  NodeId  get_id () { return NodeData::get_id(); }
  Vector  get_pos () { return NodeData::get_pos(); }
  std::list<int>& get_links () { return NodeData::get_links(); }
      
private:
  LevelNode (const LevelNode&);
  LevelNode& operator= (const LevelNode&);
};

} // namespace WorldMapNS

#endif

/* EOF */
