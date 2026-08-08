// geomcpp - Basic collection of point, size and rect classes
// Copyright (C) 2020 Ingo Ruhnke <grumbel@gmail.com>
//
// This software is provided 'as-is', without any express or implied
// warranty.  In no event will the authors be held liable for any damages
// arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented; you must not
//    claim that you wrote the original software. If you use this software
//    in a product, an acknowledgment in the product documentation would be
//    appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be
//    misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#ifndef HEADER_GEOMCPP_IO_HPP
#define HEADER_GEOMCPP_IO_HPP

#include <ostream>
#include <sstream>
#include <stdexcept>
#include <string>

#include "origin.hpp"
#include "point.hpp"
#include "size.hpp"
#include "rect.hpp"

namespace geom {

inline
std::ostream& operator<<(std::ostream& os, ipoint const& point) {
  return os << "ipoint("
            << point.x() << ", "
            << point.y() << ")";
}

inline
std::ostream& operator<<(std::ostream& os, isize const& size) {
  return os << "isize("
            << size.width() << ", "
            << size.height() << ")";
}

inline
std::ostream& operator<<(std::ostream& os, irect const& rect) {
  return os << "irect("
            << rect.left() << ", "
            << rect.top() << ", "
            << rect.right() << ", "
            << rect.bottom() << ")";
}

inline
std::ostream& operator<<(std::ostream& os, fpoint const& point) {
  return os << "fpoint("
            << point.x() << ", "
            << point.y() << ")";
}

inline
std::ostream& operator<<(std::ostream& os, fsize const& size) {
  return os << "fsize("
            << size.width() << ", "
            << size.height() << ")";
}

inline
std::ostream& operator<<(std::ostream& os, frect const& rect) {
  return os << "frect("
            << rect.left() << ", "
            << rect.top() << ", "
            << rect.right() << ", "
            << rect.bottom() << ")";
}

inline
ipoint ipoint_from_string(std::string const& text)
{
  int pos;
  int x;
  int y;
  if (sscanf(text.c_str(), " ipoint( %d, %d ) %n", &x, &y, &pos) == 2 &&
      pos == static_cast<int>(text.size())) {
    return ipoint(x, y);
  } else if (sscanf(text.c_str(), " %d, %d %n", &x, &y, &pos) == 2 &&
             pos == static_cast<int>(text.size())) {
    return ipoint(x, y);
  } else {
    std::ostringstream os;
    os << "ipoint_from_string(): couldn't parse '" << text << "'";
    throw std::invalid_argument(os.str());
  }
}

inline
fpoint fpoint_from_string(std::string const& text)
{
  int pos;
  float x;
  float y;
  if (sscanf(text.c_str(), " fpoint( %f, %f ) %n", &x, &y, &pos) == 2 &&
      pos == static_cast<int>(text.size())) {
    return fpoint(x, y);
  } else if (sscanf(text.c_str(), " %f, %f %n", &x, &y, &pos) == 2 &&
             pos == static_cast<int>(text.size())) {
    return fpoint(x, y);
  } else {
    std::ostringstream os;
    os << "fpoint_from_string(): couldn't parse '" << text << "'";
    throw std::invalid_argument(os.str());
  }
}

inline
isize isize_from_string(std::string const& text)
{
  int pos;
  int w;
  int h;
  if (sscanf(text.c_str(), " %dx%d %n", &w, &h, &pos) == 2 &&
      pos == static_cast<int>(text.size())) {
    return isize(w, h);
  } else if (sscanf(text.c_str(), " isize( %d, %d ) %n", &w, &h, &pos) == 2 &&
             pos == static_cast<int>(text.size())) {
    return isize(w, h);
  } else if (sscanf(text.c_str(), " %d, %d %n", &w, &h, &pos) == 2 &&
             pos == static_cast<int>(text.size())) {
    return isize(w, h);
  } else {
    std::ostringstream os;
    os << "isize_from_string(): couldn't parse '" << text << "'";
    throw std::invalid_argument(os.str());
  }
}

inline
fsize fsize_from_string(std::string const& text)
{
  int pos;
  float w;
  float h;
  if (sscanf(text.c_str(), " %fx%f %n", &w, &h, &pos) == 2 &&
      pos == static_cast<int>(text.size())) {
    return fsize(w, h);
  } else if (sscanf(text.c_str(), " fsize( %f, %f ) %n", &w, &h, &pos) == 2 &&
             pos == static_cast<int>(text.size())) {
    return fsize(w, h);
  } else if (sscanf(text.c_str(), " %f, %f %n", &w, &h, &pos) == 2 &&
             pos == static_cast<int>(text.size())) {
    return fsize(w, h);
  } else {
    std::ostringstream os;
    os << "fsize_from_string(): couldn't parse '" << text << "'";
    throw std::invalid_argument(os.str());
  }
}

inline
irect irect_from_string(std::string const& text)
{
  int pos;
  int x1;
  int y1;
  int x2;
  int y2;
  int w;
  int h;
  if (sscanf(text.c_str(), " %dx%d+%d+%d %n", &w, &h, &x1, &y1, &pos) == 4 &&
      pos == static_cast<int>(text.size())) {
    return irect(ipoint(x1, y1), isize(w, h));
  } else if (sscanf(text.c_str(), " %dx%d %n", &w, &h, &pos) == 2 &&
             pos == static_cast<int>(text.size())) {
    return irect(0, 0, w, h);
  } else if (sscanf(text.c_str(), " %d, %d, %d, %d %n", &x1, &y1, &x2, &y2, &pos) == 4 &&
             pos == static_cast<int>(text.size())) {
    return irect(x1, y1, x2, y2);
  } else if (sscanf(text.c_str(), " irect( %d, %d, %d, %d ) %n", &x1, &y1, &x2, &y2, &pos) == 4 &&
             pos == static_cast<int>(text.size())) {
    return irect(x1, y1, x2, y2);
  } else {
    std::ostringstream os;
    os << "irect_from_string(): couldn't parse '" << text << "'";
    throw std::invalid_argument(os.str());
  }
}

inline
frect frect_from_string(std::string const& text)
{
  int pos;
  float x1;
  float y1;
  float x2;
  float y2;
  float w;
  float h;
  if (sscanf(text.c_str(), " %fx%f+%f+%f %n", &w, &h, &x1, &y1, &pos) == 4 &&
      pos == static_cast<int>(text.size())) {
    return frect(fpoint(x1, y1), fsize(w, h));
  } else if (sscanf(text.c_str(), " %fx%f %n", &w, &h, &pos) == 2 &&
             pos == static_cast<int>(text.size())) {
    return frect(0, 0, w, h);
  } else if (sscanf(text.c_str(), " %f, %f, %f, %f %n", &x1, &y1, &x2, &y2, &pos) == 4 &&
             pos == static_cast<int>(text.size())) {
    return frect(x1, y1, x2, y2);
  } else if (sscanf(text.c_str(), " frect( %f, %f, %f, %f ) %n", &x1, &y1, &x2, &y2, &pos) == 4 &&
             pos == static_cast<int>(text.size())) {
    return frect(x1, y1, x2, y2);
  } else {
    std::ostringstream os;
    os << "frect_from_string(): couldn't parse '" << text << "'";
    throw std::invalid_argument(os.str());
  }
}

inline
std::ostream& operator<<(std::ostream& os, origin origin)
{
  switch(origin)
  {
    case origin::TOP_LEFT: return os << "top-left";
    case origin::TOP_CENTER: return os << "top-center";
    case origin::TOP_RIGHT: return os << "top-right";
    case origin::CENTER_LEFT: return os << "center-left";
    case origin::CENTER: return os << "center";
    case origin::CENTER_RIGHT: return os << "center-right";
    case origin::BOTTOM_LEFT: return os << "bottom-left";
    case origin::BOTTOM_CENTER: return os << "bottom-center";
    case origin::BOTTOM_RIGHT: return os << "bottom-right";
    default: throw std::invalid_argument("invalid value for origin");
  }
}

} // namespace geom

#endif

/* EOF */
