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

namespace pingus {

Origin string2origin(const std::string& str)
{
  if (str == "top_left")
    return Origin::TOP_LEFT;
  else if (str == "top_center")
    return Origin::TOP_CENTER;
  else if (str == "top_right")
    return Origin::TOP_RIGHT;
  else if (str == "center_left")
    return Origin::CENTER_LEFT;
  else if (str == "center")
    return Origin::CENTER;
  else if (str == "center_right")
    return Origin::CENTER_RIGHT;
  else if (str == "bottom_left")
    return Origin::BOTTOM_LEFT;
  else if (str == "bottom_center")
    return Origin::BOTTOM_CENTER;
  else if (str == "bottom_right")
    return Origin::BOTTOM_RIGHT;
  else
    return Origin::TOP_LEFT;
}

std::string origin2string(Origin origin)
{
  switch(origin)
  {
    case Origin::TOP_LEFT:
      return "top_left";

    case Origin::TOP_CENTER:
      return "top_center";

    case Origin::TOP_RIGHT:
      return "top_right";

    case Origin::CENTER_LEFT:
      return "center_left";

    case Origin::CENTER:
      return "center";

    case Origin::CENTER_RIGHT:
      return "center_right";

    case Origin::BOTTOM_LEFT:
      return "bottom_left";

    case Origin::BOTTOM_CENTER:
      return "bottom_center";

    case Origin::BOTTOM_RIGHT:
      return "bottom_right";

    default:
      assert(false && "never reached");
      return {};
  }
}

} // namespace pingus

/* EOF */

