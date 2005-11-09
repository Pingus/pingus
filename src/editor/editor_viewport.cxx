//  $Id: editor_viewport.cxx,v 1.00 2005/11/09 23:41:12 Jave27 Exp $
//
//  Pingus - A free Lemmings clone
//  Copyright (C) 1999 Ingo Ruhnke <grumbel@gmx.de>
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

#include <ClanLib/display.h>
#include <vector>
#include <string>
#include <iostream>
#include "../vector.hxx"
#include "../display/drawing_context.hxx"
#include "../graphic_context_state.hxx"
#include "editor_viewport.hxx"

namespace Pingus {
namespace Editor {

// Constructor
EditorViewport::EditorViewport(EditorScreen* e) :
	editor(e),
	bg_surface(0),
	start_pos(Vector(10.0f, 60.0f)),
	end_pos(Vector((float)(CL_Display::get_width() - 10), 
								(float)(CL_Display::get_height() - 10))),
  scene_context(new SceneContext()),
	autoscroll(true),
	state((int)(end_pos.x - start_pos.x), (int)(end_pos.y - start_pos.y))
{
	state.set_limit(CL_Rect((int)start_pos.x, (int)start_pos.y, 
		(int)end_pos.x, (int)end_pos.y));
	state.set_pos(CL_Pointf((float)(state.get_width() / 2), 
		(float)(state.get_height() / 2)));
}

// Destructor
EditorViewport::~EditorViewport ()
{
	if (bg_surface) delete bg_surface;
}

// When someone right-clicks inside the viewport
void
EditorViewport::on_secondary_button_click(int x, int y)
{
	std::cout << "Right-click at " << x - start_pos.x << ", " 
		<< y - start_pos.y << std::endl;
}

// Draws all of the objects in the viewport and the background (if any)
void
EditorViewport::draw(DrawingContext &gc)
{
	scene_context->clear();

	state.push(*scene_context);

	// Now, draw all of the objects

	// FIXME: Should draw the background over the whole viewport (stretched or tiled)
	if (bg_surface)
		scene_context->color().draw(*bg_surface, start_pos);
	else
		scene_context->color().draw_fillrect(start_pos.x, start_pos.y, end_pos.x, end_pos.y, CL_Color::darkgray);


	scene_context->color().draw_circle(25, 25, 10, CL_Color::white);

	state.pop(*scene_context);

	gc.draw(new SceneContextDrawingRequest(scene_context, 0));
}

// Returns true if the viewport is at the x,y coordinate
bool
EditorViewport::is_at(int x, int y)
{
  return (x > start_pos.x && x < end_pos.x
	  && y > start_pos.y && y < end_pos.y);
}

// 
void
EditorViewport::on_pointer_move(int x, int y)
{
	const int autoscroll_border = 10;
	if (is_at(x, y))
	{
		// Autoscroll if necessary
		if (x - start_pos.x < autoscroll_border)
			std::cout << "AutoScroll left" << std::endl;
		else if (end_pos.x - x < autoscroll_border)
			std::cout << "AutoScroll right" << std::endl;
		else if (y - start_pos.y < autoscroll_border)
			state.set_pos(state.get_pos() - CL_Pointf(0, 5.0f));
		else if (end_pos.y - y < autoscroll_border)
			state.set_pos(state.get_pos() + CL_Pointf(0, 5.0f));
			//std::cout << "AutoScroll down" << std::endl;
	}
}

} // Editor namespace
} // Pingus namespace

/* EOF */