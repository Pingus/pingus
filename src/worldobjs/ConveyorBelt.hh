//  $Id: ConveyorBelt.hh,v 1.4 2000/12/06 08:54:41 grumbel Exp $
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

#ifndef CONVEYORBELT_HH
#define CONVEYORBELT_HH

#include <ClanLib/core.h>
#include "../WorldObj.hh"
#include "../WorldObjData.hh"
#include "../editor/EditorWorldObj.hh"

class ConveyorBeltData : public WorldObjData
{
public:
  Position pos;
  int width;
  int speed;

  ConveyorBeltData ();
  virtual ~ConveyorBeltData () {}

  /** Writte the content of this object formated as xml to the given
      stream */
  virtual void write_xml(ofstream* xml);
  ///
  static WorldObjData* create(xmlDocPtr doc, xmlNodePtr cur);
};

class ConveyorBelt : private ConveyorBeltData,
		     public  WorldObj
{
private:
  CL_Surface* left_sur;
  CL_Surface* right_sur;
  CL_Surface* middle_sur;
  int counter;
  int catch_counter;
public:
  ///
  ConveyorBelt (WorldObjData*);
  ///
  virtual ~ConveyorBelt () {}
  ///
  virtual void draw_offset (int x_of, int y_of, float s = 1.0);
  ///
  virtual void draw_colmap();
  ///
  virtual void let_move(void);
  ///
  virtual int  get_z_pos() const { return pos.z_pos; }
};

class EditorConveyorBeltObj : public EditorWorldObj,
			      public ConveyorBeltData
{
private:
  CL_Surface* left_sur;
  CL_Surface* right_sur;
  CL_Surface* middle_sur;
  int counter;

public:
  EditorConveyorBeltObj (WorldObjData* obj);
  virtual ~EditorConveyorBeltObj ();

  virtual EditorObj* duplicate();
  virtual void draw_offset(int, int);
  virtual void draw_scroll_map(int x_pos, int y_pos, int arg_width, int arg_height);
  
  /// The saveing will be done in EditorTeleporterObj::save_xml
  static std::list<EditorObj*> create (WorldObjData* obj);

  /** Create the object with resonable defaults */
  static std::list<EditorObj*> create (const Position& pos);

  virtual void save_xml (std::ofstream* xml);
  virtual std::string status_line();
};

#endif

/* EOF */
