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
#include "sprite.hpp"
#include "math/vector2i.hpp"
#include "editor_screen.hpp"
#include "display/drawing_context.hpp"
#include "fonts.hpp"
#include "display/scene_context.hpp"
#include "gui/gui_manager.hpp"
#include "object_selector.hpp"

#include "resource.hpp"

namespace Editor {

class ObjectSelectorButton : public GUI::Component
{
private:
  ObjectSelector* object_selector;

  Sprite button_raised;
  Sprite button_pressed;
  Sprite sprite;
  bool   mouse_over;
  bool   mouse_down;
  Vector2i    pos;
  std::string tooltip;

  typedef void (ObjectSelector::*Callback)();
  Callback callback;

public:
  ObjectSelectorButton(ObjectSelector* object_selector_,
                       const Vector2i& pos_, const std::string& sprite, const std::string& tooltip_, 
                       Callback callback_ = 0)
    : object_selector(object_selector_),
      button_raised(Resource::load_sprite("core/editor/obj_button-raised")),
      button_pressed(Resource::load_sprite("core/editor/obj_button-pressed")),
      sprite(Resource::load_sprite(sprite)),
      mouse_over(false),
      mouse_down(false),
      pos(pos_),
      tooltip(tooltip_),
      callback(callback_)
  {
  }

  void draw (DrawingContext& gc)
  {
    if (mouse_down)
      gc.draw(button_pressed, pos);
    else if (mouse_over)
      gc.draw(button_raised, pos);

    gc.draw(sprite, pos + Vector2i(3,3));

    if (mouse_over)
      {
        Rect rect(int(gc.get_width()) - 244,  38,
                  int(gc.get_width()),        int(gc.get_height()));
        
        gc.print_left(Fonts::courier_small,
                      //pos.x + 17.f, pos.y + 38.f,
                      rect.left+2 + 2, rect.top+2 + 62 + 2,
                      tooltip, 1000);
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
    //if (mouse_over && callback)
      //  ((*editor).*callback)();
  }
  
  bool is_at(int x, int y)
  {
    return (pos.x <= x && pos.x + 34 > x &&
            pos.y <= y && pos.y + 34 > y);
  }

  void update (float delta)
  {
    sprite.update();
  }
  
  int get_width() const {
    return 30;
  }
};

ObjectSelector::ObjectSelector(EditorScreen* editor_)
  : editor(editor_),
    button_pos(Display::get_width() - 242,  40),
    scene_context(new SceneContext()),
    rect(Vector2i(Display::get_width() - 244 + 2,  38 + 3 + 62),
         Size(240, 495)),
    offset(0),
    old_offset(0),
    mode(NOTHING)
{
  scene_context->translate(rect.left, rect.top);
  scene_context->set_cliprect(rect);

  editor->get_gui_manager()->add(this, true);
  
  add_button("core/editor/obj_entrance", "Entrance");
  add_button("core/editor/obj_gp_ground",      "Groundpiece (ground)");
  add_button("core/editor/obj_gp_solid",       "Groundpiece (solid)");
  add_button("core/editor/obj_gp_bridge",      "Groundpiece (bridge)");
  add_button("core/editor/obj_gp_transparent", "Groundpiece (transparent)");
  add_button("core/editor/obj_gp_remove",      "Groundpiece (remove)");
  add_button("core/editor/obj_hotspot",    "Hotspot");
  add_button("core/editor/obj_background", "Background");
  // -------------------------------
  add_button("core/editor/obj_exit", "Exit");
  add_button("core/editor/obj_liquid", "Liquid");
  add_button("core/editor/obj_trap", "Trap");
  add_button("core/editor/obj_weather", "Weather");
  add_button("core/editor/obj_worldobj", "Special Object");

}

ObjectSelector::~ObjectSelector()
{
}

void
ObjectSelector::draw(DrawingContext& parent_gc)
{
  // FIXME: SceneContext is overkill for this, DrawingContext would be
  // enough, but that can't do cliprects right know
  scene_context->clear();

  scene_context->push_modelview();
  scene_context->translate(0, offset);

  Rect rect(int(parent_gc.get_width()) - 244,  38,
            int(parent_gc.get_width()),        int(parent_gc.get_height()));

  // FIXME: Should use draw_line
  parent_gc.draw_fillrect(rect.left, rect.top, rect.right, rect.bottom,
                   Color(255, 255, 255));

  parent_gc.draw_fillrect(rect.left+1, rect.top+1, rect.right, rect.bottom,
                   Color(169, 157, 140));
                   
  parent_gc.draw_fillrect(rect.left+1, rect.top+1, rect.right-1, rect.bottom-1,
                   Color(237, 233, 227));

  DrawingContext& gc = scene_context->color();

  // black
  //gc.fill_screen(Color(0,0,0));

  for(int y = 0; y < 20; ++y)
    for(int x = 0; x < 5; ++x)
      {
        gc.draw_fillrect(x * 48,      y * 48, 
                         x * 48 + 48, y * 48 + 48, 
                         (((x-(y%2)) % 2) ? Color(0,0,0) : Color(100,100,100)));
      }
  
  parent_gc.draw(new SceneContextDrawingRequest(scene_context, Vector3f(0,0,0)));
  scene_context->pop_modelview();
}

void
ObjectSelector::update(float delta)
{
}

void
ObjectSelector::add_button(const std::string& image, const std::string& tooltip, Callback callback)
{
  editor->get_gui_manager()->add
    (new ObjectSelectorButton(this,
                              button_pos, image, tooltip,
                              callback), true);

  button_pos.x += 30;
  if (button_pos.x >= Display::get_width())
    {
      button_pos.x = Display::get_width() - 240;
      button_pos.y += 30;
    }
}

bool
ObjectSelector::is_at(int x, int y)
{
  return rect.is_inside(Vector2i(x, y));
}

void
ObjectSelector::on_primary_button_press (int x, int y)
{
  std::cout << "Button: " << x << " " << y << std::endl;
}

void
ObjectSelector::on_primary_button_release (int x, int y)
{
  std::cout << "Button" << std::endl;
}

void
ObjectSelector::on_secondary_button_press (int x, int y)
{
  drag_start = Vector2i(x,y);
  mode = SCROLLING;
  old_offset = offset;
}

void
ObjectSelector::on_secondary_button_release (int x, int y)
{  
  mode = NOTHING;
  std::cout << "Button" << std::endl;
}

void
ObjectSelector::on_pointer_move (int x, int y)
{
  if (mode == SCROLLING)
    {
      offset = old_offset + (y - drag_start.y);
    }
}

} // namespace Editor

/* EOF */
