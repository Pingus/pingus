//  $Id$
//
//  Pingus - A free Lemmings clone
//  Copyright (C) 2007 Ingo Ruhnke <grumbel@gmx.de>
//
//  This program is free software; you can redistribute it and/or
//  modify it under the terms of the GNU General Public License
//  as published by the Free Software Foundation; either version 2
//  of the License, or (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

#include <assert.h>
#include "group_component.hpp"
#include "component.hpp"

namespace GUI {

GroupComponent*
Component::get_parent() const
{
  return parent; 
}

void
Component::set_parent(GroupComponent* p)
{
  assert(parent == 0);
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

} // namespace GUI

/* EOF */
