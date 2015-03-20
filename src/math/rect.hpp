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

class Rectf;

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

  explicit Rect(const Rectf& rect);

  Rect(int new_left, int new_top, int new_right, int new_bottom)
    : left(new_left),
      top(new_top),
      right(new_right),
      bottom(new_bottom)
  {}

  Rect(const Vector2i &p, const Size &size)
    : left(p.x),
      top(p.y),
      right(left + size.width),
      bottom(top + size.height)
  {}

  Rect(const Rect &rect)
    : left(rect.left),
      top(rect.top),
      right(rect.right),
      bottom(rect.bottom)
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
    right = left + size.width;
    bottom = top + size.height;
  }

  //: Calculates and returns the union of two rectangles.
  Rect calc_union(const Rect &rect)
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

  //: Applies an origin and offset pair to this rectangle
  //param origin: The new origin to adjust to from default upper-left position
  //param x, y: Offsets applied negatively to each corner of the rectangle
  void apply_alignment(Origin origin, int x, int y)
  {
    Vector2i offset = calc_origin(origin, get_size());
    offset.x -= x;
    offset.y -= y;

    left += offset.x;
    top += offset.y;
    right += offset.x;
    bottom += offset.y;
  }
};

//: 2D (left,top,right,bottom) floating point rectangle structure.
class Rectf
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
  Rectf()
    : left(0.0f),
      top(0.0f),
      right(0.0f),
      bottom(0.0f)
  {}

  Rectf(const Rect& rect)
    : left(static_cast<float>(rect.left)),
      top(static_cast<float>(rect.top)),
      right(static_cast<float>(rect.right)),
      bottom(static_cast<float>(rect.bottom))
  {}

  Rectf(float new_left, float new_top, float new_right, float new_bottom)
    : left(new_left),
      top(new_top),
      right(new_right),
      bottom(new_bottom)
  {}

  Rectf(const Vector2f &p, const Sizef &size)
    : left(p.x),
      top(p.y),
      right(left + size.width),
      bottom(top + size.height)
  {}

  Rectf(const Vector2f& p1, const Vector2f& p2)
    :  left(p1.x),
       top(p1.y),
       right(p2.x),
       bottom(p2.y)
  {}

  Rectf(const Rectf &rect)
    : left(rect.left),
      top(rect.top),
      right(rect.right),
      bottom(rect.bottom)
  {}

  //: Rect += Rect operator.
  Rectf &operator+=(const Rectf &r)
  { left += r.left; top += r.top; right += r.right; bottom += r.bottom; return *this; }

  //: Rect -= Rect operator.
  Rectf &operator-=(const Rectf &r)
  { left -= r.left; top -= r.top; right -= r.right; bottom -= r.bottom; return *this; }

  //: Rect += Vector2i operator.
  Rectf &operator+=(const Vector2f &p)
  { left += p.x; top += p.y; right += p.x; bottom += p.y; return *this; }

  //: Rect -= Vector2i operator.
  Rectf &operator-=(const Vector2f &p)
  { left -= p.x; top -= p.y; right -= p.x; bottom -= p.y; return *this; }

  //: Rect + Rect operator.
  Rectf operator+(const Rectf &r) const
  { return Rectf(left + r.left, top + r.top, right + r.right, bottom + r.bottom); }

  //: Rect - Rect operator.
  Rectf operator-(const Rectf &r) const
  { return Rectf(left - r.left, top - r.top, right - r.right, bottom - r.bottom); }

  //: Rect + Vector2i operator.
  Rectf operator+(const Vector2f &p) const
  { return Rectf(left + p.x, top + p.y, right + p.x, bottom + p.y); }

  //: Rect - Vector2i operator.
  Rectf operator-(const Vector2f &p) const
  { return Rectf(left - p.x, top - p.y, right - p.x, bottom - p.y); }

  //: Rect == Rect operator.
  bool operator==(const Rectf &r) const
  { return (left == r.left && top == r.top && right == r.right && bottom == r.bottom); }

  //: Rect != Rect operator.
  bool operator!=(const Rect &r) const
  { return (left != r.left || top != r.top || right != r.right || bottom != r.bottom); }

  //! Attributes:
