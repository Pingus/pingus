//  Pingus - A free Lemmings clone
//  Copyright (C) 2007 Ingo Ruhnke <grumbel@gmx.de>
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//  
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//  
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "editor/object_selector.hpp"

#include <boost/signal.hpp>

#include "editor/generic_level_obj.hpp"
#include "editor/gui_style.hpp"
#include "editor/level_obj.hpp"
#include "editor/group_level_obj.hpp"
#include "editor/object_selector_set.hpp"
#include "pingus/fonts.hpp"
#include "pingus/gettext.h"
#include "pingus/resource.hpp"
#include "util/log.hpp"
#include "util/system.hpp"
#include "util/string_util.hpp"

namespace Editor {

struct Groundpiece : public ObjectSelectorList::Object 
{
  ResDescriptor desc;
  std::string   type;
  
  Groundpiece(const std::string& name_, const std::string& type_) : 
    Object(Sprite(name_),
           Resource::load_thumb_sprite(name_)),
    desc(name_),
    type(type_)
  {}   
  
  LevelObjPtr create(const Vector2i& pos) { 
    LevelObjPtr obj(new GenericLevelObj("groundpiece"));
    obj->set_pos(pos);
    obj->set_res_desc(desc);
    obj->set_ground_type(type);
    return obj;
  }
};

struct Entrance : public ObjectSelectorList::Object 
{
  Entrance()
    : Object(Sprite("entrances/generic"),
             Resource::load_thumb_sprite("entrances/generic"))
  {}

  LevelObjPtr create(const Vector2i& pos) { 
    LevelObjPtr obj(new GenericLevelObj("entrance"));
    obj->set_type("generic");
    obj->set_pos(pos);
    obj->set_pos_z(110);
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
    : Object(Sprite(name), 
             Resource::load_thumb_sprite(name)),
      desc(name)
  {}

  LevelObjPtr create(const Vector2i& pos) { 
    LevelObjPtr obj(new GenericLevelObj("exit"));
    obj->set_pos(pos);
    obj->set_res_desc(desc);
    // obj->set_para();
    return obj;
  }
};

struct Hotspot : public ObjectSelectorList::Object 
{
  ResDescriptor desc;
  int z_pos;
  
  Hotspot(const std::string& name_, int z_pos_ = 0)
    : Object(Sprite(name_),
             Resource::load_thumb_sprite(name_)),
      desc(name_),
      z_pos(z_pos_)
  {}

  LevelObjPtr create(const Vector2i& pos) { 
    LevelObjPtr obj(new GenericLevelObj("hotspot"));
    obj->set_pos(pos);
    obj->set_pos_z(static_cast<float>(z_pos));
    obj->set_res_desc(desc);
    // obj->set_para();
    return obj;
  }
};

struct SurfaceBackground : public ObjectSelectorList::Object 
{
  ResDescriptor desc;
  
  SurfaceBackground(const std::string& name)
    : Object(Sprite(name),
             Resource::load_thumb_sprite(name)),
      desc(name)
  {}

  LevelObjPtr create(const Vector2i& pos) { 
    LevelObjPtr obj(new GenericLevelObj("surface-background"));
    obj->set_pos(Vector3f(static_cast<float>(pos.x), static_cast<float>(pos.y), -1000.0f)); // FIXME: Hack, z-pos handling is messed up
    obj->set_para_x(0.5f);
    obj->set_para_y(0.5f);
    obj->set_scroll_x(0.0f);
    obj->set_scroll_y(0.0f);
    obj->set_res_desc(desc);
    // obj->set_para();

    return obj;
  }
};

struct SolidColorBackground : public ObjectSelectorList::Object
{
  SolidColorBackground() 
    : Object(Sprite("core/editor/solidcolorbackground"),
             Resource::load_thumb_sprite("core/editor/solidcolorbackground_thumb"))
  {}
  
  LevelObjPtr create(const Vector2i& pos) {
    LevelObjPtr obj(new GenericLevelObj("solidcolor-background"));
    obj->set_pos(Vector3f(static_cast<float>(pos.x), static_cast<float>(pos.y), -1000.0f)); // FIXME: Hack, z-pos handling is messed up
    obj->set_color(Color(255, 0, 255));
    return obj;
  }
};

struct StarfieldBackground : public ObjectSelectorList::Object
{
  StarfieldBackground() 
    : Object(Sprite("core/editor/starfield"),
             Resource::load_thumb_sprite("core/editor/starfield_thumb"))
  {}
  
