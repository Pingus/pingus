//  $Id: EditorObj.hh,v 1.14 2000/07/31 23:45:02 grumbel Exp $
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

#ifndef EDITOROBJ_HH
#define EDITOROBJ_HH

#include <fstream>
#include <functional>
#include <ClanLib/core.h>

#include "../ResDescriptor.hh"
#include "../ExitData.hh"
#include "../EntranceData.hh"

// FIXME: The definitions for trap_data and hotspot_data should go to
// a seperate file.
#include "../PSMParser.hh"
#include "../Trap.hh"
#include "../PLF.hh"
#include "../Position.hh"

///
class EditorObj
{
protected:
  Position pos;
  
public:
  ///
  int x_of, y_of;

  ///
  struct Color {
    Color(float r = 1.0, float g = 1.0, float b = 1.0, float a = 1.0) {
      this->r = r;
      this->g = g;
      this->b = b;
      this->a = a;
    }
    ///
    float r;
    ///
    float g;
    ///
    float b;
    ///
    float a;
  };
  Color mark_color;

  ///
  CL_Surface* surf;
  ///
  ResDescriptor desc;
  
  ///
  EditorObj();
  ///
  virtual ~EditorObj();

  ///
  static EditorObj* create(SurfaceData);
  ///
  static EditorObj* create(EntranceData);
  ///
  static EditorObj* create(ExitData);
  ///
  static EditorObj* create(TrapData);
  ///
  static EditorObj* create(HotspotData);
  ///
  static EditorObj* create(LiquidData);

  /** @name Z-Pos sort operators */
  //@{
  /** True if the z_pos of this is smaller than that of w */
  bool operator< (const EditorObj& w);
  /** True if the z_pos of this is greater than that of w */
  bool operator> (const EditorObj& w);
  //@}

  /// Return true if the object is a group of objects
  virtual bool is_group() { return false; } 

  /** Members to manipulate the objects position */
  //@{
  /** Move the object to the given coordinates */
  virtual void set_position(int new_x_pos, int new_y_pos);
  /** Move the object to the given coordinates */
  virtual void set_position_offset(int x_pos_add, int y_pos_add, 
				   int z_pos_add =0);
  /// Return the object x_pos
  virtual int get_x_pos() { return pos.x_pos; }
  /// Return the object y_pos
  virtual int get_y_pos() { return pos.y_pos; }
  /// Return the object z_pos
  virtual int get_z_pos() { return pos.z_pos; }
  //@}

  /** Draw the object */
  virtual void   draw_offset(int, int);

  /** Draw the caputre rectangle around the object */
  virtual void   draw_mark_offset(int, int, EditorObj::Color* color = 0);
  
  /** Return true when the mouse is over the current object */
  virtual bool   mouse_over(int, int);

  /** Return true if the current object is inside the current
      selection rectangle */
  virtual bool   is_in_rect(int x1, int y1, int x2, int y2);

  /** Save the give object in the ofstream, this member uses the old
      plf/psm syntax which is considered obsolete, save_xml() should
      be used */
  virtual void   save(std::ofstream* plf, std::ofstream* psm) = 0;
  /** Save the given object in the ofstream as xml */
  virtual void   save_xml(std::ofstream* xml) = 0;
  /** Writes the given res_desc to the ofstream */
  static void save_desc_xml(std::ofstream* xml, ResDescriptor desc);
  /** Write a position to the xml ofstream */
  static void save_position_xml(std::ofstream* xml, Position pos);
  ///
  virtual std::string obj_type();
  ///
  virtual std::string status_line();
  ///
  virtual EditorObj* duplicate() = 0;
};

// Structure for the sorting algorithm (stable_sort)
class EditorObj_less : public std::binary_function<EditorObj*, EditorObj*, bool>
{
public:
  bool operator() (EditorObj* a, EditorObj* b) const 
    {
      return (*a) < (*b);
    }
};

class EditorObjGroup : public EditorObj
{
private:
  list<EditorObj*> objs;
public:
  EditorObjGroup();
  virtual ~EditorObjGroup();


  /// Return true if the object is a group of objects
  virtual bool is_group() { return true; } 
  
  ///
  virtual void set_position_offset(int x_pos_add, int y_pos_add, 
				   int z_pos_add =0);
  ///
  virtual void set_position(int new_x_pos, int new_y_pos);

  /** Draw the object */
  virtual void   draw_offset(int, int);

  /** Draw the caputre rectangle around the object */
  virtual void   draw_mark_offset(int, int, EditorObj::Color* arg_color = 0);

  /** Return true when the mouse is over the current object */
  virtual bool   mouse_over(int, int);

  /** Return true if the current object is inside the current
      selection rectangle */
  virtual bool   is_in_rect(int x1, int y1, int x2, int y2);

  /** Add an object to the group */
  virtual void EditorObjGroup::push_back(EditorObj*);

  ///
  virtual list<EditorObj*>* EditorObjGroup::get_objs();

  ///
  virtual void   save(std::ofstream* plf, std::ofstream* psm);
  ///
  virtual void   save_xml(std::ofstream* xml);
  ///
  virtual EditorObj* duplicate();
};

#endif

/* EOF */

