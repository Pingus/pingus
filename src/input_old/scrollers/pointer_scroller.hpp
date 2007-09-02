//  $Id$
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

#ifndef HEADER_PINGUS_INPUT_POINTER_SCROLLER_HXX
#define HEADER_PINGUS_INPUT_POINTER_SCROLLER_HXX

#include "../scroller.hpp"

namespace Input {

class Button;
class Pointer;

namespace Scrollers {

/**
   @brief allows a Pointer to be used as a Scroller

   XML definition: <pointer-scroller> <pointer><button> </pointer-scroller>

   A PointerScroller creates ScrollEvents whenever the associated Button is pressed.
   The Pointer itself is then reset to it's original position to prevent the visible
   pointer from moving while the Pointer is used as a Scroller.
*/
class PointerScroller : public Scroller {
private:
  Pointer* const pointer;
  Button*  const modifier;

  float x_delta;
  float y_delta;
  float x_pos;
  float y_pos;

public:

  PointerScroller (Pointer* pointer_, Button* modifier_);
  ~PointerScroller ();

  const float& get_x_delta () const;
  const float& get_y_delta () const;

  void  get_delta (float& x, float& y) const;

  void  update (float delta);

private:
  PointerScroller (const PointerScroller&);
  PointerScroller& operator= (const PointerScroller&);
};

} // namespace Pointer
} // namespace Input

#endif

/* EOF */
