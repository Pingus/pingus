//  $Id: EditorObj.hh,v 1.31 2001/08/07 11:24:40 grumbel Exp $
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
#include "../boost/smart_ptr.hpp"
#include "../boost/dummy_ptr.hpp"

#include "EditorView.hh"
#include "../ResDescriptor.hh"
#include "../ExitData.hh"
#include "../EntranceData.hh"
#include "../WeatherData.hh"

#include "../PSMParser.hh"
#include "../Trap.hh"
#include "../PLF.hh"

class Editor;

///
class EditorObj
{
private:
  /** If the child class does not provide a pos member, we you this
      one */
  CL_Vector private_pos;
protected:
  static Editor* editor;

  /** The position is a pointer to the position object of the child
      class, when the child class doesn't provide a position element,
      EditorObj creates one... FIXME: UGGGggglllllyyyyy.... */
  CL_Vector* position;
  ///
  int x_of, y_of;
  ///
  int width;
  ///
  int height;

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
  CL_Surface surf;

  /** Stupid thing to convert a single object into a list containing
      that object */
  static std::list<boost::shared_ptr<EditorObj> > make_list(EditorObj* obj) 
  {
    assert (obj);
    std::list<boost::shared_ptr<EditorObj> > objs;
    objs.push_back (boost::shared_ptr<EditorObj>(obj));
    return objs;
  }

public:
  ///
  EditorObj();
  ///
  virtual ~EditorObj();
  ///
  static std::list<boost::shared_ptr<EditorObj> > create(GroundpieceData);
  ///
  static std::list<boost::shared_ptr<EditorObj> > create(EntranceData);
  ///
  static std::list<boost::shared_ptr<EditorObj> > create(ExitData);
  ///
  static std::list<boost::shared_ptr<EditorObj> > create(TrapData);
  ///
  static std::list<boost::shared_ptr<EditorObj> > create(HotspotData);
  ///
  static std::list<boost::shared_ptr<EditorObj> > create(LiquidData);
  ///
  static std::list<boost::shared_ptr<EditorObj> > create (WeatherData);
  ///
  static std::list<boost::shared_ptr<EditorObj> > create (WorldObjData*);

  /** @name Z-Pos sort operators */
  //@{
  /** True if the z_pos of this is smaller than that of w */
  bool operator< (const EditorObj& w);
  /** True if the z_pos of this is greater than that of w */
  bool operator> (const EditorObj& w);
  //@}

  /** Members to manipulate the objects position */
  //@{
  /** Move the object to the given coordinates */
  virtual void set_position(float new_x_pos, float new_y_pos);
  /** Move the object to the given coordinates */
  virtual void set_position_offset(float x_pos_add, float y_pos_add, 
				   float z_pos_add =0);
  /// Return the object x_pos
  virtual int get_x_pos() { return (int) position->x; }
  /// Return the object y_pos
  virtual int get_y_pos() { return (int) position->y; }
  /// Return the object z_pos
  virtual int get_z_pos() { return (int) position->z; }

  /// Return the object width
  virtual int get_width() { return width; }
  /// Return the object height
  virtual int get_height() { return height; }
  //@}

  /** Generic operations that can make an object larger, what exactly
      happens is object dependend */
  virtual void make_larger ();

  /** Generic operations that can make an object smaller, what exactly
      happens is object dependend */
  virtual void make_smaller ();

  /** Open a gui dialog to edit the object */
  virtual void gui_edit_obj();

  /** Draw the object */
  virtual void draw (boost::dummy_ptr<EditorView> view);

  virtual void draw_scroll_map(int x_pos, int y_pos, int arg_width, int arg_height);

  /** Draw the caputre rectangle around the object */
  virtual void   draw_mark (boost::dummy_ptr<EditorView> view);
  
  /** Return true when the object is under the given coordinates */
  virtual bool   is_over(int, int);

  /** Return true if the current object is inside the current
      selection rectangle */
  virtual bool   is_in_rect(int x1, int y1, int x2, int y2);

  /** Save the give object in the ofstream, this member uses the old
      plf/psm syntax which is considered obsolete, save_xml() should
      be used */
  virtual void   save(std::ofstream* plf, std::ofstream* psm) {};
  /** Save the given object in the ofstream as xml */
  virtual void   save_xml(std::ofstream* xml) = 0;
  ///
  virtual std::string status_line();
  ///
  virtual boost::shared_ptr<EditorObj> duplicate() { return boost::shared_ptr<EditorObj>(); }
  
  static void set_editor(Editor* e) { editor = e; }
};

// Structure for the sorting algorithm (stable_sort)
class EditorObj_less : public std::binary_function<boost::shared_ptr<EditorObj>, boost::shared_ptr<EditorObj>, bool>
{
public:
  bool operator() (boost::shared_ptr<EditorObj> a, boost::shared_ptr<EditorObj> b) const 
    {
      return (*a) < (*b);
    }
};

#include "Editor.hh"

#endif

/* EOF */

