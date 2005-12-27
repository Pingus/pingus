//  $Id: editor_viewport.hxx,v 1.00 2005/11/09 23:41:12 Jave27 Exp $
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

#ifndef HEADER_PINGUS_EDITOR_VIEWPORT_HXX
#define HEADER_PINGUS_EDITOR_VIEWPORT_HXX

#include "../gui/component.hxx"
#include "../graphic_context_state.hxx"
#include <vector>
#include <string>

namespace Pingus {

	class Vector;
	class DrawingContext;
	class SceneContext;

namespace Editor {

	class LevelObj;
	class EditorScreen;

/** This class is where the actual level graphics will display in the
	level editor.  Objects can be added, deleted, moved, modified, etc. 
	inside of the EditorViewport */
class EditorViewport : public GUI::Component {

public:
	/** Constructor
		@param e The EditorScreen to which this viewport belongs */
	EditorViewport (EditorScreen* e);

	/** Destructor */
	~EditorViewport ();

	/** Returns the list of objects inside the viewport */
	std::vector<std::string> get_objects() { return objs; }

	/** When someone right-clicks inside the viewport */
	void on_secondary_button_click(int x, int y);

	/** Draws all of the objects in the viewport */
	void draw(DrawingContext &gc);

	/** Update information about scrolling, etc. */
	void update(float delta);

	/** Returns whether or not the mouse is inside the viewport */
	bool is_at(int x, int y);

	/** Emitted when the pointer moved, x and y are the new pointer
    coordinates */
  void on_pointer_move (int x, int y);

private:
	EditorViewport();
	EditorViewport (const EditorViewport&);
  EditorViewport& operator= (const EditorViewport&);
	
	GraphicContextState state;
	SceneContext* scene_context;

	/** The EditorScreen to which this viewport belongs */
	EditorScreen* editor;

	/** Collection of objects inside the viewport (groundpieces, traps, etc.) */
	std::vector<std::string> objs;

	/** Whether or not Autoscrolling is turned on */
	bool autoscroll;

	/** Where the mouse is right now - used for autoscrolling */
	Vector mouse_at;

	/** Returns the topmost object at this x, y location */
	LevelObj* object_at(int x, int y);
};

} // Editor namespace
} // Pingus namespace

#endif

/* EOF */
