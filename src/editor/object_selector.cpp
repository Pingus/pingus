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
#include "level_objs.hpp"
#include "display/drawing_context.hpp"
#include "gui/gui_manager.hpp"
#include "object_selector.hpp"

#include "resource.hpp"

namespace Editor {

struct Groundpiece : public ObjectSelector::Object 
{
  ResDescriptor desc;
  std::string   type;
  
  Groundpiece(const std::string& name, const std::string& type)
    : Object(Resource::load_sprite(name),
             Resource::load_thumb_sprite(name)),
      desc(name),
      type(type)
  {}      
  
  LevelObj* create(const Vector2i& pos, LevelImpl* impl) { 
    LevelObj* obj = new LevelObj("groundpiece", impl);
    obj->set_pos(pos);
    obj->set_res_desc(desc);
    obj->set_type(type);
    return obj;
  }
};

struct Entrance : public ObjectSelector::Object 
{
  Entrance()
    : Object(Resource::load_sprite("entrances/generic"),
             Resource::load_thumb_sprite("entrances/generic"))
  {}

  LevelObj* create(const Vector2i& pos, LevelImpl* impl) { 
    LevelObj* obj = new LevelObj("entrance", impl);
    obj->set_type("generic");
    obj->set_pos(pos);
    obj->set_direction("misc");
    obj->set_release_rate(150);
    obj->set_owner(0);
    return obj;
  }
};

struct Exit : public ObjectSelector::Object 
{
  ResDescriptor desc;
  
  Exit(const std::string& name)
    : Object(Resource::load_sprite(name), 
             Resource::load_thumb_sprite(name)),
      desc(name)
  {}

  LevelObj* create(const Vector2i& pos, LevelImpl* impl) { 
    LevelObj* obj = new LevelObj("exit", impl);
    obj->set_pos(pos);
    obj->set_res_desc(desc);
    // obj->set_para();
    return obj;
  }
};

struct Hotspot : public ObjectSelector::Object 
{
  ResDescriptor desc;
  
  Hotspot(const std::string& name)
    : Object(Resource::load_sprite(name),
             Resource::load_thumb_sprite(name)),
      desc(name)
  {}

  LevelObj* create(const Vector2i& pos, LevelImpl* impl) { 
    LevelObj* obj = new LevelObj("hotspot", impl);
    obj->set_pos(pos);
    obj->set_res_desc(desc);
    // obj->set_para();
    return obj;
  }
};

struct SurfaceBackground : public ObjectSelector::Object 
{
  ResDescriptor desc;
  
  SurfaceBackground(const std::string& name)
    : Object(Resource::load_sprite(name),
             Resource::load_thumb_sprite(name)),
      desc(name)
  {}

  LevelObj* create(const Vector2i& pos, LevelImpl* impl) { 
    LevelObj* obj = new LevelObj("surface-background", impl);
    obj->set_pos(pos);
    obj->set_para_x(1.0f);
    obj->set_para_y(1.0f);
    obj->set_scroll_x(0.0f);
    obj->set_scroll_y(0.0f);
    obj->set_res_desc(desc);
    // obj->set_para();
    return obj;
  }
};

class ObjectSelectorButton : public GUI::RectComponent
{
private:
  ObjectSelector* object_selector;

  Sprite button_raised;
  Sprite button_pressed;
  Sprite sprite;
  bool   mouse_over;
  bool   mouse_down;
  std::string tooltip;

  typedef void (ObjectSelector::*Callback)();
  Callback callback;

public:
  ObjectSelectorButton(ObjectSelector* object_selector_,
                       const Vector2i& pos, const std::string& sprite, const std::string& tooltip_, 
                       Callback callback_ = 0)
    : RectComponent(Rect(pos, Size(30, 30))),
      object_selector(object_selector_),
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
      ((*object_selector).*callback)();
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
    button_pos(0,0),
    drawing_context(new DrawingContext(Rect(2,64, rect.get_width() - 2, rect.get_height() - 2))),
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
ObjectSelector::draw_background(DrawingContext& parent_gc)
{
  //  Rect rect(int(parent_gc.get_width()) - 244,  38,
  //            int(parent_gc.get_width()),        int(parent_gc.get_height()));

  // FIXME: Should use draw_line
  GUIStyle::draw_raised_box(parent_gc, Rect(0,0, rect.get_width(), rect.get_height()));

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
                
                gc.draw_rect(x * 48,      y * 48, 
                             x * 48 + 48, y * 48 + 48, 
                             Color(155,155,155), 10000);
              }

