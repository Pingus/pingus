//  $Id: RectEditorObj.hh,v 1.3 2002/06/08 20:19:54 torangan Exp $
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

#ifndef RECTEDITOROBJ_HH
#define RECTEDITOROBJ_HH

#include "EditorObj.hh"

/** Abstract class which can be used for all objects which have a
    rectangular shape. */
class RectEditorObj : public EditorObj
{
public:
  RectEditorObj();
  virtual ~RectEditorObj();

  /** Returns the position of the upper left corner of the object */
  virtual CL_Vector get_upper_left_corner () =0;

  /// Return the object width
  virtual int get_width() =0;
  /// Return the object height
  virtual int get_height() =0;

  /** Draw the caputre rectangle around the object */
  virtual void   draw_mark (EditorView * view);
  
  /** Return true when the object is under the given coordinates */
  virtual bool   is_over(const CL_Vector&);

  /** Return true if the current object is inside the current
      selection rectangle */
  virtual bool   is_in_rect(const CL_Rect&);
};

/*
// Structure for the sorting algorithm (stable_sort)
class EditorObj_less : public std::binary_function<boost::shared_ptr<EditorObj>, boost::shared_ptr<EditorObj>, bool>
{
public:
  bool operator() (boost::shared_ptr<EditorObj> a, boost::shared_ptr<EditorObj> b) const 
    {
      return (*a) < (*b);
    }
};
*/
#endif

/* EOF */

