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


	class Vector3f;
	class DrawingContext;
	class SceneContext;

namespace Editor {

	class LevelObj;
	class EditorScreen;
	class ContextMenu;

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

	/** Draws all of the objects in the viewport */
	void draw(DrawingContext &gc);

	/** Update information about scrolling, etc. */
	void update(float delta);

	/** Returns whether or not the mouse is inside the viewport */
	bool is_at(int x, int y);

	/** Emitted when the pointer moved, x and y are the new pointer
    coordinates */
  void on_pointer_move (int x, int y);

	/** Get rid of context menu if it exists */
	void remove_context_menu();

	/** Refresh the list of objects (do when loading or creating a new level) */
	void refresh();
	
	/** Turns the "snap-to-grid" option on or off */
	void set_snap_to(bool s) { snap_to = s; }

	/** Add an object to the currently displayed vector of objects */
	void add_object(LevelObj* obj);

	/** Return a pointer to the EditorScreen object */
	EditorScreen* get_screen() { return editor; }

private:
	EditorViewport();
	EditorViewport (const EditorViewport&);
  EditorViewport& operator= (const EditorViewport&);
	
	GraphicContextState state;
	SceneContext* scene_context;

	/** The EditorScreen to which this viewport belongs */
	EditorScreen* editor;

	/** Whether or not Autoscrolling is turned on */
	bool autoscroll;

	/** Where the mouse is right now - used for autoscrolling */
	Vector3f mouse_at;

	/** Where the mouse is at in relation to the world/level */
	Vector3f mouse_at_world;

	/** Where the mouse started dragging from */
	Vector3f drag_start_pos;

	/** All objects in the level */
	std::vector<LevelObj*> objs;

	/** The currently selected LevelObjs */
	std::vector<LevelObj*> current_objs;

	/** The region that is currently highlighted */
	Rect highlighted_area;

	/** Returns the topmost object at this x, y location */
	LevelObj* object_at(int x, int y);

	/** There should only be 0 or 1 context menus on the screen */
	ContextMenu* context_menu;

	/** Whether or not the "snap-to-grid" functionality is on. */
	bool snap_to;

	/** What is the currently selected action that the mouse is doing */
	enum ActionType { NOTHING = 0, HIGHLIGHTING = 1, DRAGGING = 2 } current_action;

	/// Mouse actions
	void on_primary_button_press(int x, int y);
	void on_primary_button_release(int x, int y);
	void on_secondary_button_click(int x, int y);
};

} // Editor namespace

#endif

/* EOF */
