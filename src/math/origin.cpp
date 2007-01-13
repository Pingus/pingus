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

#include "origin.hpp"

Point calc_origin(Origin origin, const Size &size)
{
	switch(origin)
	{
	case origin::top_left:
	default:
		return Point(0, 0);
		break;
	case origin::top_center:
		return Point(size.width / 2, 0);
		break;
	case origin::top_right:
		return Point(size.width, 0);
		break;
	case origin::center_left:
		return Point(0, size.height / 2);
		break;
	case origin::center:
		return Point(size.width / 2, size.height / 2);
		break;
	case origin::center_right:
		return Point(size.width, size.height / 2);
		break;
	case origin::bottom_left:
		return Point(0, size.height);
		break;
	case origin::bottom_center:
		return Point(size.width / 2, size.height);
		break;
	case origin::bottom_right:
		return Point(size.width, size.height);
		break;
	}
}

Vector calc_origin(Origin origin, const Sizef &size)
{
	switch(origin)
	{
	case origin::top_left:
	default:
		return Vector(0, 0);
		break;
	case origin::top_center:
		return Vector(size.width / 2, 0);
		break;
	case origin::top_right:
		return Vector(size.width, 0);
		break;
	case origin::center_left:
		return Vector(0, size.height / 2);
		break;
	case origin::center:
		return Vector(size.width / 2, size.height / 2);
		break;
	case origin::center_right:
		return Vector(size.width, size.height / 2);
		break;
	case origin::bottom_left:
		return Vector(0, size.height);
		break;
	case origin::bottom_center:
		return Vector(size.width / 2, size.height);
		break;
	case origin::bottom_right:
		return Vector(size.width, size.height);
		break;
	}
}

/* EOF */
