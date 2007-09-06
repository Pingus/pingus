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
#include "math.hpp"
#include "display/drawing_context.hpp"
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
                      rect.left+2 + 2, 
                      rect.top+2 + 62 + 2,
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
    if (mouse_over && callback)
      ((*object_selector).*callback)();
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
    button_pos(0,0),
    rect(Vector2i(Display::get_width() - 244 + 2,  38 + 3 + 62),
         Size(240, 495)),
    drawing_context(new DrawingContext(rect)),
    offset(0),
    old_offset(0),
    mode(NOTHING),
    current_object(-1),
    drag_object(-1)
{
  editor->get_gui_manager()->add(this, true);
  
  add_button("core/editor/obj_entrance",   "Entrance", &ObjectSelector::set_entrance);
  add_button("core/editor/obj_gp_ground",      "Groundpiece (ground)", &ObjectSelector::set_gp_ground);
  add_button("core/editor/obj_gp_solid",       "Groundpiece (solid)", &ObjectSelector::set_gp_solid);
  add_button("core/editor/obj_gp_bridge",      "Groundpiece (bridge)", &ObjectSelector::set_gp_bridge);
  add_button("core/editor/obj_gp_transparent", "Groundpiece (transparent)", &ObjectSelector::set_gp_transparent);
  add_button("core/editor/obj_gp_remove",      "Groundpiece (remove)", &ObjectSelector::set_gp_remove);
  add_button("core/editor/obj_hotspot",    "Hotspot", &ObjectSelector::set_hotspot);
  add_button("core/editor/obj_background", "Background", &ObjectSelector::set_background);
  // -------------------------------
  add_button("core/editor/obj_exit", "Exit", &ObjectSelector::set_exit);
  add_button("core/editor/obj_liquid", "Liquid", &ObjectSelector::set_liquid);
  add_button("core/editor/obj_trap", "Trap", &ObjectSelector::set_trap);
  add_button("core/editor/obj_weather", "Weather", &ObjectSelector::set_weather);
  add_button("core/editor/obj_worldobj", "Special Object", &ObjectSelector::set_worldobj);

}

ObjectSelector::~ObjectSelector()
{
}

void
ObjectSelector::draw(DrawingContext& parent_gc)
{
  Rect rect(int(parent_gc.get_width()) - 244,  38,
            int(parent_gc.get_width()),        int(parent_gc.get_height()));

  // FIXME: Should use draw_line
  parent_gc.draw_fillrect(rect.left, rect.top, rect.right, rect.bottom,
                          Color(255, 255, 255));

  parent_gc.draw_fillrect(rect.left+1, rect.top+1, rect.right, rect.bottom,
                          Color(169, 157, 140));
                   
  parent_gc.draw_fillrect(rect.left+1, rect.top+1, rect.right-1, rect.bottom-1,
                          Color(237, 233, 227));

  DrawingContext& gc = *drawing_context;

  gc.clear();

  gc.push_modelview();
  gc.translate(0, offset);

  Objects::iterator i = objects.begin();

  for(int y = 0; y < 20; ++y)
    for(int x = 0; x < 5; ++x)
      {
        if (i != objects.end())
          {
            if (current_object != -1 && (i - objects.begin()) == current_object)
              {
                gc.draw_fillrect(x * 48,      y * 48, 
                                 x * 48 + 48, y * 48 + 48, 
                                 Color(150,150,150));


                gc.draw_rect(x * 48,      y * 48, 
                             x * 48 + 48, y * 48 + 48, 
                             Color(255,255,255), 10000);
              }
            else
              {
                gc.draw_fillrect(x * 48,      y * 48, 
                                 x * 48 + 48, y * 48 + 48, 
                                 (((x-(y%2)) % 2) ? Color(0,0,0) : Color(100,100,100)));
                
              }

            gc.draw(i->sprite, 
                    x * 48 + std::max(0, (48 - i->sprite.get_width())/2), 
                    y * 48 + std::max(0, (48 - i->sprite.get_height())/2));
            ++i;
          }
        else
          {
            gc.draw_fillrect(x * 48,      y * 48, 
                             x * 48 + 48, y * 48 + 48, 
                             (((x-(y%2)) % 2) ? Color(0,0,0) : Color(100,100,100)));
          }
      }
  
  gc.pop_modelview();

  if (mode == OBJECT_DRAG)
    {
      parent_gc.draw(objects[current_object].sprite, Vector3f(real_mouse_pos.x,
                                                              real_mouse_pos.y,
                                                              2000.0f));
    }
  parent_gc.draw(gc);
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
                              Vector2i(Display::get_width() - 242 + button_pos.x * 30,  
                                       40 + button_pos.y * 30),
                              image, tooltip,
                              callback), true);

  button_pos.x += 1;
  if (button_pos.x > 7)
    {
      button_pos.x  = 0;
      button_pos.y += 1;
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
  if (mode == NOTHING && current_object != -1)
    {
      drag_object = current_object;
      mode = OBJECT_DRAG;
    }
}