  LevelObjPtr create(const Vector2i& pos) {
    LevelObjPtr obj(new GenericLevelObj("starfield-background"));
    // FIXME: Hack, z-pos handling is messed up
    obj->set_pos(Vector3f(static_cast<float>(pos.x), static_cast<float>(pos.y), -1000.0f)); 
    obj->set_small_stars(500);
    obj->set_middle_stars(250);
    obj->set_large_stars(125);
    return obj;    
  }
};

struct Liquid : public ObjectSelectorList::Object
{
  ResDescriptor desc;

  Liquid(const std::string& name) 
    : Object(Sprite(name),
             Resource::load_thumb_sprite(name)),
      desc(name)
  {}
  
  LevelObjPtr create(const Vector2i& pos) {
    LevelObjPtr obj(new GenericLevelObj("liquid"));
    obj->set_pos(Vector3f(static_cast<float>(pos.x), static_cast<float>(pos.y)));
    obj->set_res_desc(desc);
    obj->set_repeat(1);
    return obj;     
  }
};

struct Guillotine : public ObjectSelectorList::Object
{
  Guillotine() :
    Object(Sprite("traps/guillotineidle"),
           Resource::load_thumb_sprite("traps/guillotineidle"))
  {}
  
  LevelObjPtr create(const Vector2i& pos) {
    LevelObjPtr obj(new GenericLevelObj("guillotine"));
    obj->set_pos(Vector3f(static_cast<float>(pos.x), static_cast<float>(pos.y)));
    return obj;     
  }
};

struct FakeExit : public ObjectSelectorList::Object
{
  FakeExit() :
    Object(Sprite("traps/fake_exit"),
           Resource::load_thumb_sprite("traps/fake_exit"))
  {}
  
  LevelObjPtr create(const Vector2i& pos) {
    LevelObjPtr obj(new GenericLevelObj("fake_exit"));
    obj->set_pos(Vector3f(static_cast<float>(pos.x), static_cast<float>(pos.y)));
    return obj;     
  }
};

struct LaserExit : public ObjectSelectorList::Object
{
  LaserExit() :
    Object(Sprite("traps/laser_exit"),
           Resource::load_thumb_sprite("traps/laser_exit"))
  {}
  
  LevelObjPtr create(const Vector2i& pos) {
    LevelObjPtr obj(new GenericLevelObj("laser_exit"));
    obj->set_pos(Vector3f(static_cast<float>(pos.x), static_cast<float>(pos.y)));
    return obj;
  }
};

struct Smasher : public ObjectSelectorList::Object
{
  Smasher() :
    Object(Sprite("traps/smasher"),
           Resource::load_thumb_sprite("traps/smasher"))
  {}
  
  LevelObjPtr create(const Vector2i& pos) {
    LevelObjPtr obj(new GenericLevelObj("smasher"));
    obj->set_pos(Vector3f(static_cast<float>(pos.x), static_cast<float>(pos.y)));
    return obj;
  }
};

struct Hammer : public ObjectSelectorList::Object
{
  Hammer() :
    Object(Sprite("traps/hammer"),
           Resource::load_thumb_sprite("traps/hammer"))
  {}
  
  LevelObjPtr create(const Vector2i& pos) {
    LevelObjPtr obj(new GenericLevelObj("hammer"));
    obj->set_pos(Vector3f(static_cast<float>(pos.x), static_cast<float>(pos.y)));
    return obj;
  }
};

struct SnowGenerator : public ObjectSelectorList::Object
{
  SnowGenerator() :
    Object(Sprite("core/editor/weather_snow"),
           Resource::load_thumb_sprite("core/editor/weather_snow"))
  {}
  
