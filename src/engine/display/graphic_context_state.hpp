// Pingus - A free Lemmings clone
// Copyright (C) 2004 Ingo Ruhnke <grumbel@gmail.com>
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef HEADER_PINGUS_ENGINE_DISPLAY_GRAPHIC_CONTEXT_STATE_HPP
#define HEADER_PINGUS_ENGINE_DISPLAY_GRAPHIC_CONTEXT_STATE_HPP

#include <memory>

#include "engine/display/scene_context.hpp"

namespace pingus {

class GraphicContextStateImpl;

/** Helper class for capturing the state of a GraphicContext, with
    additional convenience functions to make handling GraphicContexts
    easier */
class GraphicContextState
{
public:
  GraphicContextState();
  GraphicContextState(int w, int h);
  GraphicContextState(geom::irect const& rect);

  /** Limit the graphic context to the given limits rectangle, meaning
      that no point of the current visible region will be outside the
      limit */
  void set_limit(geom::irect const& limit);
  void set_unlimited();

  void set_size(int w, int h);

  void push(DrawingContext& gc);
  void pop (DrawingContext& gc);

  void push(SceneContext& gc);
  void pop (SceneContext& gc);

  /** Return a rectangle in world coordinates that represents the area
      visible on the screen */
  geom::irect get_clip_rect();

  int get_width()  const;
  int get_height() const;

  /** Move the center of the visible area to pos */
  void      set_pos(geom::ipoint const& pos);
  geom::ipoint  get_pos() const;

  geom::ipoint screen2world(geom::ipoint const& pos) const;

private:
  std::shared_ptr<GraphicContextStateImpl> impl;
};

} // namespace pingus

#endif

/* EOF */
