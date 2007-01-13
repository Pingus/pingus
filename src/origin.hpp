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

//! clanCore="Math"
//! header=core.h

#ifndef header_origin
#define header_origin

#if _MSC_VER > 1000
#pragma once
#endif

//: Alignment origins.
//- !group=Core/Math!
//- !header=core.h!
enum Origin
{
	origin_top_left,
	origin_top_center,
	origin_top_right,
	origin_center_left,
	origin_center,
	origin_center_right,
	origin_bottom_left,
	origin_bottom_center,
	origin_bottom_right
};

#if 0
//: Returns the anchor point for the origin within the dimensions of the size structure.
//- !group=Display/Display 2D!
//- !header=display.h!
CL_API_CORE CL_Point calc_origin(CL_Origin origin, const CL_Size &size);
CL_API_CORE CL_Pointf calc_origin(CL_Origin origin, const CL_Sizef &size);
CL_API_CORE CL_Pointd calc_origin(CL_Origin origin, const CL_Sized &size);
#endif 

#endif