  LevelObjPtr create(const Vector2i& pos) {
    LevelObjPtr obj(new GenericLevelObj("snow-generator"));
    obj->set_pos(Vector3f(static_cast<float>(pos.x), static_cast<float>(pos.y)));
    return obj;
  }
};

struct RainGenerator : public ObjectSelectorList::Object
{
  RainGenerator() :
    Object(Sprite("core/editor/weather_rain"),
           Resource::load_thumb_sprite("core/editor/weather_rain"))
  {}
  
  LevelObjPtr create(const Vector2i& pos) {
    LevelObjPtr obj(new GenericLevelObj("rain-generator"));
    obj->set_pos(Vector3f(static_cast<float>(pos.x), static_cast<float>(pos.y)));
    return obj;
  }
};

struct Teleporter : public ObjectSelectorList::Object
{
  Teleporter() :
    Object(Sprite("worldobjs/teleporter"),
           Resource::load_thumb_sprite("worldobjs/teleporter"))
  {}
  
  LevelObjPtr create(const Vector2i& pos) {
    LevelObjPtr obj(new GenericLevelObj("teleporter"));
    obj->set_pos(Vector3f(static_cast<float>(pos.x), static_cast<float>(pos.y)));
    obj->set_target_id("");
    return obj;
  }
};

struct TeleporterTarget : public ObjectSelectorList::Object
{
  TeleporterTarget() :
    Object(Sprite("worldobjs/teleportertarget"),
           Resource::load_thumb_sprite("worldobjs/teleportertarget"))
  {}
  
  LevelObjPtr create(const Vector2i& pos) {
    LevelObjPtr obj(new GenericLevelObj("teleporter-target"));
    obj->set_pos(Vector3f(static_cast<float>(pos.x), static_cast<float>(pos.y)));
    obj->set_id("id" + StringUtil::to_string(rand()));
    return obj;
  }
};

struct IceBlock : public ObjectSelectorList::Object
{
  IceBlock() :
    Object(Sprite("worldobjs/iceblock"),
           Resource::load_thumb_sprite("worldobjs/iceblock"))
  {}
  
  LevelObjPtr create(const Vector2i& pos) {
    LevelObjPtr obj(new GenericLevelObj("iceblock"));
    obj->set_pos(Vector3f(static_cast<float>(pos.x), static_cast<float>(pos.y)));
    obj->set_repeat(1);
    return obj;
  }
};

struct Conveyorbelt : public ObjectSelectorList::Object
{
  Conveyorbelt() :
    Object(Sprite("worldobjs/conveyorbelt_middle"),
           Resource::load_thumb_sprite("worldobjs/conveyorbelt_middle"))
  {}
  
  LevelObjPtr create(const Vector2i& pos) {
    LevelObjPtr obj(new GenericLevelObj("conveyorbelt"));
    obj->set_pos(Vector3f(static_cast<float>(pos.x), static_cast<float>(pos.y)));
    obj->set_repeat(1);
    return obj;
  }
};

struct SwitchDoorSwitch : public ObjectSelectorList::Object
{
  SwitchDoorSwitch() :
    Object(Sprite("worldobjs/switchdoor_switch"),
           Resource::load_thumb_sprite("worldobjs/switchdoor_switch"))
  {}
  
  LevelObjPtr create(const Vector2i& pos) {
    LevelObjPtr obj(new GenericLevelObj("switchdoor-switch"));
    obj->set_pos(Vector3f(static_cast<float>(pos.x), static_cast<float>(pos.y)));
    obj->set_height(15);
    obj->set_target_id("");
    return obj;
  }
};

struct SwitchDoorDoor : public ObjectSelectorList::Object
{
  SwitchDoorDoor() :
    Object(Sprite("worldobjs/switchdoor_box"),
           Resource::load_thumb_sprite("worldobjs/switchdoor_box"))
  {}
  
  LevelObjPtr create(const Vector2i& pos) {
    LevelObjPtr obj(new GenericLevelObj("switchdoor-door"));
    obj->set_pos(Vector3f(static_cast<float>(pos.x), static_cast<float>(pos.y)));
    obj->set_repeat(1);
    obj->set_id("id" + StringUtil::to_string(rand()));
    return obj;
  }
};

struct Prefab : public ObjectSelectorList::Object
{
private:
  std::string m_name;

public:
  Prefab(const std::string& name) :
    Object(Sprite("worldobjs/infobox"),
           Resource::load_thumb_sprite("worldobjs/infobox")),
    m_name(name)
  {}
  
