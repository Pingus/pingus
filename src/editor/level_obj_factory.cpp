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
#include "pingus/object_schema.hpp"
#include "util/raise_exception.hpp"
#include "util/reader.hpp"

namespace pingus::editor {

LevelObjPtr
LevelObjFactory::create(ReaderObject const& reader_object)
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
    ObjectTypeDef const* type = ObjectSchema::instance().find(reader_object.get_name());
    if (!type) {
      raise_exception(std::runtime_error, "unknown object type: '" << reader_object.get_name() << "'");
    }

    ObjectData data = ObjectData::from_reader(*type, reader_object.get_name(), reader_object.get_mapping());
    return std::make_shared<GenericLevelObj>(std::move(data));
  }
}

} // namespace pingus::editor

/* EOF */
