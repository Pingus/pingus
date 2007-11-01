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

#include "context_menu.hpp"
#include "level_objs.hpp"
#include "viewport.hpp"
#include "editor_screen.hpp"
#include "../gui/gui_manager.hpp"
#include "../string_util.hpp"
#include "../fonts.hpp"

namespace Editor {

// Determine which actions are available for these objects
ContextMenu::ContextMenu(std::vector<LevelObj*> o, Vector2i p, Viewport* vp, bool base_menu)
  : objs(o), 
    viewport(vp),
    pos(p),
    selected_action_offset(0),
    displayed_child(0),
    item_height(Fonts::verdana11.get_height())
{
  if (base_menu)
    {
      // Create all available child menus
      width = 110;
      show = true;
      create_child_menus();
    }
  else
    {
      width = 200;
      show = false;
    }

  total_height  = item_height * actions.size();
}


ContextMenu::~ContextMenu()
{
}


// Keep track of where the mouse is for highlighting
void
ContextMenu::on_pointer_move(int x, int y)
{
  mouse_at.x = x; 
  mouse_at.y = y;

  selected_action_offset = (unsigned)((mouse_at.y - pos.y) / item_height);
}

void
ContextMenu::draw(DrawingContext &gc)
{
  if (show)
    {
      // Draw the box
      gc.draw_fillrect(pos.x, pos.y, pos.x + width, pos.y + total_height, 
                       Color(211,211,211,100));
      // Draw the border
      gc.draw_rect(pos.x, pos.y, pos.x + width, pos.y + total_height, 
                   Color(0,0,0));
      // Draw the highlighted action if the mouse is in the box
      if (hover)
        {
          gc.draw_fillrect(pos.x, pos.y + selected_action_offset * 
                           item_height, pos.x + width, pos.y + (selected_action_offset + 1) * item_height,
                           Color(128,128,128,150));
        }

      // Draw the action names
      for (unsigned i = 0; i < actions.size(); i++)
        gc.print_left(Fonts::verdana11, pos.x, pos.y + 
                      (i * item_height), actions[i].friendly_name);
    }
}

bool
ContextMenu::is_at(int x, int y)
{
  if (show)
    return (x > pos.x && x < pos.x + (int)width &&
            y > pos.y && y < pos.y + (int)total_height);
  else
    return false;
}

void 
ContextMenu::on_primary_button_click(int x, int y)
{
  if (!actions[selected_action_offset].child)
    {
      for (unsigned i = 0; i < objs.size(); i++)
        {
          switch (actions[selected_action_offset].modifier)
            {
              case (REMOVE) : 
                objs[i]->remove();
                break;
              case (ROTATE) :
                objs[i]->set_modifier(actions[selected_action_offset].parameter);
                break;
              case (SET_OWNER) :
                objs[i]->set_owner(StringUtil::to<int>(actions[selected_action_offset].parameter));
                break;
              case (SET_DIRECTION) :
                objs[i]->set_direction(actions[selected_action_offset].parameter);
                break;
              case (SET_Z_POS) :
                objs[i]->set_pos(Vector3f(objs[i]->get_pos().x, objs[i]->get_pos().y, 
                                          (float)StringUtil::to<int>(actions[selected_action_offset].parameter)));
                objs[i]->set_orig_pos(objs[i]->get_pos());
                break;
              default :
                break;
            }
        }
      // FIXME: should be handled differently: viewport->remove_context_menu();
    }
}

void
ContextMenu::on_secondary_button_click(int x, int y)
{
  // Does the same as the primary button
  on_primary_button_click(x, y);
}

void
ContextMenu::create_child_menus()
{
  // Create Remove button - available to all objects
  actions.push_back(ContextItem("Remove", "", REMOVE, 0));

  // Determine which actions are available to the selected objects
  unsigned available_attribs = 0xffff;
  for (unsigned i = 0; i < (unsigned)objs.size(); i++)
    available_attribs = available_attribs & objs[i]->get_attribs();

  ContextMenu* menu;
  if (available_attribs & CAN_ROTATE)
    {
      menu = new ContextMenu(objs, Vector2i(pos.x + width, pos.y), viewport, false);
      viewport->get_screen()->get_gui_manager()->add(menu, true);
      menu->add_action(ContextItem("0 degrees", "ROT0", ROTATE, 0));
      menu->add_action(ContextItem("90 Degrees", "ROT90", ROTATE, 0));
      menu->add_action(ContextItem("180 Degrees", "ROT180", ROTATE, 0));
      menu->add_action(ContextItem("270 Degrees", "ROT270", ROTATE, 0));
      menu->add_action(ContextItem("0 Degrees + Flip", "ROT0FLIP", ROTATE, 0));
      menu->add_action(ContextItem("90 Degrees + Flip", "ROT90FLIP", ROTATE, 0));
      menu->add_action(ContextItem("180 Degrees + Flip", "ROT180FLIP", ROTATE, 0));
      menu->add_action(ContextItem("270 Degrees + Flip", "ROT270FLIP", ROTATE, 0));
      add_action(ContextItem("Rotate >", "", ROTATE, menu));
    }
  if (available_attribs & HAS_OWNER)
    {
      menu = new ContextMenu(objs, Vector2i(pos.x + width, pos.y), viewport, false);
      viewport->get_screen()->get_gui_manager()->add(menu, true);
      menu->add_action(ContextItem("0", "0", SET_OWNER, 0));
      menu->add_action(ContextItem("1", "1", SET_OWNER, 0));
      menu->add_action(ContextItem("2", "2", SET_OWNER, 0));
      menu->add_action(ContextItem("3", "3", SET_OWNER, 0));
      add_action(ContextItem("Set Owner >", "", SET_OWNER, menu));
    }
  if (available_attribs & HAS_DIRECTION)
    {
      menu = new ContextMenu(objs, Vector2i(pos.x + width, pos.y), viewport, false);
      viewport->get_screen()->get_gui_manager()->add(menu, true);
      menu->add_action(ContextItem("Left", "left", SET_DIRECTION, 0));
      menu->add_action(ContextItem("Right", "right", SET_DIRECTION, 0));
      menu->add_action(ContextItem("Misc.", "misc", SET_DIRECTION, 0));
      add_action(ContextItem("Direction >", "", SET_DIRECTION, menu));
    }
  menu = new ContextMenu(objs, Vector2i(pos.x + width, pos.y), viewport, false);
  viewport->get_screen()->get_gui_manager()->add(menu, true);
  menu->add_action(ContextItem("-50", "-50", SET_Z_POS, 0));
  menu->add_action(ContextItem("-25", "-25", SET_Z_POS, 0));
  menu->add_action(ContextItem("0", "0", SET_Z_POS, 0));
  menu->add_action(ContextItem("25", "25", SET_Z_POS, 0));
  menu->add_action(ContextItem("50", "50", SET_Z_POS, 0));
  add_action(ContextItem("Set Z Pos. >", "", SET_Z_POS, menu));
		
  // TODO - Add more menu options here
}

void
ContextMenu::add_action(ContextItem item)
{
  actions.push_back(item);
  total_height += item_height;
}

void
ContextMenu::display(bool should_display)
{
  show = should_display;
  if (!show && displayed_child)
    displayed_child->display(false);
}

void
ContextMenu::update(float delta)
{
  UNUSED_ARG(delta);

  if (displayed_child != actions[selected_action_offset].child)
    {
      if (displayed_child)
        {
          displayed_child->display(false);
          displayed_child = 0;
        }
      if (actions[selected_action_offset].child)
        {
          displayed_child = actions[selected_action_offset].child;
          displayed_child->display(true);
        }
    }
}

} // Editor namespace

/* EOF */