public:
  //: X1-coordinate.
  float left;

  //: Y1-coordinate.
  float top;

  //: X2-coordinate.
  float right;

  //: Y2-coordinate.
  float bottom;

  //: Returns the width of the rectangle.
  float get_width() const { return right - left; }

  //: Returns the height of the rectangle.
  float get_height() const { return bottom - top; }

  //: Returns the size of the rectangle.
  Sizef get_size() const { return Sizef(right - left, bottom - top); }

  //: Returns true if point is inside the rectangle.
  bool is_inside(const Vector2f &p) const { return (p.x >= left && p.y >= top && p.x <= right && p.y <= bottom); }

  //: Returns true if rectangle passed is overlapping or inside this rectangle.
  bool is_overlapped(const Rectf &r) const
  {
    return (r.left < right && r.right > left && r.top < bottom && r.bottom > top);
  }

  //! Operations:
public:
  //: Sets the size of the rectangle, maintaining top/left position.
  void set_size(const Size &size)
  {
    right  = left + static_cast<float>(size.width);
    bottom = top  + static_cast<float>(size.height);
  }

  //: Calculates and returns the union of two rectangles.
  Rectf calc_union(const Rectf &rect)
  {
    Rectf result;
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

  //: Normalize rectangle. Ensures that left<right and top<bottom.
  void normalize()
  {
    if (left > right)
    {
      float temp = right;
      right = left;
      left = temp;
    }

    if (top > bottom)
    {
      float temp = bottom;
      bottom = top;
      top = temp;
    }
  }

  /*
  //: Applies an origin and offset pair to this rectangle
  //param origin: The new origin to adjust to from default upper-left position
  //param x, y: Offsets applied negatively to each corner of the rectangle
  void apply_alignment(Origin origin, float x, float y)
  {
  Vector2f offset = calc_origin(origin, get_size());
  offset.x -= x;
  offset.y -= y;

  left += offset.x;
  top += offset.y;
  right += offset.x;
  bottom += offset.y;
  }
  */

  Vector2f get_center() const {
    return Vector2f((left + right) / 2.0f,
                    (top + bottom) / 2.0f);
  }

  // Moves each edge f away from the center, thus width = old_width + 2*f
  Rectf grow(float f) const {
    return Rectf(left   - f,
                 top    - f,
                 right  + f,
                 bottom + f);
  }

  Rectf grow(float x, float y) const {
    return Rectf(left   - x,
                 top    - y,
                 right  + x,
                 bottom + y);
  }

  // Construct a rectangle large enough
  Rectf grow(const Rectf& rect) const {
    return Rectf(Math::min(left, rect.left),
                 Math::min(top, rect.top),
                 Math::max(right, rect.right),
                 Math::max(bottom, rect.bottom));
  }

  float get_diagonal() const
  {
    return sqrtf((get_width() * get_width()) + (get_height() * get_height()));
  }

  //: Returns true if point is inside the rectangle.
  bool contains(const Vector2f &p) const { return (p.x >= left && p.y >= top && p.x <= right && p.y <= bottom); }

  //: Check if rect is inside this
  bool contains(const Rectf& rect) const
  {
    return
      left   <= rect.left  &&
      right  >= rect.right &&
      top    <= rect.top   &&
      bottom >= rect.bottom;
  }

  Rectf clip_to(const Rectf& cliprect) const
  {
    return Rectf(Math::max(left,   cliprect.left),
                 Math::max(top,    cliprect.top),
                 Math::min(right,  cliprect.right),
                 Math::min(bottom, cliprect.bottom));
  }
};

inline Rect::Rect(const Rectf& rect)
  : left(static_cast<int>(rect.left)),
    top(static_cast<int>(rect.top)),
    right(static_cast<int>(rect.right)),
    bottom(static_cast<int>(rect.bottom))
{}

std::ostream& operator<<(std::ostream& s, const Rect& rect);
std::ostream& operator<<(std::ostream& s, const Rectf& rect);

#endif

