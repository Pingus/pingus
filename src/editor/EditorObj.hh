//  $Id: EditorObj.hh,v 1.13 2000/07/30 01:47:37 grumbel Exp $
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
  
public:
  Position pos;
  ///
  int x_of, y_of;

  ///
  struct {
    ///
    float r;
    ///
    float g;
    ///
    float b;
    ///
    float a;
  } ///
 mark_color;

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

  ///
  virtual void   draw_offset(int, int);
  ///
  virtual void   draw_mark_offset(int, int);
  ///
  virtual bool   mouse_over(int, int);
  ///
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
}///
;

// Structure for the sorting algorithm (stable_sort)
class EditorObj_less : public std::binary_function<EditorObj*, EditorObj*, bool>
{
public:
  bool operator() (EditorObj* a, EditorObj* b) const 
    {
      return (*a) < (*b);
    }
};

class EditorGroup : public EditorObj
{
private:
  list<EditorObj*> objs;
public:
  EditorGroup();
  virtual ~EditorGroup();
  ///
  virtual void   save(std::ofstream* plf, std::ofstream* psm);
  ///
  virtual void   save_xml(std::ofstream* xml);
  ///
  virtual EditorObj* duplicate();
};

#endif

/* EOF */

