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

#include "engine/gui/group_component.hpp"

namespace pingus::gui {

GroupComponent*
Component::get_parent() const
{
  return parent;
}

void
Component::set_parent(GroupComponent* p)
{
  assert(parent == nullptr);
  parent = p;
}

void
Component::grab()
{
  if (parent)
    parent->grab(this);
}

void
Component::ungrab()
{
  if (parent)
    parent->ungrab(this);
}

} // namespace pingus::gui

/* EOF */