void
ObjectSelector::on_primary_button_release (int x, int y)
{
  if (mode == OBJECT_DRAG)
    {
      mode = NOTHING;

      //LevelObj* obj = new LevelObj();
      // find out mouse_co in the level
      //editor->add_object(obj);
    }
}

void
ObjectSelector::on_secondary_button_press (int x, int y)
{
  if (mode == NOTHING)
    {
      drag_start = Vector2i(x,y);
      mode = SCROLLING;
      old_offset = offset;
    }
}

void
ObjectSelector::on_secondary_button_release (int x, int y)
{  
  if (mode == SCROLLING)
    mode = NOTHING;
}

void
ObjectSelector::on_pointer_move (int x, int y)
{
  real_mouse_pos = Vector2i(x, y);
  mouse_pos = Vector2i(x - rect.left, 
                       y - rect.top);

  if (mode != OBJECT_DRAG)
    {
      int obj_x = Math::clamp(0, mouse_pos.x / 48, 4);
      int obj_y = Math::clamp(0, int(mouse_pos.y - offset) / 48, 200); // FIXME: 200 is placeholder

      current_object = Math::clamp(-1, (obj_y * 5) + obj_x, int(objects.size()-1));
    }

  if (mode == SCROLLING)
    {
      offset = old_offset + (y - drag_start.y);
    }
}

void
ObjectSelector::set_objects(const std::string& prefix)
{
  std::vector<std::string> lst = Resource::resmgr.get_section(prefix);
  objects.clear();
  for(std::vector<std::string>::const_iterator i = lst.begin(); i != lst.end(); ++i)
    {
      std::cout << "Objects: " << *i << std::endl;
      Sprite sprite = Resource::load_sprite(*i);
      //sprite.scale(48, 48);
      // need to reset the align to top/left
      objects.push_back(Object(sprite));
    }
}

void
ObjectSelector::set_gp_ground()
{
  set_objects("groundpieces/ground");
}

void
ObjectSelector::set_gp_solid()
{
  set_objects("groundpieces/solid");
}

void
ObjectSelector::set_gp_bridge()
{
  set_objects("groundpieces/bridge");
}

void
ObjectSelector::set_gp_transparent()
{
  set_objects("groundpieces/transparent");
}

void
ObjectSelector::set_gp_remove()
{
  set_objects("groundpieces/remove");
}

void
ObjectSelector::set_hotspot()
{
  set_objects("hotspots");
}

void
ObjectSelector::set_background()
{
  set_objects("textures");
}

void
ObjectSelector::set_entrance()
{
  set_objects("entrances");
}

void
ObjectSelector::set_exit()
{
  set_objects("exits");
}

void
ObjectSelector::set_liquid()
{
  set_objects("liquids");
}

void
ObjectSelector::set_trap()
{
  set_objects("traps");
}

void
ObjectSelector::set_weather()
{
  //set_objects("weather");
  std::cout << "ObjectSelector: unimplemented: " << __FILE__ << ":" << __LINE__ << std::endl;
}

void
ObjectSelector::set_worldobj()
{
  std::cout << "ObjectSelector: unimplemented: " << __FILE__ << ":" << __LINE__ << std::endl;
}


} // namespace Editor

/* EOF */
