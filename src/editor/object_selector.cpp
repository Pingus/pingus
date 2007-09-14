/*  $Id$
**   __      __ __             ___        __   __ __   __
**  /  \    /  \__| ____    __| _/_______/  |_|__|  | |  |   ____
**  \   \/\/   /  |/    \  / __ |/  ___/\   __\  |  | |  | _/ __ \
**   \        /|  |   |  \/ /_/ |\___ \  |  | |  |  |_|  |_\  ___/
**    \__/\  / |__|___|  /\____ /____  > |__| |__|____/____/\___  >
**         \/          \/      \/    \/                         \/
**  Copyright (C) 2007 Ingo Ruhnke <grumbel@gmx.de>
**
**  This program is free software; you can redistribute it and/or
**  modify it under the terms of the GNU General Public License
**  as published by the Free Software Foundation; either version 2
**  of the License, or (at your option) any later version.
**
**  This program is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**  GNU General Public License for more details.
** 
**  You should have received a copy of the GNU General Public License
**  along with this program; if not, write to the Free Software
**  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
**  02111-1307, USA.
*/

#include "gui/display.hpp"
#include "gui_style.hpp"
#include "sprite.hpp"
#include "math/vector2i.hpp"
#include "editor_screen.hpp"
#include "display/drawing_context.hpp"
#include "fonts.hpp"
#include "math.hpp"
#include "editor_viewport.hpp"
#include "editor_level.hpp"
#include "object_selector_list.hpp"
#include "level_objs.hpp"
#include "display/drawing_context.hpp"
#include "gui/gui_manager.hpp"
#include "object_selector.hpp"

#include "resource.hpp"

namespace Editor {

class ObjectSelectorButton : public GUI::RectComponent
{
private:
  ObjectSelectorList* object_list;

  Sprite button_raised;
  Sprite button_pressed;
  Sprite sprite;
  bool   mouse_over;
  bool   mouse_down;
  std::string tooltip;

  typedef void (ObjectSelectorList::*Callback)();
  Callback callback;

public:
  ObjectSelectorButton(ObjectSelectorList* object_list_,
                       const Vector2i& pos, const std::string& sprite, const std::string& tooltip_, 
                       Callback callback_ = 0)
    : RectComponent(Rect(pos, Size(30, 30))),
      object_list(object_list_),
      button_raised(Resource::load_sprite("core/editor/obj_button-raised")),
      button_pressed(Resource::load_sprite("core/editor/obj_button-pressed")),
      sprite(Resource::load_sprite(sprite)),
      mouse_over(false),
      mouse_down(false),
      tooltip(tooltip_),
      callback(callback_)
  {
  }

  void draw(DrawingContext& gc)
  {
    if (mouse_down)
      gc.draw(button_pressed, Vector2i(rect.left, rect.top));
    else if (mouse_over)
      gc.draw(button_raised, Vector2i(rect.left, rect.top));

    gc.draw(sprite, Vector2i(rect.left + 3, rect.top + 3));

    if (mouse_over)
      {
        int t_w = Fonts::verdana11.get_width(tooltip);
        Rect t_r(rect.left + 17 - t_w/2 - 4, rect.top + 38 - 2, 
                 rect.left + 17 + t_w/2 + 4, rect.top + 38 + Fonts::verdana11.get_height() + 2);
        gc.draw_fillrect(t_r, Color(255, 255, 200), 1000.0f);
        gc.draw_rect(t_r, Color(0,0,0), 1000.0f);
        gc.print_center(Fonts::verdana11, rect.left + 17, rect.top + 38, tooltip, 1000.0f);
      }
  }

  /** Emmitted when pointer enters the region of the component */
  void on_pointer_enter () 
  {
    mouse_over = true;
  }

  /** Emmitted when pointer leaves the region of the component */
  void on_pointer_leave () 
  {
    mouse_over = false;
  }
  
  void on_primary_button_press (int x, int y) 
  {
    mouse_down = true;
  }

  void on_primary_button_release (int x, int y) 
  { 
    mouse_down = false;
    if (mouse_over && callback)
      ((*object_list).*callback)();
  }
  
  void update (float delta)
  {
    sprite.update();
  }
  
  int get_width() const {
    return 30;
  }

  void update_layout() {}
};



ObjectSelector::ObjectSelector(EditorScreen* editor_, const Rect& rect_)
  : GroupComponent(rect_, false),
    editor(editor_),
    button_pos(0,0)
{
  add(object_list = new ObjectSelectorList(editor, this, 
                                           Rect(2, 2 + 60 + 2, rect.get_width() - 2, rect.get_height() - 2)), 
      true);

  add_button("core/editor/obj_entrance",   "Entrance", &ObjectSelectorList::set_entrance);
  add_button("core/editor/obj_gp_ground",  "Groundpiece (ground)", &ObjectSelectorList::set_gp_ground);
  add_button("core/editor/obj_gp_solid",   "Groundpiece (solid)", &ObjectSelectorList::set_gp_solid);
  add_button("core/editor/obj_gp_bridge",      "Groundpiece (bridge)", &ObjectSelectorList::set_gp_bridge);
  add_button("core/editor/obj_gp_transparent", "Groundpiece (transparent)", &ObjectSelectorList::set_gp_transparent);
  add_button("core/editor/obj_gp_remove",      "Groundpiece (remove)", &ObjectSelectorList::set_gp_remove);
  add_button("core/editor/obj_hotspot",    "Hotspot", &ObjectSelectorList::set_hotspot);
  add_button("core/editor/obj_background", "Background", &ObjectSelectorList::set_background);
  // -------------------------------
  add_button("core/editor/obj_exit", "Exit", &ObjectSelectorList::set_exit);
  add_button("core/editor/obj_liquid", "Liquid", &ObjectSelectorList::set_liquid);
  add_button("core/editor/obj_trap", "Trap", &ObjectSelectorList::set_trap);
  add_button("core/editor/obj_weather", "Weather", &ObjectSelectorList::set_weather);
  add_button("core/editor/obj_worldobj", "Special Object", &ObjectSelectorList::set_worldobj);
}

ObjectSelector::~ObjectSelector()
{
}

void
ObjectSelector::draw_background(DrawingContext& parent_gc)
{
  GUIStyle::draw_raised_box(parent_gc, Rect(0,0, rect.get_width(), rect.get_height()));
}

void
ObjectSelector::add_button(const std::string& image, const std::string& tooltip, Callback callback)
{
  add(new ObjectSelectorButton(object_list,
                               Vector2i(2 + button_pos.x * 30,  
                                        2 + button_pos.y * 30),
                               image, tooltip,
                               callback), true);
  
  button_pos.x += 1;
  if (button_pos.x > 7)
    {
      button_pos.x  = 0;
      button_pos.y += 1;
    }
}

} // namespace Editor

/* EOF */
