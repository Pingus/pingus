//  $Id: input_box.hxx,v 1.16 2006/1/21 21:37:06 Jave27 Exp $
//
//  Pingus - A free Lemmings clone
//  Copyright (C) 2006 Ingo Ruhnke <grumbel@gmx.de>
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

#ifndef HEADER_PINGUS_GUI_INPUTBOX_HXX
#define HEADER_PINGUS_GUI_INPUTBOX_HXX

#include "component.hxx"
#include "../vector.hxx"
#include <string>

namespace Pingus {

	class DrawingContext;

namespace GUI {

/** This class allows keyboard input to be displayed in a box */
class InputBox : public GUI::Component {
protected:
	std::string str;
	Vector pos;
	float width;
	float height;

public:
	InputBox(float width_, Vector p, const std::string& 
		default_value = std::string());

	void set_string(const std::string& s) { str = s; }
	std::string get_string() { return str; }
	
	void on_key_pressed(const char c) { str += c; }
	
	/// GUI Component functions:
	///
	void update(float delta) { }
	
	bool is_at(int x, int y);
	
	void draw(DrawingContext &gc);
};	// InputBox class

}	// GUI namespace
}	// Pingus namespace

#endif

/* EOF */
