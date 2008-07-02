//  $Id: drawing_request.hpp 3582 2008-06-29 16:56:56Z grumbel $
// 
//  Pingus - A free Lemmings clone
//  Copyright (C) 2002 Ingo Ruhnke <grumbel@gmx.de>
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

#ifndef HEADER_DRAWING_REQUEST_HPP
#define HEADER_DRAWING_REQUEST_HPP

#include "SDL.h"
#include "math/vector3f.hpp"
#include "math/rect.hpp"

/** 
 */
class DrawingRequest
{
protected:
  Vector3f pos;
  bool     valid;

  std::vector<Rect> dirty_rects;

public:
  DrawingRequest(const Vector3f& pos_) : pos(pos_), valid(true) {}
  virtual ~DrawingRequest() {};
  
  virtual void render(SDL_Surface* gc, const Rect& rect) = 0;

  virtual void mark(const Rect& r) { dirty_rects.push_back(r); }
  
  /** Returns true if the request contains an alpha channel and needs
      to be drawn in order */
  virtual bool has_alpha() { return true; }

  /** Returns the position at which the request should be drawn */
  virtual float get_z_pos() { return pos.z; }

  virtual void set_valid(bool v) { valid = v; }
  virtual bool is_valid() const { return valid; }
private:
  DrawingRequest (const DrawingRequest&);
  DrawingRequest& operator= (const DrawingRequest&);
};

#endif

/* EOF */
