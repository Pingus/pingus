// Pingus - A free Lemmings clone
// Copyright (C) 1998-2011 Ingo Ruhnke <grumbel@gmail.com>
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef HEADER_PINGUS_EDITOR_OBJECT_SELECTOR_LIST_OBJECTS_HPP
#define HEADER_PINGUS_EDITOR_OBJECT_SELECTOR_LIST_OBJECTS_HPP

#include "pingus/resource.hpp"
#include "util/string_util.hpp"
#include "util/log.hpp"

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
    LevelObjPtr obj = std::make_shared<GenericLevelObj>("groundpiece");
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
    LevelObjPtr obj = std::make_shared<GenericLevelObj>("entrance");
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
    LevelObjPtr obj = std::make_shared<GenericLevelObj>("exit");
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
    LevelObjPtr obj = std::make_shared<GenericLevelObj>("hotspot");
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
    LevelObjPtr obj = std::make_shared<GenericLevelObj>("surface-background");
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
    LevelObjPtr obj = std::make_shared<GenericLevelObj>("solidcolor-background");
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
    LevelObjPtr obj = std::make_shared<GenericLevelObj>("starfield-background");
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
    LevelObjPtr obj = std::make_shared<GenericLevelObj>("liquid");
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
    LevelObjPtr obj = std::make_shared<GenericLevelObj>("guillotine");
    obj->set_pos(Vector3f(static_cast<float>(pos.x), static_cast<float>(pos.y)));
    return obj;
  }
};

struct Spike : public ObjectSelectorList::Object
{
  Spike() :
    Object(Sprite("traps/spike_editor"),
           Resource::load_thumb_sprite("traps/spike_editor"))
  {}

  LevelObjPtr create(const Vector2i& pos) {
    LevelObjPtr obj = std::make_shared<GenericLevelObj>("spike");
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
    LevelObjPtr obj = std::make_shared<GenericLevelObj>("fake_exit");
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
    LevelObjPtr obj = std::make_shared<GenericLevelObj>("laser_exit");
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
    LevelObjPtr obj = std::make_shared<GenericLevelObj>("smasher");
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
    LevelObjPtr obj = std::make_shared<GenericLevelObj>("hammer");
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
    LevelObjPtr obj = std::make_shared<GenericLevelObj>("snow-generator");
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
    LevelObjPtr obj = std::make_shared<GenericLevelObj>("rain-generator");
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
    LevelObjPtr obj = std::make_shared<GenericLevelObj>("teleporter");
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
    LevelObjPtr obj = std::make_shared<GenericLevelObj>("teleporter-target");
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
    LevelObjPtr obj = std::make_shared<GenericLevelObj>("iceblock");
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
    LevelObjPtr obj = std::make_shared<GenericLevelObj>("conveyorbelt");
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
    LevelObjPtr obj = std::make_shared<GenericLevelObj>("switchdoor-switch");
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
    LevelObjPtr obj = std::make_shared<GenericLevelObj>("switchdoor-door");
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
    Object(Sprite(name),
           Resource::load_thumb_sprite(name)),
    m_name(name)
  {
  }

  LevelObjPtr create(const Vector2i& pos) {
    std::shared_ptr<GroupLevelObj> group = GroupLevelObj::from_prefab(m_name);
    if (group)
    {
      group->set_pos(Vector3f(static_cast<float>(pos.x), static_cast<float>(pos.y)));
    }
    return group;
  }
};

} // namespace Editor

#endif

/* EOF */
