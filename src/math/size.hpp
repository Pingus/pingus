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

#ifndef HEADER_MATH_SIZE_HPP
#define HEADER_MATH_SIZE_HPP

#if _MSC_VER > 1000
#pragma once
#endif

#include <iosfwd>

class Sizef;

//: 2D (width,height) size structure.
//- !group=Core/Math!
//- !header=core.h!
class Size
{
//! Construction:
public:
	//: Constructs a size structure.
	//param width: Initial width of size structure.
	//param height: Initial height of size structure.
	//param size: Size structure to construct this one from.
	Size() : width(0), height(0) { return; }

	Size(int width, int height)
	: width(width), height(height) { }

	Size(const Size &s)
	{ width = s.width; height = s.height; }

	explicit Size(const Sizef& s);

//! Attributes:
public:
	//: Size width.
	int width;

	//: Size height.
	int height;

//! Operations:
public:
	//: Size += Size operator.
	Size &operator+=(const Size &s)
	{ width += s.width; height += s.height; return *this; }

	//: Size -= Size operator.
	Size &operator-=(const Size &s)
	{ width -= s.width; height -= s.height; return *this; }
	
	//: Size + Size operator.
	Size operator+(const Size &s) const
	{ return Size(width + s.width, height + s.height); }

	//: Size - Size operator.
	Size operator-(const Size &s) const
	{ return Size(width - s.width, height - s.height); }

	//: Size == Size operator (deep compare).
	bool operator==(const Size &s) const
	{ return (width == s.width) && (height == s.height); }

	//: Size != Size operator (deep compare).
	bool operator!=(const Size &s) const
	{ return (width != s.width) || (height != s.height); }
  
	int get_area() const { return width * height; }
};

//: 2D (width,height) floating point size structure.
class Sizef
{
//! Construction:
public:
	//: Constructs a size structure.
	//param width: Initial width of size structure.
	//param height: Initial height of size structure.
	//param size: Size structure to construct this one from.
	Sizef() : width(0.f), height(0.f) { return; }

	Sizef(const Size& s) 
          : width(static_cast<float>(s.width)),
            height(static_cast<float>(s.height))
	{}

	Sizef(float width, float height)
	: width(width), height(height) { }

	Sizef(const Sizef &s)
	{ width = s.width; height = s.height; }

//! Attributes:
public:
	//: Size width.
	float width;

	//: Size height.
	float height;

//! Operations:
public:
	//: Size += Size operator.
	Sizef &operator+=(const Sizef &s)
	{ width += s.width; height += s.height; return *this; }

	//: Size -= Size operator.
	Sizef &operator-=(const Sizef &s)
	{ width -= s.width; height -= s.height; return *this; }
	
	//: Size + Size operator.
	Sizef operator+(const Sizef &s) const
	{ return Sizef(width + s.width, height + s.height); }

	//: Size - Size operator.
	Sizef operator-(const Sizef &s) const
	{ return Sizef(width - s.width, height - s.height); }

	//: Size == Size operator (deep compare).
	bool operator==(const Sizef &s) const
	{ return (width == s.width) && (height == s.height); }

	//: Size != Size operator (deep compare).
	bool operator!=(const Size &s) const
	{ return (width != s.width) || (height != s.height); }
};

inline Size::Size(const Sizef& s)
	: width(static_cast<int>(s.width)),
	  height(static_cast<int>(s.height))
{}

std::ostream& operator<<(std::ostream& os, const Size& size);

#endif
