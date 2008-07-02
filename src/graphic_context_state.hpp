//  $Id: graphic_context_state.hpp 3582 2008-06-29 16:56:56Z grumbel $
// 
//  Pingus - A free Lemmings clone
//  Copyright (C) 2004 Ingo Ruhnke <grumbel@gmx.de>
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//  
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//  
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef HEADER_GRAPHIC_CONTEXT_STATE_HPP
#define HEADER_GRAPHIC_CONTEXT_STATE_HPP

#include <boost/shared_ptr.hpp>
#include "math/rect.hpp"
#include "math/vector3f.hpp"
#include "math/vector2i.hpp"
#include "math/vector2f.hpp"
#include "display/drawing_context.hpp"
#include "display/scene_context.hpp"

class GraphicContextStateImpl;

/** Helper class for capturing the state of a GraphicContext, with
    additional convenience functions to make handling GraphicContexts
    easier */
class GraphicContextState
{
public:
  GraphicContextState();
  GraphicContextState(int w, int h);
  GraphicContextState(const Rect& rect);

  /** Limit the graphic context to the given limits rectangle, meaning
      that no point of the current visible region will be outside the
      limit */
  void set_limit(const Rect& limit);
  void set_unlimited();

  void set_size(int w, int h);

  void push(DrawingContext& gc);
  void pop (DrawingContext& gc);

  void push(SceneContext& gc);
  void pop (SceneContext& gc);

  /** Return a rectangle in world coordinates that represents the area
      visible on the screen */
  Rect get_clip_rect();

  int get_width()  const;
  int get_height() const;

  /** Set the current rotation angel */
  void  set_rotation(float angle);

  /** Return the current rotation angel */
  float get_rotation();

  /** Move the center of the visible area to pos */
  void      set_pos(const Vector2f& pos);
  Vector2f  get_pos() const;

  /** Set zoom to z, while ensuring that the screen position \a pos
      (normaly the position of the mouse pointer) stays in the same
      position even after zoomed in/out */
  void  set_zoom(Vector2f pos, float z);
  void  set_zoom(float z);
  float get_zoom(); 

  void zoom_to (const Rectf& rect);

  Vector2f screen2world(const Vector2i& pos) const;

private:
  boost::shared_ptr<GraphicContextStateImpl> impl;
};

#endif

/* EOF */
