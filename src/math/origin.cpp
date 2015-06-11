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

#include <assert.h>

#include "math/origin.hpp"

Origin string2origin(const std::string& str)
{
  if (str == "top_left")
    return origin_top_left;
  else if (str == "top_center")
    return origin_top_center;
  else if (str == "top_right")
    return origin_top_right;
  else if (str == "center_left")
    return origin_center_left;
  else if (str == "center")
    return origin_center;
  else if (str == "center_right")
    return origin_center_right;
  else if (str == "bottom_left")
    return origin_bottom_left;
  else if (str == "bottom_center")
    return origin_bottom_center;
  else if (str == "bottom_right")
    return origin_bottom_right;
  else
    return origin_top_left;
}

std::string origin2string(Origin origin)
{
  switch(origin)
  {
    case origin_top_left:
      return "top_left";

    case origin_top_center:
      return "top_center";

    case origin_top_right:
      return "top_right";

    case origin_center_left:
      return "center_left";

    case origin_center:
      return "center";

    case origin_center_right:
      return "center_right";

    case origin_bottom_left:
      return "bottom_left";

    case origin_bottom_center:
      return "bottom_center";

    case origin_bottom_right:
      return "bottom_right";

    default:
      assert(false && "never reached");
      return {};
  }
}

Vector2i calc_origin(Origin origin, const Size &size)
{
  switch(origin)
  {
    case origin_top_left:
    default:
      return Vector2i(0, 0);
      break;
    case origin_top_center:
      return Vector2i(size.width / 2, 0);
      break;
    case origin_top_right:
      return Vector2i(size.width, 0);
      break;
    case origin_center_left:
      return Vector2i(0, size.height / 2);
      break;
    case origin_center:
      return Vector2i(size.width / 2, size.height / 2);
      break;
    case origin_center_right:
      return Vector2i(size.width, size.height / 2);
      break;
    case origin_bottom_left:
      return Vector2i(0, size.height);
      break;
    case origin_bottom_center:
      return Vector2i(size.width / 2, size.height);
      break;
    case origin_bottom_right:
      return Vector2i(size.width, size.height);
      break;
  }
}

/* EOF */