  LevelObjPtr create(const Vector2i& pos) {
    std::shared_ptr<GroupLevelObj> group = GroupLevelObj::from_prefab(m_name);
    if (group)
    {
      group->set_orig_pos(Vector3f(0, 0, 0));
      group->set_pos(Vector3f(static_cast<float>(pos.x), static_cast<float>(pos.y)));
    }
    return group;
  }
};

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
  
public:
  boost::signal<void()> on_click;

public:
  ObjectSelectorButton(ObjectSelectorList* object_list_,
                       const Vector2i& pos_, const std::string& sprite_, const std::string& tooltip_) :
    RectComponent(Rect(pos_, Size(30, 30))),
    object_list(object_list_),
    button_raised(Sprite("core/editor/obj_button-raised")),
    button_pressed(Sprite("core/editor/obj_button-pressed")),
    sprite(Sprite(sprite_)),
    mouse_over(false),
    mouse_down(false),
    tooltip(tooltip_),
    on_click()
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
      int t_w = static_cast<int>(Fonts::verdana11.get_width(tooltip));
      Rect t_r(rect.left + 17 - t_w/2 - 4, rect.top + 38 - 2, 
               rect.left + 17 + t_w/2 + 4, rect.top + 38 + Fonts::verdana11.get_height() + 4);
      gc.draw_fillrect(t_r, Color(255, 255, 200), 1000.0f);
      gc.draw_rect(t_r, Color(0,0,0), 1000.0f);
      gc.print_center(Fonts::verdana11, Vector2i(rect.left + 17, rect.top + 38), tooltip, 1000.0f);
    }
  }

  /** Emmitted when pointer enters the region of the component */
  void on_pointer_enter() 
  {
    mouse_over = true;
  }

  /** Emmitted when pointer leaves the region of the component */
  void on_pointer_leave() 
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
    if (mouse_over)
      on_click();
  }
  
  void update (float delta)
  {
    sprite.update();
  }
  
  int get_width() const {
    return 30;
  }

  void update_layout() {}

private:
  ObjectSelectorButton(const ObjectSelectorButton&);
  ObjectSelectorButton & operator=(const ObjectSelectorButton&);
};

