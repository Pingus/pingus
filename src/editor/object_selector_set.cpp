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

#include "editor/object_selector_set.hpp"

namespace Editor {

ObjectSelectorSet::ObjectSelectorSet(ObjectSelectorList* list_, int thumb_w, int thumb_h) :
  list(list_),
  thumb_size(thumb_w, thumb_h),
  objects(),
  offset(0)
{
}

ObjectSelectorSet::~ObjectSelectorSet()
{
}

void
ObjectSelectorSet::add(std::unique_ptr<ObjectSelectorList::Object> obj)
{
  objects.push_back(std::move(obj));
}

int
ObjectSelectorSet::get_width() const
{
  return list->get_rect().get_width() / thumb_size.width;
}

int
ObjectSelectorSet::get_height() const
{
  return (static_cast<int>(objects.size()) / get_width()) + ((static_cast<int>(objects.size()) % get_width() > 0) ? 1 : 0);
}

} // namespace Editor

/* EOF */
