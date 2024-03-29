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

#include "editor/group_level_obj.hpp"

#include <logmich/log.hpp>

#include "pingus/prefab_file.hpp"
#include "editor/level_obj_factory.hpp"
#include "util/reader.hpp"

namespace pingus::editor {

std::shared_ptr<GroupLevelObj>
GroupLevelObj::from_prefab(std::string const& name)
{
  try
  {
    PrefabFile prefab = PrefabFile::from_resource(name);

    std::shared_ptr<GroupLevelObj> group = std::make_shared<GroupLevelObj>();

    group->m_name = name;
    for(auto const& obj_desc : prefab.get_objects().get_objects())
    {
      LevelObjPtr obj = LevelObjFactory::create(obj_desc);
      if (obj)
      {
        group->add_child(obj);
      }
    }

    group->set_overrides(prefab.get_overrides());

    return group;
  }
  catch(std::exception const& err)
  {
    log_error("{}", err.what());
    return std::shared_ptr<GroupLevelObj>();
  }
}

GroupLevelObj::GroupLevelObj() :
  m_name(),
  m_objects(),
  m_pos(),
  m_orig_pos(),
  m_overrides(),
  m_repeat(),
  m_owner_id(),
  m_release_rate(),
  m_direction()
{
}

GroupLevelObj::~GroupLevelObj()
{
}

std::string
GroupLevelObj::get_section_name() const
{
  if (is_prefab())
  {
    return "prefab";
  }
  else
  {
    return "group";
  }
}

void
GroupLevelObj::add_child(LevelObjPtr const& obj)
{
  m_objects.push_back(obj);
}

void
GroupLevelObj::set_overrides(ReaderMapping const& reader)
{
  if (reader.read("repeat", m_repeat))
  {
    set_repeat(m_repeat);
    m_overrides |= HAS_REPEAT;
  }

  if (reader.read("owner-id", m_owner_id))
  {
    set_owner(m_owner_id);
    m_overrides |= HAS_OWNER;
  }

  if (reader.read("release-rate", m_release_rate))
  {
    set_release_rate(m_release_rate);
    m_overrides |= HAS_RELEASE_RATE;
  }

  if (reader.read("direction",  m_direction))
  {
    set_direction(m_direction);
    m_overrides |= HAS_DIRECTION;
  }
}

void
GroupLevelObj::write_properties(Writer& writer)
{
  if (m_name.empty())
  {
    writer.begin_object("group");
    writer.begin_collection("objects");
    for(auto it = m_objects.begin(); it != m_objects.end(); ++it)
    {
      (*it)->write_properties(writer);
    }
    writer.end_collection();
    writer.end_object();
  }
  else
  {
    writer.begin_object("prefab");
    writer.write("name", m_name);
    writer.write("position", OutVector2fZ{m_pos, z_index()});
    writer.begin_mapping("overrides");
    if (m_overrides & HAS_REPEAT)       writer.write("repeat", m_repeat);
    if (m_overrides & HAS_RELEASE_RATE) writer.write("release-rate", m_release_rate);
    if (m_overrides & HAS_DIRECTION)    writer.write("direction", m_direction);
    if (m_overrides & HAS_OWNER)        writer.write("owner-id", m_owner_id);
    writer.end_mapping();
    writer.end_object();
  }
}

void
GroupLevelObj::draw(DrawingContext& gc)
{
  for(auto it = m_objects.begin(); it != m_objects.end(); ++it)
  {
    (*it)->draw(gc);
  }
}

void
GroupLevelObj::draw_selection(DrawingContext& gc)
{
  for(auto it = m_objects.begin(); it != m_objects.end(); ++it)
  {
    (*it)->draw_selection(gc);
  }
}

void
GroupLevelObj::set_pos(Vector2f const& p)
{
  Vector2f diff = p.as_vec() - m_pos.as_vec();
  m_pos = p;
  for(auto it = m_objects.begin(); it != m_objects.end(); ++it)
  {
    (*it)->set_pos((*it)->get_pos().as_vec() + diff.as_vec());
  }
}

Rect
GroupLevelObj::get_rect() const
{
  if (m_objects.empty())
  {
    return Rect();
  }
  else
  {
    auto it = m_objects.begin();
    Rect rect = (*it)->get_rect();
    for(; it != m_objects.end(); ++it)
    {
      Rect const& rhs = (*it)->get_rect();

      rect = Rect(std::min(rect.left(), rhs.left()),
                  std::min(rect.top(), rhs.top()),
                  std::max(rect.right(), rhs.right()),
                  std::max(rect.bottom(), rhs.bottom()));
    }
    return rect;
  }
}

bool
GroupLevelObj::is_at(int x, int y)
{
  for(auto it = m_objects.begin(); it != m_objects.end(); ++it)
  {
    if ((*it)->is_at(x, y))
      return true;
  }
  return false;
}

void
GroupLevelObj::set_release_rate(int r)
{
  m_release_rate = r;

  for(auto it = m_objects.begin(); it != m_objects.end(); ++it)
  {
    (*it)->set_release_rate(m_release_rate);
  }
}

void
GroupLevelObj::set_owner(int owner)
{
  m_owner_id = owner;

  for(auto it = m_objects.begin(); it != m_objects.end(); ++it)
  {
    (*it)->set_owner(m_owner_id);
  }
}

void
GroupLevelObj::set_direction(std::string const& direction)
{
  m_direction = direction;

  for(auto it = m_objects.begin(); it != m_objects.end(); ++it)
  {
    (*it)->set_direction(m_direction);
  }
}

void
GroupLevelObj::set_repeat(int repeat)
{
  m_repeat = repeat;

  for(auto it = m_objects.begin(); it != m_objects.end(); ++it)
  {
    (*it)->set_repeat(m_repeat);
  }
}

LevelObjPtr
GroupLevelObj::duplicate(Vector2i const& offset) const
{
  std::shared_ptr<GroupLevelObj> group = std::make_shared<GroupLevelObj>();

  group->m_name = m_name;
  group->m_pos = Vector2f(m_pos.as_vec() + Vector2f(offset).as_vec());
  group->m_orig_pos = m_orig_pos + geom::foffset(geom::ioffset(offset.x(), offset.y()));

  group->m_overrides    = m_overrides;
  group->m_repeat       = m_repeat;
  group->m_owner_id     = m_owner_id;
  group->m_release_rate = m_release_rate;
  group->m_direction    = m_direction;

  for(auto it = m_objects.begin(); it != m_objects.end(); ++it)
  {
    group->add_child((*it)->duplicate(offset));
  }
  return group;
}

} // namespace pingus::editor

/* EOF */
