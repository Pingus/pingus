//  $Id: SwitchDoor.hh,v 1.16 2001/08/10 19:59:20 grumbel Exp $
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

#ifndef SWITCHDOOR_HH
#define SWITCHDOOR_HH

#include <ClanLib/core.h>
#include "../boost/smart_ptr.hpp"
#include "../WorldObj.hh"
#include "../WorldObjData.hh"
#include "../editor/EditorWorldObj.hh"

/** A variable height door which can block the way and which can be
    opened by a switch */
class SwitchDoorData : public WorldObjData
{
private:
  
public:
  /// The upper/middle pos of the door 
  CL_Vector door_pos;
  
  /// The bottom/middle pos of the switch
  CL_Vector switch_pos;

  // The height of the door in graphic tiles
  int door_height;

  SwitchDoorData ();

  /** Write the content of this object formatted as xml to the given
      stream */
  void write_xml(std::ofstream* xml);
  ///
  static boost::shared_ptr<WorldObjData> create(xmlDocPtr doc, xmlNodePtr cur);

  /** Create an WorldObj from the given data object */
  boost::shared_ptr<WorldObj> create_WorldObj ();

  /** Create an EditorObj from the given data object */
  std::list<boost::shared_ptr<EditorObj> > create_EditorObj ();
};

/** A door and a switch, if a pingu is passing the switch, the door
    will open. */
class SwitchDoor : public SwitchDoorData,
		   public WorldObj
{
private:
  CL_Surface door_box;
  CL_Surface door_tile;
  CL_Surface door_tile_cmap;
  CL_Surface switch_sur;
  
  /** True if the door is opening */
  bool is_opening;

  /** The current height of the door, it might decrease when the door
      is opening, it will be zero when the door is fully opened */
  int current_door_height;

public:
  SwitchDoor (WorldObjData* data);
  
  void draw_colmap();
  void draw_offset(int x, int y, float s = 1.0);
  void update(float delta);
  /// The switch and the door should stay above the pingus
  int  get_z_pos() const { return 100; }
};

/** A dummy object to represent the switch for a switchdoor, all real
    work is done inside EditorSwitchDoorObj */
class EditorSwitchDoorSwitchObj : public EditorObj
{
public:
  EditorSwitchDoorSwitchObj (SwitchDoorData* obj);
  
  boost::shared_ptr<EditorObj> duplicate();

  void save_xml (std::ofstream* xml);
  std::string status_line();
};

class EditorSwitchDoorObj : public SwitchDoorData,
			    public EditorWorldObj
{
private:
  CL_Surface door_box;
  CL_Surface door_tile;

public:
  EditorSwitchDoorObj (WorldObjData* obj);
  
  static std::list<boost::shared_ptr<EditorObj> > create (WorldObjData* obj);

  /** Create this object (and child objects) with reasonable defaults
      for the editor */
  static std::list<boost::shared_ptr<EditorObj> > create (const CL_Vector& pos);

  boost::shared_ptr<EditorObj> duplicate();

  void make_larger ();
  void make_smaller ();

  void draw (boost::dummy_ptr<EditorView> view);
  void save_xml (std::ofstream* xml);
  std::string status_line();
};

#endif

/* EOF */
