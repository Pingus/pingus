//  $Id$
//
//  Pingus - A free Lemmings clone
//  Copyright (C) 2007 Jason Green <jave27@gmail.com>,
//                     Ingo Ruhnke <grumbel@gmx.de>
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

#ifndef HEADER_PINGUS_EDITOR_CONTEXT_MENU_HXX
#define HEADER_PINGUS_EDITOR_CONTEXT_MENU_HXX

#include <vector>
#include <string>
#include "../math/vector2i.hpp"
#include "../gui/component.hpp"

namespace Editor {

class LevelObj;
class EditorViewport;
class ContextMenu;

typedef enum ItemModifier { REMOVE, ROTATE, SET_OWNER, SET_DIRECTION, 
                            STRETCH, SET_Z_POS };

class ContextItem {
public:
  std::string friendly_name;
  std::string parameter;
  ItemModifier modifier;
  ContextMenu* child;

public:
  ContextItem(std::string friendly_name_, std::string parameter_, ItemModifier mod, 
              ContextMenu* child_menu)
    : friendly_name(friendly_name_),
      parameter(parameter_),
      modifier(mod),
      child(child_menu)
  { }
};

class ContextMenu : public GUI::Component {
private:
  /** Creates the child menu structure and detemines which actions are available */
  void create_child_menus();

  /** Level objects to be affected by this menu */
  std::vector<LevelObj*> objs;

  /** Viewport to which this menu belongs */
  EditorViewport* viewport;

  /** List of actions available in this menu */
  std::vector<ContextItem> actions;

  /** Where the mouse is located */
  Vector2i mouse_at;

  /** Location of context menu */
  Vector2i pos;

  /** Is the mouse over the menu? */
  bool hover;

  /** Should this be showing? */
  bool show;

  /** The offset into actions vector of the currently highlighted action */
  unsigned selected_action_offset;
		
  /** Currently displayed child menu (if any) */
  ContextMenu* displayed_child;

  /** Height of a single action */
  unsigned item_height;

  unsigned total_height;
  unsigned width;

public:
  // Constructor
  ContextMenu (std::vector<LevelObj*>, Vector2i p, EditorViewport* v, bool base_menu = true);
		
  // Desctructor
  ~ContextMenu ();

  /** Add an action to the list */
  void add_action(ContextItem item);

  void display (bool should_display);

  /// GUI Component Functions
  bool is_at(int x, int y);
  void draw (DrawingContext& gc);
  void update (float delta);
  void on_pointer_move (int x, int y);
  void on_primary_button_click(int x, int y);
  void on_secondary_button_click(int x, int y);
  void on_pointer_enter () { hover = true; }
  void on_pointer_leave () { hover = false; }

private:
  ContextMenu ();
  ContextMenu (const ContextMenu&);
  ContextMenu& operator= (const ContextMenu&);

};

}  // Editor namespace

#endif

/* EOF */
