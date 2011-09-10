//  Pingus - A free Lemmings clone
//  Copyright (C) 1998-2011 Ingo Ruhnke <grumbel@gmx.de>
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

#include "editor/group_level_obj.hpp"

#include "util/log.hpp"

namespace Editor {

GroupLevelObj::GroupLevelObj() :
  m_objects(),
  m_pos(),
  m_orig_pos()
{
}

GroupLevelObj::~GroupLevelObj()
{
}

void
GroupLevelObj::add_child(LevelObj* obj)
{
  m_objects.push_back(obj);
}

void
GroupLevelObj::remove_child(LevelObj* obj)
{
  m_objects.push_back(obj);
}

void
GroupLevelObj::write_properties(FileWriter& writer)
{
  writer.begin_section("group");
  for(auto it = m_objects.begin(); it != m_objects.end(); ++it)
  {
    (*it)->write_properties(writer);
  }
  writer.end_section();
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
GroupLevelObj::set_pos(const Vector3f p)
{
  log_tmp("group pos: " << p);

  Vector3f diff = p - m_pos;
  m_pos = p;
  for(auto it = m_objects.begin(); it != m_objects.end(); ++it)
  {
    (*it)->set_pos((*it)->get_pos() + diff);
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
      const Rect& rhs = (*it)->get_rect();

      rect.left = std::min(rect.left, rhs.left);
      rect.top  = std::min(rect.top, rhs.top);

      rect.right  = std::max(rect.right, rhs.right);
      rect.bottom = std::max(rect.bottom, rhs.bottom);
    }
    log_tmp("group rect: " << rect);
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

LevelObj*
GroupLevelObj::duplicate(const Vector2i& offset) const
{
  GroupLevelObj* group = new GroupLevelObj;
  for(auto it = m_objects.begin(); it != m_objects.end(); ++it)
  {
    group->add_child((*it)->duplicate(offset));
  }
  return group;
}

} // namespace Editor

/* EOF */
