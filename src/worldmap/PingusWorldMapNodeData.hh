//  $Id: PingusWorldMapNodeData.hh,v 1.5 2002/06/10 13:03:36 torangan Exp $
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

#ifndef PINGUSWORLDMAPNODEDATA_HH
#define PINGUSWORLDMAPNODEDATA_HH

#include <list>
#include <string>
#include <ClanLib/Core/Math/cl_vector.h>

class _xmlDoc;  typedef _xmlDoc*  xmlDocPtr;
class _xmlNode; typedef _xmlNode* xmlNodePtr;


/* Wannabe syntax of worldmaps:
   ----------------------------

   <level id="1" accessible="1">
   <node>
   <position>
   <x-pos>50</x-pos>
   <y-pos>377</y-pos>
   </position>
   <link id="2"/>
   </node>
      
   <level name="level1.xml"/>
   </level>

   <tube id="...">
   <node>
   ...
*/

namespace Pingus 
{
  namespace WorldMap 
  {
    class Node;

    /** Node */
    class NodeData
    {
    protected:
      /** The id of this node, id's have to be uniq in a worldmap */
      int id;

      /** List of connections to other node, the int values are the id's
	  of other nodes */
      std::list<int> links;  

      /** The position of the node in the worldmap, pos.z is used for
	  z-sorting */
      CL_Vector pos;

    public:
      virtual ~NodeData () {}

      int       get_id () { return id; } 
      CL_Vector get_pos () { return pos; } 
      std::list<int>& get_links () { return links; } 

      void assign (const NodeData& data) 
      {
	id    = data.id;
	links = data.links;
	pos   = data.pos;
      }
      virtual Node* create ();

      static NodeData* create(xmlDocPtr doc, xmlNodePtr cur);
    };

    /** EmptyNode */ 
    class EmptyNodeData
      : public NodeData
    {
    protected:
    public:
      virtual ~EmptyNodeData () {}

      virtual Node* create ();

      static NodeData* create(xmlDocPtr doc, xmlNodePtr cur);
    };

    /** Level */
    class LevelNodeData
      : public NodeData
    {
    protected:
      /** The filename of the level to start 

      FIXME: Is the filename relative to the worldmap directory,
      FIXME: relative to the ~/.pingus/ directory or what?
      */
      std::string levelname;
    public:
      virtual ~LevelNodeData () {}

      virtual Node* create ();

      static LevelNodeData* create(xmlDocPtr doc, xmlNodePtr cur);
      void write_xml(std::ofstream* xml) {}
    };

    /** Tube */
    class TubeNodeData
      : public NodeData
    {
    protected:
      /** The world to which this tupe 'beams' */
      std::string worldmap;
  
      /** The node id in the worldmap to which this tube beams/links */
      int link_node;
    public:
      virtual ~TubeNodeData () {}

      virtual Node* create ();

      static TubeNodeData* create(xmlDocPtr doc, xmlNodePtr cur);
    };
  }
}

#endif

/* EOF */



