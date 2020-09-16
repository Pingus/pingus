/*
**  ClanLib SDK
**  Copyright (c) 1997-2005 The ClanLib Team
**
**  This software is provided 'as-is', without any express or implied
**  warranty.  In no event will the authors be held liable for any damages
**  arising from the use of this software.
**
**  Permission is granted to anyone to use this software for any purpose,
**  including commercial applications, and to alter it and redistribute it
**  freely, subject to the following restrictions:
**
**  1. The origin of this software must not be misrepresented; you must not
**     claim that you wrote the original software. If you use this software
**     in a product, an acknowledgment in the product documentation would be
**     appreciated but is not required.
**  2. Altered source versions must be plainly marked as such, and must not be
**     misrepresented as being the original software.
**  3. This notice may not be removed or altered from any source distribution.
**
**  Note: Some of the libraries ClanLib may link to may have additional
**  requirements or restrictions.
**
**  File Author(s):
**
**    Magnus Norddahl
**    (if your name is missing here, please add it)
*/

#ifndef __MATH_RECT_HPP__
#define __MATH_RECT_HPP__

#include <math.h>

#include "math/origin.hpp"
#include "math/vector2f.hpp"
#include "math/vector2i.hpp"
#include "math/size.hpp"

//: 2D (left,top,right,bottom) rectangle structure.
//- !group=Core/Math!
//- !header=core.h!
class Rect
{
  //! Construction:
public:
  //: Constructs an rectangle.
  //param left: Initial left position of rectangle.
  //param top: Initial top position of rectangle.
  //param right: Initial right position of rectangle.
  //param bottom: Initial bottom position of rectangle.
  //param point: Initial top-left position of rectangle.
  //param size: Initial size of rectangle.
  //param rect: Initial rectangle position and size.
  Rect()
    : left(0),
      top(0),
      right(0),
      bottom(0)
  {}

  Rect(const Rect&) = default;
  Rect& operator=(const Rect&) = default;

  Rect(int new_left, int new_top, int new_right, int new_bottom)
    : left(new_left),
      top(new_top),
      right(new_right),
      bottom(new_bottom)
  {}

  Rect(const Vector2i &p, const Size &size)
    : left(p.x),
      top(p.y),
      right(left + size.width()),
      bottom(top + size.height())
  {}

  Rect grow(int b) const {
    return Rect(left   - b,
                top    - b,
                right  + b,
                bottom + b);
  }

  //: Rect += Rect operator.
  Rect &operator+=(const Rect &r)
  { left += r.left; top += r.top; right += r.right; bottom += r.bottom; return *this; }

  //: Rect -= Rect operator.
  Rect &operator-=(const Rect &r)
  { left -= r.left; top -= r.top; right -= r.right; bottom -= r.bottom; return *this; }

  //: Rect += Vector2i operator.
  Rect &operator+=(const Vector2i &p)
  { left += p.x; top += p.y; right += p.x; bottom += p.y; return *this; }

  //: Rect -= Vector2i operator.
  Rect &operator-=(const Vector2i &p)
  { left -= p.x; top -= p.y; right -= p.x; bottom -= p.y; return *this; }

  //: Rect + Rect operator.
  Rect operator+(const Rect &r) const
  { return Rect(left + r.left, top + r.top, right + r.right, bottom + r.bottom); }

  //: Rect - Rect operator.
  Rect operator-(const Rect &r) const
  { return Rect(left - r.left, top - r.top, right - r.right, bottom - r.bottom); }

  //: Rect + Vector2i operator.
  Rect operator+(const Vector2i &p) const
  { return Rect(left + p.x, top + p.y, right + p.x, bottom + p.y); }

  //: Rect - Vector2i operator.
  Rect operator-(const Vector2i &p) const
  { return Rect(left - p.x, top - p.y, right - p.x, bottom - p.y); }

  //: Rect == Rect operator.
  bool operator==(const Rect &r) const
  { return (left == r.left && top == r.top && right == r.right && bottom && r.bottom); }

  //: Rect != Rect operator.
  bool operator!=(const Rect &r) const
  { return (left != r.left || top != r.top || right != r.right || bottom != r.bottom); }

  //! Attributes:
public:
  //: X1-coordinate.
  int left;

  //: Y1-coordinate.
  int top;

  //: X2-coordinate.
  int right;

  //: Y2-coordinate.
  int bottom;

  //: Returns the width of the rectangle.
  int get_width() const { return right - left; }

  //: Returns the height of the rectangle.
  int get_height() const { return bottom - top; }

  //: Returns the size of the rectangle.
  Size get_size() const { return Size(right - left, bottom - top); }

  //: Returns true if rectangle passed is overlapping or inside this rectangle.
  bool is_overlapped(const Rect &r) const
  {
    return (r.left < right && r.right > left && r.top < bottom && r.bottom > top);
  }

  bool contains(const Vector2i& pos) const
  {
    return
      left <= pos.x && pos.x < right &&
      top  <= pos.y && pos.y < bottom;
  }

  //: Check if rect is inside this
  bool contains(const Rect& rect) const
  {
    return
      left   <= rect.left  &&
      right  >= rect.right &&
      top    <= rect.top   &&
      bottom >= rect.bottom;
  }

  //: Returns another Rect containing a rotated version of this one.
  //param hotspot: Vector2i to rotate around.
  //param origin: Determines the hotspot point within the rectangle
  //param x, y: Offsets applied negatively to the hotspot point
  //param angle: Angle to rotate in degrees.
  Rect get_rot_bounds(const Vector2i &hotspot, float angle) const;
  Rect get_rot_bounds(Origin origin, int x, int y, float angle) const;

  //! Operations:
public:
  //: Sets the size of the rectangle, maintaining top/left position.
  void set_size(const Size &size)
  {
    right = left + size.width();
    bottom = top + size.height();
  }

  //: Calculates and returns the union of two rectangles.
  Rect calc_union(const Rect &rect) const
  {
    Rect result;
    if (left   > rect.left)   result.left   = left;   else result.left   = rect.left;
    if (right  < rect.right)  result.right  = right;  else result.right  = rect.right;
    if (top    > rect.top)    result.top    = top;    else result.top    = rect.top;
    if (bottom < rect.bottom) result.bottom = bottom; else result.bottom = rect.bottom;
    return result;
  }

  bool is_normal() const
  {
    return left <= right && top <= bottom;
  }

  //: Normalize rectangle. Ensures that left is less than right and top is less than bottom.
  void normalize()
  {
    if (left > right)
    {
      int temp = right;
      right = left;
      left = temp;
    }

    if (top > bottom)
    {
      int temp = bottom;
      bottom = top;
      top = temp;
    }
  }
};

std::ostream& operator<<(std::ostream& s, const Rect& rect);

template<>
struct fmt::formatter<Rect>
{
  template<typename ParseContext>
  constexpr auto parse(ParseContext& ctx)
  {
    return ctx.begin();
  }

  template<typename FormatContext>
  auto format(Rect const& v, FormatContext& ctx)
  {
    std::ostringstream os;
    os << v;
    return fmt::format_to(ctx.out(), os.str());
  }
};

#endif

