//  $Id: spike_obj.cxx,v 1.13 2003/10/19 12:25:47 grumbel Exp $
//
//  Pingus - A free Lemmings clone
//  Copyright (C) 1999 Ingo Ruhnke <grumbel@gmx.de>
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

#include <stdio.h>
#include "spike_obj.hxx"
#include "../editor/editor_view.hxx"
#include "../worldobjsdata/spike_data.hxx"

namespace Pingus {
namespace EditorObjs {

SpikeObj::SpikeObj (const WorldObjsData::SpikeData& data_)
  : data(new WorldObjsData::SpikeData(data_))
{
  data->pos.z = -100;
  sprite = Sprite("Traps/spike", "traps");
  pos_ref = &(data->pos);
}

SpikeObj::~SpikeObj ()
{
  delete data;
}

EditorNS::EditorObjLst
SpikeObj::create (const Vector& pos)
{
  WorldObjsData::SpikeData spike_data;
  spike_data.pos = pos;
  return EditorNS::EditorObjLst(1, new SpikeObj(spike_data));
}

EditorNS::EditorObj*
SpikeObj::duplicate ()
{
  return new SpikeObj(*data);
}

void
SpikeObj::write_xml (std::ostream& xml)
{
  data->write_xml(xml);
}

void
SpikeObj::draw (EditorNS::EditorView* view)
{
  view->draw(sprite, data->pos, 2);
}

std::string
SpikeObj::status_line ()
{
  char str[64];
  snprintf (str, 64, "SpikeObj: %4.2fx%4.2fx%4.2f", data->pos.x, data->pos.y, data->pos.z);
  return str;
}

} // namespace EditorObjs
} // namespace Pingus

/* EOF */