            gc.draw((*i)->thumbnail, 
                    Vector2i(x * 48 + std::max(0, (48 - (*i)->sprite.get_width())/2), 
                             y * 48 + std::max(0, (48 - (*i)->sprite.get_height())/2)));
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
      parent_gc.draw(objects[current_object]->sprite, Vector3f((float)real_mouse_pos.x,
                                                               (float)real_mouse_pos.y,
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
  add(new ObjectSelectorButton(this,
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

void
ObjectSelector::on_primary_button_press (int x, int y)
{
  GroupComponent::on_primary_button_press(x, y);

  if (mode == NOTHING && current_object != -1)
    {
      drag_object = current_object;
      mode = OBJECT_DRAG;
    }
}

void
ObjectSelector::on_primary_button_release (int x, int y)
{
  GroupComponent::on_primary_button_release(x, y);

  if (mode == OBJECT_DRAG)
    {
      mode = NOTHING;
      
      if (current_object != -1)
        {
          LevelObj* obj = objects[current_object]->create(editor->get_viewport()->screen2world(x,y),
                                                          editor->get_level()->get_level_impl());
          if (obj)
            editor->add_object(obj);
          else
            std::cout << "ObjectSelector::Object: create() not implemented" << std::endl;
        }
    }
}

void
ObjectSelector::on_secondary_button_press (int x, int y)
{
  GroupComponent::on_secondary_button_press(x, y);

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
  GroupComponent::on_secondary_button_release(x, y);

  if (mode == SCROLLING)
    mode = NOTHING;
}

void
ObjectSelector::on_pointer_move (int x, int y)
{
  GroupComponent::on_pointer_move(x, y);
  real_mouse_pos = Vector2i(x - rect.left, y - rect.top);

  mouse_pos = Vector2i(x - rect.left - drawing_context->get_rect().left,
                       y - rect.top  - drawing_context->get_rect().top);

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
ObjectSelector::clear_object_list()
{
  for(Objects::iterator i = objects.begin(); i != objects.end(); ++i)
    delete (*i);
  objects.clear();
}

void
ObjectSelector::set_objects(const std::string& prefix)
{
  clear_object_list();

  // FIXME: Simple debugging aid, needs to be replaced with custom code for the object types
  std::vector<std::string> lst = Resource::resmgr.get_section(prefix);
  for(std::vector<std::string>::const_iterator i = lst.begin(); i != lst.end(); ++i)
    {
      std::cout << "Objects: " << *i << std::endl;
      //sprite.scale(48, 48);
      // need to reset the align to top/left
      objects.push_back(new Object(Resource::load_sprite(*i),
                                   Resource::load_thumb_sprite(*i)));
    }
}

void
ObjectSelector::set_groundpiece(const std::string& prefix, const std::string& type)
{
  clear_object_list();

  std::vector<std::string> lst = Resource::resmgr.get_section(prefix);
  for(std::vector<std::string>::const_iterator i = lst.begin(); i != lst.end(); ++i)
    {
      //sprite.scale(48, 48);
      objects.push_back(new Groundpiece(*i, type));
    }
}

void
ObjectSelector::set_gp_ground()
{
  set_groundpiece("groundpieces/ground", "ground");
}

void
ObjectSelector::set_gp_solid()
{
  set_groundpiece("groundpieces/solid", "solid");
}

void
ObjectSelector::set_gp_bridge()
{
  set_groundpiece("groundpieces/bridge", "bridge");
}

void
ObjectSelector::set_gp_transparent()
{
  set_groundpiece("groundpieces/transparent", "transparent");
}

void
ObjectSelector::set_gp_remove()
{
  set_groundpiece("groundpieces/remove", "remove");
}

void
ObjectSelector::set_hotspot()
{
  clear_object_list();

  std::vector<std::string> lst = Resource::resmgr.get_section("hotspots");
  for(std::vector<std::string>::const_iterator i = lst.begin(); i != lst.end(); ++i)
    {
      //sprite.scale(48, 48);
      objects.push_back(new Hotspot(*i));
    }
}

void
ObjectSelector::set_background()
{
  clear_object_list();

  std::vector<std::string> lst = Resource::resmgr.get_section("textures");
  for(std::vector<std::string>::const_iterator i = lst.begin(); i != lst.end(); ++i)
    {
      //sprite.scale(48, 48);
      objects.push_back(new SurfaceBackground(*i));
    }
}

void
ObjectSelector::set_entrance()
{
  clear_object_list();

  objects.push_back(new Entrance());

  std::vector<std::string> lst = Resource::resmgr.get_section("entrances");
  for(std::vector<std::string>::const_iterator i = lst.begin(); i != lst.end(); ++i)
    {
      //sprite.scale(48, 48);
      if (*i != "entrances/generic")
        objects.push_back(new Hotspot(*i));
    }
}

void
ObjectSelector::set_exit()
{
  clear_object_list();

  std::vector<std::string> lst = Resource::resmgr.get_section("exit");
  for(std::vector<std::string>::const_iterator i = lst.begin(); i != lst.end(); ++i)
    {
      //sprite.scale(48, 48);
      objects.push_back(new Exit(*i));
    }
}

void
ObjectSelector::set_liquid()
{
  set_objects("liquids");
}

void
ObjectSelector::set_trap()
{
  // Need to differentiate the different trap types
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
