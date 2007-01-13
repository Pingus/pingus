//  $Id$
// 
//  Pingus - A free Lemmings clone
//  Copyright (C) 2004 Ingo Ruhnke <grumbel@gmx.de>
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

#ifndef HEADER_GRAPHIC_CONTEXT_STATE_HXX
#define HEADER_GRAPHIC_CONTEXT_STATE_HXX

#include "display/drawing_context.hxx"
#include "display/scene_context.hxx"

class GraphicContextStateImpl;

/** Helper class for capturing the state of a GraphicContext, with
    additional convenience functions to make handling GraphicContexts
    easier */
class GraphicContextState
{
public:
  GraphicContextState();
  GraphicContextState(int w, int h);

  /** Limit the graphic context to the given limits rectangle, meaning
      that no point of the current visible region will be outside the
      limit */
  void set_limit(const CL_Rect& limit);
  void set_unlimited();

  void set_size(int w, int h);

#if 0
  void push(CL_GraphicContext* gc = 0);
  void pop (CL_GraphicContext* gc = 0);
#endif 

  void push(Pingus::DrawingContext& gc);
  void pop (Pingus::DrawingContext& gc);

  void push(Pingus::SceneContext& gc);
  void pop (Pingus::SceneContext& gc);

  /** Return a rectangle in world coordinates that represents the area
      visible on the screen */
  CL_Rect get_clip_rect();

  int get_width()  const;
  int get_height() const;

  /** Set the current rotation angel */
  void  set_rotation(float angle);

  /** Return the current rotation angel */
  float get_rotation();

  /** Move the center of the visible area to pos */
  void      set_pos(const Vector& pos);
  Vector    get_pos() const;

  /** Set zoom to z, while ensuring that the screen position \a pos
      (normaly the position of the mouse pointer) stays in the same
      position even after zoomed in/out */
  void  set_zoom(CL_Pointf pos, float z);
  void  set_zoom(float z);
  float get_zoom(); 

  void zoom_to (const CL_Rectf& rect);

  Vector screen2world(const CL_Point& pos);

private:
  CL_SharedPtr<GraphicContextStateImpl> impl;
};

#endif

/* EOF */