ObjectSelector::ObjectSelector(EditorScreen* editor_, const Rect& rect_) :
  GroupComponent(rect_, false),
  editor(editor_),
  button_pos(0,0),
  object_list(0),
  gp_ground_set(0),
  gp_solid_set(0),
  gp_bridge_set(0),
  gp_transparent_set(0),
  gp_remove_set(0),
  hotspot_set(0),
  background_set(0),
  entrance_set(0),
  exit_set(0),
  liquid_set(0),
  trap_set(0),
  weather_set(0),
  worldobj_set(0),
  prefab_set(),
  callback()
{
  add(object_list = new ObjectSelectorList(editor, this, 
                                           Rect(2, 2 + 60 + 2, rect.get_width() - 2, rect.get_height() - 2)));

  gp_ground_set  = create_gp_ground();
  gp_solid_set   = create_gp_solid();
  gp_bridge_set  = create_gp_bridge();
  gp_transparent_set = create_gp_transparent();
  gp_remove_set  = create_gp_remove();
  hotspot_set    = create_hotspot();
  background_set = create_background();
  entrance_set   = create_entrance();
  exit_set       = create_exit();
  liquid_set     = create_liquid();
  trap_set       = create_trap();
  weather_set    = create_weather();
  worldobj_set   = create_worldobj();
  prefab_set     = create_prefab();

  add_button("core/editor/obj_entrance",   _("Entrance"), entrance_set.get());
  add_button("core/editor/obj_gp_ground",  _("Groundpiece (ground)"), gp_ground_set.get());
  add_button("core/editor/obj_gp_solid",   _("Groundpiece (solid)"), gp_solid_set.get());
  add_button("core/editor/obj_gp_bridge",  _("Groundpiece (bridge)"), gp_bridge_set.get());
  add_button("core/editor/obj_gp_transparent", _("Groundpiece (transparent)"), gp_transparent_set.get());
  add_button("core/editor/obj_gp_remove",  _("Groundpiece (remove)"), gp_remove_set.get());
  add_button("core/editor/obj_hotspot",    _("Hotspot"), hotspot_set.get());
  add_button("core/editor/obj_background", _("Background"), background_set.get());
  // -------------------------------
  add_button("core/editor/obj_exit",     _("Exit"), exit_set.get());
  add_button("core/editor/obj_liquid",   _("Liquid"), liquid_set.get());
  add_button("core/editor/obj_trap",     _("Trap"), trap_set.get());
  add_button("core/editor/obj_weather",  _("Weather"), weather_set.get());
  add_button("core/editor/obj_worldobj", _("Special Object"), worldobj_set.get());
  add_button("core/editor/obj_prefab",   _("Prefab Object"), prefab_set.get());
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
ObjectSelector::add_button(const std::string& image, const std::string& tooltip, 
                           ObjectSelectorSet* set)
{
  ObjectSelectorButton* button;
  add(button = new ObjectSelectorButton(object_list,
                                        Vector2i(2 + button_pos.x * 30,  
                                                 2 + button_pos.y * 30),
                                        image, tooltip));
  button->on_click.connect(std::bind(&ObjectSelectorList::set_objects, object_list, set));

  button_pos.x += 1;
  if (button_pos.x > 7)
  {
    button_pos.x  = 0;
    button_pos.y += 1;
  }
}

void
ObjectSelector::scroll_up()
{
  object_list->scroll(+48.0f);
}

void
ObjectSelector::scroll_down()
{
  object_list->scroll(-48.0f);
}

std::unique_ptr<ObjectSelectorSet>
ObjectSelector::create_objects(const std::string& prefix)
{
  std::unique_ptr<ObjectSelectorSet> set(new ObjectSelectorSet(object_list, 48, 48));

  // FIXME: Simple debugging aid, needs to be replaced with custom code for the object types
  std::vector<std::string> lst = Resource::resmgr.get_section(prefix);
  for(std::vector<std::string>::const_iterator i = lst.begin(); i != lst.end(); ++i)
  {
    // need to reset the align to top/left
    set->add(new ObjectSelectorList::Object(Sprite(*i),
                                            Resource::load_thumb_sprite(*i)));
  }
  
  return set;
}

std::unique_ptr<ObjectSelectorSet>
ObjectSelector::create_groundpiece(const std::string& prefix, const std::string& type)
{
  std::unique_ptr<ObjectSelectorSet> set(new ObjectSelectorSet(object_list, 48, 48));

  std::vector<std::string> lst = Resource::resmgr.get_section(prefix);
  for(std::vector<std::string>::const_iterator i = lst.begin(); i != lst.end(); ++i)
  {
    //sprite.scale(48, 48);
    set->add(new Groundpiece(*i, type));
  }

  return set;
}

std::unique_ptr<ObjectSelectorSet>
ObjectSelector::create_gp_ground()
{
  return create_groundpiece("groundpieces/ground", "ground");
}

std::unique_ptr<ObjectSelectorSet>
ObjectSelector::create_gp_solid()
{
  return create_groundpiece("groundpieces/solid", "solid");
}

std::unique_ptr<ObjectSelectorSet>
ObjectSelector::create_gp_bridge()
{
  return create_groundpiece("groundpieces/bridge", "bridge");
}

std::unique_ptr<ObjectSelectorSet>
ObjectSelector::create_gp_transparent()
{
  return create_groundpiece("groundpieces/transparent", "transparent");
}

std::unique_ptr<ObjectSelectorSet>
ObjectSelector::create_gp_remove()
{
  return create_groundpiece("groundpieces/remove", "remove");
}

std::unique_ptr<ObjectSelectorSet>
ObjectSelector::create_hotspot()
{
  std::unique_ptr<ObjectSelectorSet> set(new ObjectSelectorSet(object_list, 48, 48));

  std::vector<std::string> lst = Resource::resmgr.get_section("hotspots");
  for(std::vector<std::string>::const_iterator i = lst.begin(); i != lst.end(); ++i)
    set->add(new Hotspot(*i));
  
  return set;
}

std::unique_ptr<ObjectSelectorSet>
ObjectSelector::create_background()
{
  std::unique_ptr<ObjectSelectorSet> set(new ObjectSelectorSet(object_list, 48, 48));
  
  set->add(new SolidColorBackground());
  set->add(new StarfieldBackground());

  std::vector<std::string> lst = Resource::resmgr.get_section("textures");
  for(std::vector<std::string>::const_iterator i = lst.begin(); i != lst.end(); ++i)
    set->add(new SurfaceBackground(*i));

  return set;
}

std::unique_ptr<ObjectSelectorSet>
ObjectSelector::create_entrance()
{
  std::unique_ptr<ObjectSelectorSet> set(new ObjectSelectorSet(object_list, 48, 48));
  
  set->add(new Entrance());

  std::vector<std::string> lst = Resource::resmgr.get_section("entrances");
  for(std::vector<std::string>::const_iterator i = lst.begin(); i != lst.end(); ++i)
  {
    //sprite.scale(48, 48);
    if (*i != "entrances/generic")
      set->add(new Hotspot(*i, 100));
  }

  return set;
}

std::unique_ptr<ObjectSelectorSet>
ObjectSelector::create_exit()
{
  std::unique_ptr<ObjectSelectorSet> set(new ObjectSelectorSet(object_list, 48, 48));

  std::vector<std::string> lst = Resource::resmgr.get_section("exit");
  for(std::vector<std::string>::const_iterator i = lst.begin(); i != lst.end(); ++i)
  {
    set->add(new Exit(*i));
  }
 
  return set;
}

std::unique_ptr<ObjectSelectorSet>
ObjectSelector::create_liquid()
{
  std::unique_ptr<ObjectSelectorSet> set(new ObjectSelectorSet(object_list, 48, 48));

  std::vector<std::string> lst = Resource::resmgr.get_section("liquids");
  for(std::vector<std::string>::const_iterator i = lst.begin(); i != lst.end(); ++i)
  {
    set->add(new Liquid(*i));
  }

  return set;
}

std::unique_ptr<ObjectSelectorSet>
ObjectSelector::create_trap()
{
  std::unique_ptr<ObjectSelectorSet> set(new ObjectSelectorSet(object_list, 48, 48));
  set->add(new Guillotine);
  set->add(new LaserExit);
  set->add(new FakeExit);
  set->add(new Hammer);
  set->add(new Smasher);
  return set;
}

std::unique_ptr<ObjectSelectorSet>
ObjectSelector::create_weather()
{
  std::unique_ptr<ObjectSelectorSet> set(new ObjectSelectorSet(object_list, 48, 48));
  set->add(new RainGenerator);
  set->add(new SnowGenerator);
  return set;
}

std::unique_ptr<ObjectSelectorSet>
ObjectSelector::create_worldobj()
{
  std::unique_ptr<ObjectSelectorSet> set(new ObjectSelectorSet(object_list, 48, 48));
  set->add(new Teleporter);
  set->add(new TeleporterTarget);
  set->add(new IceBlock);
  set->add(new Conveyorbelt);
  set->add(new SwitchDoorDoor);
  set->add(new SwitchDoorSwitch);
  return set;
}

std::unique_ptr<ObjectSelectorSet>
ObjectSelector::create_prefab()
{
  std::unique_ptr<ObjectSelectorSet> set(new ObjectSelectorSet(object_list, 48, 48));

  std::string path = Pathname("prefabs", Pathname::DATA_PATH).get_sys_path();

  // FIXME: doesn't recurse down the prefabs/ directory, doesn't
  // handle ~/.pingus/prefabs/ or other user or modifier directories
  System::Directory directory = System::opendir(path, "*.prefab");
  for(System::Directory::iterator i = directory.begin(); i != directory.end(); ++i)
  {
    set->add(new Prefab("prefabs/" + System::cut_file_extension(i->name)));
  }

  return set;
}

void
ObjectSelector::update_layout()
{
  GroupComponent::update_layout();
  object_list->set_rect(Rect(2, 2 + 60 + 2, rect.get_width() - 2, rect.get_height() - 2)); 
}

} // namespace Editor

/* EOF */
