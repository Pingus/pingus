//  $Id: checkbox.hxx,v 1.00 2006/1/15 23:41:12 Jave27 Exp $
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

#ifndef HEADER_PINGUS_GUI_CHECKBOX_HXX
#define HEADER_PINGUS_GUI_CHECKBOX_HXX

#include "../gui/component.hpp"
#include "../math/vector3f.hpp"
#include "../sprite.hpp"

	class DrawingContext;
		
namespace GUI {
	
	class CheckboxListener;		

class Checkbox : public GUI::Component
{
private:
	Sprite box;
	Sprite checkmark;

	bool	is_checked;
	Vector3f pos;

	float width;
	float height;
	
	CheckboxListener* listener;
	std::string label;

public:
	Checkbox(Vector3f p, std::string label_, CheckboxListener* l);

	void draw(DrawingContext& gc);
	bool is_at(int x, int y);
	void on_primary_button_click(int x, int y);
	void set_checkmark(bool check) { is_checked = check; }
	
private:
	Checkbox();
};	// Checkbox class

}	// GUI namespace

#endif

/* EOF */
