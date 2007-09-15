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

#include <iostream>
#include "math.hpp"
#include "editor_screen.hpp"
#include "object_selector.hpp"
#include "editor_viewport.hpp"
#include "editor_level.hpp"
#include "groundtype.hpp"
#include "resource.hpp"
#include "display/drawing_context.hpp"
#include "object_selector_list.hpp"

namespace Editor {

struct Groundpiece : public ObjectSelectorList::Object 
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

struct Entrance : public ObjectSelectorList::Object 
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

struct Exit : public ObjectSelectorList::Object 
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

struct Hotspot : public ObjectSelectorList::Object 
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

struct SurfaceBackground : public ObjectSelectorList::Object 
{
  ResDescriptor desc;
  
  SurfaceBackground(const std::string& name)
    : Object(Resource::load_sprite(name),
             Resource::load_thumb_sprite(name)),
      desc(name)
  {}

  LevelObj* create(const Vector2i& pos, LevelImpl* impl) { 
    LevelObj* obj = new LevelObj("surface-background", impl);
    obj->set_pos(Vector3f(pos.x, pos.y, -1000.0f)); // FIXME: Hack, z-pos handling is messed up
    obj->set_para_x(1.0f);
    obj->set_para_y(1.0f);
    obj->set_scroll_x(0.0f);
    obj->set_scroll_y(0.0f);
    obj->set_res_desc(desc);
    // obj->set_para();

    return obj;
  }
};

ObjectSelectorList::ObjectSelectorList(EditorScreen* editor_, ObjectSelector* object_selector_, const Rect& rect_)
  : RectComponent(rect_),
    editor(editor_),
    object_selector(object_selector_),
    drawing_context(new DrawingContext(rect)),
    offset(0),
    old_offset(0),
    mode(NOTHING),
    current_object(-1),
    drag_object(-1)
{  
}

void
ObjectSelectorList::draw(DrawingContext& parent_gc)
{
  parent_gc.fill_screen(Color(0,0,0));

  DrawingContext& gc = *drawing_context;
  gc.clear();

  gc.push_modelview();
  gc.translate(0, offset);

  Objects::iterator i = objects.begin();

  for(int y = 0; y < 20; ++y) // FIXME: This is incorrect
    for(int x = 0; x < 5; ++x)
      {
        if (i != objects.end())
          { // draw a item
            if (has_mouse_over() && current_object != -1 && (i - objects.begin()) == current_object)
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

            gc.draw((*i)->thumbnail, Vector2i(x * 48, y * 48));
            ++i;
          }
        else
          { // draw the quads for empty slots
            gc.draw_fillrect(x * 48,      y * 48, 
                             x * 48 + 48, y * 48 + 48, 
                             (((x-(y%2)) % 2) ? Color(0,0,0) : Color(100,100,100)));
          }
      }
  
  gc.pop_modelview();

  parent_gc.draw(gc);

  if (mode == OBJECT_DRAG)
    {
      parent_gc.draw(objects[current_object]->sprite, 
                     real_mouse_pos - Vector2i(objects[current_object]->sprite.get_width()/2,
                                               objects[current_object]->sprite.get_height()/2), 
                     2000.0f);
    }
}

void
ObjectSelectorList::on_primary_button_press (int x, int y)
{
  if (mode == NOTHING && current_object != -1)
    {
      drag_object = current_object;
      mode = OBJECT_DRAG;
    }
}

void
ObjectSelectorList::on_primary_button_release (int x, int y)
{
  if (mode == OBJECT_DRAG)
    {
      mode = NOTHING;
      
      if (current_object != -1)
        {
          // FIXME: Should check if the current mouse over component
          // is the Viewport, else no drag should take place, this
          // checks if the current mouse_over_comp is the
          // ObjectSelector, which is good enough but not perfect 
          if (!object_selector->get_rect().is_inside(Vector2i(x + object_selector->get_rect().left,
                                                              y + object_selector->get_rect().top)))
            {
              Vector2i p = editor->get_viewport()->screen2world(x + object_selector->get_rect().left,
                                                                y + object_selector->get_rect().top);

              // place object with left/top instead of center origin
              p -= Vector2i(objects[current_object]->sprite.get_width()/2,
                            objects[current_object]->sprite.get_height()/2);

              LevelObj* obj = objects[current_object]->create(p, editor->get_level()->get_level_impl());
              if (obj)
                editor->add_object(obj);
              else
                std::cout << "ObjectSelectorList::Object: create() not implemented" << std::endl;
            }
        }
    }
}

void
ObjectSelectorList::on_secondary_button_press (int x, int y)
{
  if (mode == NOTHING)
    {
      drag_start = Vector2i(x,y);
      mode = SCROLLING;
      old_offset = offset;
    }
}

void
ObjectSelectorList::on_secondary_button_release (int x, int y)
{  
  if (mode == SCROLLING)
    mode = NOTHING;
}

void
ObjectSelectorList::on_pointer_move (int x, int y)
{
  real_mouse_pos = Vector2i(x, y);

  mouse_pos = Vector2i(x - rect.left, y - rect.top);

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
ObjectSelectorList::clear_object_list()
{
  for(Objects::iterator i = objects.begin(); i != objects.end(); ++i)
    delete (*i);
  objects.clear();
}

void
ObjectSelectorList::set_objects(const std::string& prefix)
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
ObjectSelectorList::set_groundpiece(const std::string& prefix, const std::string& type)
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
ObjectSelectorList::set_gp_ground()
{
  set_groundpiece("groundpieces/ground", "ground");
}

void
ObjectSelectorList::set_gp_solid()
{
  set_groundpiece("groundpieces/solid", "solid");
}

void
ObjectSelectorList::set_gp_bridge()
{
  set_groundpiece("groundpieces/bridge", "bridge");
}

void
ObjectSelectorList::set_gp_transparent()
{
  set_groundpiece("groundpieces/transparent", "transparent");
}

void
ObjectSelectorList::set_gp_remove()
{
  set_groundpiece("groundpieces/remove", "remove");
}

void
ObjectSelectorList::set_hotspot()
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
ObjectSelectorList::set_background()
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
ObjectSelectorList::set_entrance()
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
ObjectSelectorList::set_exit()
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
ObjectSelectorList::set_liquid()
{
  set_objects("liquids");
}

void
ObjectSelectorList::set_trap()
{
  // Need to differentiate the different trap types
  set_objects("traps");
}

void
ObjectSelectorList::set_weather()
{
  //set_objects("weather");
  std::cout << "ObjectSelector: unimplemented: " << __FILE__ << ":" << __LINE__ << std::endl;
}

void
ObjectSelectorList::set_worldobj()
{
  std::cout << "ObjectSelector: unimplemented: " << __FILE__ << ":" << __LINE__ << std::endl;
}

} // namespace Editor

/* EOF */
