//  $Id: rect_editorobj.hxx,v 1.7 2002/09/28 11:52:23 torangan Exp $
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

#ifndef HEADER_PINGUS_EDITOR_RECT_EDITOROBJ_HXX
#define HEADER_PINGUS_EDITOR_RECT_EDITOROBJ_HXX

#include "editorobj.hxx"

/** Abstract class which can be used for all objects which have a
    rectangular shape. */
class RectEditorObj : public EditorObj
{
public:
  RectEditorObj ();

  /** Returns the position of the upper left corner of the object */
  virtual Vector get_upper_left_corner () =0;

  /// Return the object width
  virtual int get_width () =0;
  /// Return the object height
  virtual int get_height () =0;

  /** Draw the caputre rectangle around the object */
  virtual void   draw_mark (EditorNS::EditorView * view);
  
  /** Return true when the object is under the given coordinates */
  virtual bool   is_over (const Vector&);

  /** Return true if the current object is inside the current
      selection rectangle */
  virtual bool   is_in_rect (const CL_Rect&);
  
protected:
  RectEditorObj (const RectEditorObj& old);
  RectEditorObj& operator= (const RectEditorObj& old);
};

#endif

/* EOF */

