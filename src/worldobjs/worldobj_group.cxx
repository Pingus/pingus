//  $Id: worldobj_group.cxx,v 1.2 2002/09/15 21:21:47 grumbel Exp $
//
//  Pingus - A free Lemmings clone
//  Copyright (C) 2002 Ingo Ruhnke <grumbel@gmx.de>
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

#include "../graphic_context.hxx"
#include "worldobj_group.hxx"

namespace WorldObj {

WorldObjGroup::WorldObjGroup (const WorldObjsData::WorldObjGroupData& data_)
  : data (new WorldObjsData::WorldObjGroupData(data_))
{
}

void
WorldObjGroup::update (float delta)
{
  for (std::vector<WorldObj*>::iterator i = objs.begin (); i != objs.end (); ++i)
    (*i)->update (delta);
}

void
WorldObjGroup::draw (GraphicContext& gc)
{
  for (std::vector<WorldObj*>::iterator i = objs.begin (); i != objs.end (); ++i)
    (*i)->draw (gc);  
}

} // namespace WorldObj

/* EOF */
