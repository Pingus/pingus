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

#include "editor/level_obj_factory.hpp"

#include "editor/generic_level_obj.hpp"
#include "editor/group_level_obj.hpp"
#include "util/reader.hpp"

namespace pingus::editor {

LevelObjPtr
LevelObjFactory::create(const ReaderObject& reader_object)
{
  if (reader_object.get_name() == "group")
  {
    std::shared_ptr<GroupLevelObj> group = std::make_shared<GroupLevelObj>();

    ReaderMapping reader = reader_object.get_mapping();
    ReaderCollection collection;
    reader.read("objects", collection);
    std::vector<ReaderObject> objects = collection.get_objects();
    for(auto it = objects.begin(); it != objects.end(); ++it)
    {
      LevelObjPtr obj = create(*it);
      if (obj)
      {
        group->add_child(obj);
      }
    }
    return group;
  }
  else if (reader_object.get_name() == "prefab")
  {
    ReaderMapping reader = reader_object.get_mapping();

    std::string name;
    reader.read("name", name);

    Vector2f p;
    float z_index = 0.0f;
    InVector2fZ in_vec{p, z_index};
    reader.read("position", in_vec);

    std::shared_ptr<GroupLevelObj> group = GroupLevelObj::from_prefab(name);
    if (!group)
    {
      return LevelObjPtr();
    }
    else
    {
      ReaderMapping overrides;
      if (reader.read("overrides", overrides))
        group->set_overrides(overrides);

      group->set_orig_pos(p);
      group->set_pos(p);
      group->set_z_index(z_index);

      return group;
    }
  }
  else
  {
    // Temporary objects
    unsigned attribs;
    Vector2f p;
    float z_index = 0.0f;
    ResDescriptor desc;
    std::string   tmp_str;
    int   tmp_int;
    float tmp_float;
    bool  tmp_bool;

    // Create new object
    ReaderMapping reader = reader_object.get_mapping();
    LevelObjPtr obj = std::make_shared<GenericLevelObj>(reader_object.get_name());
    attribs = obj->get_attribs();

    // All objects have a position - get that.
    InVector2fZ in_vec{p, z_index};
    if (!reader.read("position", in_vec))
    { // Workaround for lack of position for background
      if (reader_object.get_name() == "surface-background") {
        p = Vector2f(0.f, 0.f);
        z_index = -150.f;
      }
    }

    obj->set_orig_pos(p);
    obj->set_pos(p);
    obj->set_z_index(z_index);

    // Get optional attributes based on the attribs value
    if (attribs & HAS_SPRITE)
    {
      if (reader.read("surface", desc))
      {
        obj->set_res_desc(desc);
      }
    }

    if (attribs & HAS_TYPE)
    {
      if (reader.read("type", tmp_str))
      {
        obj->set_type(tmp_str);
      }
    }

    if (attribs & HAS_GPTYPE)
    {
      if (reader.read("type", tmp_str))
      {
        obj->set_ground_type(tmp_str);
      }
    }

    if (attribs & HAS_SPEED)
    {
      if (reader.read("speed", tmp_int))
      {
        obj->set_speed(tmp_int);
      }
    }

    if (attribs & HAS_REPEAT)
    {
      if (reader.read("repeat", tmp_int) ||
          reader.read("width", tmp_int))
      {
        obj->set_repeat(tmp_int);
      }
    }

    if (attribs & HAS_PARALLAX)
    {
      if (reader.read("parallax", tmp_float))
      {
        obj->set_parallax(tmp_float);
      }
    }

    if (attribs & HAS_OWNER)
    {
      if (reader.read("owner-id", tmp_int))
      {
        obj->set_owner(tmp_int);
      }
    }

    if (attribs & HAS_DIRECTION)
    {
      if (reader.read("direction", tmp_str))
      {
        obj->set_direction(tmp_str);
      }
    }

    if (attribs & HAS_COLOR)
    {
      Color tmp_color;
      Colorf tmp_colorf;
      if (reader.read("colori", tmp_color)) {
        obj->set_color(tmp_color);
      } else if (reader.read("colori", tmp_color)) {
        reader.read("color", tmp_colorf);
        obj->set_color(tmp_colorf.to_color());
      }
    }

    if (attribs & HAS_SCROLL)
    {
      if (reader.read("scroll-x", tmp_float))
      {
        obj->set_scroll_x(tmp_float);
      }

      if (reader.read("scroll-y", tmp_float))
      {
        obj->set_scroll_y(tmp_float);
      }
    }

    if (attribs & HAS_STRETCH)
    {
      if (reader.read("stretch-x", tmp_bool))
      {
        obj->set_stretch_x(tmp_bool);
      }

      if (reader.read("stretch-y", tmp_bool))
      {
        obj->set_stretch_y(tmp_bool);
      }

      if (reader.read("keep-aspect", tmp_bool))
      {
        obj->set_keep_aspect(tmp_bool);
      }
    }

    if (attribs & HAS_PARA)
    {
      if (reader.read("para-x", tmp_float))
      {
        obj->set_para_x(tmp_float);
      }

      if (reader.read("para-y", tmp_float))
      {
        obj->set_para_y(tmp_float);
      }
    }

    if (attribs & HAS_RELEASE_RATE)
    {
      if (reader.read("release-rate", tmp_int))
      {
        obj->set_release_rate(tmp_int);
      }
    }

    if (attribs & HAS_ID)
    {
      if (reader.read("id", tmp_str))
      {
        obj->set_id(tmp_str);
      }
    }

    if (attribs & HAS_TARGET_ID)
    {
      if (reader.read("target-id", tmp_str))
      {
        obj->set_target_id(tmp_str);
      }
    }

    if (attribs & HAS_STARFIELD)
    {
      if (reader.read("small-stars", tmp_int))
      {
        obj->set_small_stars(tmp_int);
      }

      if (reader.read("middle-stars", tmp_int))
      {
        obj->set_middle_stars(tmp_int);
      }

      if (reader.read("large-stars", tmp_int))
      {
        obj->set_large_stars(tmp_int);
      }
    }

    if (attribs & HAS_HEIGHT)
    {
      if (reader.read("height", tmp_int))
      {
        obj->set_height(tmp_int);
      }
    }

    return obj;
  }
}

} // namespace pingus::editor

/* EOF */
