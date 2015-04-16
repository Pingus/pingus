// Pingus - A free Lemmings clone
// Copyright (C) 2007 Ingo Ruhnke <grumbel@gmail.com>
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

#include "editor/object_selector.hpp"

#include <boost/signals2.hpp>

#include "editor/generic_level_obj.hpp"
#include "editor/gui_style.hpp"
#include "editor/level_obj.hpp"
#include "editor/group_level_obj.hpp"
#include "editor/object_selector_set.hpp"
#include "editor/object_selector_list_objects.hpp"
#include "pingus/fonts.hpp"
#include "pingus/gettext.h"
#include "pingus/globals.hpp"
#include "pingus/resource.hpp"
#include "util/log.hpp"
#include "util/system.hpp"
#include "util/string_util.hpp"

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

public:
  boost::signals2::signal<void()> on_click;

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
  object_list(),
  gp_ground_set(),
  gp_solid_set(),
  gp_bridge_set(),
  gp_transparent_set(),
  gp_remove_set(),
  hotspot_set(),
  background_set(),
  entrance_set(),
  exit_set(),
  liquid_set(),
  trap_set(),
  weather_set(),
  worldobj_set(),
  prefab_set()
{
  object_list = create<ObjectSelectorList>(editor, this,
                                           Rect(2, 2 + 60 + 2, rect.get_width() - 2, rect.get_height() - 2));

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
  button = create<ObjectSelectorButton>(object_list,
                                        Vector2i(2 + button_pos.x * 30,
                                                 2 + button_pos.y * 30),
                                        image, tooltip);
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
ObjectSelector::create_groundpiece(const std::string& prefix, const std::string& type)
{
  auto set = util::make_unique<ObjectSelectorSet>(object_list, 48, 48);

  std::vector<std::string> lst = Resource::resmgr.get_section(prefix);
  for(std::vector<std::string>::const_iterator i = lst.begin(); i != lst.end(); ++i)
  {
    //sprite.scale(48, 48);
    set->add(util::make_unique<Groundpiece>(*i, type));
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
    set->add(util::make_unique<Hotspot>(*i));

  return set;
}

std::unique_ptr<ObjectSelectorSet>
ObjectSelector::create_background()
{
  auto set = util::make_unique<ObjectSelectorSet>(object_list, 48, 48);

  set->add(util::make_unique<SolidColorBackground>());
  set->add(util::make_unique<StarfieldBackground>());

  std::vector<std::string> lst = Resource::resmgr.get_section("textures");
  for(std::vector<std::string>::const_iterator i = lst.begin(); i != lst.end(); ++i)
    set->add(util::make_unique<SurfaceBackground>(*i));

  return set;
}

std::unique_ptr<ObjectSelectorSet>
ObjectSelector::create_entrance()
{
  auto set = util::make_unique<ObjectSelectorSet>(object_list, 48, 48);

  set->add(util::make_unique<Entrance>());

  // prefab entrances
  auto directory = Pathname("prefabs/entrances", Pathname::DATA_PATH).opendir("*.prefab");
  for(auto i = directory.begin(); i != directory.end(); ++i)
  {
    set->add(util::make_unique<Prefab>(System::cut_file_extension(i->get_raw_path())));
  }

  if (globals::developer_mode)
  {
    // entrance hotspots
    std::vector<std::string> lst = Resource::resmgr.get_section("entrances");
    for(std::vector<std::string>::const_iterator i = lst.begin(); i != lst.end(); ++i)
    {
      if (*i != "entrances/generic")
        set->add(util::make_unique<Hotspot>(*i, 100));
    }
  }

  return set;
}

std::unique_ptr<ObjectSelectorSet>
ObjectSelector::create_exit()
{
  auto set = util::make_unique<ObjectSelectorSet>(object_list, 48, 48);

  std::vector<std::string> lst = Resource::resmgr.get_section("exit");
  for(std::vector<std::string>::const_iterator i = lst.begin(); i != lst.end(); ++i)
  {
    set->add(util::make_unique<Exit>(*i));
  }

  return set;
}

std::unique_ptr<ObjectSelectorSet>
ObjectSelector::create_liquid()
{
  auto set = util::make_unique<ObjectSelectorSet>(object_list, 48, 48);

  std::vector<std::string> lst = Resource::resmgr.get_section("liquids");
  for(std::vector<std::string>::const_iterator i = lst.begin(); i != lst.end(); ++i)
  {
    set->add(util::make_unique<Liquid>(*i));
  }

  // prefab liquids
  auto directory = Pathname("prefabs/liquids", Pathname::DATA_PATH).opendir("*.prefab");
  for(auto i = directory.begin(); i != directory.end(); ++i)
  {
    set->add(util::make_unique<Prefab>(System::cut_file_extension(i->get_raw_path())));
  }

  return set;
}

std::unique_ptr<ObjectSelectorSet>
ObjectSelector::create_trap()
{
  auto set = util::make_unique<ObjectSelectorSet>(object_list, 48, 48);
  set->add(util::make_unique<Guillotine>());
  set->add(util::make_unique<LaserExit>());
  set->add(util::make_unique<FakeExit>());
  set->add(util::make_unique<Hammer>());
  set->add(util::make_unique<Smasher>());
  set->add(util::make_unique<Spike>());
  return set;
}

std::unique_ptr<ObjectSelectorSet>
ObjectSelector::create_weather()
{
  auto set = util::make_unique<ObjectSelectorSet>(object_list, 48, 48);
  set->add(util::make_unique<RainGenerator>());
  set->add(util::make_unique<SnowGenerator>());
  return set;
}

std::unique_ptr<ObjectSelectorSet>
ObjectSelector::create_worldobj()
{
  auto set = util::make_unique<ObjectSelectorSet>(object_list, 48, 48);
  set->add(util::make_unique<Teleporter>());
  set->add(util::make_unique<TeleporterTarget>());
  set->add(util::make_unique<IceBlock>());
  set->add(util::make_unique<Conveyorbelt>());
  set->add(util::make_unique<SwitchDoorDoor>());
  set->add(util::make_unique<SwitchDoorSwitch>());
  return set;
}

std::unique_ptr<ObjectSelectorSet>
ObjectSelector::create_prefab()
{
  auto set = util::make_unique<ObjectSelectorSet>(object_list, 48, 48);

  // FIXME: doesn't recurse down the prefabs/ directory
  auto directory = Pathname("prefabs/misc", Pathname::DATA_PATH).opendir("*.prefab");
  for(auto i = directory.begin(); i != directory.end(); ++i)
  {
    set->add(util::make_unique<Prefab>(System::cut_file_extension(i->get_raw_path())));
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
