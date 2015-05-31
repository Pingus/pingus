// Pingus - A free Lemmings clone
// Copyright (C) 2002 Ingo Ruhnke <grumbel@gmail.com>
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

#ifndef HEADER_PINGUS_ENGINE_DISPLAY_DRAWING_REQUEST_HPP
#define HEADER_PINGUS_ENGINE_DISPLAY_DRAWING_REQUEST_HPP

#include <SDL.h>

#include "math/rect.hpp"
#include "math/vector2i.hpp"

class Framebuffer;

class DrawingRequest
{
protected:
  Vector2i pos;
  float    z;

public:
  DrawingRequest(const Vector2i& pos_, float z_) : pos(pos_), z(z_) {}
  virtual ~DrawingRequest() {};

  /** \a rect is the rectangle that is managed by the parent
      DrawingContext, all calls to fb have to be offset with
      (rect.left,rect.top)  */
  virtual void render(Framebuffer& fb, const Rect& rect) = 0;

  /** Returns true if the request contains an alpha channel and needs
      to be drawn in order */
  virtual bool has_alpha() { return true; }

  /** Returns the position at which the request should be drawn */
  virtual float get_z_pos() { return z; }

private:
  DrawingRequest (const DrawingRequest&);
  DrawingRequest& operator= (const DrawingRequest&);
};

#endif

/* EOF */
