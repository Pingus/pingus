//  $Id: SwitchDoor.hh,v 1.9 2001/04/21 10:55:17 grumbel Exp $
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

  /** Writte the content of this object formated as xml to the given
      stream */
  virtual void write_xml(ofstream* xml);
  ///
  static boost::shared_ptr<WorldObjData> create(xmlDocPtr doc, xmlNodePtr cur);
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

  /** The current height of the door, it might get less when the door
      is opening, it gets zero when the door is fully opened */
  int current_door_height;

public:
  ///
  SwitchDoor (WorldObjData* data);
  ///
  virtual ~SwitchDoor ();
  ///
  virtual void draw_colmap();
  ///
  virtual void draw_offset(int x, int y, float s = 1.0);
  ///
  virtual void update(float delta);
  /// The switch and the door should stay above the pingus
  virtual int  get_z_pos() const { return 100; }
};

/** A dummy object to represent the switch for a switchdoor, all real
    work is done inside EditorSwitchDoorObj */
class EditorSwitchDoorSwitchObj : public EditorObj
{
public:
  EditorSwitchDoorSwitchObj (SwitchDoorData* obj);
  virtual ~EditorSwitchDoorSwitchObj ();

  virtual void save_xml (std::ofstream* xml);
  virtual std::string status_line();
};

class EditorSwitchDoorObj : public SwitchDoorData,
			    public EditorWorldObj
{
private:
  CL_Surface door_box;
  CL_Surface door_tile;

public:
  EditorSwitchDoorObj ();
  EditorSwitchDoorObj (WorldObjData* obj);
  virtual ~EditorSwitchDoorObj ();

  virtual void draw_offset(int x_of, int y_of);
  static std::list<boost::shared_ptr<EditorObj> > create (WorldObjData* obj);

  /** Create this object (and child objects) with resonable defaults
      for the editor */
  static std::list<boost::shared_ptr<EditorObj> > create (const CL_Vector& pos);

  virtual void save_xml (std::ofstream* xml);
  virtual std::string status_line();
};

#endif

/* EOF */
