//  $Id: editorobj.hxx,v 1.19 2003/04/19 10:23:18 torangan Exp $
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

#ifndef HEADER_PINGUS_EDITOR_EDITOROBJ_HXX
#define HEADER_PINGUS_EDITOR_EDITOROBJ_HXX

#include "../pingus.hxx"
#include <string>
#include <iosfwd>
#include <vector>

class CL_Rect;
class Vector;
class CL_Component;

class EditorObj;

typedef std::vector<EditorObj*> EditorObjLst;

namespace EditorNS {
class PropertyFrame;
class Editor;
class EditorView;
} // namespace EditorNS

/** Interface for all objects which can be shown in the editor */
class EditorObj
{
private:
  static EditorNS::Editor* editor;

public:
  /** Set the parent editor of all objects */
  static void set_editor(EditorNS::Editor* e) { editor = e; }

  static EditorNS::Editor* get_editor () { return editor; }

  EditorObj ();
  virtual ~EditorObj ();

  /** Draw the object to the given view */
  virtual void draw (EditorNS::EditorView * view) =0;

  /** Called once a game loop, the delta is the time passed since the
      last update in seconds */
  virtual void update (float /*delta*/) {};

  /** Draw a rectangle or shape around the object to the given view */
  virtual void draw_mark (EditorNS::EditorView * view) =0;

  /** Return true when the object is under the given coordinates */
  virtual bool is_over(const Vector&) =0;

  /** Return true if the object is inside the given rectangle */
  virtual bool is_in_rect(const CL_Rect& rect) =0;

  /** Return the z-position of the object, the objects are sorted
      after this value */
  virtual float get_z_pos() =0;

  /** Move the object to the given offset */
  virtual void set_position_offset(const Vector& offset) =0;

  /** Generic operations that can make an object larger, what exactly
      happens is object dependend. Default is to do nothing */
  virtual void make_larger () {}

  /** Generic operations that can make an object smaller, what exactly
      happens is object dependend. Default is to do nothing */
  virtual void make_smaller () {}

  /** Flip the object vertical */
  virtual void vertical_flip () {}

  /** Flip the object horizontal */
  virtual void horizontal_flip () {}

  /** Rotate the object 90 degrees */
  virtual void rotate_90 () {}

  /** Rotate the object -90 degrees */
  virtual void rotate_270 () {}

  /** This function is called once at the start of an interactive move */
  virtual void drag () {}

  /** This function is called once at the end of an interactive move */
  virtual void drop () {}

  /** Write the given object down into a XML file */
  virtual void write_xml(std::ostream& xml) =0;

  /** Return a string that is shown in the status line */
  virtual std::string status_line();

  /** Duplicate the given editor object and return a the copied
      object */
  virtual EditorObj* duplicate() =0;

  /** Return a pointer to a CL_Component which can be used to
      manipulate the properties of this gameobject. The caller is
      responsible for deleting the object after usage. The
      CL_Component* should/must be a CL_Frame so that it can get
      embedded in the properties window */
  virtual EditorNS::PropertyFrame* get_gui_dialog (EditorNS::Editor* parent) =0;

protected:
  EditorObj (const EditorObj&) { }
  EditorObj& operator= (const EditorObj&) { return *this; }
};

#endif

/* EOF */
